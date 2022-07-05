#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "core/reference.h"
#include "core/ustring.h"

#include "core/object.h"

class Database;

class DatabaseManager : public Object {
	GDCLASS(DatabaseManager, Object);

public:
	Ref<Database> get_ddb();
	void set_ddb(const Ref<Database> &db);

	void load();

	static DatabaseManager *get_singleton();

	DatabaseManager();
	~DatabaseManager();

protected:
	Vector<Ref<Database>> _databases;
	Ref<Database> _ddb;

private:
	static DatabaseManager *_instance;
};

#endif
