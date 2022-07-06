#include "database.h"

#include "query_builder.h"
#include "query_result.h"
#include "table_builder.h"

void DatabaseConnection::connect(const String &connection_str) {
}

Ref<QueryResult> DatabaseConnection::query(const String &query) {
	return Ref<QueryResult>();
}
void DatabaseConnection::query_run(const String &query) {
}

Ref<QueryBuilder> DatabaseConnection::get_query_builder() {
	return Ref<QueryBuilder>(new QueryBuilder());
}

Ref<TableBuilder> DatabaseConnection::get_table_builder() {
	return Ref<TableBuilder>(new TableBuilder());
}

String DatabaseConnection::escape(const String &str) {
	return String();
}

void DatabaseConnection::escape_to(const String &str, String *to) {
}

DatabaseConnection::DatabaseConnection() {
}

DatabaseConnection::~DatabaseConnection() {
}

void DatabaseConnection::_bind_methods() {
	ClassDB::bind_method(D_METHOD("connect", "connection_str"), &DatabaseConnection::connect);
	ClassDB::bind_method(D_METHOD("query", "query"), &DatabaseConnection::query);
	ClassDB::bind_method(D_METHOD("query_run", "query"), &DatabaseConnection::query_run);
	ClassDB::bind_method(D_METHOD("get_query_builder"), &DatabaseConnection::get_query_builder);
	ClassDB::bind_method(D_METHOD("get_table_builder"), &DatabaseConnection::get_table_builder);
	ClassDB::bind_method(D_METHOD("escape", "str"), &DatabaseConnection::escape);
}

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
