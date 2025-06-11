/*************************************************************************/
/*  user_login_web_page.cpp                                              */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "user_login_web_page.h"

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

String UserLoginWebPage::get_redirect_on_success_url() {
	return _redirect_on_success_url;
}
void UserLoginWebPage::set_redirect_on_success_url(const String &val) {
	_redirect_on_success_url = val;
}

void UserLoginWebPage::_render_index(Ref<WebServerRequest> request) {
	LoginRequestData data;

	if (request->get_method() == HTTPServerEnums::HTTP_METHOD_POST) {
		PoolStringArray errors = _login_validator->validate(request);
		for (int i = 0; i < errors.size(); ++i) {
			data.error_str += errors[i] + "<br>";
		}

		data.uname_val = request->get_parameter("username");
		data.pass_val = request->get_parameter("password");

		Ref<User> user = UserDB::get_singleton()->get_user_name(data.uname_val);

		if (user.is_valid()) {
			if (!user->check_password(data.pass_val)) {
				data.error_str += "Invalid username or password!";
			} else {
				Ref<HTTPSession> session = request->get_or_create_session();

				session->add("user_id", user->get_user_id());

				Ref<WebServerCookie> c;
				c.instance();
				c->set_data("session_id", session->get_session_id());
				c->set_path("/");
				request->response_add_cookie(c);

				emit_signal("user_logged_in", request, user);

				if (has_method("_render_user_page")) {
					Dictionary d;

					d["type"] = "render_login_success";
					d["user"] = user;

					call("_render_user_page", request, d);
				} else {
					render_login_success(request);
				}

				return;
			}
		} else {
			data.error_str += "Invalid username or password!";
		}
	}

	if (has_method("_render_user_page")) {
		Dictionary d;

		d["type"] = "render_login_request_default";
		d["error_str"] = data.error_str;
		d["uname_val"] = data.uname_val;
		d["pass_val"] = data.pass_val;

		call("_render_user_page", request, d);
	} else {
		render_login_request_default(request, &data);
	}
}

void UserLoginWebPage::render_login_request_default(Ref<WebServerRequest> request, LoginRequestData *data) {
	HTMLBuilder b;

	b.w("Login");
	b.br();

	{
		if (data->error_str.size() != 0) {
			b.div()->cls("error");

			b.w(data->error_str);

			b.cdiv();
		}
	}

	b.div()->cls("login");
	{
		// todo href path helper
		b.form()->method("POST")->href("/user/login");
		{
			b.csrf_tokenr(request);

			b.w("Username");
			b.br();
			b.input()->type("text")->name("username")->value(data->uname_val);
			b.cinput();
			b.br();

			b.w("Password");
			b.br();
			b.input()->type("password")->name("password");
			b.cinput();
			b.br();

			b.input()->type("submit")->value("Send");
			b.cinput();
		}
		b.cform();
	}
	b.cdiv();

	request->body += b.get_result();

	request->compile_and_send_body();
}

void UserLoginWebPage::render_login_success(Ref<WebServerRequest> request) {
	request->body = "Login Success!<br>";

	// request->compile_and_send_body();
	request->send_redirect(_redirect_on_success_url);
}

UserLoginWebPage::UserLoginWebPage() {
	// Login
	_login_validator.instance();

	_login_validator->new_field("username", "Username")->need_to_exist()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
	Ref<FormField> pw = _login_validator->new_field("password", "Password");
	pw->need_to_exist();
	pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
	pw->need_minimum_length(5);
	pw->need_maximum_length(256);

	_logged_out_render_type = RENDER_TYPE_RENDER;
	_logged_in_render_type = RENDER_TYPE_ERROR;
}

UserLoginWebPage::~UserLoginWebPage() {
}

void UserLoginWebPage::_bind_methods() {
	ADD_SIGNAL(MethodInfo("user_logged_in", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest"), PropertyInfo(Variant::OBJECT, "user", PROPERTY_HINT_RESOURCE_TYPE, "User")));

	ClassDB::bind_method(D_METHOD("get_redirect_on_success_url"), &UserLoginWebPage::get_redirect_on_success_url);
	ClassDB::bind_method(D_METHOD("set_redirect_on_success_url", "val"), &UserLoginWebPage::set_redirect_on_success_url);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "redirect_on_success_url"), "set_redirect_on_success_url", "get_redirect_on_success_url");
}
