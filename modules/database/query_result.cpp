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

#include "query_result.h"

bool QueryResult::next_row() {
	_current_cell_index = 0;

	return false;
}

String QueryResult::get_cell(const int index) {
	return String();
}

bool QueryResult::get_cell_bool(const int index) {
	return get_cell_int(index);
}

int QueryResult::get_cell_int(const int index) {
	if (is_cell_null(index)) {
		return 0;
	}

	return get_cell(index).to_int();
}

float QueryResult::get_cell_float(const int index) {
	if (is_cell_null(index)) {
		return 0;
	}

	return get_cell(index).to_float();
}
double QueryResult::get_cell_double(const int index) {
	if (is_cell_null(index)) {
		return 0;
	}

	return get_cell(index).to_double();
}

bool QueryResult::is_cell_null(const int index) {
	return true;
}

int QueryResult::get_current_cell_index() const {
	return  _current_cell_index;
}
void QueryResult::set_current_cell_index(const int p_index) {
	_current_cell_index = p_index;
}

String QueryResult::get_next_cell() {
	return get_cell(_current_cell_index++);
}
bool QueryResult::get_next_cell_bool() {
	return get_cell_bool(_current_cell_index++);
}
int QueryResult::get_next_cell_int() {
	return get_cell_int(_current_cell_index++);
}
float QueryResult::get_next_cell_float() {
	return get_cell_float(_current_cell_index++);
}
double QueryResult::get_next_cell_double() {
	return get_cell_double(_current_cell_index++);
}
bool QueryResult::is_next_cell_null() {
	return is_cell_null(_current_cell_index++);
}

int QueryResult::get_last_insert_rowid() {
	return 0;
}

String QueryResult::get_error_message() {
	return "";
}

QueryResult::QueryResult() {
	_current_cell_index = 0;
}

QueryResult::~QueryResult() {
}

void QueryResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("next_row"), &QueryResult::next_row);

	ClassDB::bind_method(D_METHOD("get_cell", "index"), &QueryResult::get_cell);
	ClassDB::bind_method(D_METHOD("get_cell_bool", "index"), &QueryResult::get_cell_bool);
	ClassDB::bind_method(D_METHOD("get_cell_int", "index"), &QueryResult::get_cell_int);
	ClassDB::bind_method(D_METHOD("get_cell_float", "index"), &QueryResult::get_cell_float);
	ClassDB::bind_method(D_METHOD("get_cell_double", "index"), &QueryResult::get_cell_double);

	ClassDB::bind_method(D_METHOD("is_cell_null", "index"), &QueryResult::is_cell_null);

	ClassDB::bind_method(D_METHOD("get_current_cell_index"), &QueryResult::get_current_cell_index);
	ClassDB::bind_method(D_METHOD("set_current_cell_index", "index"), &QueryResult::set_current_cell_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_cell_index"), "set_current_cell_index", "get_current_cell_index");

	ClassDB::bind_method(D_METHOD("get_next_cell"), &QueryResult::get_next_cell);
	ClassDB::bind_method(D_METHOD("get_next_cell_bool"), &QueryResult::get_next_cell_bool);
	ClassDB::bind_method(D_METHOD("get_next_cell_int"), &QueryResult::get_next_cell_int);
	ClassDB::bind_method(D_METHOD("get_next_cell_float"), &QueryResult::get_next_cell_float);
	ClassDB::bind_method(D_METHOD("get_next_cell_double"), &QueryResult::get_next_cell_double);
	ClassDB::bind_method(D_METHOD("is_next_cell_null"), &QueryResult::is_next_cell_null);

	ClassDB::bind_method(D_METHOD("get_last_insert_rowid"), &QueryResult::get_last_insert_rowid);

	ClassDB::bind_method(D_METHOD("get_error_message"), &QueryResult::get_error_message);
}
