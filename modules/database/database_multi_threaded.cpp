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
