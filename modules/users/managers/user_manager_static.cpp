/*************************************************************************/
/*  user_manager_static.cpp                                              */
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

#include "user_manager_static.h"

#include "../users/user.h"

Ref<User> UserManagerStatic::_get_user(const int id) {
	ERR_FAIL_INDEX_V(id, _users.size(), Ref<User>());

	return _users[id];
}
Ref<User> UserManagerStatic::_get_user_name(const String &user_name) {
	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_user_name() == user_name) {
				return u;
			}
		}
	}

	return Ref<User>();
}
Ref<User> UserManagerStatic::_get_user_email(const String &user_email) {
	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_email() == user_email) {
				return u;
			}
		}
	}

	return Ref<User>();
}

void UserManagerStatic::_save_user(Ref<User> user) {
	//With this class Users are serialized via editor properties, ignore
}
Ref<User> UserManagerStatic::_create_user() {
	Ref<User> u;
	u.instance();

	u->set_user_id(_users.size());

	_users.push_back(u);

	return u;
}
bool UserManagerStatic::_is_username_taken(const String &user_name) {
	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_user_name() == user_name) {
				return true;
			}
		}
	}

	return false;
}
bool UserManagerStatic::_is_email_taken(const String &email) {
	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_email() == email) {
				return true;
			}
		}
	}

	return false;
}

Array UserManagerStatic::_get_all_users() {
	Array ret;

	ret.resize(_users.size());

	for (int i = 0; i < _users.size(); ++i) {
		ret[i] = Variant(_users[i].get_ref_ptr());
	}

	return ret;
}

Vector<Variant> UserManagerStatic::get_users() {
	Vector<Variant> r;
	for (int i = 0; i < _users.size(); i++) {
		r.push_back(_users[i].get_ref_ptr());
	}
	return r;
}
void UserManagerStatic::set_users(const Vector<Variant> &users) {
	_users.clear();
	for (int i = 0; i < users.size(); i++) {
		Ref<User> u = Ref<User>(users.get(i));

		_users.push_back(u);
	}
}

String UserManagerStatic::get_create_user_name_bind() {
	return _create_user_name;
}
void UserManagerStatic::set_create_user_name_bind(const String &val) {
	_create_user_name = val;
}

String UserManagerStatic::get_create_user_email_bind() {
	return _create_user_email;
}
void UserManagerStatic::set_create_user_email_bind(const String &val) {
	_create_user_email = val;
}

String UserManagerStatic::get_create_user_password_bind() {
	return _create_user_password;
}
void UserManagerStatic::set_create_user_password_bind(const String &val) {
	_create_user_password = val;
}

void UserManagerStatic::_editor_create_user_button(const StringName &p_property) {
	Ref<User> u = create_user();

	u->set_user_name(_create_user_name);
	u->set_email(_create_user_email);
	u->create_password(_create_user_password);
	u->save();

	_create_user_password = "";
	_create_user_email = "";
	_create_user_name = "";

	property_list_changed_notify();
}

UserManagerStatic::UserManagerStatic() {
}

UserManagerStatic::~UserManagerStatic() {
}

void UserManagerStatic::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_users"), &UserManagerStatic::get_users);
	ClassDB::bind_method(D_METHOD("set_users", "users"), &UserManagerStatic::set_users);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "users", PROPERTY_HINT_NONE, "23/20:User", PROPERTY_USAGE_DEFAULT, "User"), "set_users", "get_users");

	ClassDB::bind_method(D_METHOD("get_create_user_name"), &UserManagerStatic::get_create_user_name_bind);
	ClassDB::bind_method(D_METHOD("set_create_user_name", "val"), &UserManagerStatic::set_create_user_name_bind);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "create_user_name"), "set_create_user_name", "get_create_user_name");

	ClassDB::bind_method(D_METHOD("get_create_user_email"), &UserManagerStatic::get_create_user_email_bind);
	ClassDB::bind_method(D_METHOD("set_create_user_email", "val"), &UserManagerStatic::set_create_user_email_bind);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "create_user_email"), "set_create_user_email", "get_create_user_email");

	ClassDB::bind_method(D_METHOD("get_create_user_password"), &UserManagerStatic::get_create_user_password_bind);
	ClassDB::bind_method(D_METHOD("set_create_user_password", "val"), &UserManagerStatic::set_create_user_password_bind);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "create_user_password"), "set_create_user_password", "get_create_user_password");

	ClassDB::bind_method(D_METHOD("_editor_create_user_button"), &UserManagerStatic::_editor_create_user_button);
	ADD_PROPERTY(PropertyInfo(Variant::NIL, "create_user", PROPERTY_HINT_BUTTON, "_editor_create_user_button:Add/EditorIcons"), "", "");
}
