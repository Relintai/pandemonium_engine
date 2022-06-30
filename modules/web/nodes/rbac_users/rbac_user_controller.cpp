#include "rbac_user_controller.h"

#include "web/http/http_session.h"
#include "web/http/request.h"

#include "web_modules/rbac/rbac_controller.h"
#include "web_modules/rbac/rbac_default_permissions.h"
#include "rbac_user.h"

Ref<User> RBACUserController::db_get_user(const int id) {
	Ref<RBACUser> u = UserController::db_get_user(id);

	if (u.is_valid()) {
		u->rbac_rank = RBACController::get_singleton()->get_rank(u->rank);
	}

	return u;
}
Ref<User> RBACUserController::db_get_user(const String &user_name_input) {
	Ref<RBACUser> u = UserController::db_get_user(user_name_input);

	if (u.is_valid()) {
		u->rbac_rank = RBACController::get_singleton()->get_rank(u->rank);
	}

	return u;
}

Vector<Ref<User> > RBACUserController::db_get_all() {
	Vector<Ref<User> > users = UserController::db_get_all();

	for (int i = 0; i < users.size(); ++i) {
		Ref<RBACUser> u = users[i];

		if (u.is_valid()) {
			u->rbac_rank = RBACController::get_singleton()->get_rank(u->rank);
		}
	}

	return users;
}

Ref<User> RBACUserController::create_user() {
	Ref<RBACUser> u;
	u.instance();

	u->rank = RBACController::get_singleton()->get_default_user_rank_id();
	u->rbac_rank = RBACController::get_singleton()->get_rank(u->rank);

	return u;
}

RBACUserController::RBACUserController() :
		UserController() {
}

RBACUserController::~RBACUserController() {
}

// returnring true means handled, false means continue
bool RBACUserSessionSetupMiddleware::on_before_handle_request_main(Request *request) {
	if (request->session.is_valid()) {
		int user_id = request->session->get_int("user_id");

		if (user_id != 0) {

			Ref<RBACUser> u = UserController::get_singleton()->db_get_user(user_id);

			if (u.is_valid()) {
				request->reference_data["user"] = u;
			} else {
				// log
				request->session->remove("user_id");
			}
		}
	}

	return false;
}

RBACUserSessionSetupMiddleware::RBACUserSessionSetupMiddleware() {
}
RBACUserSessionSetupMiddleware::~RBACUserSessionSetupMiddleware() {
}

// returnring true means handled, false means continue
bool RBACDefaultUserSessionSetupMiddleware::on_before_handle_request_main(Request *request) {
	// note: add a new file handler middleware func, so basic file handling is easy to set up before this

	Ref<RBACRank> rank;

	if (request->session.is_valid()) {
		int user_id = request->session->get_int("user_id");

		if (user_id != 0) {

			Ref<RBACUser> u = UserController::get_singleton()->db_get_user(user_id);

			if (u.is_valid()) {
				rank = u->rbac_rank;

				request->reference_data["user"] = u;
			} else {
				// log
				request->session->remove("user_id");
			}
		}
	}

	if (!rank.is_valid()) {
		rank = RBACController::get_singleton()->get_default_rank();

		if (!rank.is_valid()) {
			if (RBACController::get_singleton()->continue_on_missing_default_rank()) {
				RLOG_ERR("get_default_rank() has not been set up properly!!! Continuing!");
				return false;
			} else {
				RLOG_ERR("get_default_rank() has not been set up properly!!! Sending 404!");
				request->send_error(404);
				return true;
			}

			
		}
	}

	if (!rank->has_permission(request, User::PERMISSION_READ)) {
		if (rank->has_rank_permission(RBAC_RANK_PERMISSION_USE_REDIRECT)) {
			// Note this can make the webapp prone to enumerations, if not done correctly
			// e.g. redirect from /admin, but sending 404 on a non existing uri, which does not have
			// a special rbac entry
			request->send_redirect(RBACController::get_singleton()->get_redirect_url());
			return true;
		}

		request->send_error(404);
		return true;
	}

	return false;
}

RBACDefaultUserSessionSetupMiddleware::RBACDefaultUserSessionSetupMiddleware() {
}
RBACDefaultUserSessionSetupMiddleware::~RBACDefaultUserSessionSetupMiddleware() {
}
