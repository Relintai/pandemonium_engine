#include "user.h"
#include "core/class_db.h"

String User::get_name_user_input() const {
	return _name_user_input;
}
void User::set_name_user_input(const String &val) {
	_name_user_input = val;
}

String User::get_email_user_input() const {
	return _email_user_input;
}
void User::set_email_user_input(const String &val) {
	_email_user_input = val;
}

int User::get_rank() const {
	return _rank;
}
void User::set_rank(const int &val) {
	_rank = val;
}

String User::get_pre_salt() const {
	return _pre_salt;
}
void User::set_pre_salt(const String &val) {
	_pre_salt = val;
}

String User::get_post_salt() const {
	return _post_salt;
}
void User::set_post_salt(const String &val) {
	_post_salt = val;
}

String User::get_password_hash() const {
	return _password_hash;
}
void User::set_password_hash(const String &val) {
	_password_hash = val;
}

bool User::get_banned() const {
	return _banned;
}
void User::set_banned(const bool &val) {
	_banned = val;
}

String User::get_password_reset_token() const {
	return _password_reset_token;
}
void User::set_password_reset_token(const String &val) {
	_password_reset_token = val;
}

bool User::get_locked() const {
	return _locked;
}
void User::set_locked(const bool &val) {
	_locked = val;
}

bool User::check_password(const String &p_password) {
	return call("_check_password", p_password);
}
void User::create_password(const String &p_password) {
	call("_create_password", p_password);
}
String User::hash_password(const String &p_password) {
	return call("_hash_password", p_password);
}

bool User::_check_password(const String &p_password) {
	return hash_password(p_password) == get_password_hash();
}
void User::_create_password(const String &p_password) {
	// todo improve a bit
	set_pre_salt(hash_password(get_name_user_input() + get_email_user_input()));
	set_post_salt(hash_password(get_email_user_input() + get_name_user_input()));

	set_password_hash(hash_password(p_password));
}
String User::_hash_password(const String &p_password) {
	String p = get_pre_salt() + p_password + get_post_salt();
	return p.sha256_text();
}

User::User() {
	_rank = 0;
	_banned = false;
	_locked = false;
}

User::~User() {
}

void User::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_name_user_input"), &User::get_name_user_input);
	ClassDB::bind_method(D_METHOD("set_name_user_input", "val"), &User::set_name_user_input);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name_user_input"), "set_name_user_input", "get_name_user_input");

	ClassDB::bind_method(D_METHOD("get_email_user_input"), &User::get_email_user_input);
	ClassDB::bind_method(D_METHOD("set_email_user_input", "val"), &User::set_email_user_input);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "email_user_input"), "set_email_user_input", "get_email_user_input");

	ClassDB::bind_method(D_METHOD("get_rank"), &User::get_rank);
	ClassDB::bind_method(D_METHOD("set_rank", "val"), &User::set_rank);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rank"), "set_rank", "get_rank");

	ClassDB::bind_method(D_METHOD("get_pre_salt"), &User::get_pre_salt);
	ClassDB::bind_method(D_METHOD("set_pre_salt", "val"), &User::set_pre_salt);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "pre_salt"), "set_pre_salt", "get_pre_salt");

	ClassDB::bind_method(D_METHOD("get_post_salt"), &User::get_post_salt);
	ClassDB::bind_method(D_METHOD("set_post_salt", "val"), &User::set_post_salt);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "post_salt"), "set_post_salt", "get_post_salt");

	ClassDB::bind_method(D_METHOD("get_password_hash"), &User::get_password_hash);
	ClassDB::bind_method(D_METHOD("set_password_hash", "val"), &User::set_password_hash);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "password_hash"), "set_password_hash", "get_password_hash");

	ClassDB::bind_method(D_METHOD("get_banned"), &User::get_banned);
	ClassDB::bind_method(D_METHOD("set_banned", "val"), &User::set_banned);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "banned"), "set_banned", "get_banned");

	ClassDB::bind_method(D_METHOD("get_password_reset_token"), &User::get_password_reset_token);
	ClassDB::bind_method(D_METHOD("set_password_reset_token", "val"), &User::set_password_reset_token);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "password_reset_token"), "set_password_reset_token", "get_password_reset_token");

	ClassDB::bind_method(D_METHOD("get_locked"), &User::get_locked);
	ClassDB::bind_method(D_METHOD("set_locked", "val"), &User::set_locked);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "locked"), "set_locked", "get_locked");

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_check_password", PropertyInfo(Variant::STRING, "password")));
	BIND_VMETHOD(MethodInfo("_create_password", PropertyInfo(Variant::STRING, "password")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::STRING, "ret"), "_hash_password", PropertyInfo(Variant::STRING, "password")));

	ClassDB::bind_method(D_METHOD("check_password", "password"), &User::check_password);
	ClassDB::bind_method(D_METHOD("create_password", "password"), &User::create_password);
	ClassDB::bind_method(D_METHOD("hash_password", "password"), &User::hash_password);

	ClassDB::bind_method(D_METHOD("_check_password", "password"), &User::_check_password);
	ClassDB::bind_method(D_METHOD("_create_password", "password"), &User::_create_password);
	ClassDB::bind_method(D_METHOD("_hash_password", "password"), &User::_hash_password);

	BIND_ENUM_CONSTANT(PERMISSION_CREATE);
	BIND_ENUM_CONSTANT(PERMISSION_READ);
	BIND_ENUM_CONSTANT(PERMISSION_UPDATE);
	BIND_ENUM_CONSTANT(PERMISSION_DELETE);
	BIND_ENUM_CONSTANT(PERMISSION_ALL);
	BIND_ENUM_CONSTANT(PERMISSION_NONE);
}
