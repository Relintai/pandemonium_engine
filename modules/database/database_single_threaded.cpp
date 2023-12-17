/*************************************************************************/
/*  database_single_threaded.cpp                                         */
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

#include "database_single_threaded.h"

#include "database_connection.h"
#include "query_builder.h"
#include "query_result.h"
#include "table_builder.h"

Ref<DatabaseConnection> DatabaseSingleThreaded::get_connection() {
	if (!_connection.is_valid()) {
		_connection = _allocate_connection();
	}

	return _connection;
}

Ref<DatabaseConnection> DatabaseSingleThreaded::_allocate_connection() {
	Ref<DatabaseConnection> dbc;
	dbc.instance();
	dbc->set_owner(this);
	dbc->database_connect(_connection_string);
	return dbc;
}

DatabaseSingleThreaded::DatabaseSingleThreaded() {
}

DatabaseSingleThreaded::~DatabaseSingleThreaded() {
	if (_connection.is_valid()) {
		_connection->set_owner(nullptr);
	}

	_connection.unref();
}

void DatabaseSingleThreaded::_bind_methods() {
}
