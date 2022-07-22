#ifndef USER_WEB_PAGE_H
#define USER_WEB_PAGE_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "modules/web/http/web_node.h"

class WebServerRequest;

class UserWebPage : public WebNode {
	GDCLASS(UserWebPage, WebNode);

public:
	UserWebPage();
	~UserWebPage();

protected:
};

#endif
