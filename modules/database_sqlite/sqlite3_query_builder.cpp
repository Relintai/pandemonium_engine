#include "sqlite3_query_builder.h"

#include "sqlite3_connection.h"
#include "sqlite3_database.h"
#include "sqlite3_query_result.h"

QueryBuilder *SQLite3QueryBuilder::select() {
	query_result += "SELECT ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::update() {
	query_result += "UPDATE ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::del() {
	query_result += "DELETE FROM ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::cvalues() {
	query_result[query_result.length() - 2] = ' ';
	query_result += ") ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::next_value() {
	query_result[query_result.length() - 2] = ' ';
	query_result += "), (";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::begin_transaction() {
	query_result += "BEGIN TRANSACTION;";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::commit() {
	query_result += "COMMIT;";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::str() {
	query_result += "'";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::cstr() {
	query_result += "'";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::like(const String &str) {
	if (str == "") {
		query_result += "LIKE ";
	} else {
		nlike(escape(str));
	}

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nselect(const String &params) {
	query_result += "SELECT ";
	query_result += params;
	query_result += " ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::nupdate(const String &params) {
	query_result += "UPDATE ";
	query_result += params;
	query_result += " ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::ndel(const String &params) {
	query_result += "DELETE FROM ";
	query_result += params;
	query_result += " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nwhere(const String &params) {
	query_result += "WHERE ";

	if (params != "") {
		query_result += params;
		query_result += " ";
	}

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nfrom(const String &params) {
	query_result += "FROM ";

	if (params != "") {
		query_result += params;
		query_result += " ";
	}

	return this;
}

QueryBuilder *SQLite3QueryBuilder::insert(const String &table_name, const String &columns) {
	query_result += "INSERT INTO ";

	if (table_name != "") {
		query_result += table_name;
		query_result += " ";
	}

	if (columns != "") {
		query_result += "(";
		query_result += columns;
		query_result += ") ";
	}

	return this;
}
QueryBuilder *SQLite3QueryBuilder::nvalues(const String &params_str) {
	query_result += "VALUES(";

	if (params_str != "") {
		query_result += params_str;
		query_result += ") ";
	}

	return this;
}

QueryBuilder *SQLite3QueryBuilder::val() {
	//query_result += "DEFAULT, ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nval(const String &param) {
	query_result += "'";
	query_result += param;
	query_result += "', ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::vals(const String &param) {
	query_result += "'";
	query_result += param;
	query_result += "', ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::vals(const char *param) {
	query_result += "'";
	query_result += String(param);
	query_result += "', ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::vali(const int param) {
	query_result += itos(param);
	query_result += ", ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::valb(const bool param) {
	if (param) {
		query_result += "1, ";
	} else {
		query_result += "0, ";
	}

	return this;
}

QueryBuilder *SQLite3QueryBuilder::valf(const float param) {
	query_result += String::num(param);
	query_result += ", ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::vald(const double param) {
	query_result += String::num(param);
	query_result += ", ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nlike(const String &str) {
	query_result += "LIKE '";
	query_result += str;
	query_result += "' ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::sets() {
	query_result += "SET ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::cset() {
	query_result[query_result.length() - 2] = ' ';

	return this;
}
QueryBuilder *SQLite3QueryBuilder::nsetp(const String &col, const String &param) {
	query_result += col + "='";
	query_result += col + param;
	query_result += col + "', ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::setps(const String &col, const char *param) {
	query_result += col;
	query_result += "='";
	query_result += String(param);
	query_result += "', ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::setpi(const String &col, const int param) {
	query_result += col;
	query_result += "=";
	query_result += itos(param);
	query_result += ", ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::setpb(const String &col, const bool param) {
	if (param) {
		query_result += col;
		query_result += "=1, ";
	} else {
		query_result += col;
		query_result += "=0, ";
	}

	return this;
}
QueryBuilder *SQLite3QueryBuilder::setpf(const String &col, const float param) {
	query_result += col;
	query_result += "=";
	query_result += String::num(param);
	query_result += ", ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::setpd(const String &col, const double param) {
	query_result += col;
	query_result += "=";
	query_result += String::num(param);
	query_result += ", ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nwp(const String &col, const String &param) {
	query_result += col;
	query_result += "='";
	query_result += param;
	query_result += "' ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::wps(const String &col, const char *param) {
	query_result += col;
	query_result += "='";
	query_result += String(param);
	query_result += "' ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::wpi(const String &col, const int param) {
	query_result += col;
	query_result += "=";
	query_result += itos(param);
	query_result += " ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::wpb(const String &col, const bool param) {
	if (param) {
		query_result += col;
		query_result += "=1 ";
	} else {
		query_result += col;
		query_result += "=0 ";
	}

	return this;
}

QueryBuilder *SQLite3QueryBuilder::limit(const int num) {
	query_result += "LIMIT ";
	query_result += itos(num);
	query_result += " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::offset(const int num) {
	query_result += "OFFSET ";
	query_result += itos(num);
	query_result += " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::land() {
	query_result += "AND ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::lor() {
	query_result += "OR ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::wildcard() {
	query_result += "%";

	return this;
}

String SQLite3QueryBuilder::escape(const String &params) {
	if (!_connection.is_valid()) {
		printf("SQLite3QueryBuilder::escape !db!\n");

		return "";
	}

	return _connection->escape(params);
}

QueryBuilder *SQLite3QueryBuilder::prepare() {
	return this;
}

QueryBuilder *SQLite3QueryBuilder::set_param(const int index, const String &value) {
	return this;
}
QueryBuilder *SQLite3QueryBuilder::set_param(const int index, const int value) {
	return this;
}
QueryBuilder *SQLite3QueryBuilder::set_param(const int index, const float value) {
	return this;
}

QueryBuilder *SQLite3QueryBuilder::end_command() {
	query_result += ";";

	return this;
}

Ref<QueryResult> SQLite3QueryBuilder::run() {
	if (!_connection.is_valid()) {
		printf("SQLite3QueryBuilder::run !db!\n");

		return nullptr;
	}

	return _connection->query(query_result);
}

void SQLite3QueryBuilder::run_query() {
	if (!_connection.is_valid()) {
		printf("SQLite3QueryBuilder::run_query !db!\n");

		return;
	}

	_connection->query_run(query_result);
}

QueryBuilder *SQLite3QueryBuilder::select_last_insert_id() {
	return this;
}

SQLite3QueryBuilder::SQLite3QueryBuilder() {
}
SQLite3QueryBuilder::~SQLite3QueryBuilder() {
}
