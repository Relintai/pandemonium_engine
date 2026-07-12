/*************************************************************************/
/*  prepared_statement.cpp                                               */
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

#include "prepared_statement.h"

#include "database_connection.h"

String PreparedStatement::get_sql() {
	return _sql;
}
void PreparedStatement::set_sql(const String &p_sql) {
	_sql = p_sql;
}

int PreparedStatement::get_current_column_index() const {
	return _current_column_index;
}
void PreparedStatement::set_current_column_index(const int p_index) {
	_current_column_index = p_index;
}

String PreparedStatement::get_next_column_name() {
	return column_name_get(_current_column_index++);
}
String PreparedStatement::get_next_column_decltype() {
	return column_decltype_get(_current_column_index++);
}
PreparedStatement::Type PreparedStatement::get_next_column_type() {
	return column_type_get(_current_column_index++);
}

String PreparedStatement::get_next_column_database_name() {
	return column_database_name_get(_current_column_index++);
}
String PreparedStatement::get_next_column_table_name() {
	return column_table_name_get(_current_column_index++);
}
String PreparedStatement::get_next_column_origin_name() {
	return column_origin_name_get(_current_column_index++);
}

String PreparedStatement::get_current_column_name() {
	return column_name_get(_current_column_index);
}
String PreparedStatement::get_current_column_decltype() {
	return column_decltype_get(_current_column_index);
}
PreparedStatement::Type PreparedStatement::get_current_column_type() {
	return column_type_get(_current_column_index);
}

String PreparedStatement::get_current_column_database_name() {
	return column_database_name_get(_current_column_index);
}
String PreparedStatement::get_current_column_table_name() {
	return column_table_name_get(_current_column_index);
}
String PreparedStatement::get_current_column_origin_name() {
	return column_origin_name_get(_current_column_index);
}

Vector<uint8_t> PreparedStatement::get_next_column_blob() {
	return column_blob_get(_current_column_index++);
}
float PreparedStatement::get_next_column_float() {
	return column_float_get(_current_column_index++);
}
double PreparedStatement::get_next_column_double() {
	return column_double_get(_current_column_index++);
}
int64_t PreparedStatement::get_next_column_int() {
	return column_int_get(_current_column_index++);
}
int PreparedStatement::get_next_column_int64() {
	return column_int64_get(_current_column_index++);
}
String PreparedStatement::get_next_column_text() {
	return column_text_get(_current_column_index++);
}
Variant PreparedStatement::get_next_column_value() {
	return column_value_get(_current_column_index++);
}

// Binding auto next api
Error PreparedStatement::set_next_bind_blob(const Vector<uint8_t> &p_value) {
	return bind_blob(_current_column_index++, p_value);
}
Error PreparedStatement::set_next_bind_float(const float p_value) {
	return bind_float(_current_column_index++, p_value);
}
Error PreparedStatement::set_next_bind_double(const double p_value) {
	return bind_double(_current_column_index++, p_value);
}
Error PreparedStatement::set_next_bind_int(const int p_value) {
	return bind_int(_current_column_index++, p_value);
}
Error PreparedStatement::set_next_bind_int64(const int64_t p_value) {
	return bind_int64(_current_column_index++, p_value);
}
Error PreparedStatement::set_next_bind_null() {
	return bind_null(_current_column_index++);
}
Error PreparedStatement::set_next_bind_text(const String &p_value) {
	return bind_text(_current_column_index++, p_value);
}
Error PreparedStatement::set_next_bind_zeroblob(const int p_num) {
	return bind_zeroblob(_current_column_index++, p_num);
}
Error PreparedStatement::set_next_bind_value(const Variant &p_value) {
	return bind_value(_current_column_index++, p_value);
}

int PreparedStatement::next_column() {
	return ++_current_column_index;
}

bool PreparedStatement::has_data() {
	return get_data_count() > 0;
}

PreparedStatement::PreparedStatement() {
	_current_column_index = 0;
}

PreparedStatement::~PreparedStatement() {
}

