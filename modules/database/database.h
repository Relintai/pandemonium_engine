#ifndef DATABASE_H
#define DATABASE_H

#include "core/ustring.h"

#include "core/reference.h"

class DatabaseConnection;

class Database : public Reference {
	GDCLASS(Database, Reference);

public:
	String get_connection_string();
	void set_connection_string(const String &val);

	virtual Ref<DatabaseConnection> get_connection();

	Database();
	~Database();

protected:
	virtual Ref<DatabaseConnection> _allocate_connection();

	static void _bind_methods();

	String _connection_string;
};

#endif
