#ifndef USER_SETTINGS_WEB_PAGE_H
#define USER_SETTINGS_WEB_PAGE_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "user_web_page.h"

class WebServerRequest;
class FormValidator;
class User;

class UserSettingsWebPage : public UserWebPage {
	GDCLASS(UserSettingsWebPage, UserWebPage);

public:
	void _handle_request_main(Ref<WebServerRequest> request);

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

	UserSettingsWebPage();
	~UserSettingsWebPage();

protected:
	Ref<FormValidator> _profile_validator;
};

#endif
