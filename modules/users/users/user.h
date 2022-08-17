#ifndef USER_H
#define USER_H

#include "core/os/rw_lock.h"
#include "core/string/ustring.h"

#include "core/object/resource.h"

class UserModule;

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

	int get_user_id() const;
	void set_user_id(const int val);

	String get_user_name() const;
	void set_user_name(const String &val);

	String get_email() const;
	void set_email(const String &val);

	int get_rank() const;
	void set_rank(const int val);

	String get_pre_salt() const;
	void set_pre_salt(const String &val);

	String get_post_salt() const;
	void set_post_salt(const String &val);

	String get_password_hash() const;
	void set_password_hash(const String &val);

	bool get_banned() const;
	void set_banned(const bool val);

	String get_password_reset_token() const;
	void set_password_reset_token(const String &val);

	bool get_locked() const;
	void set_locked(const bool val);

	void add_module(const Ref<UserModule> &module);
	Ref<UserModule> get_module(const int index);
	Ref<UserModule> get_module_named(const String &name);
	void remove_module(const int index);
	int get_module_count();

	Vector<Variant> get_modules();
	void set_modules(const Vector<Variant> &data);

	bool check_password(const String &p_password);
	void create_password(const String &p_password);
	String hash_password(const String &p_password);

	virtual bool _check_password(const String &p_password);
	virtual void _create_password(const String &p_password);
	virtual String _hash_password(const String &p_password);

	Dictionary to_dict();
	void from_dict(const Dictionary &dict);

	virtual Dictionary _to_dict();
	virtual void _from_dict(const Dictionary &dict);

	String to_json();
	void from_json(const String &data);

	void save();

	void read_lock();
	void read_unlock();
	void write_lock();
	void write_unlock();

	User();
	~User();

protected:
	static void _bind_methods();

	int _user_id;
	String _user_name;
	String _email;
	int _rank;
	String _pre_salt;
	String _post_salt;
	String _password_hash;
	bool _banned;
	String _password_reset_token;
	bool _locked;

	Vector<Ref<UserModule>> _modules;

	RWLock _rw_lock;
};

VARIANT_ENUM_CAST(User::Permissions);

#endif
