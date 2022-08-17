#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "core/reference.h"
#include "core/ustring.h"
#include "core/containers/vector.h"

#include "core/object.h"

class Database;

class DatabaseManager : public Object {
	GDCLASS(DatabaseManager, Object);

public:
	//ddb = default database
	Ref<Database> get_ddb();
	void set_ddb(const Ref<Database> &db);

	void add_database(const Ref<Database> &db, bool set_as_default = true);
	void remove_database(const int index, const bool unset_if_default = true);
	Ref<Database> get_database(const int index);
	int get_database_count();

	Vector<Ref<Database>> get_databases();
	Vector<Variant> get_databases_bind();

	void load();

	void migrate(const bool p_clear, const bool p_should_seed, const int p_seed);

	static DatabaseManager *get_singleton();

	DatabaseManager();
	~DatabaseManager();

protected:
	Vector<Ref<Database>> _databases;
	Ref<Database> _ddb;

private:
	static void _bind_methods();

	static DatabaseManager *_instance;
};

#endif
