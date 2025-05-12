/*************************************************************************/
/*  table_builder.cpp                                                    */
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

#include "table_builder.h"

#include "core/string/print_string.h"
#include "query_result.h"

String TableBuilder::get_result() {
	return result.as_string();
}
void TableBuilder::set_result(const String &val) {
	result.clear();
	result.append(val);
}

TableBuilder *TableBuilder::create_table(const String &name) {
	return this;
}

TableBuilder *TableBuilder::alter_table(const String &p_table_name) {
	return this;
}

TableBuilder *TableBuilder::rename_to(const String &p_table_name) {
	return this;
}

TableBuilder *TableBuilder::rename_column_to(const String &p_column_name, const String &p_new_column_name) {
	return this;
}
TableBuilder *TableBuilder::add_column() {
	return this;
}
TableBuilder *TableBuilder::drop_column(const String &p_column_name) {
	return this;
}

TableBuilder *TableBuilder::integer(const String &name, const int length) {
	return this;
}

TableBuilder *TableBuilder::tiny_integer(const String &name, const int length) {
	return this;
}

TableBuilder *TableBuilder::small_integer(const String &name, const int length) {
	return this;
}

TableBuilder *TableBuilder::real_float(const String &name, const int size, const int d) {
	return this;
}

TableBuilder *TableBuilder::real_double(const String &name, const int size, const int d) {
	return this;
}

TableBuilder *TableBuilder::date(const String &name) {
	return this;
}

TableBuilder *TableBuilder::varchar(const String &name, const int length) {
	return this;
}

TableBuilder *TableBuilder::text(const String &name) {
	return this;
}

TableBuilder *TableBuilder::not_null() {
	return this;
}

TableBuilder *TableBuilder::null() {
	return this;
}

TableBuilder *TableBuilder::defval(const String &val) {
	return this;
}

TableBuilder *TableBuilder::auto_increment() {
	return this;
}

TableBuilder *TableBuilder::primary_key(const String &name) {
	return this;
}

TableBuilder *TableBuilder::next_row() {
	return this;
}

TableBuilder *TableBuilder::ccreate_table() {
	return this;
}

TableBuilder *TableBuilder::drop_table_if_exists() {
	return this;
}
TableBuilder *TableBuilder::drop_table(const String &name) {
	return this;
}
TableBuilder *TableBuilder::drop_table_if_exists(const String &name) {
	return this;
}
TableBuilder *TableBuilder::cdrop_table() {
	return this;
}

TableBuilder *TableBuilder::foreign_key(const String &name) {
	return this;
}
TableBuilder *TableBuilder::references(const String &table, const String &name) {
	return this;
}

TableBuilder *TableBuilder::end_command() {
	return this;
}

Ref<QueryResult> TableBuilder::run() {
	return Ref<QueryResult>();
}

void TableBuilder::run_query() {
}

void TableBuilder::reset() {
	result.clear();
}

void TableBuilder::print() {
	//printf("%s\n", result.get_data());
	print_error(result);
}

TableBuilder::TableBuilder() {
}

TableBuilder::~TableBuilder() {
}

void TableBuilder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_result"), &TableBuilder::get_result);
	ClassDB::bind_method(D_METHOD("set_result", "value"), &TableBuilder::set_result);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "result"), "set_result", "get_result");

	ClassDB::bind_method(D_METHOD("create_table", "table_name"), &TableBuilder::_create_table_bind);
	ClassDB::bind_method(D_METHOD("alter_table", "table_name"), &TableBuilder::_alter_table_bind);

	ClassDB::bind_method(D_METHOD("rename_to", "table_name"), &TableBuilder::_rename_to_bind);

	ClassDB::bind_method(D_METHOD("rename_column_to", "column_name", "new_column_name"), &TableBuilder::_rename_column_to_bind);
	ClassDB::bind_method(D_METHOD("add_column"), &TableBuilder::_add_column_bind);
	ClassDB::bind_method(D_METHOD("drop_column", "column_name"), &TableBuilder::_drop_column_bind);

	ClassDB::bind_method(D_METHOD("integer", "name", "length"), &TableBuilder::_integer_bind, -1);
	ClassDB::bind_method(D_METHOD("tiny_integer", "name", "length"), &TableBuilder::_tiny_integer_bind, -1);
	ClassDB::bind_method(D_METHOD("small_integer", "name", "length"), &TableBuilder::_small_integer_bind, -1);
	ClassDB::bind_method(D_METHOD("real_float", "name", "size", "d"), &TableBuilder::_real_float_bind, -1, -1);
	ClassDB::bind_method(D_METHOD("real_double", "name", "size", "d"), &TableBuilder::_real_double_bind, -1, -1);

	ClassDB::bind_method(D_METHOD("date", "name"), &TableBuilder::_date_bind);
	ClassDB::bind_method(D_METHOD("varchar", "name", "length"), &TableBuilder::_varchar_bind, -1);
	ClassDB::bind_method(D_METHOD("text", "name"), &TableBuilder::_text_bind);

	ClassDB::bind_method(D_METHOD("not_null"), &TableBuilder::_not_null_bind);
	ClassDB::bind_method(D_METHOD("null"), &TableBuilder::_null_bind);
	ClassDB::bind_method(D_METHOD("defval", "val"), &TableBuilder::_defval_bind);
	ClassDB::bind_method(D_METHOD("auto_increment"), &TableBuilder::_auto_increment_bind);
	ClassDB::bind_method(D_METHOD("primary_key", "name"), &TableBuilder::_primary_key_bind, "");
	ClassDB::bind_method(D_METHOD("next_row"), &TableBuilder::_next_row_bind);
	ClassDB::bind_method(D_METHOD("ccreate_table"), &TableBuilder::_ccreate_table_bind);

	ClassDB::bind_method(D_METHOD("drop_table", "name"), &TableBuilder::_drop_table_bind, "");
	ClassDB::bind_method(D_METHOD("drop_table_if_exists", "name"), &TableBuilder::_drop_table_if_exists_bind, "");
	ClassDB::bind_method(D_METHOD("cdrop_table"), &TableBuilder::_cdrop_table_bind);

	ClassDB::bind_method(D_METHOD("foreign_key", "name"), &TableBuilder::_foreign_key_bind);
	ClassDB::bind_method(D_METHOD("references", "table", "name"), &TableBuilder::_references_bind);

	ClassDB::bind_method(D_METHOD("end_command"), &TableBuilder::_end_command_bind);

	ClassDB::bind_method(D_METHOD("run"), &TableBuilder::run);
	ClassDB::bind_method(D_METHOD("run_query"), &TableBuilder::run_query);

	ClassDB::bind_method(D_METHOD("reset"), &TableBuilder::reset);
}

