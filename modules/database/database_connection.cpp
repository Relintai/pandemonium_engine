/*************************************************************************/
/*  database_connection.cpp                                              */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "database_connection.h"

#include "database.h"
#include "prepared_statement.h"
#include "query_builder.h"
#include "query_result.h"
#include "table_builder.h"

Error DatabaseConnection::database_connect(const String &connection_str) {
	return ERR_PRINTER_ON_FIRE;
}

Ref<QueryResult> DatabaseConnection::query(const String &query) {
	return Ref<QueryResult>();
}
void DatabaseConnection::query_run(const String &query) {
}

Ref<QueryBuilder> DatabaseConnection::get_query_builder() {
	return Ref<QueryBuilder>();
}

Ref<TableBuilder> DatabaseConnection::get_table_builder() {
	return Ref<TableBuilder>();
}

Ref<PreparedStatement> DatabaseConnection::create_prepared_statement() {
	return Ref<PreparedStatement>();
}

String DatabaseConnection::escape(const String &str) {
	return String();
}

void DatabaseConnection::escape_to(const String &str, String *to) {
}

Ref<Database> DatabaseConnection::get_owner() {
	return Ref<Database>(_owner);
}

int DatabaseConnection::get_table_version(const String &table) {
	ensure_version_table_exists();

	return 0;
}
void DatabaseConnection::set_table_version(const String &table, const int version) {
}
void DatabaseConnection::ensure_version_table_exists() {
}

void DatabaseConnection::set_owner(Database *owner) {
	_owner = owner;
}

DatabaseConnection::DatabaseConnection() {
	_owner = nullptr;
}

DatabaseConnection::~DatabaseConnection() {
	_owner = nullptr;
}

void DatabaseConnection::_bind_methods() {
	ClassDB::bind_method(D_METHOD("database_connect", "connection_str"), &DatabaseConnection::database_connect);
	ClassDB::bind_method(D_METHOD("query", "query"), &DatabaseConnection::query);
	ClassDB::bind_method(D_METHOD("query_run", "query"), &DatabaseConnection::query_run);

	ClassDB::bind_method(D_METHOD("get_query_builder"), &DatabaseConnection::get_query_builder);
	ClassDB::bind_method(D_METHOD("get_table_builder"), &DatabaseConnection::get_table_builder);
	ClassDB::bind_method(D_METHOD("create_prepared_statement"), &DatabaseConnection::create_prepared_statement);

	ClassDB::bind_method(D_METHOD("escape", "str"), &DatabaseConnection::escape);

	ClassDB::bind_method(D_METHOD("get_table_version", "table"), &DatabaseConnection::get_table_version);
	ClassDB::bind_method(D_METHOD("set_table_version", "table", "version"), &DatabaseConnection::set_table_version);
	ClassDB::bind_method(D_METHOD("ensure_version_table_exists"), &DatabaseConnection::ensure_version_table_exists);

	ClassDB::bind_method(D_METHOD("get_owner"), &DatabaseConnection::get_owner);
}
