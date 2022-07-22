#include "redirect_web_page.h"

#include "../../http/http_server_enums.h"
#include "../../http/web_server_request.h"
#include "core/os/file_access.h"

String RedirectWebPage::get_redirect_path() {
	return _redirect_path;
}
void RedirectWebPage::set_redirect_path(const String &val) {
	_redirect_path = val;
}

int RedirectWebPage::get_code() {
	return _code;
}
void RedirectWebPage::set_code(const int val) {
	_code = val;
}

void RedirectWebPage::_handle_request(Ref<WebServerRequest> request) {
	request->send_redirect(_redirect_path, static_cast<HTTPServerEnums::HTTPStatusCode>(_code));
}

RedirectWebPage::RedirectWebPage() {
	_code = 302;
}

RedirectWebPage::~RedirectWebPage() {
}

void RedirectWebPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_redirect_path"), &RedirectWebPage::get_redirect_path);
	ClassDB::bind_method(D_METHOD("set_redirect_path", "val"), &RedirectWebPage::set_redirect_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "redirect_path"), "set_redirect_path", "get_redirect_path");

	ClassDB::bind_method(D_METHOD("get_code"), &RedirectWebPage::get_code);
	ClassDB::bind_method(D_METHOD("set_code", "val"), &RedirectWebPage::set_code);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "code"), "set_code", "get_code");
}
