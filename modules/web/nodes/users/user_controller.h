#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "web/http/web_node.h"

#include "user.h"
#include "web/http/middleware.h"

class Request;
class FormValidator;

//TODO
// Break this up into multiple small webnodes (per page)
// that would make this a lot more customizable
// Only User management, save, load etc should be kept here
// and this node should be expected to be autoloaded.
// It should keep get_singleton() and c++ stuff that deal with users should expect it's presence
// they should use err macros to not crash the app though
// Rename this UserManager
// Also users are useful for not just web stuff, so don't rename them
// Make this inherit from Node instead, only inherit the web user handling from webnodes.
// Also for other webnodes that have admin functionality, break those into separate nodes. It's a lot safer,
// and I think they will work better in this setting like this.

class UserController : public WebNode {
	RCPP_OBJECT(UserController, WebNode);

public:
	void handle_request_main(Request *request);

	struct LoginRequestData {
		String error_str;
		String uname_val;
		String pass_val;
	};

	virtual void handle_login_request_default(Request *request);
	virtual void render_login_request_default(Request *request, LoginRequestData *data);

	struct RegisterRequestData {
		String error_str;
		String uname_val;
		String email_val;
		String pass_val;
		String pass_check_val;
	};

	virtual void handle_register_request_default(Request *request);
	virtual void render_register_request_default(Request *request, RegisterRequestData *data);
	virtual void render_register_success(Request *request);

	virtual void render_already_logged_in_error(Request *request);
	virtual void render_login_success(Request *request);

	virtual void handle_request(Ref<User> &user, Request *request);
	virtual void handle_main_page_request(Ref<User> &user, Request *request);

	struct SettingsRequestData {
		String error_str;

		String uname_val;
		String email_val;
		String pass_val;
		String pass_check_val;
	};

	virtual void handle_settings_request(Ref<User> &user, Request *request);
	virtual void render_settings_request(Ref<User> &user, Request *request, SettingsRequestData *data);

	virtual void handle_password_reset_request(Ref<User> &user, Request *request);
	virtual void handle_logout_request(Ref<User> &user, Request *request);
	virtual void handle_delete_request(Ref<User> &user, Request *request);

	virtual void create_validators();

	// db

	virtual Ref<User> db_get_user(const int id);
	virtual Ref<User> db_get_user(const String &user_name_input);
	virtual void db_save_user(Ref<User> &user);

	virtual Vector<Ref<User>> db_get_all();

	virtual Ref<User> create_user();

	bool is_username_taken(const String &user_name_input);
	bool is_email_taken(const String &email_input);

	virtual bool check_password(const Ref<User> &user, const String &p_password);
	virtual void create_password(Ref<User> &user, const String &p_password);
	virtual String hash_password(const Ref<User> &user, const String &p_password);

	virtual void create_table();
	virtual void drop_table();
	virtual void create_default_entries();

	static UserController *get_singleton();

	UserController();
	~UserController();

protected:
	static UserController *_self;

	static FormValidator *_login_validator;
	static FormValidator *_registration_validator;
	static FormValidator *_profile_validator;

	String _file_path;

	static String _path;
	static String _table_name;
};

// just session setup
class UserSessionSetupMiddleware : public Middleware {
	RCPP_OBJECT(UserSessionSetupMiddleware, Middleware);

public:
	//returnring true means handled, false means continue
	bool on_before_handle_request_main(Request *request);

	UserSessionSetupMiddleware();
	~UserSessionSetupMiddleware();
};

#endif
