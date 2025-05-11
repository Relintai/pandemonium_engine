/*************************************************************************/
/*  user_register_web_page.cpp                                           */
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

#include "user_register_web_page.h"

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

String UserRegisterWebPage::get_redirect_on_success_url() {
	return _redirect_on_success_url;
}
void UserRegisterWebPage::set_redirect_on_success_url(const String &val) {
	_redirect_on_success_url = val;
}

void UserRegisterWebPage::_render_index(Ref<WebServerRequest> request) {
	RegisterRequestData data;

	if (request->get_method() == HTTPServerEnums::HTTP_METHOD_POST) {
		PoolStringArray errors = _registration_validator->validate(request);

		for (int i = 0; i < errors.size(); ++i) {
			data.error_str += errors[i] + "<br>";
		}

		data.uname_val = request->get_parameter("username");
		data.email_val = request->get_parameter("email");
		data.pass_val = request->get_parameter("password");
		data.pass_check_val = request->get_parameter("password_check");

		// todo username length etc check
		// todo pw length etc check

		if (UserDB::get_singleton()->is_username_taken(data.uname_val)) {
			data.error_str += "Username already taken!<br>";
		}

		if (UserDB::get_singleton()->is_email_taken(data.email_val)) {
			data.error_str += "Email already in use!<br>";
		}

		if (data.pass_val != data.pass_check_val) {
			data.error_str += "The passwords did not match!<br>";
		}

		if (data.error_str.size() == 0) {
			Ref<User> user;
			user = UserDB::get_singleton()->create_user();

			user->set_user_name(data.uname_val);
			user->set_email(data.email_val);

			user->create_password(data.pass_val);
			user->save();

			emit_signal("user_registered", request, user);

			if (has_method("_render_user_page")) {
				Dictionary d;

				d["type"] = "render_register_success";
				d["user"] = user;

				call("_render_user_page", request, d);
			} else {
				render_register_success(request);
			}

			return;
		}
	}

	if (has_method("_render_user_page")) {
		Dictionary d;

		d["type"] = "render_register_request_default";
		d["error_str"] = data.error_str;
		d["uname_val"] = data.uname_val;
		d["email_val"] = data.email_val;
		d["pass_val"] = data.pass_val;
		d["pass_check_val"] = data.pass_check_val;

		call("_render_user_page", request, d);
	} else {
		render_register_request_default(request, &data);
	}
}

void UserRegisterWebPage::render_register_success(Ref<WebServerRequest> request) {
	HTMLBuilder b;

	b.div()->cls("success");
	{
		b.w("Registration successful! You can now log in!");
		b.br();
		b.a()->href(_redirect_on_success_url);
		b.w(">> Login <<");
		b.ca();
	}
	b.cdiv();

	request->body += b.result;

	request->compile_and_send_body();
}

void UserRegisterWebPage::render_register_request_default(Ref<WebServerRequest> request, RegisterRequestData *data) {
	HTMLBuilder b;

	b.w("Registration");
	b.br();

	{
		if (data->error_str.size() != 0) {
			b.div()->cls("error");

			b.w(data->error_str);

			b.cdiv();
		}
	}

	b.div()->cls("register");
	{
		// todo href path helper
		b.form()->method("POST")->href("/user/register");
		{
			b.csrf_tokenr(request);

			b.w("Username");
			b.br();
			b.input()->type("text")->name("username")->value(data->uname_val);
			b.cinput();
			b.br();

			b.w("Email");
			b.br();
			b.input()->type("email")->name("email")->value(data->email_val);
			b.cinput();
			b.br();

			b.w("Password");
			b.br();
			b.input()->type("password")->name("password");
			b.cinput();
			b.br();

			b.w("Password again");
			b.br();
			b.input()->type("password")->name("password_check");
			b.cinput();
			b.br();

			b.input()->type("submit")->value("Register");
			b.cinput();
		}
		b.cform();
	}
	b.cdiv();

	request->body += b.result;

	request->compile_and_send_body();
}

UserRegisterWebPage::UserRegisterWebPage() {
	// Registration
	_registration_validator.instance();

	_registration_validator->new_field("username", "Username")->need_to_exist()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
	_registration_validator->new_field("email", "Email")->need_to_exist()->need_to_be_email();

	Ref<FormField> pw = _registration_validator->new_field("password", "Password");
	pw->need_to_exist();
	pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
	pw->need_minimum_length(5);
	pw->need_maximum_length(256);

	_registration_validator->new_field("password_check", "Password check")->need_maximum_length(256)->need_to_match("password");

	_registration_validator->new_field("email", "Email")->need_to_exist()->need_to_be_email();

	_logged_out_render_type = RENDER_TYPE_RENDER;
	_logged_in_render_type = RENDER_TYPE_ERROR;
}

UserRegisterWebPage::~UserRegisterWebPage() {
}

void UserRegisterWebPage::_bind_methods() {
	ADD_SIGNAL(MethodInfo("user_registered", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest"), PropertyInfo(Variant::OBJECT, "user", PROPERTY_HINT_RESOURCE_TYPE, "User")));

	ClassDB::bind_method(D_METHOD("get_redirect_on_success_url"), &UserRegisterWebPage::get_redirect_on_success_url);
	ClassDB::bind_method(D_METHOD("set_redirect_on_success_url", "val"), &UserRegisterWebPage::set_redirect_on_success_url);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "redirect_on_success_url"), "set_redirect_on_success_url", "get_redirect_on_success_url");
}
