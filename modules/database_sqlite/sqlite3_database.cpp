#include "sqlite3_database.h"

#include "sqlite3_connection.h"

Ref<DatabaseConnection> SQLite3Database::_allocate_connection() {
	Ref<SQLite3DatabaseConnection> dbc;
	dbc.instance();
	dbc->set_owner(this);
	dbc->database_connect(_connection_string);
	return dbc;
}

SQLite3Database::SQLite3Database() {
}

SQLite3Database::~SQLite3Database() {
}
