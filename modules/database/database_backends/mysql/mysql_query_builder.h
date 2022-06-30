#ifndef MYSQL_QUERY_BUILDER_H
#define MYSQL_QUERY_BUILDER_H

#include <memory>
#include "core/string.h"

#include "database/query_builder.h"

class MysqlDatabase;
class MysqlQueryResult;

class MysqlQueryBuilder : public QueryBuilder {
	RCPP_OBJECT(MysqlQueryBuilder, QueryBuilder);

public:
	QueryBuilder *select();
	QueryBuilder *where();
	QueryBuilder *from();
	QueryBuilder *insert();
	QueryBuilder *values();
	QueryBuilder *cvalues();

	QueryBuilder *nselect(const String &params);
	QueryBuilder *nwhere(const String &params);
	QueryBuilder *nfrom(const String &params);
	QueryBuilder *ninsert(const String &table_name);
	QueryBuilder *nvalues(const String &params_str);

	QueryBuilder *limit(const int num);
	QueryBuilder *offset(const int num);

	String escape(const String &params);

	QueryBuilder *prepare();
	QueryBuilder *set_param(const int index, const String &value);
	QueryBuilder *set_param(const int index, const int value);
	QueryBuilder *set_param(const int index, const float value);

	QueryBuilder *end_command();

	Ref<QueryResult> run();
	void run_query();

	MysqlQueryBuilder();
	~MysqlQueryBuilder();

	MysqlDatabase *_db;
};

#endif