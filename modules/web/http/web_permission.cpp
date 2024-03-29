/*************************************************************************/
/*  web_permission.cpp                                                   */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "web_permission.h"

#include "http_server_enums.h"
#include "web_server_request.h"

bool WebPermission::activate(const Ref<WebServerRequest> &request) {
	return call("_activate", request);
}
int WebPermission::get_permissions(const Ref<WebServerRequest> &request) {
	return call("_get_permissions", request);
}
void WebPermission::handle_view_permission_missing(const Ref<WebServerRequest> &request) {
	call("_handle_view_permission_missing", request);
}

bool WebPermission::_activate(Ref<WebServerRequest> request) {
	request->set_active_permission(Ref<WebPermission>(this));
	request->set_permissions(get_permissions(request));

	if (!request->can_view()) {
		handle_view_permission_missing(request);
		return true;
	}

	return false;
}

int WebPermission::_get_permissions(Ref<WebServerRequest> request) {
	return WEB_PERMISSION_ALL;
}

void WebPermission::_handle_view_permission_missing(Ref<WebServerRequest> request) {
	request->send_error(HTTPServerEnums::HTTP_STATUS_CODE_404_NOT_FOUND);
}

WebPermission::WebPermission() {
}

WebPermission::~WebPermission() {
}

void WebPermission::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_activate", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));
	BIND_VMETHOD(MethodInfo("_get_permissions", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));
	BIND_VMETHOD(MethodInfo("_handle_view_permission_missing", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));

	ClassDB::bind_method(D_METHOD("activate", "request"), &WebPermission::activate);
	ClassDB::bind_method(D_METHOD("get_permissions", "request"), &WebPermission::get_permissions);
	ClassDB::bind_method(D_METHOD("handle_view_permission_missing", "request"), &WebPermission::handle_view_permission_missing);

	ClassDB::bind_method(D_METHOD("_activate", "request"), &WebPermission::_activate);
	ClassDB::bind_method(D_METHOD("_get_permissions", "request"), &WebPermission::_get_permissions);
	ClassDB::bind_method(D_METHOD("_handle_view_permission_missing", "request"), &WebPermission::_handle_view_permission_missing);

	BIND_ENUM_CONSTANT(WEB_PERMISSION_VIEW);
	BIND_ENUM_CONSTANT(WEB_PERMISSION_CREATE);
	BIND_ENUM_CONSTANT(WEB_PERMISSION_EDIT);
	BIND_ENUM_CONSTANT(WEB_PERMISSION_DELETE);

	BIND_ENUM_CONSTANT(WEB_PERMISSION_ALL);
	BIND_ENUM_CONSTANT(WEB_PERMISSION_NONE);
}
