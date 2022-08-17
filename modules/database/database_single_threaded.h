#ifndef DATABASE_SINGLE_THREADED_H
#define DATABASE_SINGLE_THREADED_H

#include "core/string/ustring.h"

#include "database.h"

class QueryBuilder;
class TableBuilder;
class QueryResult;
class DatabaseConnection;

class DatabaseSingleThreaded : public Database {
	GDCLASS(DatabaseSingleThreaded, Database);

public:
	String get_connection_string();
	void set_connection_string(const String &val);

	Ref<DatabaseConnection> get_connection();

	DatabaseSingleThreaded();
	~DatabaseSingleThreaded();

protected:
	Ref<DatabaseConnection> _allocate_connection();

	static void _bind_methods();

	Ref<DatabaseConnection> _connection;
};

#endif
