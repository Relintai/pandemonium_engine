#ifndef USER_MANAGER_FILE_H
#define USER_MANAGER_FILE_H

#include "core/os/rw_lock.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "user_manager.h"

class User;

class UserManagerFile : public UserManager {
	GDCLASS(UserManagerFile, UserManager);

public:
	String get_save_folder_path();
	void set_save_folder_path(const String &val);

	Ref<User> _get_user(const int id);
	Ref<User> _get_user_name(const String &user);
	void _save_user(Ref<User> user);

	Ref<User> _create_user();

	bool _is_username_taken(const String &user_name);
	bool _is_email_taken(const String &email);

	Vector<Ref<User>> get_all();

	UserManagerFile();
	~UserManagerFile();

protected:
	void load();
	void save();

	void _on_user_changed();

	void _notification(int p_what);

	static void _bind_methods();

	Vector<Ref<User>> _users;
	RWLock _rw_lock;
	bool _save_queued;

	String _save_folder_path;
};

#endif
