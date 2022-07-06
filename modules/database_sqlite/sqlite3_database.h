#ifndef SQLITE3_CONNECTION
#define SQLITE3_CONNECTION

#include "database/database.h"

//Brynet has it aswell, and because of using namespace it is defined here aswell
//later this will be fixed better
//#ifdef IS_NUM
//#undef IS_NUM
//#endif

#include <cstdio>

#include "./sqlite/sqlite3.h"

class SQLite3Database : public Database {
public:
	static Database *_creation_func();
	static void _register();
	static void _unregister();

	Ref<QueryBuilder> get_query_builder();
	Ref<TableBuilder> get_table_builder();

	void connect(const String &connection_str);
	Ref<QueryResult> query(const String &query);
	void query_run(const String &query);

	String escape(const String str);
	void escape(const String str, String *to);

	SQLite3Database();
	~SQLite3Database();

	sqlite3 *conn;
};

//#undef IS_NUM

#endif