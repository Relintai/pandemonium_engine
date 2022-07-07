#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "scene/main/node.h"

class User;

class UserManager : public Node {
	GDCLASS(UserManager, Node);

public:
	Ref<User> get_user(const int id);
	Ref<User> get_user_name(const String &user_name);
	void save_user(const Ref<User> &user);

	Ref<User> create_user();

	bool is_username_taken(const String &user_name);
	bool is_email_taken(const String &email);

	bool check_password(const Ref<User> &user, const String &p_password);
	void create_password(const Ref<User> &user, const String &p_password);
	String hash_password(const Ref<User> &user, const String &p_password);

	virtual Ref<User> _get_user(const int id);
	virtual Ref<User> _get_user_name(const String &user);
	virtual void _save_user(Ref<User> user);

	virtual Ref<User> _create_user();

	virtual bool _is_username_taken(const String &user_name);
	virtual bool _is_email_taken(const String &email);

	virtual bool _check_password(const Ref<User> &user, const String &p_password);
	virtual void _create_password(Ref<User> user, const String &p_password);
	virtual String _hash_password(const Ref<User> &user, const String &p_password);

	Vector<Ref<User>> get_all();

	UserManager();
	~UserManager();

protected:
	static void _bind_methods();
};

#endif
