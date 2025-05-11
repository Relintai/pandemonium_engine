/*************************************************************************/
/*  user_db.cpp                                                          */
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

#include "user_db.h"

#include "../managers/user_manager.h"
#include "../users/user.h"

Ref<User> UserDB::get_user(const int id) {
	ERR_FAIL_COND_V(!_user_manager, Ref<User>());

	return _user_manager->get_user(id);
}
Ref<User> UserDB::get_user_name(const String &user_name) {
	ERR_FAIL_COND_V(!_user_manager, Ref<User>());

	return _user_manager->get_user_name(user_name);
}
Ref<User> UserDB::get_user_email(const String &user_email) {
	ERR_FAIL_COND_V(!_user_manager, Ref<User>());

	return _user_manager->get_user_email(user_email);
}
void UserDB::save_user(const Ref<User> &user) {
	ERR_FAIL_COND(!_user_manager);

	_user_manager->save_user(user);
}

Ref<User> UserDB::create_user() {
	ERR_FAIL_COND_V(!_user_manager, Ref<User>());

	return _user_manager->create_user();
}

bool UserDB::is_username_taken(const String &user_name) {
	ERR_FAIL_COND_V(!_user_manager, false);

	return _user_manager->is_username_taken(user_name);
}
bool UserDB::is_email_taken(const String &email) {
	ERR_FAIL_COND_V(!_user_manager, false);

	return _user_manager->is_email_taken(email);
}

Array UserDB::get_all_users() {
	ERR_FAIL_COND_V(!_user_manager, Array());

	return _user_manager->get_all_users();
}

UserManager *UserDB::get_user_manager() {
	return _user_manager;
}
void UserDB::set_user_manager(UserManager *um) {
	_user_manager = um;
}
void UserDB::unset_user_manager(UserManager *um) {
	if (_user_manager == um) {
		_user_manager = NULL;
	}
}
Node *UserDB::get_user_manager_bind() {
	return _user_manager;
}

UserDB *UserDB::get_singleton() {
	return _self;
}

UserDB::UserDB() {
	_self = this;
	_user_manager = NULL;
}

UserDB::~UserDB() {
	_self = NULL;
}

void UserDB::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_user", "id"), &UserDB::get_user);
	ClassDB::bind_method(D_METHOD("get_user_name", "user_name"), &UserDB::get_user_name);
	ClassDB::bind_method(D_METHOD("get_user_email", "user_email"), &UserDB::get_user_email);

	ClassDB::bind_method(D_METHOD("save_user", "user"), &UserDB::save_user);

	ClassDB::bind_method(D_METHOD("create_user"), &UserDB::create_user);

	ClassDB::bind_method(D_METHOD("is_username_taken", "user_name"), &UserDB::is_username_taken);
	ClassDB::bind_method(D_METHOD("is_email_taken", "email"), &UserDB::is_email_taken);

	ClassDB::bind_method(D_METHOD("get_all_users"), &UserDB::get_all_users);

	ClassDB::bind_method(D_METHOD("get_user_manager"), &UserDB::get_user_manager_bind);
}

UserDB *UserDB::_self = nullptr;
