#include "database.h"

#include "database_connection.h"
#include "query_builder.h"
#include "query_result.h"
#include "table_builder.h"

String Database::get_connection_string() {
	return _connection_string;
}
void Database::set_connection_string(const String &val) {
	_connection_string = val;
}

Ref<DatabaseConnection> Database::get_connection() {
	return _allocate_connection();
}

Ref<DatabaseConnection> Database::_allocate_connection() {
	//Ref<DatabaseConnection> dbc;
	//dbc.instance();
	//dbc->set_owner(this); //if needed
	//dbc->connect(_connection_string);
	//return dbc;

	return Ref<DatabaseConnection>();
}

Database::Database() {
}

Database::~Database() {
}

void Database::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_connection_string"), &Database::get_connection_string);
	ClassDB::bind_method(D_METHOD("set_connection_string", "value"), &Database::set_connection_string);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "connection_string"), "set_connection_string", "get_connection_string");

	ClassDB::bind_method(D_METHOD("get_connection"), &Database::get_connection);
}