Ref<TableBuilder> TableBuilder::_create_table_bind(const String &name) {
	return Ref<TableBuilder>(create_table(name));
}

Ref<TableBuilder> TableBuilder::_alter_table_bind(const String &p_table_name) {
	return Ref<TableBuilder>(alter_table(p_table_name));
}

Ref<TableBuilder> TableBuilder::_rename_to_bind(const String &p_table_name) {
	return Ref<TableBuilder>(rename_to(p_table_name));
}

Ref<TableBuilder> TableBuilder::_rename_column_to_bind(const String &p_column_name, const String &p_new_column_name) {
	return Ref<TableBuilder>(rename_column_to(p_column_name, p_new_column_name));
}
Ref<TableBuilder> TableBuilder::_add_column_bind() {
	return Ref<TableBuilder>(add_column());
}
Ref<TableBuilder> TableBuilder::_drop_column_bind(const String &p_column_name) {
	return Ref<TableBuilder>(drop_column(p_column_name));
}

Ref<TableBuilder> TableBuilder::_integer_bind(const String &name, const int length) {
	return Ref<TableBuilder>(integer(name, length));
}
Ref<TableBuilder> TableBuilder::_tiny_integer_bind(const String &name, const int length) {
	return Ref<TableBuilder>(tiny_integer(name, length));
}
Ref<TableBuilder> TableBuilder::_small_integer_bind(const String &name, const int length) {
	return Ref<TableBuilder>(small_integer(name, length));
}
Ref<TableBuilder> TableBuilder::_real_float_bind(const String &name, const int size, const int d) {
	return Ref<TableBuilder>(real_float(name, size, d));
}
Ref<TableBuilder> TableBuilder::_real_double_bind(const String &name, const int size, const int d) {
	return Ref<TableBuilder>(real_double(name, size, d));
}

Ref<TableBuilder> TableBuilder::_date_bind(const String &name) {
	return Ref<TableBuilder>(date(name));
}

Ref<TableBuilder> TableBuilder::_varchar_bind(const String &name, const int length) {
	return Ref<TableBuilder>(varchar(name, length));
}
Ref<TableBuilder> TableBuilder::_text_bind(const String &name) {
	return Ref<TableBuilder>(text(name));
}

Ref<TableBuilder> TableBuilder::_not_null_bind() {
	return Ref<TableBuilder>(not_null());
}
Ref<TableBuilder> TableBuilder::_null_bind() {
	return Ref<TableBuilder>(null());
}
Ref<TableBuilder> TableBuilder::_defval_bind(const String &val) {
	return Ref<TableBuilder>(defval(val));
}
Ref<TableBuilder> TableBuilder::_auto_increment_bind() {
	return Ref<TableBuilder>(auto_increment());
}
Ref<TableBuilder> TableBuilder::_primary_key_bind(const String &name) {
	return Ref<TableBuilder>(primary_key(name));
}
Ref<TableBuilder> TableBuilder::_next_row_bind() {
	return Ref<TableBuilder>(next_row());
}
Ref<TableBuilder> TableBuilder::_ccreate_table_bind() {
	return Ref<TableBuilder>(ccreate_table());
}

Ref<TableBuilder> TableBuilder::_drop_table_bind(const String &name) {
	return Ref<TableBuilder>(drop_table(name));
}
Ref<TableBuilder> TableBuilder::_drop_table_if_exists_bind(const String &name) {
	return Ref<TableBuilder>(drop_table_if_exists(name));
}
Ref<TableBuilder> TableBuilder::_cdrop_table_bind() {
	return Ref<TableBuilder>(cdrop_table());
}

Ref<TableBuilder> TableBuilder::_foreign_key_bind(const String &name) {
	return Ref<TableBuilder>(foreign_key(name));
}
Ref<TableBuilder> TableBuilder::_references_bind(const String &table, const String &name) {
	return Ref<TableBuilder>(references(table, name));
}

Ref<TableBuilder> TableBuilder::_end_command_bind() {
	return Ref<TableBuilder>(end_command());
}
