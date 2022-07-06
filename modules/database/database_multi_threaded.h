#ifndef DATABASE_MULTI_THREADED_H
#define DATABASE_MULTI_THREADED_H

#include "core/map.h"
#include "core/os/rw_lock.h"
#include "core/os/thread.h"

#include "database.h"

class QueryBuilder;
class TableBuilder;
class QueryResult;
class DatabaseConnection;

class DatabaseMultiThreaded : public Database {
	GDCLASS(DatabaseMultiThreaded, Database);

public:
	Ref<DatabaseConnection> get_connection();

	DatabaseMultiThreaded();
	~DatabaseMultiThreaded();

protected:
	Ref<DatabaseConnection> _allocate_connection();

	static void _bind_methods();

	RWLock _connection_map_lock;
	Map<Thread::ID, Ref<DatabaseConnection>> _connections;
};

#endif
