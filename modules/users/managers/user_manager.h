#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "scene/main/node.h"

class User;

//TODO UserManagerStatic, UserManagerDB, UserManagerFile

class UserManager : public Node {
	GDCLASS(UserManager, Node);

public:
	Ref<User> get_user(const int id);
	Ref<User> get_user_name(const String &user_name);
	void save_user(const Ref<User> &user);

	Ref<User> create_user();

	bool is_username_taken(const String &user_name);
	bool is_email_taken(const String &email);

	virtual Ref<User> _get_user(const int id);
	virtual Ref<User> _get_user_name(const String &user);
	virtual void _save_user(Ref<User> user);

	virtual Ref<User> _create_user();

	virtual bool _is_username_taken(const String &user_name);
	virtual bool _is_email_taken(const String &email);

	virtual Vector<Ref<User>> get_all();

	UserManager();
	~UserManager();

protected:
	void _notification(int p_what);

	static void _bind_methods();
};

#endif
