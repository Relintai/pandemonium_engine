#include "sqlite3_table_builder.h"

#include "../database/query_result.h"

#include "sqlite3_connection.h"

TableBuilder *SQLite3TableBuilder::create_table(const String &name) {
	result += "CREATE TABLE ";
	result += name;
	result += " ( ";

	return this;
}

TableBuilder *SQLite3TableBuilder::alter_table(const String &p_table_name) {
	result += "ALTER TABLE ";
	result += p_table_name;
	result += " ";

	return this;
}

TableBuilder *SQLite3TableBuilder::rename_to(const String &p_table_name) {
	result += "RENAME TO ";
	result += p_table_name;
	result += " ";

	return this;
}

TableBuilder *SQLite3TableBuilder::rename_column_to(const String &p_column_name, const String &p_new_column_name) {
	result += "RENAME COLUMN ";
	result += p_column_name;
	result += " TO ";
	result += p_new_column_name;
	result += " ";

	return this;
}
TableBuilder *SQLite3TableBuilder::add_column() {
	result += "ADD COLUMN ";

	return this;
}
TableBuilder *SQLite3TableBuilder::drop_column(const String &p_column_name) {
	result += "DROP COLUMN ";
	result += p_column_name;
	result += " ";

	return this;
}

TableBuilder *SQLite3TableBuilder::integer(const String &name, const int length) {
	result += name;
	result += " INTEGER ";

	if (length != -1) {
		result += "(";
		result += itos(length);
		result += ") ";
	}

	return this;
}

TableBuilder *SQLite3TableBuilder::tiny_integer(const String &name, const int length) {
	result += name;
	result += " INTEGER(";

	if (length == -1) {
		result += "4";
	} else {
		result += itos(length);
	}

	result += ") ";

	return this;
}

TableBuilder *SQLite3TableBuilder::small_integer(const String &name, const int length) {
	result += name;
	result += " INTEGER(";

	if (length == -1) {
		result += "6";
	} else {
		result += itos(length);
	}

	result += ") ";

	return this;
}

TableBuilder *SQLite3TableBuilder::real_float(const String &name, const int size, const int d) {
	result += name;
	result += " FLOAT ";

	return this;
}

TableBuilder *SQLite3TableBuilder::real_double(const String &name, const int size, const int d) {
	result += name;
	result += " DOUBLE ";

	return this;
}

TableBuilder *SQLite3TableBuilder::date(const String &name) {
	result += name;
	result += " DATE ";

	return this;
}

TableBuilder *SQLite3TableBuilder::varchar(const String &name, const int length) {
	result += name;
	result += " VARCHAR ";

	if (length != -1) {
		result += "(";
		result += itos(length);
		result += ") ";
	}

	return this;
}

TableBuilder *SQLite3TableBuilder::text(const String &name) {
	result += name;
	result += " TEXT ";

	return this;
}

TableBuilder *SQLite3TableBuilder::not_null() {
	//result += "NOT NULL ";

	return this;
}

TableBuilder *SQLite3TableBuilder::null() {
	result += "NULL ";

	return this;
}

TableBuilder *SQLite3TableBuilder::defval(const String &val) {
	result += "DEFAULT '";
	result += val;
	result += "'";

	return this;
}

TableBuilder *SQLite3TableBuilder::auto_increment() {
	//result += "AUTO_INCREMENT ";

	return this;
}

TableBuilder *SQLite3TableBuilder::primary_key(const String &name) {
	result += "PRIMARY KEY (";
	result += name;
	result += ") ";

	return this;
}

TableBuilder *SQLite3TableBuilder::primary_key() {
	result += "PRIMARY KEY ";

	return this;
}

TableBuilder *SQLite3TableBuilder::next_row() {
	result += ", ";

	return this;
}

TableBuilder *SQLite3TableBuilder::ccreate_table() {
	result += ");";

	return this;
}

TableBuilder *SQLite3TableBuilder::drop_table() {
	result += "DROP TABLE ";

	return this;
}
TableBuilder *SQLite3TableBuilder::drop_table_if_exists() {
	result += "DROP TABLE IF EXISTS ";

	return this;
}
TableBuilder *SQLite3TableBuilder::drop_table(const String &name) {
	result += "DROP TABLE ";
	result += name;
	result += ";";

	return this;
}
TableBuilder *SQLite3TableBuilder::drop_table_if_exists(const String &name) {
	result += "DROP TABLE IF EXISTS ";
	result += name;
	result += ";";

	return this;
}
TableBuilder *SQLite3TableBuilder::cdrop_table() {
	result += ";";

	return this;
}

TableBuilder *SQLite3TableBuilder::foreign_key(const String &name) {
	result += "FOREIGN KEY (";
	result += name;
	result += ") ";

	return this;
}
TableBuilder *SQLite3TableBuilder::references(const String &table, const String &name) {
	result += "REFERENCES ";
	result += table;
	result += " (";
	result += name;
	result += ") ";

	return this;
}

TableBuilder *SQLite3TableBuilder::end_command() {
	result += ";";

	return this;
}

Ref<QueryResult> SQLite3TableBuilder::run() {
	if (!_connection.is_valid()) {
		printf("SQLite3TableBuilder::run !db!\n");

		return nullptr;
	}

	return _connection->query(result);
}

void SQLite3TableBuilder::run_query() {
	if (!_connection.is_valid()) {
		printf("SQLite3TableBuilder::run_query !db!\n");

		return;
	}

	_connection->query_run(result);
}

SQLite3TableBuilder::SQLite3TableBuilder() {
}

SQLite3TableBuilder::~SQLite3TableBuilder() {
}
