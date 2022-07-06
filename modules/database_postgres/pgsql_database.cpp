#include "pgsql_database.h"

#include "database/database_manager.h"

Database *PGSQLDatabase::_creation_func() {
	return new PGSQLDatabase();
}

void PGSQLDatabase::_register() {
	DatabaseManager::_register_db_creation_func("pgsql", PGSQLDatabase::_creation_func);
}

void PGSQLDatabase::_unregister() {
	DatabaseManager::_unregister_db_creation_func("pgsql");
}
