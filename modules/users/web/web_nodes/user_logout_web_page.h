#ifndef USER_LOGOUT_WEB_PAGE_H
#define USER_LOGOUT_WEB_PAGE_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "user_web_page.h"

class WebServerRequest;
class FormValidator;
class User;

class UserLogoutWebPage : public UserWebPage {
	GDCLASS(UserLogoutWebPage, UserWebPage);

public:
	void _render_index(Ref<WebServerRequest> request);

	UserLogoutWebPage();
	~UserLogoutWebPage();

protected:
};

#endif
