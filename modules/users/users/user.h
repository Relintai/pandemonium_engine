#ifndef USER_H
#define USER_H

#include "core/ustring.h"

#include "core/resource.h"

//TODO remove _user_input postfixes

class User : public Resource {
	GDCLASS(User, Resource);

public:
	enum Permissions {
		PERMISSION_CREATE = 1 << 0,
		PERMISSION_READ = 1 << 1,
		PERMISSION_UPDATE = 1 << 2,
		PERMISSION_DELETE = 1 << 3,

		PERMISSION_ALL = PERMISSION_CREATE | PERMISSION_READ | PERMISSION_UPDATE | PERMISSION_DELETE,
		PERMISSION_NONE = 0
	};

	String get_name_user_input() const;
	void set_name_user_input(const String &val);

	String get_email_user_input() const;
	void set_email_user_input(const String &val);

	int get_rank() const;
	void set_rank(const int &val);

	String get_pre_salt() const;
	void set_pre_salt(const String &val);

	String get_post_salt() const;
	void set_post_salt(const String &val);

	String get_password_hash() const;
	void set_password_hash(const String &val);

	bool get_banned() const;
	void set_banned(const bool &val);

	String get_password_reset_token() const;
	void set_password_reset_token(const String &val);

	bool get_locked() const;
	void set_locked(const bool &val);

	bool check_password(const String &p_password);
	void create_password(const String &p_password);
	String hash_password(const String &p_password);

	virtual bool _check_password(const String &p_password);
	virtual void _create_password(const String &p_password);
	virtual String _hash_password(const String &p_password);

	void save();

	User();
	~User();

protected:
	static void _bind_methods();

	String _name_user_input;
	String _email_user_input;
	int _rank;
	String _pre_salt;
	String _post_salt;
	String _password_hash;
	bool _banned;
	String _password_reset_token;
	bool _locked;
};

VARIANT_ENUM_CAST(User::Permissions);

#endif
