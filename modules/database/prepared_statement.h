#ifndef PREPARED_STATEMENT_H
#define PREPARED_STATEMENT_H

/*************************************************************************/
/*  prepared_statement.h                                                 */
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

class DatabaseConnection;

class PreparedStatement : public Reference {
	GDCLASS(PreparedStatement, Reference);

public:
	enum Type {
		TYPE_NULL,
		TYPE_BLOB,
		TYPE_FLOAT,
		TYPE_DOUBLE,
		TYPE_INT,
		TYPE_INT64,
		TYPE_TEXT,
		TYPE_VARCHAR,
		TYPE_VALUE,
		TYPE_BYTES,
		TYPE_TYPE,
		TYPE_UNKNOWN,
	};

	String get_sql();
	void set_sql(const String &p_sql);

	virtual String get_expanded_sql() = 0;
	virtual String get_normalized_sql() = 0;

	// Binding
	virtual Error bind_blob(const int p_index, const Vector<uint8_t> &p_value) = 0;
	virtual Error bind_float(const int p_index, const float p_value) = 0;
	virtual Error bind_double(const int p_index, const double p_value) = 0;
	virtual Error bind_int(const int p_index, const int p_value) = 0;
	virtual Error bind_int64(const int p_index, const int64_t p_value) = 0;
	virtual Error bind_null(const int p_index) = 0;
	virtual Error bind_text(const int p_index, const String &p_value) = 0;
	virtual Error bind_zeroblob(const int p_index, const int p_num) = 0;
	virtual Error bind_value(const int p_index, const Variant &p_value) = 0;

	virtual int bind_parameter_index(const String &p_name) = 0;
	virtual String bind_parameter_name(const int p_index) = 0;

	virtual int bind_parameter_count() = 0;

	virtual Error clear_bindings() = 0;

	// Querying
	virtual String column_name(const int p_index) = 0;
	virtual String column_decltype(const int p_index) = 0;
	virtual Type column_type(const int p_index) = 0;

	virtual String column_database_name(const int p_index) = 0;
	virtual String column_table_name(const int p_index) = 0;
	virtual String column_origin_name(const int p_index) = 0;

	virtual Vector<uint8_t> column_blob(const int p_index) = 0;
	virtual float column_float(const int p_index) = 0;
	virtual double column_double(const int p_index) = 0;
	virtual int64_t column_int(const int p_index) = 0;
	virtual int column_int64(const int p_index) = 0;
	virtual String column_text(const int p_index) = 0;
	virtual Variant column_value(const int p_index) = 0;

	virtual int column_count() = 0;

	virtual int get_last_insert_rowid() = 0;

	// Control
	virtual Error prepare() = 0;
	virtual Error step() = 0;
	virtual int data_count() = 0;
	virtual Error reset() = 0;
	virtual Error finalize() = 0;

	virtual Ref<DatabaseConnection> get_connection() const = 0;

	PreparedStatement();
	virtual ~PreparedStatement();

protected:
	static void _bind_methods();

	String _sql;
};

VARIANT_ENUM_CAST(PreparedStatement::Type);

#endif
