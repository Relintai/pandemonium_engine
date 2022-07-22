#ifndef USER_MANAGER_DB_H
#define USER_MANAGER_DB_H

#include "core/os/rw_lock.h"
#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "user_manager.h"

class User;

class UserManagerDB : public UserManager {
	GDCLASS(UserManagerDB, UserManager);

public:
	Ref<User> _get_user(const int id);
	Ref<User> _get_user_name(const String &user);
	void _save_user(Ref<User> user);

	Ref<User> _create_user();

	bool _is_username_taken(const String &user_name);
	bool _is_email_taken(const String &email);

	Vector<Ref<User>> get_all();

	virtual void create_table();
	virtual void drop_table();
	virtual void create_default_entries();

	UserManagerDB();
	~UserManagerDB();

protected:
	static void _bind_methods();
};

#endif