void PreparedStatement::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_sql"), &PreparedStatement::get_sql);
	ClassDB::bind_method(D_METHOD("set_sql"), &PreparedStatement::set_sql);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "sql"), "set_sql", "get_sql");

	ClassDB::bind_method(D_METHOD("get_expanded_sql"), &PreparedStatement::get_expanded_sql);
	ClassDB::bind_method(D_METHOD("get_normalized_sql"), &PreparedStatement::get_normalized_sql);

	// Binding

	ClassDB::bind_method(D_METHOD("bind_blob", "index", "value"), &PreparedStatement::bind_blob);
	ClassDB::bind_method(D_METHOD("bind_float", "index", "value"), &PreparedStatement::bind_float);
	ClassDB::bind_method(D_METHOD("bind_double", "index", "value"), &PreparedStatement::bind_double);
	ClassDB::bind_method(D_METHOD("bind_int", "index", "value"), &PreparedStatement::bind_int);
	ClassDB::bind_method(D_METHOD("bind_int64", "index", "value"), &PreparedStatement::bind_int64);
	ClassDB::bind_method(D_METHOD("bind_null", "index"), &PreparedStatement::bind_null);
	ClassDB::bind_method(D_METHOD("bind_text", "index", "value"), &PreparedStatement::bind_text);
	ClassDB::bind_method(D_METHOD("bind_zeroblob", "index", "num"), &PreparedStatement::bind_zeroblob);
	ClassDB::bind_method(D_METHOD("bind_value", "index", "value"), &PreparedStatement::bind_value);

	ClassDB::bind_method(D_METHOD("get_bind_parameter_index", "name"), &PreparedStatement::get_bind_parameter_index);
	ClassDB::bind_method(D_METHOD("get_bind_parameter_name", "index"), &PreparedStatement::get_bind_parameter_name);

	ClassDB::bind_method(D_METHOD("get_bind_parameter_count"), &PreparedStatement::get_bind_parameter_count);

	ClassDB::bind_method(D_METHOD("clear_bindings"), &PreparedStatement::clear_bindings);

	// Querying

	ClassDB::bind_method(D_METHOD("column_name_get", "index"), &PreparedStatement::column_name_get);
	ClassDB::bind_method(D_METHOD("column_decltype_get", "index"), &PreparedStatement::column_decltype_get);
	ClassDB::bind_method(D_METHOD("column_type_get", "index"), &PreparedStatement::column_type_get);

	ClassDB::bind_method(D_METHOD("column_database_name_get", "index"), &PreparedStatement::column_database_name_get);
	ClassDB::bind_method(D_METHOD("column_table_name_get", "index"), &PreparedStatement::column_table_name_get);
	ClassDB::bind_method(D_METHOD("column_origin_name_get", "index"), &PreparedStatement::column_origin_name_get);

	ClassDB::bind_method(D_METHOD("column_blob_get", "index"), &PreparedStatement::column_blob_get);
	ClassDB::bind_method(D_METHOD("column_float_get", "index"), &PreparedStatement::column_float_get);
	ClassDB::bind_method(D_METHOD("column_double_get", "index"), &PreparedStatement::column_double_get);
	ClassDB::bind_method(D_METHOD("column_int_get", "index"), &PreparedStatement::column_int_get);
	ClassDB::bind_method(D_METHOD("column_int64_get", "index"), &PreparedStatement::column_int64_get);
	ClassDB::bind_method(D_METHOD("column_text_get", "index"), &PreparedStatement::column_text_get);
	ClassDB::bind_method(D_METHOD("column_value_get", "index"), &PreparedStatement::column_value_get);

	ClassDB::bind_method(D_METHOD("get_column_count"), &PreparedStatement::get_column_count);

	ClassDB::bind_method(D_METHOD("get_last_insert_rowid"), &PreparedStatement::get_last_insert_rowid);

	ClassDB::bind_method(D_METHOD("get_current_column_index"), &PreparedStatement::get_current_column_index);
	ClassDB::bind_method(D_METHOD("set_current_column_index", "index"), &PreparedStatement::set_current_column_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_column_index"), "set_current_column_index", "get_current_column_index");

	ClassDB::bind_method(D_METHOD("get_next_column_name"), &PreparedStatement::get_next_column_name);
	ClassDB::bind_method(D_METHOD("get_next_column_decltype"), &PreparedStatement::get_next_column_decltype);
	ClassDB::bind_method(D_METHOD("get_next_column_type"), &PreparedStatement::get_next_column_type);

	ClassDB::bind_method(D_METHOD("get_next_column_database_name"), &PreparedStatement::get_next_column_database_name);
	ClassDB::bind_method(D_METHOD("get_next_column_table_name"), &PreparedStatement::get_next_column_table_name);
	ClassDB::bind_method(D_METHOD("get_next_column_origin_name"), &PreparedStatement::get_next_column_origin_name);

	ClassDB::bind_method(D_METHOD("get_current_column_name"), &PreparedStatement::get_current_column_name);
	ClassDB::bind_method(D_METHOD("get_current_column_decltype"), &PreparedStatement::get_current_column_decltype);
	ClassDB::bind_method(D_METHOD("get_current_column_type"), &PreparedStatement::get_current_column_type);

	ClassDB::bind_method(D_METHOD("get_current_column_database_name"), &PreparedStatement::get_current_column_database_name);
	ClassDB::bind_method(D_METHOD("get_current_column_table_name"), &PreparedStatement::get_current_column_table_name);
	ClassDB::bind_method(D_METHOD("get_current_column_origin_name"), &PreparedStatement::get_current_column_origin_name);

	ClassDB::bind_method(D_METHOD("get_next_column_blob"), &PreparedStatement::get_next_column_blob);
	ClassDB::bind_method(D_METHOD("get_next_column_float"), &PreparedStatement::get_next_column_float);
	ClassDB::bind_method(D_METHOD("get_next_column_double"), &PreparedStatement::get_next_column_double);
	ClassDB::bind_method(D_METHOD("get_next_column_int"), &PreparedStatement::get_next_column_int);
	ClassDB::bind_method(D_METHOD("get_next_column_int64"), &PreparedStatement::get_next_column_int64);
	ClassDB::bind_method(D_METHOD("get_next_column_text"), &PreparedStatement::get_next_column_text);
	ClassDB::bind_method(D_METHOD("get_next_column_value"), &PreparedStatement::get_next_column_value);

	// Binding auto next api

	ClassDB::bind_method(D_METHOD("set_next_bind_blob", "value"), &PreparedStatement::set_next_bind_blob);
	ClassDB::bind_method(D_METHOD("set_next_bind_float", "value"), &PreparedStatement::set_next_bind_float);
	ClassDB::bind_method(D_METHOD("set_next_bind_double", "value"), &PreparedStatement::set_next_bind_double);
	ClassDB::bind_method(D_METHOD("set_next_bind_int", "value"), &PreparedStatement::set_next_bind_int);
	ClassDB::bind_method(D_METHOD("set_next_bind_int64", "value"), &PreparedStatement::set_next_bind_int64);
	ClassDB::bind_method(D_METHOD("set_next_bind_null"), &PreparedStatement::set_next_bind_null);
	ClassDB::bind_method(D_METHOD("set_next_bind_text", "value"), &PreparedStatement::set_next_bind_text);
	ClassDB::bind_method(D_METHOD("set_next_bind_zeroblob", "value"), &PreparedStatement::set_next_bind_zeroblob);
	ClassDB::bind_method(D_METHOD("set_next_bind_value", "value"), &PreparedStatement::set_next_bind_value);

	ClassDB::bind_method(D_METHOD("next_column"), &PreparedStatement::next_column);

	ClassDB::bind_method(D_METHOD("has_data"), &PreparedStatement::has_data);

	// Control

	ClassDB::bind_method(D_METHOD("prepare"), &PreparedStatement::prepare);
	ClassDB::bind_method(D_METHOD("step"), &PreparedStatement::step);
	ClassDB::bind_method(D_METHOD("get_data_count"), &PreparedStatement::get_data_count);
	ClassDB::bind_method(D_METHOD("reset"), &PreparedStatement::reset);
	ClassDB::bind_method(D_METHOD("finalize"), &PreparedStatement::finalize);

	ClassDB::bind_method(D_METHOD("get_connection"), &PreparedStatement::get_connection);

	BIND_ENUM_CONSTANT(TYPE_NULL);
	BIND_ENUM_CONSTANT(TYPE_BLOB);
	BIND_ENUM_CONSTANT(TYPE_FLOAT);
	BIND_ENUM_CONSTANT(TYPE_DOUBLE);
	BIND_ENUM_CONSTANT(TYPE_INT);
	BIND_ENUM_CONSTANT(TYPE_INT64);
	BIND_ENUM_CONSTANT(TYPE_TEXT);
	BIND_ENUM_CONSTANT(TYPE_VARCHAR);
	BIND_ENUM_CONSTANT(TYPE_VALUE);
	BIND_ENUM_CONSTANT(TYPE_BYTES);
	BIND_ENUM_CONSTANT(TYPE_TYPE);
	BIND_ENUM_CONSTANT(TYPE_UNKNOWN);
}
