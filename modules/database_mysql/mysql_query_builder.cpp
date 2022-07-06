#include "mysql_query_builder.h"

#include "mysql_database.h"
#include "mysql_query_result.h"

QueryBuilder *MysqlQueryBuilder::select() {
	query_result += "SELECT ";

	return this;
}
QueryBuilder *MysqlQueryBuilder::where() {
	query_result += "WHERE ";

	return this;
}
QueryBuilder *MysqlQueryBuilder::from() {
	query_result += "FROM ";

	return this;
}
QueryBuilder *MysqlQueryBuilder::insert() {
	query_result += "INSERT INTO ";

	return this;
}
QueryBuilder *MysqlQueryBuilder::values() {
	query_result += "VALUES(";

	return this;
}
QueryBuilder *MysqlQueryBuilder::cvalues() {
	query_result += ") ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::nselect(const String &params) {
	query_result += "SELECT " + params + " ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::nwhere(const String &params) {
	query_result += "WHERE " + params + " ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::nfrom(const String &params) {
	query_result += "FROM " + params + " ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::ninsert(const String &table_name) {
	query_result += "INSERT INTO " + table_name + " ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::nvalues(const String &params_str) {
	query_result += "VALUES(" + params_str + ") ";

	return this;
}

String MysqlQueryBuilder::escape(const String &params) {
	if (!_db) {
		printf("MysqlQueryBuilder::escape !db!\n");

		return "";
	}

	return _db->escape(params);
}

QueryBuilder *MysqlQueryBuilder::prepare() {
	return this;
}

QueryBuilder *MysqlQueryBuilder::set_param(const int index, const String &value) {
	return this;
}
QueryBuilder *MysqlQueryBuilder::set_param(const int index, const int value) {
	return this;
}
QueryBuilder *MysqlQueryBuilder::set_param(const int index, const float value) {
	return this;
}

QueryBuilder *MysqlQueryBuilder::end_command() {
	query_result += ";";

	return this;
}

Ref<QueryResult> MysqlQueryBuilder::run() {
	end_command();

	if (!_db) {
		printf("MysqlQueryBuilder::run !db!\n");

		return nullptr;
	}

	return _db->query(query_result);
}

void MysqlQueryBuilder::run_query() {
	end_command();

	if (!_db) {
		printf("MysqlQueryBuilder::run_query !db!\n");

		return;
	}

	_db->query_run(query_result);
}

QueryBuilder *MysqlQueryBuilder::limit(const int num) {
	//query_result += "LIMIT " + num + " ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::offset(const int num) {
	//query_result += "OFFSET " + num + " ";

	return this;
}

MysqlQueryBuilder::MysqlQueryBuilder() {
}
MysqlQueryBuilder::~MysqlQueryBuilder() {
}