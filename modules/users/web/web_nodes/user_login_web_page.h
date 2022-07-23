#ifndef USER_LOGIN_REQUEST_WEB_PAGE_H
#define USER_LOGIN_REQUEST_WEB_PAGE_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "user_web_page.h"

class WebServerRequest;
class FormValidator;
class User;

class UserLoginWebPage : public UserWebPage {
	GDCLASS(UserLoginWebPage, UserWebPage);

public:
	String get_redirect_on_success_url();
	void set_redirect_on_success_url(const String &val);

	struct LoginRequestData {
		String error_str;
		String uname_val;
		String pass_val;
	};

	void _render_index(Ref<WebServerRequest> request);

	virtual void render_login_request_default(Ref<WebServerRequest> request, LoginRequestData *data);
	virtual void render_login_success(Ref<WebServerRequest> request);

	UserLoginWebPage();
	~UserLoginWebPage();

protected:
	static void _bind_methods();

	Ref<FormValidator> _login_validator;

	String _redirect_on_success_url;
};

#endif
