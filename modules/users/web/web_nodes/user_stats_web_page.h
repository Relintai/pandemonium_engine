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
	UserStatsWebPage();
	~UserStatsWebPage();

protected:
};

#endif
