#include "user_password_reset_web_page.h"

#include "../../singleton/user_db.h"
#include "../../users/user.h"

#include "core/variant.h"
#include "modules/web/html/form_validator.h"
#include "modules/web/html/html_builder.h"
#include "modules/web/http/http_server_enums.h"
#include "modules/web/http/http_session.h"
#include "modules/web/http/http_session_manager.h"
#include "modules/web/http/web_permission.h"
#include "modules/web/http/web_server.h"
#include "modules/web/http/web_server_cookie.h"
#include "modules/web/http/web_server_request.h"

void UserPasswordResetWebPage::handle_password_reset_request(Ref<User> &user, Ref<WebServerRequest> request) {
	request->body += "handle_password_reset_request";

	request->compile_and_send_body();
}

UserPasswordResetWebPage::UserPasswordResetWebPage() {
}

UserPasswordResetWebPage::~UserPasswordResetWebPage() {
}
