#ifndef QUERY_BUILDER_H
#define QUERY_BUILDER_H

#include "core/string.h"

#include "core/reference.h"

class QueryResult;

//methods that start with an e escape their params.

class QueryBuilder : public Reference {
	RCPP_OBJECT(QueryBuilder, Reference);

public:
	virtual QueryBuilder *select();
	virtual QueryBuilder *update();
	virtual QueryBuilder *del();

	virtual QueryBuilder *where();
	virtual QueryBuilder *from();
	virtual QueryBuilder *insert();
	virtual QueryBuilder *values();
	virtual QueryBuilder *cvalues();
	virtual QueryBuilder *next_value();
	
	virtual QueryBuilder *begin_transaction();
	virtual QueryBuilder *commit();

	virtual QueryBuilder *nl();

	virtual QueryBuilder *str();
	virtual QueryBuilder *cstr();

	virtual QueryBuilder *like();

	virtual QueryBuilder *select(const String &params);
	virtual QueryBuilder *update(const String &params);
	virtual QueryBuilder *del(const String &params);

	virtual QueryBuilder *where(const String &params);
	virtual QueryBuilder *from(const String &params);
	virtual QueryBuilder *insert(const String &table_name);
	virtual QueryBuilder *insert(const String &table_name, const String &columns);
	virtual QueryBuilder *values(const String &params_str);
	virtual QueryBuilder *val();
	virtual QueryBuilder *val(const String &param);
	virtual QueryBuilder *val(const char *param);
	virtual QueryBuilder *val(const int param);
	virtual QueryBuilder *val(const bool param);
	virtual QueryBuilder *valf(const float param);
	virtual QueryBuilder *vald(const double param);

	virtual QueryBuilder *like(const String &str);

	virtual QueryBuilder *set();
	virtual QueryBuilder *cset();

	virtual QueryBuilder *setp(const String &col, const String &param);
	virtual QueryBuilder *setp(const String &col, const char *param);
	virtual QueryBuilder *setp(const String &col, const int param);
	virtual QueryBuilder *setp(const String &col, const bool param);
	virtual QueryBuilder *setpf(const String &col, const float param);
	virtual QueryBuilder *setpd(const String &col, const double param);

	virtual QueryBuilder *wp(const String &col, const String &param);
	virtual QueryBuilder *wp(const String &col, const char *param);
	virtual QueryBuilder *wp(const String &col, const int param);
	virtual QueryBuilder *wp(const String &col, const bool param);

	virtual QueryBuilder *nselect(const String &params);
	virtual QueryBuilder *nupdate(const String &params);
	virtual QueryBuilder *ndel(const String &params);

	virtual QueryBuilder *nwhere(const String &params);
	virtual QueryBuilder *nfrom(const String &params);
	virtual QueryBuilder *nlike(const String &str);
	virtual QueryBuilder *nvalues(const String &params_str);
	virtual QueryBuilder *nval(const String &param);
	//note col is NOT escaped
	virtual QueryBuilder *nsetp(const String &col, const String &escape_param);
	//note col is NOT escaped
	virtual QueryBuilder *nwp(const String &col, const String &escape_param);

	virtual QueryBuilder *limit(const int num);
	virtual QueryBuilder *offset(const int num);

	virtual QueryBuilder *order_by_asc(const String &col);
	virtual QueryBuilder *order_by_desc(const String &col);
	virtual QueryBuilder *order_by(const String &col);

	virtual QueryBuilder *order_by();
	virtual QueryBuilder *corder_by();
	virtual QueryBuilder *asc();
	virtual QueryBuilder *desc();
	virtual QueryBuilder *order_by_add_col(const String &col);
	virtual QueryBuilder *asc(const String &col);
	virtual QueryBuilder *desc(const String &col);

	//l=logical (and, or are operators)
	virtual QueryBuilder *land();
	virtual QueryBuilder *lor();

	virtual QueryBuilder *wildcard();

	virtual QueryBuilder *w(const String &str);
	virtual QueryBuilder *ew(const String &str);

	virtual QueryBuilder *select_last_insert_id();

	virtual String escape(const String &params);

	virtual QueryBuilder *prepare();
	virtual QueryBuilder *set_param(const int index, const String &value);
	virtual QueryBuilder *set_param(const int index, const int value);
	virtual QueryBuilder *set_param(const int index, const float value);

	virtual QueryBuilder *end_command();

	virtual QueryBuilder *reset();

	virtual Ref<QueryResult> run();
	virtual void run_query();

	String get_result();

	void print();

	QueryBuilder();
	virtual ~QueryBuilder();

	String query_result;
};

#endif