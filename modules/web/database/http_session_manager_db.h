#ifndef HTTP_SESSION_MANAGER_DB_H
#define HTTP_SESSION_MANAGER_DB_H

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

	virtual void migrate();
	virtual void create_table();
	virtual void drop_table();

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
