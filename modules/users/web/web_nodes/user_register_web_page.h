#ifndef USER_REGISTER_WEB_PAGE_H
#define USER_REGISTER_WEB_PAGE_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "user_web_page.h"

class WebServerRequest;
class FormValidator;
class User;

class UserRegisterWebPage : public UserWebPage {
	GDCLASS(UserRegisterWebPage, UserWebPage);

public:
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

	UserRegisterWebPage();
	~UserRegisterWebPage();

protected:
	Ref<FormValidator> _registration_validator;
	Ref<FormValidator> _profile_validator;
};

#endif
