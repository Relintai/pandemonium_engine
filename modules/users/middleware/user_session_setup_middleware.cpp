#include "user_session_setup_middleware.h"

#include "web/html/form_validator.h"
#include "web/html/html_builder.h"
#include "web/http/cookie.h"
#include "web/http/http_session.h"
#include "web/http/request.h"
#include "web/http/session_manager.h"
#include "web/http/web_permission.h"

#include "database/database.h"
#include "database/database_manager.h"
#include "database/query_builder.h"
#include "database/query_result.h"
#include "database/table_builder.h"

#include "crypto/hash/sha256.h"

void UserController::handle_request_main(Request *request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	if (request->session.is_valid()) {
		Ref<User> u = request->reference_data["user"];

		if (u.is_valid()) {
			handle_request(u, request);

			return;
		}
	}

	const String &segment = request->get_current_path_segment();

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

void UserController::handle_login_request_default(Request *request) {
	LoginRequestData data;

	if (request->get_method() == HTTP_METHOD_POST) {
		// this is probbaly not needed
		// it's ok for now as I need to test the validators more
		Vector<String> errors;
		_login_validator->validate(request, &errors);
		for (int i = 0; i < errors.size(); ++i) {
			data.error_str += errors[i] + "<br>";
		}
		// not needed end

		data.uname_val = request->get_parameter("username");
		data.pass_val = request->get_parameter("password");

		Ref<User> user = db_get_user(data.uname_val);

		if (user.is_valid()) {
			if (!check_password(user, data.pass_val)) {
				data.error_str += "Invalid username or password!";
			} else {
				Ref<HTTPSession> session = request->get_or_create_session();

				session->add("user_id", user->id);
				SessionManager::get_singleton()->save_session(session);

				::Cookie c = ::Cookie("session_id", session->session_id);
				c.path = "/";

				request->add_cookie(c);

				render_login_success(request);

				return;
			}
		} else {
			data.error_str += "Invalid username or password!";
		}
	}

	render_login_request_default(request, &data);
}

void UserController::render_login_request_default(Request *request, LoginRequestData *data) {
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
			b.csrf_token(request);

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

void UserController::handle_register_request_default(Request *request) {
	RegisterRequestData data;

	if (request->get_method() == HTTP_METHOD_POST) {
		Vector<String> errors;

		_registration_validator->validate(request, &errors);

		for (int i = 0; i < errors.size(); ++i) {
			data.error_str += errors[i] + "<br>";
		}

		data.uname_val = request->get_parameter("username");
		data.email_val = request->get_parameter("email");
		data.pass_val = request->get_parameter("password");
		data.pass_check_val = request->get_parameter("password_check");

		// todo username length etc check
		// todo pw length etc check

		if (is_username_taken(data.uname_val)) {
			data.error_str += "Username already taken!<br>";
		}

		if (is_email_taken(data.email_val)) {
			data.error_str += "Email already in use!<br>";
		}

		if (data.pass_val != data.pass_check_val) {
			data.error_str += "The passwords did not match!<br>";
		}

		if (data.error_str.size() == 0) {
			Ref<User> user;
			user = create_user();

			user->name_user_input = data.uname_val;
			user->email_user_input = data.email_val;

			create_password(user, data.pass_val);
			db_save_user(user);

			render_register_success(request);
			return;
		}
	}

	render_register_request_default(request, &data);
}

void UserController::render_register_success(Request *request) {
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

void UserController::render_register_request_default(Request *request, RegisterRequestData *data) {
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
			b.csrf_token(request);

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

void UserController::render_already_logged_in_error(Request *request) {
	request->body += "You are already logged in.";

	request->compile_and_send_body();
}

void UserController::render_login_success(Request *request) {
	request->body = "Login Success!<br>";

	// request->compile_and_send_body();
	request->send_redirect("/user/settings");
}

void UserController::handle_request(Ref<User> &user, Request *request) {
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

void UserController::handle_main_page_request(Ref<User> &user, Request *request) {
	request->body += "handle_main_page_request";

	request->compile_and_send_body();
}

void UserController::handle_settings_request(Ref<User> &user, Request *request) {
	SettingsRequestData data;

	if (request->get_method() == HTTP_METHOD_POST) {
		data.uname_val = request->get_parameter("username");
		data.email_val = request->get_parameter("email");
		data.pass_val = request->get_parameter("password");
		data.pass_check_val = request->get_parameter("password_check");

		bool changed = false;

		Vector<String> errors;

		bool valid = _profile_validator->validate(request, &errors);

		for (int i = 0; i < errors.size(); ++i) {
			data.error_str += errors[i] + "<br>";
		}

		if (valid) {
			if (data.uname_val == user->name_user_input) {
				data.uname_val = "";
			}

			if (data.email_val == user->email_user_input) {
				data.email_val = "";
			}

			if (data.uname_val != "") {
				if (is_username_taken(data.uname_val)) {
					data.error_str += "Username already taken!<br>";
				} else {
					// todo sanitize for html special chars!
					user->name_user_input = data.uname_val;
					changed = true;
					data.uname_val = "";
				}
			}

			if (data.email_val != "") {
				if (is_email_taken(data.email_val)) {
					data.error_str += "Email already in use!<br>";
				} else {
					// todo sanitize for html special chars!
					// also send email
					user->email_user_input = data.email_val;
					changed = true;
					data.email_val = "";
				}
			}

			if (data.pass_val != "") {
				if (data.pass_val != data.pass_check_val) {
					data.error_str += "The passwords did not match!<br>";
				} else {
					create_password(user, data.pass_val);

					changed = true;
				}
			}

			if (changed) {
				db_save_user(user);
			}
		}
	}

	render_settings_request(user, request, &data);
}

void UserController::render_settings_request(Ref<User> &user, Request *request, SettingsRequestData *data) {
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
			b.csrf_token(request);

			b.w("Username");
			b.br();
			b.input()->type("text")->name("username")->placeholder(user->name_user_input)->value(data->uname_val);
			b.cinput();
			b.br();

			b.w("Email");
			b.br();
			b.input()->type("email")->name("email")->placeholder(user->email_user_input)->value(data->email_val);
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

void UserController::handle_password_reset_request(Ref<User> &user, Request *request) {
	request->body += "handle_password_reset_request";

	request->compile_and_send_body();
}

void UserController::handle_logout_request(Ref<User> &user, Request *request) {
	request->remove_cookie("session_id");

	db_save_user(user);

	SessionManager::get_singleton()->delete_session(request->session->session_id);
	request->session = nullptr;

	HTMLBuilder b;
	b.w("Logout successful!");
	request->body += b.result;

	request->compile_and_send_body();
}

void UserController::handle_delete_request(Ref<User> &user, Request *request) {
	request->body += "handle_delete_request";

	request->compile_and_send_body();
}

void UserController::create_validators() {
	if (!_login_validator) {
		// Login
		_login_validator = new FormValidator();

		_login_validator->new_field("username", "Username")->need_to_exist()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
		FormField *pw = _login_validator->new_field("password", "Password");
		pw->need_to_exist();
		pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
		pw->need_minimum_length(5);
	}

	if (!_registration_validator) {
		// Registration
		_registration_validator = new FormValidator();

		_registration_validator->new_field("username", "Username")->need_to_exist()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
		_registration_validator->new_field("email", "Email")->need_to_exist()->need_to_be_email();

		FormField *pw = _registration_validator->new_field("password", "Password");
		pw->need_to_exist();
		pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
		pw->need_minimum_length(5);

		_registration_validator->new_field("password_check", "Password check")->need_to_match("password");

		_registration_validator->new_field("email", "Email")->need_to_exist()->need_to_be_email();
	}

	if (!_profile_validator) {
		_profile_validator = new FormValidator();

		_profile_validator->new_field("username", "Username")->ignore_if_not_exists()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
		_profile_validator->new_field("email", "Email")->ignore_if_not_exists()->need_to_be_email();

		FormField *pw = _profile_validator->new_field("password", "Password");
		pw->ignore_if_not_exists();
		pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
		pw->need_minimum_length(5);

		_profile_validator->new_field("password_check", "Password check")->ignore_if_other_field_not_exists("password")->need_to_match("password");
	}
}

Ref<User> UserController::db_get_user(const int id) {
	if (id == 0) {
		return Ref<User>();
	}

	Ref<QueryBuilder> b = get_query_builder();

	b->select("username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_table_name);

	b->where()->wp("id", id);

	b->end_command();

	Ref<QueryResult> r = b->run();

	if (!r->next_row()) {
		return Ref<User>();
	}

	Ref<User> user;
	user = create_user();

	user->id = id;
	user->name_user_input = r->get_cell(0);
	user->email_user_input = r->get_cell(1);
	user->rank = r->get_cell_int(2);
	user->pre_salt = r->get_cell(3);
	user->post_salt = r->get_cell(4);
	user->password_hash = r->get_cell(5);
	user->banned = r->get_cell_bool(6);
	user->password_reset_token = r->get_cell(7);
	user->locked = r->get_cell_bool(8);

	return user;
}

Ref<User> UserController::db_get_user(const String &user_name_input) {
	if (user_name_input == "") {
		return Ref<User>();
	}

	Ref<QueryBuilder> b = get_query_builder();

	b->select("id, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_table_name);
	b->where()->wp("username", user_name_input);
	b->end_command();

	Ref<QueryResult> r = b->run();

	if (!r->next_row()) {
		return Ref<User>();
	}

	Ref<User> user;
	user = create_user();

	user->id = r->get_cell_int(0);
	user->name_user_input = user_name_input;
	user->email_user_input = r->get_cell(1);
	user->rank = r->get_cell_int(2);
	user->pre_salt = r->get_cell(3);
	user->post_salt = r->get_cell(4);
	user->password_hash = r->get_cell(5);
	user->banned = r->get_cell_bool(6);
	user->password_reset_token = r->get_cell(7);
	user->locked = r->get_cell_bool(8);

	return user;
}

void UserController::db_save_user(Ref<User> &user) {
	Ref<QueryBuilder> b = get_query_builder();

	if (user->id == 0) {
		b->insert(_table_name, "username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");

		b->values();
		b->val(user->name_user_input);
		b->val(user->email_user_input);
		b->val(user->rank);
		b->val(user->pre_salt);
		b->val(user->post_salt);
		b->val(user->password_hash);
		b->val(user->banned);
		b->val(user->password_reset_token);
		b->val(user->locked);
		b->cvalues();

		b->end_command();
		b->select_last_insert_id();

		Ref<QueryResult> r = b->run();

		user->id = r->get_last_insert_rowid();
	} else {
		b->update(_table_name);
		b->set();
		b->setp("username", user->name_user_input);
		b->setp("email", user->email_user_input);
		b->setp("rank", user->rank);
		b->setp("pre_salt", user->pre_salt);
		b->setp("post_salt", user->post_salt);
		b->setp("password_hash", user->password_hash);
		b->setp("banned", user->banned);
		b->setp("password_reset_token", user->password_reset_token);
		b->setp("locked", user->locked);
		b->cset();
		b->where()->wp("id", user->id);

		// b->print();

		b->run_query();
	}
}

Vector<Ref<User>> UserController::db_get_all() {
	Ref<QueryBuilder> b = get_query_builder();

	b->select("id, username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_table_name);
	b->end_command();
	// b->print();

	Vector<Ref<User>> users;

	Ref<QueryResult> r = b->run();

	while (r->next_row()) {
		Ref<User> user = create_user();

		user->id = r->get_cell_int(0);
		user->name_user_input = r->get_cell(1);
		user->email_user_input = r->get_cell(2);
		user->rank = r->get_cell_int(3);
		user->pre_salt = r->get_cell(4);
		user->post_salt = r->get_cell(5);
		user->password_hash = r->get_cell(6);
		user->banned = r->get_cell_bool(7);
		user->password_reset_token = r->get_cell(8);
		user->locked = r->get_cell_bool(9);

		users.push_back(user);
	}

	return users;
}

Ref<User> UserController::create_user() {
	Ref<User> u;
	u.instance();

	return u;
}

bool UserController::is_username_taken(const String &user_name_input) {
	Ref<QueryBuilder> b = get_query_builder();

	b->select("id")->from(_table_name)->where("username")->like(user_name_input)->end_command();

	Ref<QueryResult> r = b->run();

	return r->next_row();
}
bool UserController::is_email_taken(const String &email_input) {
	Ref<QueryBuilder> b = get_query_builder();

	b->select("id")->from(_table_name)->where("username")->like(email_input)->end_command();

	Ref<QueryResult> r = b->run();

	return r->next_row();
}

bool UserController::check_password(const Ref<User> &user, const String &p_password) {
	return hash_password(user, p_password) == user->password_hash;
}

void UserController::create_password(Ref<User> &user, const String &p_password) {
	if (!user.is_valid()) {
		printf("Error UserController::create_password !user.is_valid()!\n");
		return;
	}

	// todo improve a bit
	user->pre_salt = hash_password(user, user->name_user_input + user->email_user_input);
	user->post_salt = hash_password(user, user->email_user_input + user->name_user_input);

	user->password_hash = hash_password(user, p_password);
}

String UserController::hash_password(const Ref<User> &user, const String &p_password) {
	if (!user.is_valid()) {
		printf("Error UserController::hash_password !user.is_valid()!\n");
		return "";
	}

	Ref<SHA256> s = SHA256::get();

	String p = user->pre_salt + p_password + user->post_salt;

	String c = s->compute(p);

	return c;
}

void UserController::create_table() {
	Ref<TableBuilder> tb = get_table_builder();

	tb->create_table(_table_name);
	tb->integer("id")->auto_increment()->next_row();
	tb->varchar("username", 60)->not_null()->next_row();
	tb->varchar("email", 100)->not_null()->next_row();
	tb->integer("rank")->not_null()->next_row();
	tb->varchar("pre_salt", 100)->next_row();
	tb->varchar("post_salt", 100)->next_row();
	tb->varchar("password_hash", 100)->next_row();
	tb->integer("banned")->next_row();
	tb->varchar("password_reset_token", 100)->next_row();
	tb->integer("locked")->next_row();
	tb->primary_key("id");
	tb->ccreate_table();
	tb->run_query();
	// tb->print();
}
void UserController::drop_table() {
	Ref<TableBuilder> tb = get_table_builder();

	tb->drop_table_if_exists(_table_name)->run_query();
}

void UserController::create_default_entries() {
	Ref<User> user;
	user = create_user();

	user->rank = 3;
	user->name_user_input = "admin";
	user->email_user_input = "admin@admin.com";

	create_password(user, "Password");
	db_save_user(user);

	user = create_user();

	user->rank = 1;
	user->name_user_input = "user";
	user->email_user_input = "user@user.com";

	create_password(user, "Password");
	db_save_user(user);
}

UserController *UserController::get_singleton() {
	return _self;
}

UserController::UserController() :
		WebNode() {
	if (_self) {
		printf("UserController::UserController(): Error! self is not null!/n");
	}

	_self = this;

	create_validators();
}

UserController::~UserController() {
	if (_self == this) {
		_self = nullptr;
	}
}

UserController *UserController::_self = nullptr;

FormValidator *UserController::_login_validator = nullptr;
FormValidator *UserController::_registration_validator = nullptr;
FormValidator *UserController::_profile_validator = nullptr;

String UserController::_path = "./";
String UserController::_table_name = "users";

// returnring true means handled, false means continue
bool UserSessionSetupMiddleware::on_before_handle_request_main(Request *request) {
	if (request->session.is_valid()) {
		int user_id = request->session->get_int("user_id");

		if (user_id != 0) {
			Ref<User> u = UserController::get_singleton()->db_get_user(user_id);

			if (u.is_valid()) {
				request->reference_data["user"] = u;
			} else {
				// log
				request->session->remove("user_id");
			}
		}
	}

	return false;
}

UserSessionSetupMiddleware::UserSessionSetupMiddleware() {
}
UserSessionSetupMiddleware::~UserSessionSetupMiddleware() {
}
