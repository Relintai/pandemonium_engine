#include "sqlite3_table_builder.h"

#include "../database/query_result.h"

#include "sqlite3_connection.h"

TableBuilder *SQLite3TableBuilder::create_table(const String &name) {
	result += "CREATE TABLE " + name + " ( ";

	return this;
}

TableBuilder *SQLite3TableBuilder::integer(const String &name, const int length) {
	result += name + " INTEGER ";

	if (length != -1) {
		result += "(";
		result += itos(length);
		result += ") ";
	}

	return this;
}

TableBuilder *SQLite3TableBuilder::tiny_integer(const String &name, const int length) {
	result += name + " INTEGER(";

	if (length == -1) {
		result += "4";
	} else {
		result += itos(length);
	}

	result += ") ";

	return this;
}

TableBuilder *SQLite3TableBuilder::small_integer(const String &name, const int length) {
	result += name + " INTEGER(";

	if (length == -1) {
		result += "6";
	} else {
		result += itos(length);
	}

	result += ") ";

	return this;
}

TableBuilder *SQLite3TableBuilder::real_float(const String &name, const int size, const int d) {
	result += name + " FLOAT ";

	return this;
}

TableBuilder *SQLite3TableBuilder::real_double(const String &name, const int size, const int d) {
	result += name + " DOUBLE ";

	return this;
}

TableBuilder *SQLite3TableBuilder::date(const String &name) {
	result += name + " DATE ";

	return this;
}

TableBuilder *SQLite3TableBuilder::varchar(const String &name, const int length) {
	result += name + " VARCHAR ";

	if (length != -1) {
		result += "(";
		result += itos(length);
		result += ") ";
	}

	return this;
}

TableBuilder *SQLite3TableBuilder::text(const String &name) {
	result += name + " TEXT ";

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
	result += "PRIMARY KEY (" + name + ") ";

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
	result += "DROP TABLE " + name + ";";

	return this;
}
TableBuilder *SQLite3TableBuilder::drop_table_if_exists(const String &name) {
	result += "DROP TABLE IF EXISTS " + name + ";";

	return this;
}
TableBuilder *SQLite3TableBuilder::cdrop_table() {
	result += ";";

	return this;
}

TableBuilder *SQLite3TableBuilder::foreign_key(const String &name) {
	result += "FOREIGN KEY (" + name + ") ";

	return this;
}
TableBuilder *SQLite3TableBuilder::references(const String &table, const String &name) {
	result += "REFERENCES " + table + " (" + name + ") ";

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
