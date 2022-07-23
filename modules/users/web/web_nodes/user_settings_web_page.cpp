#include "user_settings_web_page.h"

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
					// todo sanitize for html special chars!
					user->set_user_name(data.uname_val);
					changed = true;
					data.uname_val = "";
				}
			}

			if (data.email_val != "") {
				if (UserDB::get_singleton()->is_email_taken(data.email_val)) {
					data.error_str += "Email already in use!<br>";
				} else {
					// todo sanitize for html special chars!
					// also send email
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

			if (changed) {
				user->save();
			}
		}
	}

	render_settings_request(user, request, &data);
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

	_profile_validator->new_field("password_check", "Password check")->ignore_if_other_field_not_exists("password")->need_to_match("password");
}

UserSettingsWebPage::~UserSettingsWebPage() {
}
