#ifndef SQLITE3_TABLE_BUILDER_H
#define SQLITE3_TABLE_BUILDER_H

#include "core/string/ustring.h"

#include "../database/table_builder.h"

class SQLite3DatabaseConnection;

class SQLite3TableBuilder : public TableBuilder {
	GDCLASS(SQLite3TableBuilder, TableBuilder);

public:
	TableBuilder *create_table(const String &name);

	TableBuilder *integer(const String &name, const int length = -1);
	TableBuilder *tiny_integer(const String &name, const int length = -1);
	TableBuilder *small_integer(const String &name, const int length = -1);
	TableBuilder *real_float(const String &name, const int size = -1, const int d = -1);
	TableBuilder *real_double(const String &name, const int size = -1, const int d = -1);

	TableBuilder *date(const String &name);

	TableBuilder *varchar(const String &name, const int length = -1);
	TableBuilder *text(const String &name);

	TableBuilder *not_null();
	TableBuilder *null();
	TableBuilder *defval(const String &val);
	TableBuilder *auto_increment();
	TableBuilder *primary_key(const String &name = "");
	TableBuilder *primary_key();
	TableBuilder *next_row();
	TableBuilder *ccreate_table();

	TableBuilder *drop_table();
	TableBuilder *drop_table_if_exists();
	TableBuilder *drop_table(const String &name = "");
	TableBuilder *drop_table_if_exists(const String &name);
	TableBuilder *cdrop_table();

	TableBuilder *foreign_key(const String &name);
	TableBuilder *references(const String &table, const String &name);

	Ref<QueryResult> run();
	void run_query();

	SQLite3TableBuilder();
	~SQLite3TableBuilder();

	Ref<SQLite3DatabaseConnection> _connection;
};

#endif
