#include "mysql_database.h"

#include <cstdio>

#include "database/database_manager.h"

#include "mysql_query_builder.h"
#include "mysql_query_result.h"
#include "mysql_table_builder.h"

void MysqlDatabase::connect(const String &connection_str) {
	mysql = mysql_init(mysql);
	mysql_options(mysql, MYSQL_OPT_NONBLOCK, 0);

	String host = "127.0.0.1";
	String user = "";
	String password = "";
	String dbname = "testappdb";
	int port = 3306;

	mysql = mysql_real_connect(mysql, host.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, NULL, 0);

	if (mysql) {
		printf("mysql connected\n");
	}
}

Ref<QueryResult> MysqlDatabase::query(const String &query) {
	if (!mysql)
		return nullptr;

	//printf("%s\n", query.c_str());

	int error = mysql_real_query(mysql, query.c_str(), query.capacity());

	if (error) {
		const char *merr = mysql_error(mysql);

		printf("MySQL error: %s\n", merr);

		return nullptr;
	}

	MYSQL_RES *result = mysql_use_result(mysql);

	MysqlQueryResult *res = new MysqlQueryResult();

	res->result = result;
	//res->next_row();

	return Ref<QueryResult>(res);
}

void MysqlDatabase::query_run(const String &query) {
	if (!mysql)
		return;

	//printf("%s\n", query.c_str());

	int error = mysql_real_query(mysql, query.c_str(), query.capacity());

	if (error) {
		const char *merr = mysql_error(mysql);

		printf("MySQL error: %s\n", merr);
		return;
	}

	//printf("query OK\n");
	//printf("----------------\n");

	/*
	printf("----------------\n");

	MYSQL_RES *result = mysql_use_result(mysql);
	//MYSQL_RES *result = mysql_store_result(mysql);

	MYSQL_ROW row;
	while (row = mysql_fetch_row(result)) {
		printf("%s\n", row[0]);
		//printf("%s\n", row[1]);
	}

	printf("----------------\n");

	mysql_free_result(result);
	*/
}

Ref<QueryBuilder> MysqlDatabase::get_query_builder() {
	MysqlQueryBuilder *b = new MysqlQueryBuilder();
	b->_db = this;

	return Ref<QueryBuilder>(b);
}

Ref<TableBuilder> MysqlDatabase::get_table_builder() {
	return Ref<TableBuilder>(new MysqlTableBuilder());
}

String MysqlDatabase::escape(const String str) {
	String res;
	//https://dev.mysql.com/doc/c-api/8.0/en/mysql-real-escape-string.html
	//You must allocate the to buffer to be at least length*2+1 bytes long. 
	res.ensure_capacity(str.size() * 2 + 1);

	mysql_real_escape_string(mysql, res.dataw(), str.c_str(), str.size());

	return res;
}
void MysqlDatabase::escape(const String str, String *to) {
	//https://dev.mysql.com/doc/c-api/8.0/en/mysql-real-escape-string.html
	//You must allocate the to buffer to be at least length*2+1 bytes long. 
	to->ensure_capacity(str.size() * 2 + 1);

	mysql_real_escape_string(mysql, to->dataw(), str.c_str(), str.size());
}

MysqlDatabase::MysqlDatabase() :
		Database() {

	mysql = nullptr;
}

MysqlDatabase::~MysqlDatabase() {
	mysql_close(mysql);

	delete mysql;
}

Database *MysqlDatabase::_creation_func() {
	return new MysqlDatabase();
}

void MysqlDatabase::_register() {
	DatabaseManager::_register_db_creation_func("mysql", MysqlDatabase::_creation_func);
}

void MysqlDatabase::_unregister() {
	DatabaseManager::_unregister_db_creation_func("mysql");
}