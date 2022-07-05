#ifndef DATABASE_H
#define DATABASE_H

#include "core/string.h"
#include <memory>

#include "core/reference.h"

class QueryBuilder;
class TableBuilder;
class QueryResult;

class Database {
public:
	//probably needs refcount, depending on what db engines do, todo
	//virtual QueryResult *query(const String &sql);
	//virtual QueryResult *query_async(const String &sql);
	//or
	//virtual QueryErrorCode query(QueryResult *result, const String &sql);
	//virtual QueryErrorCode query_async(QueryResult *result, const String &sql);

	//also
	//virtual QueryResult *query_prepared(const String &sql, param1, param2, ...);

    //query interface (codeigniter 3 style)
    //virtual void where(""); etc

	virtual void connect(const String &connection_str);
	virtual Ref<QueryResult> query(const String &query);
	virtual void query_run(const String &query);

	virtual Ref<QueryBuilder> get_query_builder();
	virtual Ref<TableBuilder> get_table_builder();

	virtual String escape(const String str);
	virtual void escape(const String str, String *to);

	Database();
	~Database();

private:
	//std::vector<QueryBuilder *> _builders;
};

#endif