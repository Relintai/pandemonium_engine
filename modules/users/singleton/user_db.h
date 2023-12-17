#ifndef USER_DB_H
#define USER_DB_H

/*************************************************************************/
/*  user_db.h                                                            */
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

#include "core/object/object.h"

class UserManager;
class User;

class UserDB : public Object {
	GDCLASS(UserDB, Object);

public:
	//TODO
	//get_user_id
	//get_user_index
	//get_user_count
	//get_users
	//get_users_bind
	//delete_user_id
	//delete_user_index
	//delete_user_name
	//remove_user...
	//clear

	Ref<User> get_user(const int id);
	Ref<User> get_user_name(const String &user_name);
	Ref<User> get_user_email(const String &user_email);

	void save_user(const Ref<User> &user);

	Ref<User> create_user();

	bool is_username_taken(const String &user_name);
	bool is_email_taken(const String &email);

	UserManager *get_user_manager();
	void set_user_manager(UserManager *um);

	static UserDB *get_singleton();

	UserDB();
	~UserDB();

protected:
	static void _bind_methods();

	Node *get_user_manager_bind();

	UserManager *_user_manager;

	static UserDB *_self;

	Vector<Ref<User>> _users;
	RWLock _lock;
};

#endif
