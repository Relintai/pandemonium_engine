#ifndef WEB_PERMISSION_H
#define WEB_PERMISSION_H

#include "core/string.h"

#include "core/reference.h"

class Request;

class WebPermission : public Reference {
	RCPP_OBJECT(WebPermission, Reference);

public:
	enum WebPermissions {
		WEB_PERMISSION_VIEW = 1 << 0,
		WEB_PERMISSION_CREATE = 1 << 1,
		WEB_PERMISSION_EDIT = 1 << 2,
		WEB_PERMISSION_DELETE = 1 << 3,

		WEB_PERMISSION_ALL = WEB_PERMISSION_VIEW | WEB_PERMISSION_CREATE | WEB_PERMISSION_EDIT | WEB_PERMISSION_DELETE,
		WEB_PERMISSION_NONE = 0,
	};

	//like in middlewate returns whether it handled the request or not
	virtual bool activate(Request *request);
	virtual int _get_permissions(Request *request);
	virtual void handle_view_permission_missing(Request *request);

	WebPermission();
	~WebPermission();
};

#endif