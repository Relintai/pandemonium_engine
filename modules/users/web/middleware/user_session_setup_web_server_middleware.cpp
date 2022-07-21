#include "user_session_setup_web_server_middleware.h"

#include "../../singleton/user_db.h"
#include "../../users/user.h"
#include "modules/web/http/http_session.h"
#include "modules/web/http/web_server_request.h"

// returning true means handled, false means continue
bool UserSessionSetupWebServerMiddleware::_on_before_handle_request_main(Ref<WebServerRequest> request) {
	if (request->get_session().is_valid()) {
		int user_id = request->get_session()->get_int("user_id", -1);

		if (user_id != -1) {
			Ref<User> u = UserDB::get_singleton()->get_user(user_id);

			if (u.is_valid()) {
				request->set_meta("user", u);
			} else {
				// log
				request->get_session()->remove("user_id");
			}
		}
	}

	return false;
}

UserSessionSetupWebServerMiddleware::UserSessionSetupWebServerMiddleware() {
}
UserSessionSetupWebServerMiddleware::~UserSessionSetupWebServerMiddleware() {
}
