/*************************************************************************/
/*  redirect_web_page.cpp                                                */
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

#include "redirect_web_page.h"

#include "../../http/http_server_enums.h"
#include "../../http/web_server_request.h"
#include "core/os/file_access.h"

String RedirectWebPage::get_redirect_path() {
	return _redirect_path;
}
void RedirectWebPage::set_redirect_path(const String &val) {
	_redirect_path = val;
}

int RedirectWebPage::get_code() {
	return _code;
}
void RedirectWebPage::set_code(const int val) {
	_code = val;
}

void RedirectWebPage::_handle_request(Ref<WebServerRequest> request) {
	request->send_redirect(_redirect_path, static_cast<HTTPServerEnums::HTTPStatusCode>(_code));
}

RedirectWebPage::RedirectWebPage() {
	_code = 302;
}

RedirectWebPage::~RedirectWebPage() {
}

void RedirectWebPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_redirect_path"), &RedirectWebPage::get_redirect_path);
	ClassDB::bind_method(D_METHOD("set_redirect_path", "val"), &RedirectWebPage::set_redirect_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "redirect_path"), "set_redirect_path", "get_redirect_path");

	ClassDB::bind_method(D_METHOD("get_code"), &RedirectWebPage::get_code);
	ClassDB::bind_method(D_METHOD("set_code", "val"), &RedirectWebPage::set_code);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "code"), "set_code", "get_code");
}
