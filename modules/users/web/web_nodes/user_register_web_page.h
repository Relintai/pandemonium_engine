#ifndef USER_REGISTER_WEB_PAGE_H
#define USER_REGISTER_WEB_PAGE_H

#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "user_web_page.h"

class WebServerRequest;
class FormValidator;
class User;

class UserRegisterWebPage : public UserWebPage {
	GDCLASS(UserRegisterWebPage, UserWebPage);

public:
	String get_redirect_on_success_url();
	void set_redirect_on_success_url(const String &val);

	struct RegisterRequestData {
		String error_str;
		String uname_val;
		String email_val;
		String pass_val;
		String pass_check_val;
	};

	void _render_index(Ref<WebServerRequest> request);
	virtual void render_register_request_default(Ref<WebServerRequest> request, RegisterRequestData *data);
	virtual void render_register_success(Ref<WebServerRequest> request);

	UserRegisterWebPage();
	~UserRegisterWebPage();

protected:
	static void _bind_methods();

	Ref<FormValidator> _registration_validator;
	Ref<FormValidator> _profile_validator;

	String _redirect_on_success_url;
};

#endif
