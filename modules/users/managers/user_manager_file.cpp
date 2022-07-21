#include "user_manager_file.h"

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/variant.h"

#include "../users/user.h"

String UserManagerFile::get_save_folder_path() {
	return _save_folder_path;
}
void UserManagerFile::set_save_folder_path(const String &val) {
	_save_folder_path = val;
}

Ref<User> UserManagerFile::_get_user(const int id) {
	_rw_lock.read_lock();

	if (id < 0 || id >= _users.size()) {
		_rw_lock.read_unlock();

		ERR_PRINT("INDEX ERROR");

		return Ref<User>();
	}

	Ref<User> u = _users[id];

	_rw_lock.read_unlock();
	return u;
}
Ref<User> UserManagerFile::_get_user_name(const String &user_name) {
	_rw_lock.read_lock();

	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_name_user_input() == user_name) {
				_rw_lock.read_unlock();

				return u;
			}
		}
	}

	_rw_lock.read_unlock();
	return Ref<User>();
}
void UserManagerFile::_save_user(Ref<User> user) {
	_save_queued = true;
}
Ref<User> UserManagerFile::_create_user() {
	_rw_lock.write_lock();

	Ref<User> u;
	u.instance();

	u->connect("chnaged", this, "_on_user_changed");

	_users.push_back(u);

	_rw_lock.write_unlock();

	_save_queued = true;

	return u;
}
bool UserManagerFile::_is_username_taken(const String &user_name) {
	_rw_lock.read_lock();
	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_name_user_input() == user_name) {
				_rw_lock.read_unlock();
				return true;
			}
		}
	}

	_rw_lock.read_unlock();
	return false;
}
bool UserManagerFile::_is_email_taken(const String &email) {
	_rw_lock.read_lock();
	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_email_user_input() == email) {
				_rw_lock.read_unlock();
				return true;
			}
		}
	}

	_rw_lock.read_unlock();
	return false;
}

Vector<Ref<User>> UserManagerFile::get_all() {
	return _users;
}

UserManagerFile::UserManagerFile() {
	set_process_internal(true);
	_save_folder_path = "user://users/";
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

	String file = dir->get_next();

	while (file != "") {
		if (!dir->current_is_dir()) {
			if (!file.ends_with(".tres")) {
				continue;
			}

			String uids = file.get_slice(".", 0);

			if (!uids.is_valid_unsigned_integer()) {
				continue;
			}

			int id = uids.to_int();

			//Overflow
			ERR_CONTINUE(id < 0);

			Ref<User> u = ResourceLoader::load(_save_folder_path.plus_file(file), "User");

			u->connect("chnaged", this, "_on_user_changed");

			if (_users.size() <= id) {
				_users.resize(id + 1);
			}

			_users.write[id] = u;
		}
	}

	dir->list_dir_end();

	_rw_lock.write_unlock();
}

void UserManagerFile::save() {
	String abspath = DirAccess::get_filesystem_abspath_for(_save_folder_path);

	DirAccess *dir = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);

	if (!dir->dir_exists(abspath)) {
		dir->make_dir_recursive(abspath);
	}

	memdelete(dir);

	dir = DirAccess::open(_save_folder_path);

	ERR_FAIL_COND(!dir);

	dir->list_dir_begin();

	String file = dir->get_next();

	while (file != "") {
		if (!dir->current_is_dir()) {
			dir->remove(file);
		}
	}

	dir->list_dir_end();

	memdelete(dir);

	_rw_lock.read_lock();

	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u;
		u->read_lock();
		ResourceSaver::save(_save_folder_path.plus_file(itos(i) + ".tres"), u);
		u->read_unlock();
	}

	_rw_lock.read_unlock();
}

void UserManagerFile::_on_user_changed() {
	_save_queued = true;
}

void UserManagerFile::_notification(int p_what) {
	if (p_what == NOTIFICATION_INTERNAL_PROCESS) {
		if (_save_queued) {
			_save_queued = false;
			save();
		}
	} else if (p_what == NOTIFICATION_POST_ENTER_TREE) {
		load();
	}
}

void UserManagerFile::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_save_folder_path"), &UserManagerFile::get_save_folder_path);
	ClassDB::bind_method(D_METHOD("set_save_folder_path", "val"), &UserManagerFile::set_save_folder_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "save_folder_path"), "set_save_folder_path", "get_save_folder_path");

	ClassDB::bind_method(D_METHOD("_on_user_changed"), &UserManagerFile::_on_user_changed);
}
