/*************************************************************************/
/*  alias_web_page.cpp                                                   */
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

#include "alias_web_page.h"

#include "../../http/http_server_enums.h"
#include "../../http/web_server_request.h"
#include "core/os/file_access.h"

NodePath AliasWebPage::get_alias_path() {
	return _alias_path;
}
void AliasWebPage::set_alias_path(const NodePath &val) {
	_alias_path = val;
}

void AliasWebPage::_handle_request(Ref<WebServerRequest> request) {
	ERR_FAIL_COND(!_alias);
	ERR_FAIL_COND(!ObjectDB::instance_validate(_alias));

	_alias->handle_request(request);
}

AliasWebPage::AliasWebPage() {
	_alias = nullptr;
}

AliasWebPage::~AliasWebPage() {
}

void AliasWebPage::_notification(int p_what) {
	if (p_what == NOTIFICATION_ENTER_TREE) {
		_alias = Object::cast_to<WebNode>(get_node_or_null(_alias_path));
	} else if (p_what == NOTIFICATION_EXIT_TREE) {
		_alias = nullptr;
	}
}

void AliasWebPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_alias_path"), &AliasWebPage::get_alias_path);
	ClassDB::bind_method(D_METHOD("set_alias_path", "val"), &AliasWebPage::set_alias_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "alias_path"), "set_alias_path", "get_alias_path");
}
