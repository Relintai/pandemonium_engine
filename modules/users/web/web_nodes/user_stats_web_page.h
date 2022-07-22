#ifndef USER_STATS_WEB_PAGE_H
#define USER_STATS_WEB_PAGE_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "modules/web/http/web_node.h"
#include "user_web_page.h"

class WebServerRequest;
class FormValidator;
class User;

class UserStatsWebPage : public UserWebPage {
	GDCLASS(UserStatsWebPage, UserWebPage);

public:
	virtual void handle_request(Ref<User> &user, Ref<WebServerRequest> request);
	virtual void handle_main_page_request(Ref<User> &user, Ref<WebServerRequest> request);

	UserStatsWebPage();
	~UserStatsWebPage();

protected:
	Ref<FormValidator> _login_validator;
	Ref<FormValidator> _registration_validator;
	Ref<FormValidator> _profile_validator;
};

#endif
