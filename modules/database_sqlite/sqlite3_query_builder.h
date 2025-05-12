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
	QueryBuilder *vali(const int param);
	QueryBuilder *valb(const bool param);
	QueryBuilder *valf(const float param);
	QueryBuilder *vald(const double param);
	virtual QueryBuilder *valph();
	virtual QueryBuilder *valphi(const String &p_id);
	virtual QueryBuilder *valphr(const String &p_raw_id);

	QueryBuilder *nlike(const String &str);

	QueryBuilder *sets();
	QueryBuilder *cset();

	QueryBuilder *nsetp(const String &col, const String &param);

	QueryBuilder *setps(const String &col, const String &param);
	QueryBuilder *setpi(const String &col, const int param);
	QueryBuilder *setpb(const String &col, const bool param);
	QueryBuilder *setpf(const String &col, const float param);
	QueryBuilder *setpd(const String &col, const double param);
	virtual QueryBuilder *setph(const String &col);
	virtual QueryBuilder *setphi(const String &col, const String &p_id);
	virtual QueryBuilder *setphr(const String &col, const String &p_raw_id);

	QueryBuilder *nwp(const String &col, const String &param);

	QueryBuilder *wps(const String &col, const String &param);
	QueryBuilder *wpi(const String &col, const int param);
	QueryBuilder *wpb(const String &col, const bool param);
	virtual QueryBuilder *wph(const String &col);
	virtual QueryBuilder *wphi(const String &col, const String &p_id);
	virtual QueryBuilder *wphr(const String &col, const String &p_raw_id);

	QueryBuilder *limit(const int num);
	QueryBuilder *offset(const int num);

	//l=logical (and, or are operators)
	QueryBuilder *land();
	QueryBuilder *lor();

	QueryBuilder *wildcard();

	// Prepared statement placeholders
	virtual QueryBuilder *psph();
	virtual QueryBuilder *psphi(const String &p_id);
	virtual QueryBuilder *psphr(const String &p_raw_id);

	QueryBuilder *select_last_insert_id();

	String escape(const String &params);

	QueryBuilder *end_command();

	Ref<QueryResult> run();
	void run_query();

	Ref<PreparedStatement> create_prepared_statement();

	SQLite3QueryBuilder();
	~SQLite3QueryBuilder();

	Ref<SQLite3DatabaseConnection> _connection;
};

#endif
