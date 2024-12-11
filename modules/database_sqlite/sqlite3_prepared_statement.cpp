/*************************************************************************/
/*  query_result.cpp                                                     */
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

#include "sqlite3_prepared_statement.h"

#include "sqlite3_connection.h"

String SQLite3PreparedStatement::get_expanded_sql() {
	if (!_prepared_statement) {
		return String();
	}

	char *res = sqlite3_expanded_sql(_prepared_statement);
	String r = String::utf8(res);
	sqlite3_free(res);

	return r;
}
String SQLite3PreparedStatement::get_normalized_sql() {
#ifdef SQLITE_ENABLE_NORMALIZE
	if (!_prepared_statement) {
		return String();
	}

	char *res = sqlite3_normalized_sql(_prepared_statement);
	String r = String::utf8(res);
	sqlite3_free(res);

	return r;
#else
	return String();
#endif
}

// Binding
Error SQLite3PreparedStatement::bind_blob(const int p_index, const Vector<uint8_t> &p_value) {
	if (!_prepared_statement) {
		return ERR_UNCONFIGURED;
	}

	int res = sqlite3_bind_blob(_prepared_statement, p_index, p_value.ptr(), p_value.size(), SQLITE_TRANSIENT);

	if (res != SQLITE_OK) {
		return FAILED;
	}

	return OK;
}
Error SQLite3PreparedStatement::bind_float(const int p_index, const float p_value) {
	return bind_double(p_index, p_value);
}
Error SQLite3PreparedStatement::bind_double(const int p_index, const double p_value) {
	if (!_prepared_statement) {
		return ERR_UNCONFIGURED;
	}

	int res = sqlite3_bind_double(_prepared_statement, p_index, p_value);

	if (res != SQLITE_OK) {
		return FAILED;
	}

	return OK;
}
Error SQLite3PreparedStatement::bind_int(const int p_index, const int p_value) {
	if (!_prepared_statement) {
		return ERR_UNCONFIGURED;
	}

	int res = sqlite3_bind_int(_prepared_statement, p_index, p_value);

	if (res != SQLITE_OK) {
		return FAILED;
	}

	return OK;
}
Error SQLite3PreparedStatement::bind_int64(const int p_index, const int64_t p_value) {
	if (!_prepared_statement) {
		return ERR_UNCONFIGURED;
	}

	int res = sqlite3_bind_int64(_prepared_statement, p_index, p_value);

	if (res != SQLITE_OK) {
		return FAILED;
	}

	return OK;
}
Error SQLite3PreparedStatement::bind_null(const int p_index) {
	if (!_prepared_statement) {
		return ERR_UNCONFIGURED;
	}

	int res = sqlite3_bind_null(_prepared_statement, p_index);

	if (res != SQLITE_OK) {
		return FAILED;
	}

	return OK;
}
Error SQLite3PreparedStatement::bind_text(const int p_index, const String &p_value) {
	if (!_prepared_statement) {
		return ERR_UNCONFIGURED;
	}

	CharString cs = p_value.utf8();

	//cs.size() - 1 -> Don't send the null terminator, as then the text will be interpreted as blob
	int res = sqlite3_bind_text(_prepared_statement, p_index, cs.get_data(), cs.size() - 1, SQLITE_TRANSIENT);

	if (res != SQLITE_OK) {
		return FAILED;
	}

	return OK;
}
Error SQLite3PreparedStatement::bind_zeroblob(const int p_index, const int p_num) {
	if (!_prepared_statement) {
		return ERR_UNCONFIGURED;
	}

	int res = sqlite3_bind_zeroblob(_prepared_statement, p_index, p_num);

	if (res != SQLITE_OK) {
		return FAILED;
	}

	return OK;
}
Error SQLite3PreparedStatement::bind_value(const int p_index, const Variant &p_value) {
	switch (p_value.get_type()) {
		case Variant::NIL:
			return bind_null(p_index);

		case Variant::BOOL:
		case Variant::INT:
			return bind_int(p_index, p_value);

		case Variant::REAL:
			return bind_double(p_index, p_value);

		case Variant::STRING:
		case Variant::RECT2: // 5
		case Variant::RECT2I:
		case Variant::VECTOR2:
		case Variant::VECTOR2I:
		case Variant::VECTOR3:
		case Variant::VECTOR3I: // 10
		case Variant::VECTOR4:
		case Variant::VECTOR4I:
		case Variant::PLANE:
		case Variant::QUATERNION:
		case Variant::AABB: // 15
		case Variant::BASIS:
		case Variant::TRANSFORM:
		case Variant::TRANSFORM2D:
		case Variant::PROJECTION:
		case Variant::COLOR:
		case Variant::NODE_PATH:
		case Variant::RID:
		case Variant::OBJECT:
		case Variant::STRING_NAME:
		case Variant::DICTIONARY:
		case Variant::ARRAY:
		case Variant::POOL_INT_ARRAY:
		case Variant::POOL_REAL_ARRAY:
		case Variant::POOL_STRING_ARRAY:
		case Variant::POOL_VECTOR2_ARRAY:
		case Variant::POOL_VECTOR2I_ARRAY:
		case Variant::POOL_VECTOR3_ARRAY:
		case Variant::POOL_VECTOR3I_ARRAY:
		case Variant::POOL_VECTOR4_ARRAY:
		case Variant::POOL_VECTOR4I_ARRAY:
		case Variant::POOL_COLOR_ARRAY:
		case Variant::VARIANT_MAX:
			return bind_text(p_index, p_value);

		case Variant::POOL_BYTE_ARRAY:
			return bind_blob(p_index, p_value);

		default:
			break;
	}

	return ERR_UNAVAILABLE;
}

