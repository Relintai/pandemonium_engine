#ifndef SQLITE3_PREPARED_STATEMENT_H
#define SQLITE3_PREPARED_STATEMENT_H

/*************************************************************************/
/*  query_result.h                                                       */
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

#include "modules/database/prepared_statement.h"

#include "./sqlite/sqlite3.h"

class SQLite3DatabaseConnection;

class SQLite3PreparedStatement : public PreparedStatement {
	GDCLASS(SQLite3PreparedStatement, PreparedStatement);

public:
	virtual String get_expanded_sql();
	virtual String get_normalized_sql();

	// Binding
	virtual Error bind_blob(const int p_index, const Vector<uint8_t> &p_value);
	virtual Error bind_float(const int p_index, const float p_value);
	virtual Error bind_double(const int p_index, const double p_value);
	virtual Error bind_int(const int p_index, const int p_value);
	virtual Error bind_int64(const int p_index, const int64_t p_value);
	virtual Error bind_null(const int p_index);
	virtual Error bind_text(const int p_index, const String &p_value);
	virtual Error bind_zeroblob(const int p_index, const int p_num);
	virtual Error bind_value(const int p_index, const Variant &p_value);

	virtual int bind_parameter_index(const String &p_name);
	virtual String bind_parameter_name(const int p_index);

	virtual int bind_parameter_count();

	virtual Error clear_bindings();

	// Querying
	virtual String column_name(const int p_index);
	virtual String column_decltype(const int p_index);
	virtual Type column_type(const int p_index);

	virtual String column_database_name(const int p_index);
	virtual String column_table_name(const int p_index);
	virtual String column_origin_name(const int p_index);

	virtual Vector<uint8_t> column_blob(const int p_index);
	virtual float column_float(const int p_index);
	virtual double column_double(const int p_index);
	virtual int64_t column_int(const int p_index);
	virtual int column_int64(const int p_index);
	virtual String column_text(const int p_index);
	virtual Variant column_value(const int p_index);

	virtual int column_count();

	virtual int get_last_insert_rowid();

	// Control
	virtual Error prepare();
	virtual Error step();
	virtual int data_count();
	virtual Error reset();
	virtual Error finalize();

	virtual Ref<DatabaseConnection> get_connection() const;

	SQLite3PreparedStatement();
	virtual ~SQLite3PreparedStatement();

	Ref<SQLite3DatabaseConnection> _connection;

protected:
	static void _bind_methods();

	sqlite3_stmt *_prepared_statement;
};

#endif
