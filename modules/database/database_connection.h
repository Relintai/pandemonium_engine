#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include "core/ustring.h"

#include "core/reference.h"

class QueryBuilder;
class TableBuilder;
class QueryResult;

class DatabaseConnection : public Reference {
	GDCLASS(DatabaseConnection, Reference);

public:
	virtual void connect(const String &connection_str);
	virtual Ref<QueryResult> query(const String &query);
	virtual void query_run(const String &query);

	virtual Ref<QueryBuilder> get_query_builder();
	virtual Ref<TableBuilder> get_table_builder();

	virtual String escape(const String &str);
	virtual void escape_to(const String &str, String *to);

	DatabaseConnection();
	~DatabaseConnection();

protected:
	static void _bind_methods();
};

#endif
