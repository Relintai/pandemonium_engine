#ifndef TABLE_BUILDER_H
#define TABLE_BUILDER_H

#include "core/string.h"

#include "core/reference.h"

class QueryResult;

class TableBuilder : public Reference {
	RCPP_OBJECT(TableBuilder, Reference);
	
public:
	virtual TableBuilder *create_table(const String &name);
	
	virtual TableBuilder *integer(const String &name);
	virtual TableBuilder *integer(const String &name, const int length);

	virtual TableBuilder *tiny_integer(const String &name);
	virtual TableBuilder *tiny_integer(const String &name, const int length);

	virtual TableBuilder *small_integer(const String &name);
	virtual TableBuilder *small_integer(const String &name, const int length);

	virtual TableBuilder *real_float(const String &name);
	virtual TableBuilder *real_float(const String &name, const int size, const int d);

	virtual TableBuilder *real_double(const String &name);
	virtual TableBuilder *real_double(const String &name, const int size, const int d);

	virtual TableBuilder *date(const String &name);

	virtual TableBuilder *varchar(const String &name, const int length);
	virtual TableBuilder *text(const String &name);

	virtual TableBuilder *not_null();
	virtual TableBuilder *null();
	virtual TableBuilder *defval(const String &val);
	virtual TableBuilder *auto_increment();
	virtual TableBuilder *primary_key(const String &name);
	virtual TableBuilder *primary_key();
	virtual TableBuilder *next_row();
	virtual TableBuilder *ccreate_table();

	virtual TableBuilder *drop_table();
	virtual TableBuilder *drop_table_if_exists();
	virtual TableBuilder *drop_table(const String &name);
	virtual TableBuilder *drop_table_if_exists(const String &name);
	virtual TableBuilder *cdrop_table();

	virtual TableBuilder *foreign_key(const String &name);
	virtual TableBuilder *references(const String &table, const String &name);

	virtual Ref<QueryResult> run();
	virtual void run_query();

	void print();

	TableBuilder();
	virtual ~TableBuilder();

	String result;
};

#endif