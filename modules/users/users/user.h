#ifndef USER_H
#define USER_H

/*************************************************************************/
/*  user.h                                                               */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/os/rw_lock.h"
#include "core/string/ustring.h"

#include "core/object/resource.h"

class UserModule;
class UserManager;

class User : public Resource {
	GDCLASS(User, Resource);

public:
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
	Error read_try_lock();
	Error write_try_lock();

	UserManager *get_owner_user_manager();
	void set_owner_user_manager(UserManager *p_user_manager);
	void set_owner_user_manager_bind(Node *p_user_manager);

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

	UserManager *_owner_user_manager;
};

#endif