int SQLite3PreparedStatement::bind_parameter_index(const String &p_name) {
	if (!_prepared_statement) {
		return -1;
	}

	CharString cs = p_name.utf8();

	return sqlite3_bind_parameter_index(_prepared_statement, cs.get_data());
}
String SQLite3PreparedStatement::bind_parameter_name(const int p_index) {
	if (!_prepared_statement) {
		return String();
	}

	// don't free it
	const char *pname = sqlite3_bind_parameter_name(_prepared_statement, p_index);

	return String::utf8(pname);
}

int SQLite3PreparedStatement::bind_parameter_count() {
	if (!_prepared_statement) {
		return ERR_UNCONFIGURED;
	}

	return sqlite3_bind_parameter_count(_prepared_statement);
}

Error SQLite3PreparedStatement::clear_bindings() {
	if (!_prepared_statement) {
		return ERR_UNCONFIGURED;
	}

	int res = sqlite3_clear_bindings(_prepared_statement);

	if (res != SQLITE_OK) {
		return FAILED;
	}

	return OK;
}

// Querying
String SQLite3PreparedStatement::column_name(const int p_index) {
	if (!_prepared_statement) {
		return String();
	}

	// don't free it
	const char *cname = sqlite3_column_name(_prepared_statement, p_index);

	return String::utf8(cname);
}
String SQLite3PreparedStatement::column_decltype(const int p_index) {
	if (!_prepared_statement) {
		return String();
	}

	// don't free it
	const char *cname = sqlite3_column_decltype(_prepared_statement, p_index);

	return String::utf8(cname);
}
PreparedStatement::Type SQLite3PreparedStatement::column_type(const int p_index) {
	if (!_prepared_statement) {
		return TYPE_UNKNOWN;
	}

	int type = sqlite3_column_type(_prepared_statement, p_index);

	switch (type) {
		case SQLITE_BLOB:
			return TYPE_BLOB;
		case SQLITE_FLOAT:
			return TYPE_DOUBLE;
		case SQLITE_INTEGER:
			return TYPE_INT;
		case SQLITE_TEXT:
			return TYPE_TEXT;
		case SQLITE_NULL:
			return TYPE_NULL;
		default:
			break;
	}

	return TYPE_UNKNOWN;
}

String SQLite3PreparedStatement::column_database_name(const int p_index) {
#ifdef SQLITE_ENABLE_COLUMN_METADATA
	if (!_prepared_statement) {
		return String();
	}

	// don't free it
	const char *name = sqlite3_column_database_name(_prepared_statement, p_index);

	return String::utf8(name);
#else
	return String();
#endif
}
String SQLite3PreparedStatement::column_table_name(const int p_index) {
#ifdef SQLITE_ENABLE_COLUMN_METADATA
	if (!_prepared_statement) {
		return String();
	}

	// don't free it
	const char *name = sqlite3_column_table_name(_prepared_statement, p_index);

	return String::utf8(name);
#else
	return String();
#endif
}
String SQLite3PreparedStatement::column_origin_name(const int p_index) {
#ifdef SQLITE_ENABLE_COLUMN_METADATA
	if (!_prepared_statement) {
		return String();
	}

	// don't free it
	const char *name = sqlite3_column_origin_name(_prepared_statement, p_index);

	return String::utf8(name);
#else
	return String();
#endif
}

