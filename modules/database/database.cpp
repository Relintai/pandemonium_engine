#include "database.h"

#include "query_builder.h"
#include "query_result.h"
#include "table_builder.h"

void Database::connect(const String &connection_str) {
}

Ref<QueryResult> Database::query(const String &query) {
	return Ref<QueryResult>();
}
void Database::query_run(const String &query) {
}

Ref<QueryBuilder> Database::get_query_builder() {
	return Ref<QueryBuilder>(new QueryBuilder());
}

Ref<TableBuilder> Database::get_table_builder() {
	return Ref<TableBuilder>(new TableBuilder());
}

String Database::escape(const String &str) {
	return String();
}

void Database::escape_to(const String &str, String *to) {
}

Database::Database() {
}

Database::~Database() {
}

void Database::_bind_methods() {
	ClassDB::bind_method(D_METHOD("connect", "connection_str"), &Database::connect);
	ClassDB::bind_method(D_METHOD("query", "query"), &Database::query);
	ClassDB::bind_method(D_METHOD("query_run", "query"), &Database::query_run);
	ClassDB::bind_method(D_METHOD("get_query_builder"), &Database::get_query_builder);
	ClassDB::bind_method(D_METHOD("get_table_builder"), &Database::get_table_builder);
	ClassDB::bind_method(D_METHOD("escape", "str"), &Database::escape);
}
