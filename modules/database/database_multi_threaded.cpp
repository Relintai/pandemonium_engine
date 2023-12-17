/*************************************************************************/
/*  database_multi_threaded.cpp                                          */
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

#include "database_multi_threaded.h"

#include "database_connection.h"
#include "query_builder.h"
#include "query_result.h"
#include "table_builder.h"

Ref<DatabaseConnection> DatabaseMultiThreaded::get_connection() {
	_connection_map_lock.read_lock();

	Thread::ID tid = Thread::get_caller_id();
	RBMap<Thread::ID, Ref<DatabaseConnection>>::Element *e;
	e = _connections.find(tid);

	if (!e) {
		_connection_map_lock.read_unlock();

		_connection_map_lock.write_lock();
		Ref<DatabaseConnection> dbc = _allocate_connection();
		_connections.insert(tid, dbc);
		_connection_map_lock.write_unlock();

		return dbc;
	}

	Ref<DatabaseConnection> dbc = e->get();
	_connection_map_lock.read_unlock();

	return dbc;
}

Ref<DatabaseConnection> DatabaseMultiThreaded::_allocate_connection() {
	Ref<DatabaseConnection> dbc;
	dbc.instance();
	dbc->set_owner(this);
	dbc->database_connect(_connection_string);
	return dbc;
}

DatabaseMultiThreaded::DatabaseMultiThreaded() {
}

DatabaseMultiThreaded::~DatabaseMultiThreaded() {
	_connection_map_lock.write_lock();

	for (RBMap<Thread::ID, Ref<DatabaseConnection>>::Element *e = _connections.front(); e; e = e->next()) {
		Ref<DatabaseConnection> dbc = e->get();

		if (dbc.is_valid()) {
			dbc->set_owner(nullptr);
		}
	}

	_connections.clear();

	_connection_map_lock.write_unlock();
}

void DatabaseMultiThreaded::_bind_methods() {
}
