#ifndef SQLITE3_DATABASE_H
#define SQLITE3_DATABASE_H

#include "../database/database_single_threaded.h"

class SQLite3Database : public DatabaseSingleThreaded {
	GDCLASS(SQLite3Database, DatabaseSingleThreaded);

public:
	virtual StringName get_backend_name() const;

	SQLite3Database();
	~SQLite3Database();

protected:
	Ref<DatabaseConnection> _allocate_connection();
};

#endif
