#include "user_session_setup_middleware.h"

#include "modules/web/http/http_session.h"
#include "modules/web/http/web_server_request.h"

// returnring true means handled, false means continue
bool UserSessionSetupMiddleware::on_before_handle_request_main(Ref<WebServerRequest> request) {
	if (request->get_session().is_valid()) {
		int user_id = request->get_session()->get_int("user_id");

		if (user_id != 0) {
			Ref<User> u = UserController::get_singleton()->db_get_user(user_id);

			if (u.is_valid()) {
				request->reference_data["user"] = u;
			} else {
				// log
				request->get_session()->remove("user_id");
			}
		}
	}

	return false;
}

UserSessionSetupMiddleware::UserSessionSetupMiddleware() {
}
UserSessionSetupMiddleware::~UserSessionSetupMiddleware() {
}
