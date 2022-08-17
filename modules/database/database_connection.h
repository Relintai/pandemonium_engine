#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include "core/ustring.h"

#include "core/object/reference.h"

class QueryBuilder;
class TableBuilder;
class QueryResult;
class Database;

class DatabaseConnection : public Reference {
	GDCLASS(DatabaseConnection, Reference);

public:
	virtual void database_connect(const String &connection_str);
	virtual Ref<QueryResult> query(const String &query);
	virtual void query_run(const String &query);

	virtual Ref<QueryBuilder> get_query_builder();
	virtual Ref<TableBuilder> get_table_builder();

	virtual String escape(const String &str);
	virtual void escape_to(const String &str, String *to);

	virtual int get_table_version(const String &table);
	virtual void set_table_version(const String &table, const int version);
	virtual void ensure_version_table_exists();

	Ref<Database> get_owner();
	void set_owner(Database *owner);

	DatabaseConnection();
	~DatabaseConnection();

protected:
	static void _bind_methods();

	//"WeakRef"
	//Note: Set this to null if the owner Database gets destroyed!
	Database *_owner;
};

#endif
