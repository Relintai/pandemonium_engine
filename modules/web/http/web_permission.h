#ifndef WEB_PERMISSION_H
#define WEB_PERMISSION_H

/*************************************************************************/
/*  web_permission.h                                                     */
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
