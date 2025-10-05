/*************************************************************************/
/*  user_manager_file.cpp                                                */
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

#include "user_manager_file.h"

#include "core/config/engine.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/variant/variant.h"

#include "../users/user.h"

String UserManagerFile::get_save_folder_path() {
	return _save_folder_path;
}
void UserManagerFile::set_save_folder_path(const String &val) {
	_save_folder_path = val;
}

Ref<User> UserManagerFile::_get_user(const int id) {
	// A hashmap could be used

	_rw_lock.read_lock();

	if (id < 0) {
		_rw_lock.read_unlock();
		ERR_PRINT("USER ID ERROR");
		return Ref<User>();
	}

	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_user_id() == id) {
				_rw_lock.read_unlock();

				return u;
			}
		}
	}

	_rw_lock.read_unlock();

	return Ref<User>();
}
Ref<User> UserManagerFile::_get_user_name(const String &p_user_name) {
	// A hashmap could be used

	String user_name = User::string_to_internal_format(p_user_name);

	_rw_lock.read_lock();

	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_user_name_internal() == user_name) {
				_rw_lock.read_unlock();

				return u;
			}
		}
	}

	_rw_lock.read_unlock();

	return Ref<User>();
}
Ref<User> UserManagerFile::_get_user_email(const String &p_user_email) {
	// A hashmap could be used

	String user_email = User::string_to_internal_format(p_user_email);

	_rw_lock.read_lock();

	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_email_internal() == user_email) {
				_rw_lock.read_unlock();

				return u;
			}
		}
	}

	_rw_lock.read_unlock();

	return Ref<User>();
}

void UserManagerFile::_save_user(Ref<User> p_user) {
	ERR_FAIL_COND(!p_user.is_valid());

	if (!DirAccess::exists(_save_folder_path)) {
		DirAccess *diru = DirAccess::create_for_path(_save_folder_path);

		ERR_FAIL_COND(!diru);

		diru->make_dir_recursive(_save_folder_path);
		memdelete(diru);
	}

	if (p_user->get_user_id() == -1) {
		_rw_lock.write_lock();
		p_user->write_lock();

		// check again, because threading
		if (p_user->get_user_id() == -1) {
			p_user->set_user_id(_next_id++);
			_users.push_back(p_user);
		}

		p_user->write_unlock();
		_rw_lock.write_unlock();
	}

	p_user->read_lock();
	int id = p_user->get_user_id();
	String data = p_user->as_json();
	p_user->read_unlock();

	FileAccess *f = FileAccess::open(_save_folder_path.plus_file(itos(id) + ".json"), FileAccess::WRITE);

	ERR_FAIL_COND(!f);

	f->store_string(data);
	f->close();

	memdelete(f);
}
Ref<User> UserManagerFile::_create_user(Ref<User> p_user) {
	if (!p_user.is_valid()) {
		p_user.instance();
		p_user->set_user_id(-1);
	}

	return UserManager::_create_user(p_user);
}
bool UserManagerFile::_is_username_taken(const String &p_user_name) {
	// A hashmap could be used

	String user_name = User::string_to_internal_format(p_user_name);

	_rw_lock.read_lock();

	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_user_name_internal() == user_name) {
				_rw_lock.read_unlock();
				return true;
			}
		}
	}

	_rw_lock.read_unlock();

	return false;
}
bool UserManagerFile::_is_email_taken(const String &p_email) {
	// A hashmap could be used

	String email = User::string_to_internal_format(p_email);

	_rw_lock.read_lock();

	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_email_internal() == email) {
				_rw_lock.read_unlock();
				return true;
			}
		}
	}

	_rw_lock.read_unlock();

	return false;
}

Array UserManagerFile::_get_all_users() {
	Array ret;

	_rw_lock.read_lock();

	ret.resize(_users.size());

	for (int i = 0; i < _users.size(); ++i) {
		ret[i] = _users[i];
	}

	_rw_lock.read_unlock();

	return ret;
}

UserManagerFile::UserManagerFile() {
	_save_folder_path = "user://users/";
	_next_id = 1;
}

UserManagerFile::~UserManagerFile() {
}

void UserManagerFile::load() {
	DirAccess *dir = DirAccess::open(_save_folder_path);

	if (!dir) {
		return;
	}

	dir->list_dir_begin();
	_rw_lock.write_lock();

	_users.clear();
	_next_id = 1;

	String file = dir->get_next();

	while (file != "") {
		if (!dir->current_is_dir()) {
			if (!file.ends_with(".json")) {
				file = dir->get_next();
				continue;
			}

			FileAccess *f = FileAccess::open(_save_folder_path.plus_file(file), FileAccess::READ);

			ERR_CONTINUE(!f);

			String data = f->get_as_utf8_string();
			f->close();
			memdelete(f);

			Ref<User> user = create_user();
			user->from_json(data);

			int id = user->get_user_id();

			if (id >= _next_id) {
				_next_id = id + 1;
			}

			_users.push_back(user);
		}

		file = dir->get_next();
	}

	dir->list_dir_end();

	memdelete(dir);

	_rw_lock.write_unlock();
}

void UserManagerFile::save() {
	if (!DirAccess::exists(_save_folder_path)) {
		DirAccess *diru = DirAccess::create_for_path(_save_folder_path);

		ERR_FAIL_COND(!diru);

		diru->make_dir_recursive(_save_folder_path);
		memdelete(diru);
	}

	_rw_lock.read_lock();

	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			FileAccess *f = FileAccess::open(_save_folder_path.plus_file(itos(i) + ".json"), FileAccess::WRITE);

			ERR_CONTINUE(!f);

			u->read_lock();
			String data = u->as_json();
			u->read_unlock();

			f->store_string(data);
			f->close();
			memdelete(f);
		}
	}

	_rw_lock.read_unlock();
}

void UserManagerFile::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (Engine::get_singleton()->is_editor_hint()) {
				return;
			}

			load();
		} break;
		case NOTIFICATION_EXIT_TREE: {
		} break;
			if (Engine::get_singleton()->is_editor_hint()) {
				return;
			}

			save();
		default:
			break;
	}
}

void UserManagerFile::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_save_folder_path"), &UserManagerFile::get_save_folder_path);
	ClassDB::bind_method(D_METHOD("set_save_folder_path", "val"), &UserManagerFile::set_save_folder_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "save_folder_path"), "set_save_folder_path", "get_save_folder_path");
}
