#include "database_connection.h"

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

Ref<Database> DatabaseConnection::get_owner() {
	return Ref<Database>(_owner);
}

DatabaseConnection::DatabaseConnection() {
	_owner = nullptr;
}

DatabaseConnection::~DatabaseConnection() {
	_owner = nullptr;
}

void DatabaseConnection::_bind_methods() {
	ClassDB::bind_method(D_METHOD("connect", "connection_str"), &DatabaseConnection::connect);
	ClassDB::bind_method(D_METHOD("query", "query"), &DatabaseConnection::query);
	ClassDB::bind_method(D_METHOD("query_run", "query"), &DatabaseConnection::query_run);
	ClassDB::bind_method(D_METHOD("get_query_builder"), &DatabaseConnection::get_query_builder);
	ClassDB::bind_method(D_METHOD("get_table_builder"), &DatabaseConnection::get_table_builder);
	ClassDB::bind_method(D_METHOD("escape", "str"), &DatabaseConnection::escape);
	ClassDB::bind_method(D_METHOD("get_owner"), &DatabaseConnection::get_owner);
}
