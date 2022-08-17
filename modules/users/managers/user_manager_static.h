#ifndef USER_MANAGER_STATIC_H
#define USER_MANAGER_STATIC_H

#include "core/os/rw_lock.h"
#include "core/object/reference.h"
#include "core/ustring.h"
#include "core/containers/vector.h"

#include "user_manager.h"

class User;

class UserManagerStatic : public UserManager {
	GDCLASS(UserManagerStatic, UserManager);

public:
	Ref<User> _get_user(const int id);
	Ref<User> _get_user_name(const String &user);
	void _save_user(Ref<User> user);

	Ref<User> _create_user();

	bool _is_username_taken(const String &user_name);
	bool _is_email_taken(const String &email);

	Vector<Ref<User>> get_all();

	Vector<Variant> get_users();
	void set_users(const Vector<Variant> &users);

	String get_create_user_name_bind();
	void set_create_user_name_bind(const String &val);

	String get_create_user_email_bind();
	void set_create_user_email_bind(const String &val);

	String get_create_user_password_bind();
	void set_create_user_password_bind(const String &val);

	bool get_create_user_bind();
	void set_create_user_bind(const bool val);

	UserManagerStatic();
	~UserManagerStatic();

protected:
	static void _bind_methods();

	Vector<Ref<User>> _users;

	String _create_user_name;
	String _create_user_email;
	String _create_user_password;
};

#endif
