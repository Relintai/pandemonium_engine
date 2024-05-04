#ifndef WEB_NODE_H
#define WEB_NODE_H

/*************************************************************************/
/*  web_node.h                                                           */
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

#include "../../modules_enabled.gen.h"
#include "core/containers/hash_map.h"
#include "core/object/reference.h"
#include "core/os/rw_lock.h"
#include "core/variant/variant.h"
#include "scene/main/node.h"

class WebServerRequest;
class WebServer;
class WebPermission;

#ifdef MODULE_DATABASE_ENABLED
class Database;
class TableBuilder;
class QueryBuilder;
class DatabaseConnection;
#endif

//note add an alternative queue_delete() -> (with different name) -> if called node tells parent to stop getting routed to, and when rwlock can be locked gets queue_deleted.
//Also add an rwlock just around the handler map (if it's not done already), and add api to lock and unlock it

class WebNode : public Node {
	GDCLASS(WebNode, Node);

public:
	enum {
		NOTIFICATION_WEB_NODE_WRITE_LOCKED = 2100,
	};

	String get_uri_segment();
	void set_uri_segment(const String &val);

	String get_full_uri(const bool slash_at_the_end = true);
	String get_full_uri_parent(const bool slash_at_the_end = true);

	Ref<WebPermission> get_web_permission();
	void set_web_permission(const Ref<WebPermission> &wp);

	bool get_routing_enabled();
	void set_routing_enabled(const bool value);

#ifdef MODULE_DATABASE_ENABLED
	String get_database_table_name();
	void set_database_table_name(const String &val);

	Ref<Database> get_database();
	void set_database(const Ref<Database> &db);

	Ref<DatabaseConnection> get_database_connection();
	Ref<TableBuilder> get_table_builder();
	Ref<QueryBuilder> get_query_builder();

	bool get_migrations_enabled();
	void set_migrations_enabled(const bool p_enabled);
#endif

	void handle_request_main(Ref<WebServerRequest> request);
	virtual void _handle_request_main(Ref<WebServerRequest> request);

	void handle_request(Ref<WebServerRequest> request);
	virtual void _handle_request(Ref<WebServerRequest> request);

	void handle_error_send_request(Ref<WebServerRequest> request, const int error_code);
	virtual void _handle_error_send_request(Ref<WebServerRequest> request, const int error_code);

	void render_index(Ref<WebServerRequest> request);
	void render_preview(Ref<WebServerRequest> request);
	void render_menu(Ref<WebServerRequest> request);
	void render_main_menu(Ref<WebServerRequest> request);

	virtual void _render_index(Ref<WebServerRequest> request);
	virtual void _render_preview(Ref<WebServerRequest> request);
	virtual void _render_menu(Ref<WebServerRequest> request);
	virtual void _render_main_menu(Ref<WebServerRequest> request);

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

	bool try_route_request_to_children(Ref<WebServerRequest> request);
	WebNode *get_request_handler_child(Ref<WebServerRequest> request);
	void build_handler_map();
	void clear_handlers();

	void request_write_lock();

	WebServer *get_server();
	WebNode *get_web_root();
	WebNode *get_parent_webnode();

	void add_child_notify(Node *p_child);
	void remove_child_notify(Node *p_child);

	WebNode();
	~WebNode();

protected:
	void _notification(const int what);

	static void _bind_methods();

	String _uri_segment;

#ifdef MODULE_DATABASE_ENABLED
	String _database_table_name;
	Ref<Database> _database;
	int _migrations_enabled;
#endif

	bool _routing_enabled;
	WebNode *_index_node;
	HashMap<String, WebNode *> _node_route_map;
	RWLock _handler_map_lock;

	Ref<WebPermission> _web_permission;

	bool _write_lock_requested;
	RWLock _rw_lock;
};

#endif
