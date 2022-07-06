#include "database_single_threaded.h"

#include "database_connection.h"
#include "query_builder.h"
#include "query_result.h"
#include "table_builder.h"

Ref<DatabaseConnection> DatabaseSingleThreaded::get_connection() {
	if (!_connection.is_valid()) {
		_connection = _allocate_connection();
	}

	return _connection;
}

Ref<DatabaseConnection> DatabaseSingleThreaded::_allocate_connection() {
	Ref<DatabaseConnection> dbc;
	dbc.instance();
	dbc->set_owner(this);
	dbc->database_connect(_connection_string);
	return dbc;
}

DatabaseSingleThreaded::DatabaseSingleThreaded() {
}

DatabaseSingleThreaded::~DatabaseSingleThreaded() {
	if (_connection.is_valid()) {
		_connection->set_owner(nullptr);
	}

	_connection.unref();
}

void DatabaseSingleThreaded::_bind_methods() {
}
