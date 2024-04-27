/*************************************************************************/
/*  query_result.cpp                                                     */
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

PreparedStatement::PreparedStatement() {
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

	ClassDB::bind_method(D_METHOD("bind_parameter_index", "name"), &PreparedStatement::bind_parameter_index);
	ClassDB::bind_method(D_METHOD("bind_parameter_name", "index"), &PreparedStatement::bind_parameter_name);

	ClassDB::bind_method(D_METHOD("bind_parameter_count"), &PreparedStatement::bind_parameter_count);

	ClassDB::bind_method(D_METHOD("clear_bindings"), &PreparedStatement::clear_bindings);

	// Querying

	ClassDB::bind_method(D_METHOD("column_name", "index"), &PreparedStatement::column_name);
	ClassDB::bind_method(D_METHOD("column_decltype", "index"), &PreparedStatement::column_decltype);
	ClassDB::bind_method(D_METHOD("column_type", "index"), &PreparedStatement::column_type);

	ClassDB::bind_method(D_METHOD("column_database_name", "index"), &PreparedStatement::column_database_name);
	ClassDB::bind_method(D_METHOD("column_table_name", "index"), &PreparedStatement::column_table_name);
	ClassDB::bind_method(D_METHOD("column_origin_name", "index"), &PreparedStatement::column_origin_name);

	ClassDB::bind_method(D_METHOD("column_blob", "index"), &PreparedStatement::column_blob);
	ClassDB::bind_method(D_METHOD("column_float", "index"), &PreparedStatement::column_float);
	ClassDB::bind_method(D_METHOD("column_double", "index"), &PreparedStatement::column_double);
	ClassDB::bind_method(D_METHOD("column_int", "index"), &PreparedStatement::column_int);
	ClassDB::bind_method(D_METHOD("column_int64", "index"), &PreparedStatement::column_int64);
	ClassDB::bind_method(D_METHOD("column_text", "index"), &PreparedStatement::column_text);
	ClassDB::bind_method(D_METHOD("column_value", "index"), &PreparedStatement::column_value);

	ClassDB::bind_method(D_METHOD("column_count"), &PreparedStatement::column_count);

	// Control

	ClassDB::bind_method(D_METHOD("prepare"), &PreparedStatement::prepare);
	ClassDB::bind_method(D_METHOD("step"), &PreparedStatement::step);
	ClassDB::bind_method(D_METHOD("data_count"), &PreparedStatement::data_count);
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
