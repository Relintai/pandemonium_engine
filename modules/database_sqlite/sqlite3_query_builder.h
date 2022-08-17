#ifndef SQLITE3_QUERY_BUILDER_H
#define SQLITE3_QUERY_BUILDER_H

#include "core/string/ustring.h"

#include "../database/query_builder.h"

class SQLite3DatabaseConnection;

class SQLite3QueryBuilder : public QueryBuilder {
	GDCLASS(SQLite3QueryBuilder, QueryBuilder);

public:
	QueryBuilder *select();
	QueryBuilder *update();
	QueryBuilder *del();

	QueryBuilder *cvalues();
	QueryBuilder *next_value();

	QueryBuilder *begin_transaction();
	QueryBuilder *commit();

	QueryBuilder *str();
	QueryBuilder *cstr();

	QueryBuilder *like(const String &str = "");

	QueryBuilder *nselect(const String &params);
	QueryBuilder *nupdate(const String &params);
	QueryBuilder *ndel(const String &params);

	QueryBuilder *nwhere(const String &params);
	QueryBuilder *nfrom(const String &params);

	QueryBuilder *insert(const String &table_name = "", const String &columns = "");
	QueryBuilder *nvalues(const String &params_str);
	QueryBuilder *val();
	QueryBuilder *nval(const String &param);

	QueryBuilder *vals(const String &param);
	QueryBuilder *vals(const char *param);
	QueryBuilder *vali(const int param);
	QueryBuilder *valb(const bool param);
	QueryBuilder *valf(const float param);
	QueryBuilder *vald(const double param);

	QueryBuilder *nlike(const String &str);

	QueryBuilder *sets();
	QueryBuilder *cset();

	QueryBuilder *nsetp(const String &col, const String &param);

	QueryBuilder *setps(const String &col, const char *param);
	QueryBuilder *setpi(const String &col, const int param);
	QueryBuilder *setpb(const String &col, const bool param);
	QueryBuilder *setpf(const String &col, const float param);
	QueryBuilder *setpd(const String &col, const double param);

	QueryBuilder *nwp(const String &col, const String &param);

	QueryBuilder *wps(const String &col, const char *param);
	QueryBuilder *wpi(const String &col, const int param);
	QueryBuilder *wpb(const String &col, const bool param);

	QueryBuilder *limit(const int num);
	QueryBuilder *offset(const int num);

	//l=logical (and, or are operators)
	QueryBuilder *land();
	QueryBuilder *lor();

	QueryBuilder *wildcard();

	QueryBuilder *select_last_insert_id();

	String escape(const String &params);

	QueryBuilder *prepare();
	QueryBuilder *set_param(const int index, const String &value);
	QueryBuilder *set_param(const int index, const int value);
	QueryBuilder *set_param(const int index, const float value);

	QueryBuilder *end_command();

	Ref<QueryResult> run();
	void run_query();

	SQLite3QueryBuilder();
	~SQLite3QueryBuilder();

	Ref<SQLite3DatabaseConnection> _connection;
};

#endif
