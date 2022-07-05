#ifndef TABLE_BUILDER_H
#define TABLE_BUILDER_H

#include "core/ustring.h"

#include "core/reference.h"

class QueryResult;

class TableBuilder : public Reference {
	GDCLASS(TableBuilder, Reference);

public:
	String get_result();
	void set_result(const String &val);

	virtual TableBuilder *create_table(const String &name);

	virtual TableBuilder *integer(const String &name, const int length = -1);
	virtual TableBuilder *tiny_integer(const String &name, const int length = -1);
	virtual TableBuilder *small_integer(const String &name, const int length = -1);
	virtual TableBuilder *real_float(const String &name, const int size = -1, const int d = -1);
	virtual TableBuilder *real_double(const String &name, const int size = -1, const int d = -1);

	virtual TableBuilder *date(const String &name);

	virtual TableBuilder *varchar(const String &name, const int length = -1);
	virtual TableBuilder *text(const String &name);

	virtual TableBuilder *not_null();
	virtual TableBuilder *null();
	virtual TableBuilder *defval(const String &val);
	virtual TableBuilder *auto_increment();
	virtual TableBuilder *primary_key(const String &name = "");
	virtual TableBuilder *next_row();
	virtual TableBuilder *ccreate_table();

	virtual TableBuilder *drop_table_if_exists();
	virtual TableBuilder *drop_table(const String &name = "");
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

protected:
	static void _bind_methods();

	Ref<TableBuilder> _create_table_bind(const String &name);

	Ref<TableBuilder> _integer_bind(const String &name, const int length = -1);
	Ref<TableBuilder> _tiny_integer_bind(const String &name, const int length = -1);
	Ref<TableBuilder> _small_integer_bind(const String &name, const int length = -1);
	Ref<TableBuilder> _real_float_bind(const String &name, const int size = -1, const int d = -1);
	Ref<TableBuilder> _real_double_bind(const String &name, const int size = -1, const int d = -1);

	Ref<TableBuilder> _date_bind(const String &name);

	Ref<TableBuilder> _varchar_bind(const String &name, const int length = -1);
	Ref<TableBuilder> _text_bind(const String &name);

	Ref<TableBuilder> _not_null_bind();
	Ref<TableBuilder> _null_bind();
	Ref<TableBuilder> _defval_bind(const String &val);
	Ref<TableBuilder> _auto_increment_bind();
	Ref<TableBuilder> _primary_key_bind(const String &name = "");
	Ref<TableBuilder> _next_row_bind();
	Ref<TableBuilder> _ccreate_table_bind();

	Ref<TableBuilder> _drop_table_bind(const String &name = "");
	Ref<TableBuilder> _drop_table_if_exists_bind(const String &name);
	Ref<TableBuilder> _cdrop_table_bind();

	Ref<TableBuilder> _foreign_key_bind(const String &name);
	Ref<TableBuilder> _references_bind(const String &table, const String &name);
};

#endif
