#include "user_manager.h"

#include "core/engine.h"

#include "../singleton/user_db.h"
#include "../users/user.h"

Ref<User> UserManager::get_user(const int id) {
	return call("_get_user", id);
}
Ref<User> UserManager::get_user_name(const String &user_name) {
	return call("_get_user_name", user_name);
}
void UserManager::save_user(const Ref<User> &user) {
	call("_save_user", user);
}
Ref<User> UserManager::create_user() {
	return call("_create_user");
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
void UserManager::_save_user(Ref<User> user) {
}
Ref<User> UserManager::_create_user() {
	Ref<User> u;
	u.instance();

	return u;
}
bool UserManager::_is_username_taken(const String &user_name) {
	return false;
}
bool UserManager::_is_email_taken(const String &email) {
	return false;
}

Vector<Ref<User>> UserManager::get_all() {
	return Vector<Ref<User>>();
}

UserManager::UserManager() {
}

UserManager::~UserManager() {
}

void UserManager::_notification(int p_what) {
	if (p_what == NOTIFICATION_POST_ENTER_TREE) {
		if (!Engine::get_singleton()->is_editor_hint()) {
			UserDB::get_singleton()->set_user_manager(this);
		}
	}
}

void UserManager::_bind_methods() {
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "User"), "_get_user", PropertyInfo(Variant::INT, "id")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "User"), "_get_user_name", PropertyInfo(Variant::STRING, "user_name")));
	BIND_VMETHOD(MethodInfo("_save_user", PropertyInfo(Variant::OBJECT, "user", PROPERTY_HINT_RESOURCE_TYPE, "User")));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "User"), "_create_user"));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_is_username_taken", PropertyInfo(Variant::STRING, "user_name")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_is_email_taken", PropertyInfo(Variant::STRING, "email")));

	ClassDB::bind_method(D_METHOD("get_user", "id"), &UserManager::get_user);
	ClassDB::bind_method(D_METHOD("get_user_name", "user_name"), &UserManager::get_user_name);
	ClassDB::bind_method(D_METHOD("save_user", "user"), &UserManager::save_user);

	ClassDB::bind_method(D_METHOD("create_user"), &UserManager::create_user);

	ClassDB::bind_method(D_METHOD("is_username_taken", "user_name"), &UserManager::is_username_taken);
	ClassDB::bind_method(D_METHOD("is_email_taken", "email"), &UserManager::is_email_taken);

	ClassDB::bind_method(D_METHOD("_get_user", "id"), &UserManager::_get_user);
	ClassDB::bind_method(D_METHOD("_get_user_name", "user_name"), &UserManager::_get_user_name);
	ClassDB::bind_method(D_METHOD("_save_user", "user"), &UserManager::_save_user);

	ClassDB::bind_method(D_METHOD("_create_user"), &UserManager::_create_user);

	ClassDB::bind_method(D_METHOD("_is_username_taken", "user_name"), &UserManager::_is_username_taken);
	ClassDB::bind_method(D_METHOD("_is_email_taken", "email"), &UserManager::_is_email_taken);
}
