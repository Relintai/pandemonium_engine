#include "database.h"

#include "query_builder.h"
#include "table_builder.h"
#include "query_result.h"

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

String Database::escape(const String str) {
	return String();
}

void Database::escape(const String str, String *to) {

}

Database::Database() {
}

Database::~Database() {
}