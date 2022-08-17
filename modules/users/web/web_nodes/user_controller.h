#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "modules/web/http/web_node.h"

class WebServerRequest;
class FormValidator;
class User;

class UserController : public WebNode {
	GDCLASS(UserController, WebNode);

public:
	void _handle_request_main(Ref<WebServerRequest> request);

	struct LoginRequestData {
		String error_str;
		String uname_val;
		String pass_val;
	};

	virtual void handle_login_request_default(Ref<WebServerRequest> request);
	virtual void render_login_request_default(Ref<WebServerRequest> request, LoginRequestData *data);

	struct RegisterRequestData {
		String error_str;
		String uname_val;
		String email_val;
		String pass_val;
		String pass_check_val;
	};

	virtual void handle_register_request_default(Ref<WebServerRequest> request);
	virtual void render_register_request_default(Ref<WebServerRequest> request, RegisterRequestData *data);
	virtual void render_register_success(Ref<WebServerRequest> request);

	virtual void render_already_logged_in_error(Ref<WebServerRequest> request);
	virtual void render_login_success(Ref<WebServerRequest> request);

	virtual void handle_request(Ref<User> &user, Ref<WebServerRequest> request);
	virtual void handle_main_page_request(Ref<User> &user, Ref<WebServerRequest> request);

	struct SettingsRequestData {
		String error_str;

		String uname_val;
		String email_val;
		String pass_val;
		String pass_check_val;
	};

	virtual void handle_settings_request(Ref<User> &user, Ref<WebServerRequest> request);
	virtual void render_settings_request(Ref<User> &user, Ref<WebServerRequest> request, SettingsRequestData *data);

	virtual void handle_password_reset_request(Ref<User> &user, Ref<WebServerRequest> request);
	virtual void handle_logout_request(Ref<User> &user, Ref<WebServerRequest> request);
	virtual void handle_delete_request(Ref<User> &user, Ref<WebServerRequest> request);

	UserController();
	~UserController();

protected:
	Ref<FormValidator> _login_validator;
	Ref<FormValidator> _registration_validator;
	Ref<FormValidator> _profile_validator;
};

#endif
