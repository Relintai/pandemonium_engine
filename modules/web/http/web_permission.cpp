
#include "web_permission.h"

#include "request.h"

bool WebPermission::activate(Request *request) {
	request->active_permission.reference_ptr(this);
	request->permissions = _get_permissions(request);

	if (!request->can_view()) {
		handle_view_permission_missing(request);
		return true;
	}

	return false;
}

int WebPermission::_get_permissions(Request *request) {
	return WEB_PERMISSION_ALL;
}

void WebPermission::handle_view_permission_missing(Request *request) {
	request->send_error(HTTP_STATUS_CODE_404_NOT_FOUND);
}

WebPermission::WebPermission() :
		Reference() {
}

WebPermission::~WebPermission() {
}