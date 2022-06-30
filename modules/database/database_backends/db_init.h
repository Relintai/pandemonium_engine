#ifndef DB_INIT_H
#define DB_INIT_H

#if MYSQL_PRESENT
#include "mysql/mysql_database.h"
#endif

#if PGSQL_PRESENT
#include "postgres/pgsql_database.h"
#endif

#if SQLITE_PRESENT
#include "sqlite/sqlite3_database.h"
#endif

void initialize_database_backends() {
#if MYSQL_PRESENT
	MysqlDatabase::_register();
#endif

#if PGSQL_PRESENT
	PGSQLDatabase::_register();
#endif

#if SQLITE_PRESENT
	SQLite3Database::_register();
#endif
}

#endif