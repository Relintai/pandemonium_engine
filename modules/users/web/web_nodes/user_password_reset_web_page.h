#ifndef USER_PASSWORD_RESET_WEB_PAGE_H
#define USER_PASSWORD_RESET_WEB_PAGE_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

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
};

#endif
