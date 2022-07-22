#include "user_db.h"

#include "../managers/user_manager.h"
#include "../users/user.h"

Ref<User> UserDB::get_user(const int id) {
	if (_user_manager) {
		return _user_manager->get_user(id);
	}

	ERR_FAIL_INDEX_V(id, _users.size(), Ref<User>());

	_lock.read_lock();
	Ref<User> u = _users[id];
	_lock.read_unlock();

	return u;
}
Ref<User> UserDB::get_user_name(const String &user_name) {
	if (_user_manager) {
		return _user_manager->get_user_name(user_name);
	}

	_lock.read_lock();

	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_user_name() == user_name) {
				_lock.read_unlock();
				return u;
			}
		}
	}

	_lock.read_unlock();

	return Ref<User>();
}
void UserDB::save_user(const Ref<User> &user) {
	if (_user_manager) {
		_user_manager->save_user(user);
	}
}

Ref<User> UserDB::create_user() {
	if (_user_manager) {
		return _user_manager->create_user();
	}

	Ref<User> user;
	user.instance();

	_lock.write_lock();
	_users.push_back(user);
	_lock.write_unlock();

	return user;
}

bool UserDB::is_username_taken(const String &user_name) {
	if (_user_manager) {
		return _user_manager->is_username_taken(user_name);
	}

	_lock.read_lock();

	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_user_name() == user_name) {
				_lock.read_unlock();
				return true;
			}
		}
	}

	_lock.read_unlock();

	return false;
}
bool UserDB::is_email_taken(const String &email) {
	if (_user_manager) {
		return _user_manager->is_email_taken(email);
	}

	_lock.read_lock();

	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_email() == email) {
				_lock.read_unlock();
				return true;
			}
		}
	}

	_lock.read_unlock();

	return false;
}

UserManager *UserDB::get_user_manager() {
	return _user_manager;
}
void UserDB::set_user_manager(UserManager *um) {
	_user_manager = um;
}
Node *UserDB::get_user_manager_bind() {
	return _user_manager;
}

UserDB *UserDB::get_singleton() {
	return _self;
}

UserDB::UserDB() {
	_self = this;
	_user_manager = nullptr;
}

UserDB::~UserDB() {
	_self = nullptr;
}

void UserDB::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_user", "id"), &UserDB::get_user);
	ClassDB::bind_method(D_METHOD("get_user_name", "user_name"), &UserDB::get_user_name);
	ClassDB::bind_method(D_METHOD("save_user", "user"), &UserDB::save_user);

	ClassDB::bind_method(D_METHOD("create_user"), &UserDB::create_user);

	ClassDB::bind_method(D_METHOD("is_username_taken", "user_name"), &UserDB::is_username_taken);
	ClassDB::bind_method(D_METHOD("is_email_taken", "email"), &UserDB::is_email_taken);

	ClassDB::bind_method(D_METHOD("get_user_manager"), &UserDB::get_user_manager_bind);
}

UserDB *UserDB::_self = nullptr;
