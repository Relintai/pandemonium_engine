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
			if (u->get_name_user_input() == user_name) {
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
	//With this class Users are managed via the editor editor
	return Ref<User>();
}
bool UserManagerStatic::_is_username_taken(const String &user_name) {
	for (int i = 0; i < _users.size(); ++i) {
		Ref<User> u = _users[i];

		if (u.is_valid()) {
			if (u->get_name_user_input() == user_name) {
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
			if (u->get_email_user_input() == email) {
				return true;
			}
		}
	}

	return false;
}

Vector<Ref<User>> UserManagerStatic::get_all() {
	return _users;
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

UserManagerStatic::UserManagerStatic() {
}

UserManagerStatic::~UserManagerStatic() {
}

void UserManagerStatic::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_users"), &UserManagerStatic::get_users);
	ClassDB::bind_method(D_METHOD("set_users", "users"), &UserManagerStatic::set_users);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "users", PROPERTY_HINT_NONE, "17/17:User", PROPERTY_USAGE_DEFAULT, "User"), "set_users", "get_users");
}
