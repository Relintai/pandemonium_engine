#include "user_web_page.h"
#include "core/object/object.h"

#include "../../users/user.h"
#include "modules/web/http/web_server_request.h"

UserWebPage::RenderType UserWebPage::get_logged_out_render_type() {
	return _logged_out_render_type;
}
void UserWebPage::set_logged_out_render_type(const UserWebPage::RenderType val) {
	_logged_out_render_type = val;
}

String UserWebPage::get_logged_out_redirect_url() {
	return _logged_out_redirect_url;
}
void UserWebPage::set_logged_out_redirect_url(const String &val) {
	_logged_out_redirect_url = val;
}

int UserWebPage::get_logged_out_error_code() {
	return _logged_out_error_code;
}
void UserWebPage::set_logged_out_error_code(const int val) {
	_logged_out_error_code = val;
}

UserWebPage::RenderType UserWebPage::get_logged_in_render_type() {
	return _logged_in_render_type;
}
void UserWebPage::set_logged_in_render_type(const UserWebPage::RenderType val) {
	_logged_in_render_type = val;
}

String UserWebPage::get_logged_in_redirect_url() {
	return _logged_in_redirect_url;
}
void UserWebPage::set_logged_in_redirect_url(const String &val) {
	_logged_in_redirect_url = val;
}

int UserWebPage::get_logged_in_error_code() {
	return _logged_in_error_code;
}
void UserWebPage::set_logged_in_error_code(const int val) {
	_logged_in_error_code = val;
}

bool UserWebPage::get_render_menu() {
	return _render_menu;
}
void UserWebPage::set_render_menu(const bool val) {
	_render_menu = val;
}

void UserWebPage::_handle_request(Ref<WebServerRequest> request) {
	if (!request->has_meta("user")) {
		handle_not_logged_in(request);
		return;
	}

	Ref<User> u = request->get_meta("user");

	if (!u.is_valid()) {
		handle_not_logged_in(request);
		return;
	}

	handle_logged_in(request);
}

void UserWebPage::_render_index(Ref<WebServerRequest> request) {
	request->compile_and_send_body();
}

void UserWebPage::handle_not_logged_in(Ref<WebServerRequest> request) {
	call("_handle_not_logged_in", request);
}

void UserWebPage::_handle_not_logged_in(Ref<WebServerRequest> request) {
	switch (_logged_out_render_type) {
		case RENDER_TYPE_RENDER:
			if (_render_menu) {
				render_menu(request);
			}

			render_index(request);
			break;
		case RENDER_TYPE_REDIRECT:
			request->send_redirect(_logged_out_redirect_url);
			break;
		case RENDER_TYPE_ERROR:
			request->send_error(_logged_out_error_code);
			break;
		default:
			request->send_error(503);
			break;
	}
}

void UserWebPage::handle_logged_in(Ref<WebServerRequest> request) {
	call("_handle_logged_in", request);
}

void UserWebPage::_handle_logged_in(Ref<WebServerRequest> request) {
	switch (_logged_in_render_type) {
		case RENDER_TYPE_RENDER:
			if (_render_menu) {
				render_menu(request);
			}

			render_index(request);
			break;
		case RENDER_TYPE_REDIRECT:
			request->send_redirect(_logged_in_redirect_url);
			break;
		case RENDER_TYPE_ERROR:
			request->send_error(_logged_in_error_code);
			break;
		default:
			request->send_error(503);
			break;
	}
}

UserWebPage::UserWebPage() {
	_logged_out_render_type = RENDER_TYPE_ERROR;
	_logged_out_error_code = 404;

	_logged_in_render_type = RENDER_TYPE_RENDER;
	_logged_in_error_code = 404;

	_render_menu = true;
}

UserWebPage::~UserWebPage() {
}

void UserWebPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_logged_out_render_type"), &UserWebPage::get_logged_out_render_type);
	ClassDB::bind_method(D_METHOD("set_logged_out_render_type", "val"), &UserWebPage::set_logged_out_render_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "logged_out_render_type", PROPERTY_HINT_ENUM, "Render,Redirect,Error"), "set_logged_out_render_type", "get_logged_out_render_type");

	ClassDB::bind_method(D_METHOD("get_logged_out_redirect_url"), &UserWebPage::get_logged_out_redirect_url);
	ClassDB::bind_method(D_METHOD("set_logged_out_redirect_url", "val"), &UserWebPage::set_logged_out_redirect_url);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "logged_out_redirect_url"), "set_logged_out_redirect_url", "get_logged_out_redirect_url");

	ClassDB::bind_method(D_METHOD("get_logged_out_error_code"), &UserWebPage::get_logged_out_error_code);
	ClassDB::bind_method(D_METHOD("set_logged_out_error_code", "val"), &UserWebPage::set_logged_out_error_code);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "logged_out_error_code"), "set_logged_out_error_code", "get_logged_out_error_code");

	ClassDB::bind_method(D_METHOD("get_logged_in_render_type"), &UserWebPage::get_logged_in_render_type);
	ClassDB::bind_method(D_METHOD("set_logged_in_render_type", "val"), &UserWebPage::set_logged_in_render_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "logged_in_render_type", PROPERTY_HINT_ENUM, "Render,Redirect,Error"), "set_logged_in_render_type", "get_logged_in_render_type");

	ClassDB::bind_method(D_METHOD("get_logged_in_redirect_url"), &UserWebPage::get_logged_in_redirect_url);
	ClassDB::bind_method(D_METHOD("set_logged_in_redirect_url", "val"), &UserWebPage::set_logged_in_redirect_url);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "logged_in_redirect_url"), "set_logged_in_redirect_url", "get_logged_in_redirect_url");

	ClassDB::bind_method(D_METHOD("get_logged_in_error_code"), &UserWebPage::get_logged_in_error_code);
	ClassDB::bind_method(D_METHOD("set_logged_in_error_code", "val"), &UserWebPage::set_logged_in_error_code);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "logged_in_error_code"), "set_logged_in_error_code", "get_logged_in_error_code");

	ClassDB::bind_method(D_METHOD("get_render_menu"), &UserWebPage::get_render_menu);
	ClassDB::bind_method(D_METHOD("set_render_menu", "val"), &UserWebPage::set_render_menu);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "render_menu"), "set_render_menu", "get_render_menu");

	BIND_VMETHOD(MethodInfo("_handle_not_logged_in", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));
	ClassDB::bind_method(D_METHOD("handle_not_logged_in", "request"), &UserWebPage::handle_not_logged_in);
	ClassDB::bind_method(D_METHOD("_handle_not_logged_in", "request"), &UserWebPage::_handle_not_logged_in);

	BIND_VMETHOD(MethodInfo("_handle_logged_in", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));
	ClassDB::bind_method(D_METHOD("handle_logged_in", "request"), &UserWebPage::handle_logged_in);
	ClassDB::bind_method(D_METHOD("_handle_logged_in", "request"), &UserWebPage::_handle_logged_in);

	BIND_ENUM_CONSTANT(RENDER_TYPE_RENDER);
	BIND_ENUM_CONSTANT(RENDER_TYPE_REDIRECT);
	BIND_ENUM_CONSTANT(RENDER_TYPE_ERROR);
}
