#ifndef USER_MODULE_H
#define USER_MODULE_H

#include "core/os/rw_lock.h"
#include "core/ustring.h"

#include "core/resource.h"

class User;

class UserModule : public Resource {
	GDCLASS(UserModule, Resource);

public:
	int get_module_index() const;
	void set_module_index(const int val);

	String get_module_name() const;
	void set_module_name(const String &val);

	Ref<User> get_user();
	void set_user(User *user);

	void read_lock();
	void read_unlock();
	void write_lock();
	void write_unlock();

	UserModule();
	~UserModule();

protected:
	static void _bind_methods();

	int _module_id;
	String _module_name;

	User *_user;

	RWLock _rw_lock;
};

#endif
