#ifndef MYSQL_CONNECTION
#define MYSQL_CONNECTION

#include "core/string.h"

#include "database/database.h"

#include <memory>

//Brynet has it as well, and because of using namespace it is defined here as well
//later this will be fixed better
#ifdef IS_NUM
#undef IS_NUM
#endif

#include <mysql.h>

class MysqlDatabase : public Database {
public:
	void connect(const String &connection_str);
	Ref<QueryResult> query(const String &query);
	void query_run(const String &query);

	Ref<QueryBuilder> get_query_builder();
	Ref<TableBuilder> get_table_builder();

	String escape(const String str);
	void escape(const String str, String *to);

	static Database *_creation_func();
	static void _register();
	static void _unregister();

	MysqlDatabase();
	~MysqlDatabase();

	MYSQL *mysql;
};

#undef IS_NUM

#endif