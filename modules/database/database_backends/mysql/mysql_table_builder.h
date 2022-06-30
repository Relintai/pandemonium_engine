#ifndef MYSQL_TABLE_BUILDER_H
#define MYSQL_TABLE_BUILDER_H

#include "core/string.h"

#include "database/table_builder.h"

class MysqlTableBuilder : public TableBuilder {
	RCPP_OBJECT(MysqlTableBuilder, TableBuilder);

public:
	TableBuilder *create_table(const String &name);
	TableBuilder *integer(const String &name);
	TableBuilder *date(const String &name);
	TableBuilder *varchar(const String &name, const int length);
	TableBuilder *not_null();
	TableBuilder *null();
	TableBuilder *auto_increment();
	TableBuilder *primary_key(const String &name);
	TableBuilder *primary_key();
	TableBuilder *next_row();

	TableBuilder *drop_table(const String &name);

	MysqlTableBuilder();
	virtual ~MysqlTableBuilder();
};

#endif