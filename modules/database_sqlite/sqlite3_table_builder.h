#ifndef SQLITE3_TABLE_BUILDER_H
#define SQLITE3_TABLE_BUILDER_H

#include "core/string.h"

#include "database/table_builder.h"

class SQLite3Database;

class SQLite3TableBuilder : public TableBuilder {
	RCPP_OBJECT(SQLite3TableBuilder, TableBuilder);

public:
	TableBuilder *create_table(const String &name);

	TableBuilder *integer(const String &name);
	TableBuilder *integer(const String &name, const int length);

	TableBuilder *tiny_integer(const String &name);
	TableBuilder *tiny_integer(const String &name, const int length);

	TableBuilder *small_integer(const String &name);
	TableBuilder *small_integer(const String &name, const int length);

	TableBuilder *real_float(const String &name);
	TableBuilder *real_float(const String &name, const int size, const int d);

	TableBuilder *real_double(const String &name);
	TableBuilder *real_double(const String &name, const int size, const int d);

	TableBuilder *date(const String &name);

	TableBuilder *varchar(const String &name, const int length);
	TableBuilder *text(const String &name);

	TableBuilder *not_null();
	TableBuilder *null();
	TableBuilder *defval(const String &val);
	TableBuilder *auto_increment();
	TableBuilder *primary_key(const String &name);
	TableBuilder *primary_key();
	TableBuilder *next_row();
	TableBuilder *ccreate_table();

	TableBuilder *drop_table();
	TableBuilder *drop_table_if_exists();
	TableBuilder *drop_table(const String &name);
	TableBuilder *drop_table_if_exists(const String &name);
	TableBuilder *cdrop_table();

	TableBuilder *foreign_key(const String &name);
	TableBuilder *references(const String &table, const String &name);

	virtual Ref<QueryResult> run();
	virtual void run_query();

	SQLite3TableBuilder();
	virtual ~SQLite3TableBuilder();

	SQLite3Database *_db;
};

#endif