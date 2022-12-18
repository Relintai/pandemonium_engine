#ifndef USER_PASSWORD_RESET_WEB_PAGE_H
#define USER_PASSWORD_RESET_WEB_PAGE_H

#include "core/containers/vector.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"

#include "user_web_page.h"

class WebServerRequest;
class FormValidator;
class User;

class UserPasswordResetWebPage : public UserWebPage {
	GDCLASS(UserPasswordResetWebPage, UserWebPage);

public:
	virtual void handle_password_reset_request(Ref<User> &user, Ref<WebServerRequest> request);

	UserPasswordResetWebPage();
	~UserPasswordResetWebPage();

protected:
	static void _bind_methods();
};

#endif
