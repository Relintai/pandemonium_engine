#include "user_controller.h"

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

void UserController::_handle_request_main(Ref<WebServerRequest> request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	if (request->get_session().is_valid()) {
		Ref<User> u = request->get_meta("user");

		if (u.is_valid()) {
			handle_request(u, request);

			return;
		}
	}

	String segment = request->get_current_path_segment();

	if (segment == "") {
		handle_login_request_default(request);

		return;
	} else if (segment == "login") {
		handle_login_request_default(request);

		return;
	} else if (segment == "register") {
		handle_register_request_default(request);

		return;
	}

	handle_login_request_default(request);
}

void UserController::handle_login_request_default(Ref<WebServerRequest> request) {
	LoginRequestData data;

	if (request->get_method() == HTTPServerEnums::HTTP_METHOD_POST) {
		// this is probbaly not needed
		// it's ok for now as I need to test the validators more
		PoolStringArray errors = _login_validator->validate(request);
		for (int i = 0; i < errors.size(); ++i) {
			data.error_str += errors[i] + "<br>";
		}
		// not needed end

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
				c->set_data("session_id", session->session_id);
				//c.path = "/";
				request->response_add_cookie(c);

				render_login_success(request);

				return;
			}
		} else {
			data.error_str += "Invalid username or password!";
		}
	}

	render_login_request_default(request, &data);
}

void UserController::render_login_request_default(Ref<WebServerRequest> request, LoginRequestData *data) {
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

	request->body += b.result;

	request->compile_and_send_body();
}

void UserController::handle_register_request_default(Ref<WebServerRequest> request) {
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

			render_register_success(request);
			return;
		}
	}

	render_register_request_default(request, &data);
}

void UserController::render_register_success(Ref<WebServerRequest> request) {
	HTMLBuilder b;

	b.div()->cls("success");
	{
		b.w("Registration successful! You can now log in!");
		b.br();
		b.a()->href("/user/login");
		b.w(">> Login <<");
		b.ca();
	}
	b.cdiv();

	request->body += b.result;

	request->compile_and_send_body();
}

void UserController::render_register_request_default(Ref<WebServerRequest> request, RegisterRequestData *data) {
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

void UserController::render_already_logged_in_error(Ref<WebServerRequest> request) {
	request->body += "You are already logged in.";

	request->compile_and_send_body();
}

void UserController::render_login_success(Ref<WebServerRequest> request) {
	request->body = "Login Success!<br>";

	// request->compile_and_send_body();
	request->send_redirect("/user/settings");
}

void UserController::handle_request(Ref<User> &user, Ref<WebServerRequest> request) {
	const String &segment = request->get_current_path_segment();

	if (segment == "") {
		handle_main_page_request(user, request);
	} else if (segment == "settings") {
		handle_settings_request(user, request);
	} else if (segment == "password_reset") {
		handle_password_reset_request(user, request);
	} else if (segment == "logout") {
		handle_logout_request(user, request);
	} else if (segment == "delete") {
		handle_delete_request(user, request);
	} else if (segment == "login") {
		render_already_logged_in_error(request);
	} else if (segment == "register") {
		render_already_logged_in_error(request);
	} else {
		request->send_error(404);
	}
}

void UserController::handle_main_page_request(Ref<User> &user, Ref<WebServerRequest> request) {
	request->body += "handle_main_page_request";

	request->compile_and_send_body();
}

void UserController::handle_settings_request(Ref<User> &user, Ref<WebServerRequest> request) {
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

void UserController::render_settings_request(Ref<User> &user, Ref<WebServerRequest> request, SettingsRequestData *data) {
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

void UserController::handle_password_reset_request(Ref<User> &user, Ref<WebServerRequest> request) {
	request->body += "handle_password_reset_request";

	request->compile_and_send_body();
}

void UserController::handle_logout_request(Ref<User> &user, Ref<WebServerRequest> request) {
	request->response_remove_cookie_simple("session_id");

	user->save();

	HTTPSessionManager *sess_man = request->get_server()->get_session_manager();
	sess_man->delete_session(request->get_session()->get_session_id());
	request->set_session(Ref<HTTPSession>());

	HTMLBuilder b;
	b.w("Logout successful!");
	request->body += b.result;

	request->compile_and_send_body();
}

void UserController::handle_delete_request(Ref<User> &user, Ref<WebServerRequest> request) {
	request->body += "handle_delete_request";

	request->compile_and_send_body();
}

UserController::UserController() {
	{
		// Login
		_login_validator.instance();

		_login_validator->new_field("username", "Username")->need_to_exist()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
		Ref<FormField> pw = _login_validator->new_field("password", "Password");
		pw->need_to_exist();
		pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
		pw->need_minimum_length(5);
	}

	{
		// Registration
		_registration_validator.instance();

		_registration_validator->new_field("username", "Username")->need_to_exist()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
		_registration_validator->new_field("email", "Email")->need_to_exist()->need_to_be_email();

		Ref<FormField> pw = _registration_validator->new_field("password", "Password");
		pw->need_to_exist();
		pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
		pw->need_minimum_length(5);

		_registration_validator->new_field("password_check", "Password check")->need_to_match("password");

		_registration_validator->new_field("email", "Email")->need_to_exist()->need_to_be_email();
	}

	{
		_profile_validator.instance();

		_profile_validator->new_field("username", "Username")->ignore_if_not_exists()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
		_profile_validator->new_field("email", "Email")->ignore_if_not_exists()->need_to_be_email();

		Ref<FormField> pw = _profile_validator->new_field("password", "Password");
		pw->ignore_if_not_exists();
		pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
		pw->need_minimum_length(5);

		_profile_validator->new_field("password_check", "Password check")->ignore_if_other_field_not_exists("password")->need_to_match("password");
	}
}

UserController::~UserController() {
}
