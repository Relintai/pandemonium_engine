#ifndef USER_MANAGER_FILE_H
#define USER_MANAGER_FILE_H

/*************************************************************************/
/*  user_manager_file.h                                                  */
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

class UserManagerFile : public UserManager {
	GDCLASS(UserManagerFile, UserManager);

public:
	String get_save_folder_path();
	void set_save_folder_path(const String &val);

	Ref<User> _get_user(const int id);
	Ref<User> _get_user_name(const String &user);
	Ref<User> _get_user_email(const String &user_email);

	void _save_user(Ref<User> user);

	Ref<User> _create_user();

	bool _is_username_taken(const String &user_name);
	bool _is_email_taken(const String &email);

	Array _get_all_users();

	UserManagerFile();
	~UserManagerFile();

protected:
	void load();
	void save();

	void _on_user_changed();

	void _notification(int p_what);

	static void _bind_methods();

	Vector<Ref<User>> _users;
	RWLock _rw_lock;
	bool _save_queued;

	String _save_folder_path;
};

#endif
