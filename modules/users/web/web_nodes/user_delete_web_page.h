#ifndef USER_DELETE_WEB_PAGE_H
#define USER_DELETE_WEB_PAGE_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "user_web_page.h"

class WebServerRequest;
class FormValidator;
class User;

class UserDeleteWebPage : public UserWebPage {
	GDCLASS(UserDeleteWebPage, UserWebPage);

public:
	virtual void handle_delete_request(Ref<User> &user, Ref<WebServerRequest> request);

	UserDeleteWebPage();
	~UserDeleteWebPage();

protected:
};

#endif
