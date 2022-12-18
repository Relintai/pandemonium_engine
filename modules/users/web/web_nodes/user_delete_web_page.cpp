#include "user_delete_web_page.h"

#include "../../singleton/user_db.h"
#include "../../users/user.h"

#include "core/variant/variant.h"
#include "modules/web/html/form_validator.h"
#include "modules/web/html/html_builder.h"
#include "modules/web/http/http_server_enums.h"
#include "modules/web/http/http_session.h"
#include "modules/web/http/http_session_manager.h"
#include "modules/web/http/web_permission.h"
#include "modules/web/http/web_server.h"
#include "modules/web/http/web_server_cookie.h"
#include "modules/web/http/web_server_request.h"

void UserDeleteWebPage::handle_delete_request(Ref<User> &user, Ref<WebServerRequest> request) {
	request->body += "handle_delete_request";

	//emit_signal("user_deleted", request, user);

	request->compile_and_send_body();
}

UserDeleteWebPage::UserDeleteWebPage() {
}

UserDeleteWebPage::~UserDeleteWebPage() {
}

void UserDeleteWebPage::_bind_methods() {
	ADD_SIGNAL(MethodInfo("user_deleted", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest"), PropertyInfo(Variant::OBJECT, "user", PROPERTY_HINT_RESOURCE_TYPE, "User")));
}
