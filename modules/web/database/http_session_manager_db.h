#ifndef HTTP_SESSION_MANAGER_DB_H
#define HTTP_SESSION_MANAGER_DB_H

/*************************************************************************/
/*  http_session_manager_db.h                                            */
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

#include "core/containers/hash_map.h"
#include "core/containers/vector.h"
#include "core/os/mutex.h"
#include "core/string/ustring.h"

#include "core/object/reference.h"
#include "scene/main/node.h"

#include "../http/http_session_manager.h"

#include "../http/web_server_middleware.h"

class HTTPSession;
class WebServerRequest;
class Database;
class DatabaseConnection;
class TableBuilder;
class QueryBuilder;

class HTTPSessionManagerDB : public HTTPSessionManager {
	GDCLASS(HTTPSessionManagerDB, HTTPSessionManager);

public:
	String get_database_table_name();
	void set_database_table_name(const String &val);

	String get_database_data_table_name();
	void set_database_data_table_name(const String &val);

	Ref<Database> get_database();
	void set_database(const Ref<Database> &db);

	Ref<DatabaseConnection> get_database_connection();
	Ref<TableBuilder> get_table_builder();
	Ref<QueryBuilder> get_query_builder();

	Ref<HTTPSession> delete_session(const String &session_id);
	void save_session(Ref<HTTPSession> session);

	void load_sessions();

	void create_table();
	void drop_table();
	void update_table(const int p_current_table_version);
	void create_default_entries(const int p_seed);

	virtual void _create_table();
	virtual void _drop_table();
	virtual void _update_table(const int p_current_table_version);
	virtual void _create_default_entries(const int p_seed);

	void migrate(const bool p_clear, const bool p_should_seed, const int p_seed);
	virtual void _migrate(const bool p_clear, const bool p_should_seed, const int p_seed);

	HTTPSessionManagerDB();
	~HTTPSessionManagerDB();

protected:
	void _notification(const int what);

	static void _bind_methods();

	String _database_table_name;
	String _database_data_table_name;
	Ref<Database> _database;
};

#endif
