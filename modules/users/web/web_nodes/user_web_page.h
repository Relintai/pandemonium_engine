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
	enum RenderType {
		RENDER_TYPE_RENDER = 0,
		RENDER_TYPE_REDIRECT,
		RENDER_TYPE_ERROR,
	};

	//Render type + sopport properties for it, when the user is logged in, and when not logged in

	UserWebPage();
	~UserWebPage();

protected:
};

#endif