Vector<uint8_t> SQLite3PreparedStatement::column_blob(const int p_index) {
	if (!_prepared_statement) {
		return Vector<uint8_t>();
	}

	const uint8_t *val = (const uint8_t *)sqlite3_column_blob(_prepared_statement, p_index);

	if (!val) {
		return Vector<uint8_t>();
	}

	int size = sqlite3_column_bytes(_prepared_statement, p_index);

	Vector<uint8_t> r;
	r.resize(size);

	memcpy(r.ptrw(), val, size);

	return r;
}
float SQLite3PreparedStatement::column_float(const int p_index) {
	return static_cast<float>(column_double(p_index));
}
double SQLite3PreparedStatement::column_double(const int p_index) {
	if (!_prepared_statement) {
		return 0;
	}

	return sqlite3_column_double(_prepared_statement, p_index);
}
int64_t SQLite3PreparedStatement::column_int(const int p_index) {
	if (!_prepared_statement) {
		return 0;
	}

	return sqlite3_column_int(_prepared_statement, p_index);
}
int SQLite3PreparedStatement::column_int64(const int p_index) {
	if (!_prepared_statement) {
		return 0;
	}

	return sqlite3_column_int64(_prepared_statement, p_index);
}
String SQLite3PreparedStatement::column_text(const int p_index) {
	if (!_prepared_statement) {
		return String();
	}

	const char *val = (const char *)sqlite3_column_text(_prepared_statement, p_index);

	if (!val) {
		return String();
	}

	return String::utf8(val);
}

Variant SQLite3PreparedStatement::column_value(const int p_index) {
	if (!_prepared_statement) {
		return Variant();
	}

	switch (column_type(p_index)) {
		case TYPE_BLOB:
			return Variant(column_blob(p_index));
		case TYPE_DOUBLE:
			return Variant(column_double(p_index));
			break;
		case TYPE_INT:
			return Variant(column_int(p_index));
			break;
		case TYPE_TEXT:
			return Variant(column_text(p_index));
			break;
		case TYPE_NULL:
		default:
			break;
	}

	return Variant();
}

int SQLite3PreparedStatement::column_count() {
	if (!_prepared_statement) {
		return 0;
	}

	return sqlite3_column_count(_prepared_statement);
}

int SQLite3PreparedStatement::get_last_insert_rowid() {
	ERR_FAIL_COND_V(!_connection.is_valid(), 0);

	return sqlite3_last_insert_rowid(_connection->conn);
}

// Control
Error SQLite3PreparedStatement::prepare() {
	ERR_FAIL_COND_V(!_connection.is_valid(), FAILED);

	_current_column_index = 0;

	CharString cs = _sql.utf8();

	int res = sqlite3_prepare_v2(_connection->conn, cs.get_data(), cs.size(), &_prepared_statement, NULL);

	if (res != SQLITE_OK) {
		return FAILED;
	}

	return OK;
}
Error SQLite3PreparedStatement::step() {
	if (!_prepared_statement) {
		return ERR_UNCONFIGURED;
	}

	_current_column_index = 0;

	int res = sqlite3_step(_prepared_statement);

	if (res == SQLITE_ROW) {
		return OK;
	}

	if (res != SQLITE_OK) {
		return FAILED;
	}

	return OK;
}
int SQLite3PreparedStatement::data_count() {
	if (!_prepared_statement) {
		return 0;
	}

	return sqlite3_data_count(_prepared_statement);
}
Error SQLite3PreparedStatement::reset() {
	if (!_prepared_statement) {
		return ERR_UNCONFIGURED;
	}

	_current_column_index = 0;

	int res = sqlite3_reset(_prepared_statement);

	if (res != SQLITE_OK) {
		return FAILED;
	}

	return OK;
}
Error SQLite3PreparedStatement::finalize() {
	if (!_prepared_statement) {
		return OK;
	}

	_current_column_index = 0;

	sqlite3_finalize(_prepared_statement);

	_prepared_statement = NULL;

	return OK;
}

Ref<DatabaseConnection> SQLite3PreparedStatement::get_connection() const {
	return _connection;
}

SQLite3PreparedStatement::SQLite3PreparedStatement() {
	_prepared_statement = NULL;
}

SQLite3PreparedStatement::~SQLite3PreparedStatement() {
	finalize();

	_prepared_statement = NULL;
}

void SQLite3PreparedStatement::_bind_methods() {
}
