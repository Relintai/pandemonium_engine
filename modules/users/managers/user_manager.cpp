/*************************************************************************/
/*  user_manager.cpp                                                     */
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

#include "user_manager.h"

#include "core/config/engine.h"

#include "../singleton/user_db.h"
#include "../users/user.h"

bool UserManager::get_register_as_global() const {
	return _register_as_global;
}
void UserManager::set_register_as_global(const bool p_global) {
	_register_as_global = p_global;
}

Ref<User> UserManager::get_user(const int id) {
	return call("_get_user", id);
}
Ref<User> UserManager::get_user_name(const String &user_name) {
	return call("_get_user_name", user_name);
}
Ref<User> UserManager::get_user_email(const String &user_email) {
	return call("_get_user_email", user_email);
}
void UserManager::save_user(const Ref<User> &user) {
	call("_save_user", user);
}
Ref<User> UserManager::create_user() {
	return call("_create_user", Ref<User>());
}
bool UserManager::is_username_taken(const String &user_name) {
	return call("_is_username_taken", user_name);
}
bool UserManager::is_email_taken(const String &email_input) {
	return call("_is_email_taken", email_input);
}

Ref<User> UserManager::_get_user(const int id) {
	return Ref<User>();
}
Ref<User> UserManager::_get_user_name(const String &user_name) {
	return Ref<User>();
}
Ref<User> UserManager::_get_user_email(const String &user_email) {
	return Ref<User>();
}
void UserManager::_save_user(Ref<User> user) {
}
Ref<User> UserManager::_create_user(Ref<User> p_user) {
	if (!p_user.is_valid()) {
		p_user.instance();
	}

	p_user->set_owner_user_manager(this);

	return p_user;
}
bool UserManager::_is_username_taken(const String &user_name) {
	return false;
}
bool UserManager::_is_email_taken(const String &email) {
	return false;
}

Vector<Ref<User>> UserManager::get_all_as_vector() {
	Vector<Ref<User>> ret;

	Array res = call("_get_all_users");

	ret.resize(res.size());

	for (int i = 0; i < res.size(); ++i) {
		ret.write[i] = Ref<User>(res.get(i));
	}

	return ret;
}

Array UserManager::get_all_users() {
	return call("_get_all_users");
}

Array UserManager::_get_all_users() {
	return Array();
}

UserManager::UserManager() {
	_register_as_global = true;
}

UserManager::~UserManager() {
}

void UserManager::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POST_ENTER_TREE: {
			if (_register_as_global && !Engine::get_singleton()->is_editor_hint()) {
				UserDB::get_singleton()->set_user_manager(this);
			}
		} break;
		case NOTIFICATION_EXIT_TREE: {
			if (_register_as_global && !Engine::get_singleton()->is_editor_hint()) {
				UserDB::get_singleton()->unset_user_manager(this);
			}
		} break;
		default:
			break;
	}
}

void UserManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_register_as_global"), &UserManager::get_register_as_global);
	ClassDB::bind_method(D_METHOD("set_register_as_global", "global"), &UserManager::set_register_as_global);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "register_as_global"), "set_register_as_global", "get_register_as_global");

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "User"), "_get_user", PropertyInfo(Variant::INT, "id")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "User"), "_get_user_name", PropertyInfo(Variant::STRING, "user_name")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "User"), "_get_user_email", PropertyInfo(Variant::STRING, "user_email")));
	BIND_VMETHOD(MethodInfo("_save_user", PropertyInfo(Variant::OBJECT, "user", PROPERTY_HINT_RESOURCE_TYPE, "User")));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "User"), "_create_user", PropertyInfo(Variant::OBJECT, "user", PROPERTY_HINT_RESOURCE_TYPE, "User")));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_is_username_taken", PropertyInfo(Variant::STRING, "user_name")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_is_email_taken", PropertyInfo(Variant::STRING, "email")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::ARRAY, "ret"), "_get_all_users"));

	ClassDB::bind_method(D_METHOD("get_user", "id"), &UserManager::get_user);
	ClassDB::bind_method(D_METHOD("get_user_name", "user_name"), &UserManager::get_user_name);
	ClassDB::bind_method(D_METHOD("get_user_email", "user_email"), &UserManager::get_user_email);

	ClassDB::bind_method(D_METHOD("save_user", "user"), &UserManager::save_user);

	ClassDB::bind_method(D_METHOD("create_user"), &UserManager::create_user);

	ClassDB::bind_method(D_METHOD("is_username_taken", "user_name"), &UserManager::is_username_taken);
	ClassDB::bind_method(D_METHOD("is_email_taken", "email"), &UserManager::is_email_taken);

	ClassDB::bind_method(D_METHOD("_get_user", "id"), &UserManager::_get_user);
	ClassDB::bind_method(D_METHOD("_get_user_name", "user_name"), &UserManager::_get_user_name);
	ClassDB::bind_method(D_METHOD("_get_user_email", "user_email"), &UserManager::_get_user_email);
	ClassDB::bind_method(D_METHOD("_save_user", "user"), &UserManager::_save_user);

	ClassDB::bind_method(D_METHOD("_create_user", "user"), &UserManager::_create_user);

	ClassDB::bind_method(D_METHOD("_is_username_taken", "user_name"), &UserManager::_is_username_taken);
	ClassDB::bind_method(D_METHOD("_is_email_taken", "email"), &UserManager::_is_email_taken);

	ClassDB::bind_method(D_METHOD("get_all_users"), &UserManager::get_all_users);
	ClassDB::bind_method(D_METHOD("_get_all_users"), &UserManager::_get_all_users);
}
