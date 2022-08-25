#ifndef WEB_PERMISSION_H
#define WEB_PERMISSION_H

#include "core/string/ustring.h"
#include "core/variant/variant.h"

#include "core/object/resource.h"

class WebServerRequest;

class WebPermission : public Resource {
	GDCLASS(WebPermission, Resource);

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
	bool activate(const Ref<WebServerRequest> &request);
	int get_permissions(const Ref<WebServerRequest> &request);
	void handle_view_permission_missing(const Ref<WebServerRequest> &request);

	virtual bool _activate(Ref<WebServerRequest> request);
	virtual int _get_permissions(Ref<WebServerRequest> request);
	virtual void _handle_view_permission_missing(Ref<WebServerRequest> request);

	WebPermission();
	~WebPermission();

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(WebPermission::WebPermissions);

#endif
