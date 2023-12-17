#ifndef USER_REGISTER_WEB_PAGE_H
#define USER_REGISTER_WEB_PAGE_H

/*************************************************************************/
/*  user_register_web_page.h                                             */
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

#include "user_web_page.h"

class WebServerRequest;
class FormValidator;
class User;

class UserRegisterWebPage : public UserWebPage {
	GDCLASS(UserRegisterWebPage, UserWebPage);

public:
	String get_redirect_on_success_url();
	void set_redirect_on_success_url(const String &val);

	struct RegisterRequestData {
		String error_str;
		String uname_val;
		String email_val;
		String pass_val;
		String pass_check_val;
	};

	void _render_index(Ref<WebServerRequest> request);
	virtual void render_register_request_default(Ref<WebServerRequest> request, RegisterRequestData *data);
	virtual void render_register_success(Ref<WebServerRequest> request);

	UserRegisterWebPage();
	~UserRegisterWebPage();

protected:
	static void _bind_methods();

	Ref<FormValidator> _registration_validator;
	Ref<FormValidator> _profile_validator;

	String _redirect_on_success_url;
};

#endif
