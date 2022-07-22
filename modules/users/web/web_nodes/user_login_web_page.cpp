#include "user_login_web_page.h"

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

void UserLoginWebPage::handle_login_request_default(Ref<WebServerRequest> request) {
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

	request->body += b.result;

	request->compile_and_send_body();
}

void UserLoginWebPage::render_already_logged_in_error(Ref<WebServerRequest> request) {
	request->body += "You are already logged in.";

	request->compile_and_send_body();
}

void UserLoginWebPage::render_login_success(Ref<WebServerRequest> request) {
	request->body = "Login Success!<br>";

	// request->compile_and_send_body();
	request->send_redirect("/user/settings");
}

UserLoginWebPage::UserLoginWebPage() {
	// Login
	_login_validator.instance();

	_login_validator->new_field("username", "Username")->need_to_exist()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
	Ref<FormField> pw = _login_validator->new_field("password", "Password");
	pw->need_to_exist();
	pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
	pw->need_minimum_length(5);
}

UserLoginWebPage::~UserLoginWebPage() {
}
