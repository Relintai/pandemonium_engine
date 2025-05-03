#ifndef USER_MANAGER_STATIC_H
#define USER_MANAGER_STATIC_H

/*************************************************************************/
/*  user_manager_static.h                                                */
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
#include "core/os/rw_lock.h"
#include "core/string/ustring.h"

#include "user_manager.h"

class User;

class UserManagerStatic : public UserManager {
	GDCLASS(UserManagerStatic, UserManager);

public:
	Ref<User> _get_user(const int id);
	Ref<User> _get_user_name(const String &user);
	Ref<User> _get_user_email(const String &user_email);

	void _save_user(Ref<User> user);

	Ref<User> _create_user(Ref<User> p_user);

	bool _is_username_taken(const String &user_name);
	bool _is_email_taken(const String &email);

	Array _get_all_users();

	Vector<Variant> get_users();
	void set_users(const Vector<Variant> &users);

	String get_create_user_name_bind();
	void set_create_user_name_bind(const String &val);

	String get_create_user_email_bind();
	void set_create_user_email_bind(const String &val);

	String get_create_user_password_bind();
	void set_create_user_password_bind(const String &val);

	UserManagerStatic();
	~UserManagerStatic();

protected:
	void _editor_create_user_button(const StringName &p_property);

	static void _bind_methods();

	Vector<Ref<User>> _users;

	String _create_user_name;
	String _create_user_email;
	String _create_user_password;
};

#endif
