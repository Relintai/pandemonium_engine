#ifndef USER_DB_H
#define USER_DB_H

#include "core/os/rw_lock.h"
#include "core/reference.h"
#include "core/ustring.h"
#include "core/containers/vector.h"

#include "core/object.h"

class UserManager;
class User;

class UserDB : public Object {
	GDCLASS(UserDB, Object);

public:
	//TODO
	//get_user_id
	//get_user_index
	//get_user_count
	//get_users
	//get_users_bind
	//delete_user_id
	//delete_user_index
	//delete_user_name
	//remove_user...
	//clear

	Ref<User> get_user(const int id);
	Ref<User> get_user_name(const String &user_name);
	void save_user(const Ref<User> &user);

	Ref<User> create_user();

	bool is_username_taken(const String &user_name);
	bool is_email_taken(const String &email);

	UserManager *get_user_manager();
	void set_user_manager(UserManager *um);

	static UserDB *get_singleton();

	UserDB();
	~UserDB();

protected:
	static void _bind_methods();

	Node *get_user_manager_bind();

	UserManager *_user_manager;

	static UserDB *_self;

	Vector<Ref<User>> _users;
	RWLock _lock;
};

#endif
