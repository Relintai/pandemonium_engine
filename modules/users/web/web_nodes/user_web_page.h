#ifndef USER_WEB_PAGE_H
#define USER_WEB_PAGE_H

/*************************************************************************/
/*  user_web_page.h                                                      */
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

#include "core/containers/vector.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"

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

	RenderType get_logged_out_render_type();
	void set_logged_out_render_type(const RenderType val);

	String get_logged_out_redirect_url();
	void set_logged_out_redirect_url(const String &val);

	int get_logged_out_error_code();
	void set_logged_out_error_code(const int val);

	RenderType get_logged_in_render_type();
	void set_logged_in_render_type(const RenderType val);

	String get_logged_in_redirect_url();
	void set_logged_in_redirect_url(const String &val);

	int get_logged_in_error_code();
	void set_logged_in_error_code(const int val);

	bool get_should_render_menu();
	void set_should_render_menu(const bool val);

	void _handle_request(Ref<WebServerRequest> request);
	void _render_index(Ref<WebServerRequest> request);

	void handle_not_logged_in(Ref<WebServerRequest> request);
	virtual void _handle_not_logged_in(Ref<WebServerRequest> request);

	void handle_logged_in(Ref<WebServerRequest> request);
	virtual void _handle_logged_in(Ref<WebServerRequest> request);

	UserWebPage();
	~UserWebPage();

protected:
#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
#endif // DISABLE_DEPRECATED

	static void _bind_methods();

	bool _should_render_menu;

	RenderType _logged_out_render_type;
	String _logged_out_redirect_url;
	int _logged_out_error_code;

	RenderType _logged_in_render_type;
	String _logged_in_redirect_url;
	int _logged_in_error_code;
};

VARIANT_ENUM_CAST(UserWebPage::RenderType);

#endif
