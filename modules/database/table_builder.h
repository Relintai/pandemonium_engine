#ifndef TABLE_BUILDER_H
#define TABLE_BUILDER_H

/*************************************************************************/
/*  table_builder.h                                                      */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/string/ustring.h"

#include "core/object/reference.h"

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

	void clear();

	void print();

	TableBuilder();
	virtual ~TableBuilder();

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

	String result;
};

#endif
