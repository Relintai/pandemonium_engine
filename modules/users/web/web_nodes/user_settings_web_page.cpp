/*************************************************************************/
/*  user_settings_web_page.cpp                                           */
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

#include "user_settings_web_page.h"

#include "../../singleton/user_db.h"
#include "../../users/user.h"

#include "core/variant/variant.h"
#include "modules/web/html/form_validator.h"
#include "modules/web/html/html_builder.h"
#include "modules/web/http/http_server_enums.h"
#include "modules/web/http/http_session_manager.h"
#include "modules/web/http/web_permission.h"
#include "modules/web/http/web_server.h"
#include "modules/web/http/web_server_request.h"

void UserSettingsWebPage::_render_index(Ref<WebServerRequest> request) {
	Ref<User> user = request->get_meta("user");

	ERR_FAIL_COND(!user.is_valid());

	SettingsRequestData data;

	if (request->get_method() == HTTPServerEnums::HTTP_METHOD_POST) {
		data.uname_val = request->get_parameter("username");
		data.email_val = request->get_parameter("email");
		data.pass_val = request->get_parameter("password");
		data.pass_check_val = request->get_parameter("password_check");

		bool changed = false;

		PoolStringArray errors = _profile_validator->validate(request);

		for (int i = 0; i < errors.size(); ++i) {
			data.error_str += errors[i] + "<br>";
		}

		if (errors.size() == 0) {
			user->write_lock();
			
			if (data.uname_val == user->get_user_name()) {
				data.uname_val = "";
			}

			if (data.email_val == user->get_email()) {
				data.email_val = "";
			}
			

			if (data.uname_val != "") {
				if (UserDB::get_singleton()->is_username_taken(data.uname_val)) {
					data.error_str += "Username already taken!<br>";
				} else {
					user->set_user_name(data.uname_val);
					changed = true;
					data.uname_val = "";
				}
			}

			if (data.email_val != "") {
				if (UserDB::get_singleton()->is_email_taken(data.email_val)) {
					data.error_str += "Email already in use!<br>";
				} else {
					user->set_email(data.email_val);
					changed = true;
					data.email_val = "";
				}
			}

			if (data.pass_val != "") {
				if (data.pass_val != data.pass_check_val) {
					data.error_str += "The passwords did not match!<br>";
				} else {
					user->create_password(data.pass_val);

					changed = true;
				}
			}
			
			user->write_unlock();

			if (changed) {
				user->save();
				emit_signal("user_settings_changed", request, user);
			}
		}
	}

	if (has_method("_render_user_page")) {
		Dictionary d;

		d["user"] = user;
		d["error_str"] = data.error_str;
		d["uname_val"] = data.uname_val;
		d["email_val"] = data.email_val;
		d["pass_val"] = data.pass_val;
		d["pass_check_val"] = data.pass_check_val;

		call("_render_user_page", request, d);
	} else {
		render_settings_request(user, request, &data);
	}
}

void UserSettingsWebPage::render_settings_request(Ref<User> &user, Ref<WebServerRequest> request, SettingsRequestData *data) {
	HTMLBuilder b;

	b.w("Settings");
	b.br();

	{
		if (data->error_str.size() != 0) {
			b.div()->cls("error");

			b.w(data->error_str);

			b.cdiv();
		}
	}

	b.div()->cls("settings");
	{
		// todo href path helper
		b.form()->method("POST")->href("/user/settings");
		{
			b.csrf_tokenr(request);

			b.w("Username");
			b.br();
			b.input()->type("text")->name("username")->placeholder(user->get_user_name())->value(data->uname_val);
			b.cinput();
			b.br();

			b.w("Email");
			b.br();
			b.input()->type("email")->name("email")->placeholder(user->get_email())->value(data->email_val);
			b.cinput();
			b.br();

			b.w("Password");
			b.br();
			b.input()->type("password")->placeholder("*******")->name("password");
			b.cinput();
			b.br();

			b.w("Password again");
			b.br();
			b.input()->type("password")->placeholder("*******")->name("password_check");
			b.cinput();
			b.br();

			b.input()->type("submit")->value("Save");
			b.cinput();
		}
		b.cform();
	}
	b.cdiv();

	request->body += b.result;

	request->compile_and_send_body();
}

UserSettingsWebPage::UserSettingsWebPage() {
	_profile_validator.instance();

	_profile_validator->new_field("username", "Username")->ignore_if_not_exists()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
	_profile_validator->new_field("email", "Email")->ignore_if_not_exists()->need_to_be_email();

	Ref<FormField> pw = _profile_validator->new_field("password", "Password");
	pw->ignore_if_not_exists();
	pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
	pw->need_minimum_length(5);
	pw->need_maximum_length(256);

	_profile_validator->new_field("password_check", "Password check")->ignore_if_other_field_not_exists("password")->need_maximum_length(256)->need_to_match("password");
}

UserSettingsWebPage::~UserSettingsWebPage() {
}

void UserSettingsWebPage::_bind_methods() {
	ADD_SIGNAL(MethodInfo("user_settings_changed", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest"), PropertyInfo(Variant::OBJECT, "user", PROPERTY_HINT_RESOURCE_TYPE, "User")));
}