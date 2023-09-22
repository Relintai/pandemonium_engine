#ifndef USER_MANAGER_DB_H
#define USER_MANAGER_DB_H

#include "core/os/rw_lock.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "user_manager.h"

class User;
class Database;
class TableBuilder;
class QueryBuilder;
class DatabaseConnection;

class UserManagerDB : public UserManager {
	GDCLASS(UserManagerDB, UserManager);

public:
	String get_database_table_name();
	void set_database_table_name(const String &val);

	Ref<Database> get_database();
	void set_database(const Ref<Database> &db);

	Ref<DatabaseConnection> get_database_connection();
	Ref<TableBuilder> get_table_builder();
	Ref<QueryBuilder> get_query_builder();

	Ref<User> _get_user(const int id);
	Ref<User> _get_user_name(const String &user);
	void _save_user(Ref<User> user);

	Ref<User> _create_user();

	bool _is_username_taken(const String &user_name);
	bool _is_email_taken(const String &email);

	Vector<Ref<User>> get_all();

	void create_table();
	void drop_table();
	void update_table(const int p_current_table_version);
	void create_default_entries(const int p_seed);

	virtual void _create_table();
	virtual void _drop_table();
	virtual void _update_table(const int p_current_table_version);
	virtual void _create_default_entries(const int p_seed);

	void migrate(const bool p_clear, const bool p_should_seed, const int p_seed);
	virtual void _migrate(const bool p_clear, const bool p_should_seed, const int p_seed);

	UserManagerDB();
	~UserManagerDB();

protected:
	void _notification(const int what);

	static void _bind_methods();

	String _database_table_name;
	Ref<Database> _database;
};

#endif
