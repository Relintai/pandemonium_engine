#include "user_manager.h"

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
bool UserManager::check_password(const Ref<User> &user, const String &p_password) {
	return call("_check_password", user, p_password);
}
void UserManager::create_password(const Ref<User> &user, const String &p_password) {
	call("_create_password", user, p_password);
}
String UserManager::hash_password(const Ref<User> &user, const String &p_password) {
	return call("_hash_password", user, p_password);
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
bool UserManager::_check_password(const Ref<User> &user, const String &p_password) {
	return hash_password(user, p_password) == user->get_password_hash();
}
void UserManager::_create_password(Ref<User> user, const String &p_password) {
	if (!user.is_valid()) {
		printf("Error UserManager::create_password !user.is_valid()!\n");
		return;
	}

	// todo improve a bit
	user->set_pre_salt(hash_password(user, user->get_name_user_input() + user->get_email_user_input()));
	user->set_post_salt(hash_password(user, user->get_email_user_input() + user->get_name_user_input()));

	user->set_password_hash(hash_password(user, p_password));
}
String UserManager::_hash_password(const Ref<User> &user, const String &p_password) {
	ERR_FAIL_COND_V(!user.is_valid(), "");

	String p = user->get_pre_salt() + p_password + user->get_post_salt();
	return p.sha256_text();
}

Vector<Ref<User>> UserManager::get_all() {
	return Vector<Ref<User>>();
}

UserManager::UserManager() {
}

UserManager::~UserManager() {
}

void UserManager::_bind_methods() {
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "User"), "_get_user", PropertyInfo(Variant::INT, "id")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "User"), "_get_user_name", PropertyInfo(Variant::STRING, "user_name")));
	BIND_VMETHOD(MethodInfo("_save_user", PropertyInfo(Variant::OBJECT, "user", PROPERTY_HINT_RESOURCE_TYPE, "User")));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "ret", PROPERTY_HINT_RESOURCE_TYPE, "User"), "_create_user"));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_is_username_taken", PropertyInfo(Variant::STRING, "user_name")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_is_email_taken", PropertyInfo(Variant::STRING, "email")));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_check_password", PropertyInfo(Variant::OBJECT, "user", PROPERTY_HINT_RESOURCE_TYPE, "User"), PropertyInfo(Variant::STRING, "password")));
	BIND_VMETHOD(MethodInfo("_create_password", PropertyInfo(Variant::OBJECT, "user", PROPERTY_HINT_RESOURCE_TYPE, "User"), PropertyInfo(Variant::STRING, "password")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::STRING, "ret"), "_hash_password", PropertyInfo(Variant::OBJECT, "user", PROPERTY_HINT_RESOURCE_TYPE, "User"), PropertyInfo(Variant::STRING, "password")));

	ClassDB::bind_method(D_METHOD("get_user", "id"), &UserManager::get_user);
	ClassDB::bind_method(D_METHOD("get_user_name", "user_name"), &UserManager::get_user_name);
	ClassDB::bind_method(D_METHOD("save_user", "user"), &UserManager::save_user);

	ClassDB::bind_method(D_METHOD("create_user"), &UserManager::create_user);

	ClassDB::bind_method(D_METHOD("is_username_taken", "user_name"), &UserManager::is_username_taken);
	ClassDB::bind_method(D_METHOD("is_email_taken", "email"), &UserManager::is_email_taken);

	ClassDB::bind_method(D_METHOD("check_password", "user", "password"), &UserManager::check_password);
	ClassDB::bind_method(D_METHOD("create_password", "user", "password"), &UserManager::create_password);
	ClassDB::bind_method(D_METHOD("hash_password", "user", "password"), &UserManager::hash_password);

	ClassDB::bind_method(D_METHOD("_get_user", "id"), &UserManager::_get_user);
	ClassDB::bind_method(D_METHOD("_get_user_name", "user_name"), &UserManager::_get_user_name);
	ClassDB::bind_method(D_METHOD("_save_user", "user"), &UserManager::_save_user);

	ClassDB::bind_method(D_METHOD("_create_user"), &UserManager::_create_user);

	ClassDB::bind_method(D_METHOD("_is_username_taken", "user_name"), &UserManager::_is_username_taken);
	ClassDB::bind_method(D_METHOD("_is_email_taken", "email"), &UserManager::_is_email_taken);

	ClassDB::bind_method(D_METHOD("_check_password", "user", "password"), &UserManager::_check_password);
	ClassDB::bind_method(D_METHOD("_create_password", "user", "password"), &UserManager::_create_password);
	ClassDB::bind_method(D_METHOD("_hash_password", "user", "password"), &UserManager::_hash_password);
}
