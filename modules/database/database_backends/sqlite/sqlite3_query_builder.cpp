#include "sqlite3_query_builder.h"

#include "sqlite3_database.h"
#include "sqlite3_query_result.h"

#include <sstream>

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

QueryBuilder *SQLite3QueryBuilder::where() {
	query_result += "WHERE ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::from() {
	query_result += "FROM ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::insert() {
	query_result += "INSERT INTO ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::values() {
	query_result += "VALUES(";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::cvalues() {
	query_result[query_result.size() - 2] = ' ';
	query_result += ") ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::next_value() {
	query_result[query_result.size() - 2] = ' ';
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

QueryBuilder *SQLite3QueryBuilder::like() {
	query_result += "LIKE ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nselect(const String &params) {
	query_result += "SELECT " + params + " ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::nupdate(const String &params) {
	query_result += "UPDATE " + params + " ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::ndel(const String &params) {
	query_result += "DELETE FROM " + params + " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nwhere(const String &params) {
	query_result += "WHERE " + params + " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nfrom(const String &params) {
	query_result += "FROM " + params + " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::insert(const String &table_name) {
	query_result += "INSERT INTO " + table_name + " ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::insert(const String &table_name, const String &columns) {
	query_result += "INSERT INTO " + table_name + "(" + columns + ") ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::nvalues(const String &params_str) {
	query_result += "VALUES(" + params_str + ") ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::val() {
	//query_result += "DEFAULT, ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nval(const String &param) {
	query_result += "'" + param + "', ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::val(const char *param) {
	query_result += "'" + String(param) + "', ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::val(const int param) {
	//todo add a better way
	std::stringstream ss;
	ss << param;

	query_result += ss.str() + ", ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::val(const bool param) {
	if (param)
		query_result += "1, ";
	else
		query_result += "0, ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::valf(const float param) {
	//todo add a better way
	std::stringstream ss;
	ss << param;

	query_result += ss.str() + ", ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::vald(const double param) {
	//todo add a better way
	std::stringstream ss;
	ss << param;

	query_result += ss.str() + ", ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nlike(const String &str) {
	query_result += "LIKE '" + str + "' ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::set() {
	query_result += "SET ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::cset() {
	query_result[query_result.size() - 2] = ' ';

	return this;
}
QueryBuilder *SQLite3QueryBuilder::nsetp(const String &col, const String &param) {
	query_result += col + "='" + param + "', ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::setp(const String &col, const char *param) {
	query_result += col + "='" + String(param) + "', ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::setp(const String &col, const int param) {
	//todo add a better way
	std::stringstream ss;
	ss << param;

	query_result += col + "=" + ss.str() + ", ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::setp(const String &col, const bool param) {
	if (param)
		query_result += col + "=1, ";
	else
		query_result += col + "=0, ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::setpf(const String &col, const float param) {
	//todo add a better way
	std::stringstream ss;
	ss << param;

	query_result += col + "=" + ss.str() + ", ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::setpd(const String &col, const double param) {
	//todo add a better way
	std::stringstream ss;
	ss << param;

	query_result += col + "=" + ss.str() + ", ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::nwp(const String &col, const String &param) {
	query_result += col + "='" + param + "' ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::wp(const String &col, const char *param) {
	query_result += col + "='" + String(param) + "' ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::wp(const String &col, const int param) {
	//todo add a better way
	std::stringstream ss;
	ss << param;

	query_result += col + "=" + ss.str() + " ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::wp(const String &col, const bool param) {
	if (param)
		query_result += col + "=1 ";
	else
		query_result += col + "=0 ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::limit(const int num) {
	//todo better way
	std::stringstream ss;
	ss << num;

	query_result += "LIMIT " + ss.str() + " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::offset(const int num) {
	//todo better way
	std::stringstream ss;
	ss << num;

	query_result += "OFFSET " + ss.str() + " ";

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
	if (!_db) {
		printf("SQLite3QueryBuilder::escape !db!\n");

		return "";
	}

	return _db->escape(params);
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
	if (!_db) {
		printf("SQLite3QueryBuilder::run !db!\n");

		return nullptr;
	}

	return _db->query(query_result);
}

void SQLite3QueryBuilder::run_query() {
	if (!_db) {
		printf("SQLite3QueryBuilder::run_query !db!\n");

		return;
	}

	_db->query_run(query_result);
}

QueryBuilder *SQLite3QueryBuilder::select_last_insert_id() {
	return this;
}

SQLite3QueryBuilder::SQLite3QueryBuilder() {
}
SQLite3QueryBuilder::~SQLite3QueryBuilder() {
}