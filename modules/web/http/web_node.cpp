
#include "web_node.h"

#include "core/error/error_macros.h"

#include "core/object/object.h"
#include "core/string/print_string.h"
#include "http_server_enums.h"
#include "modules/database/database_connection.h"
#include "web_server_request.h"

#include "web_permission.h"
#include "web_server.h"

#ifdef MODULE_DATABASE_ENABLED
#include "../../database/database.h"
#include "../../database/database_manager.h"
#include "../../database/query_builder.h"
#include "../../database/query_result.h"
#include "../../database/table_builder.h"
#endif

String WebNode::get_uri_segment() {
	return _uri_segment;
}
void WebNode::set_uri_segment(const String &val) {
	_uri_segment = val;
}

String WebNode::get_full_uri(const bool slash_at_the_end) {
	if (slash_at_the_end) {
		return get_full_uri_parent(true) + _uri_segment + "/";
	} else {
		return get_full_uri_parent(true) + _uri_segment;
	}
}

String WebNode::get_full_uri_parent(const bool slash_at_the_end) {
	_rw_lock.read_lock();

	String uri = "/";

	WebNode *n = get_parent_webnode();

	while (n) {
		if (n->_uri_segment == "" || n->_uri_segment == "/") {
			break;
		}

		uri = "/" + n->_uri_segment + uri;

		n = n->get_parent_webnode();
	}

	_rw_lock.read_unlock();

	if (!slash_at_the_end) {
		uri.resize(uri.length() - 1);
	}

	return uri;
}

Ref<WebPermission> WebNode::get_web_permission() {
	return _web_permission;
}
void WebNode::set_web_permission(const Ref<WebPermission> &wp) {
	_web_permission = wp;
}

bool WebNode::get_routing_enabled() {
	return _routing_enabled;
}
void WebNode::set_routing_enabled(const bool value) {
	if (_routing_enabled == value) {
		return;
	}

	_routing_enabled = value;

	if (!_routing_enabled) {
		clear_handlers();
	} else {
		if (is_inside_tree()) {
			build_handler_map();
		}
	}
}

#ifdef MODULE_DATABASE_ENABLED

String WebNode::get_database_table_name() {
	return _database_table_name;
}
void WebNode::set_database_table_name(const String &val) {
	_database_table_name = val;
}

Ref<Database> WebNode::get_database() {
	if (_database.is_valid()) {
		return _database;
	}

	return DatabaseManager::get_singleton()->get_ddb();
}

void WebNode::set_database(const Ref<Database> &db) {
	_database = db;

	// todo send event to children when it's implemented?
}

Ref<DatabaseConnection> WebNode::get_database_connection() {
	Ref<Database> db = get_database();

	ERR_FAIL_COND_V(!db.is_valid(), Ref<DatabaseConnection>());

	Ref<DatabaseConnection> conn = db->get_connection();

	ERR_FAIL_COND_V(!conn.is_valid(), Ref<DatabaseConnection>());

	return conn;
}

Ref<TableBuilder> WebNode::get_table_builder() {
	Ref<Database> db = get_database();

	ERR_FAIL_COND_V(!db.is_valid(), Ref<TableBuilder>());

	Ref<DatabaseConnection> conn = db->get_connection();

	ERR_FAIL_COND_V(!conn.is_valid(), Ref<TableBuilder>());

	return conn->get_table_builder();
}

Ref<QueryBuilder> WebNode::get_query_builder() {
	Ref<Database> db = get_database();

	ERR_FAIL_COND_V(!db.is_valid(), Ref<QueryBuilder>());

	Ref<DatabaseConnection> conn = db->get_connection();

	ERR_FAIL_COND_V(!conn.is_valid(), Ref<QueryBuilder>());

	return conn->get_query_builder();
}

bool WebNode::get_migrations_enabled() {
	return _migrations_enabled;
}

void WebNode::set_migrations_enabled(const bool p_enabled) {
	if (_migrations_enabled == p_enabled) {
		return;
	}

	if (is_inside_tree()) {
		//Note that this assumes that we returned early if p_enabled and _migrations_enabled are the same.
		if (_migrations_enabled) {
			DatabaseManager::get_singleton()->disconnect("migration", this, "migrate");
		} else {
			DatabaseManager::get_singleton()->connect("migration", this, "migrate");
		}
	}

	_migrations_enabled = p_enabled;
}

#endif

void WebNode::handle_request_main(Ref<WebServerRequest> request) {
	_rw_lock.read_lock();
	call("_handle_request_main", request);
	_rw_lock.read_unlock();
}

void WebNode::_handle_request_main(Ref<WebServerRequest> request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	if (!_routing_enabled) {
		handle_request(request);
		return;
	}

	if (!try_route_request_to_children(request)) {
		handle_request(request);
	}
}

void WebNode::handle_request(Ref<WebServerRequest> request) {
	call("_handle_request", request);
}

void WebNode::_handle_request(Ref<WebServerRequest> request) {
	request->send_error(HTTPServerEnums::HTTP_STATUS_CODE_404_NOT_FOUND);
}

void WebNode::handle_error_send_request(Ref<WebServerRequest> request, const int error_code) {
	call("_handle_error_send_request", request, error_code);
}

void WebNode::_handle_error_send_request(Ref<WebServerRequest> request, const int error_code) {
	// this is a fallback error handler.
	// Webroot implements a proper one
	request->compiled_body = "<html><body>Internal server error!</body></html>";
	request->set_status_code(HTTPServerEnums::HTTP_STATUS_CODE_503_SERVICE_UNAVAILABLE);
	request->send();
}

void WebNode::render_index(Ref<WebServerRequest> request) {
	call("_render_index", request);
}
void WebNode::render_preview(Ref<WebServerRequest> request) {
	call("_render_preview", request);
}
void WebNode::render_menu(Ref<WebServerRequest> request) {
	call("_render_menu", request);
}
void WebNode::render_main_menu(Ref<WebServerRequest> request) {
	call("_render_main_menu", request);
}

void WebNode::_render_index(Ref<WebServerRequest> request) {
}
void WebNode::_render_preview(Ref<WebServerRequest> request) {
}
void WebNode::_render_menu(Ref<WebServerRequest> request) {
	WebNode *root = get_web_root();

	if (root) {
		root->render_main_menu(request);
	}
}
void WebNode::_render_main_menu(Ref<WebServerRequest> request) {
}

void WebNode::create_table() {
	call("_create_table");
}
void WebNode::drop_table() {
	call("_drop_table");
}
void WebNode::update_table(const int p_current_table_version) {
	call("_update_table", p_current_table_version);
}
void WebNode::create_default_entries(const int p_seed) {
	call("_create_default_entries", p_seed);
}

void WebNode::_create_table() {
}
void WebNode::_drop_table() {
}
void WebNode::_update_table(const int p_current_table_version) {
}
void WebNode::_create_default_entries(const int p_seed) {
}

void WebNode::migrate(const bool p_clear, const bool p_should_seed, const int p_seed) {
	call("_migrate", p_clear, p_should_seed, p_seed);
}

void WebNode::_migrate(const bool p_clear, const bool p_should_seed, const int p_seed) {
	if (p_clear) {
		drop_table();
		create_table();
	} else {
#ifdef MODULE_DATABASE_ENABLED
		Ref<DatabaseConnection> conn = get_database_connection();
		ERR_FAIL_COND(!conn.is_valid());
		int ver = conn->get_table_version(_database_table_name);
		update_table(ver);
#else
		update_table(0);
#endif
	}

	if (p_should_seed) {
		create_default_entries(p_seed);
	}
}

bool WebNode::try_route_request_to_children(Ref<WebServerRequest> request) {
	WebNode *handler = nullptr;

	_handler_map_lock.read_lock();

	// if (path == "/") {
	if (request->get_path_segment_count() == 0) {
		// quick shortcut
		handler = _index_node;
	} else {
		String main_route = request->get_current_path_segment();

		handler = _node_route_map[main_route];
	}

	_handler_map_lock.read_unlock();

	if (!handler) {
		return false;
	}

	request->push_path();
	handler->handle_request_main(request);

	return true;
}

WebNode *WebNode::get_request_handler_child(Ref<WebServerRequest> request) {
	WebNode *handler = nullptr;

	_handler_map_lock.read_lock();

	// if (path == "/") {
	if (request->get_path_segment_count() == 0) {
		// quick shortcut
		handler = _index_node;
	} else {
		const String &main_route = request->get_current_path_segment();
		handler = _node_route_map[main_route];
	}

	_handler_map_lock.read_unlock();

	return handler;
}

void WebNode::build_handler_map() {
	_handler_map_lock.write_lock();

	_index_node = nullptr;
	_node_route_map.clear();

	for (int i = 0; i < get_child_count(); ++i) {
		WebNode *c = Object::cast_to<WebNode>(get_child(i));

		if (!c) {
			continue;
		}

		String uri_segment = c->get_uri_segment();

		if (uri_segment == "") {
			// ignore
			continue;
		} else if (uri_segment == "/") {
			ERR_CONTINUE_MSG(_index_node, "You have multiple root nodes!");

			_index_node = c;
		} else {
			ERR_CONTINUE_MSG(_node_route_map[uri_segment], "You have multiple of the same uri! URI: " + uri_segment);

			_node_route_map[uri_segment] = c;
		}
	}

	_handler_map_lock.write_unlock();
}

void WebNode::clear_handlers() {
	_handler_map_lock.write_lock();

	_index_node = nullptr;
	_node_route_map.clear();

	_handler_map_lock.write_unlock();
}

void WebNode::request_write_lock() {
	_write_lock_requested = true;
}

WebServer *WebNode::get_server() {
	Node *n = this;

	while (n) {
		WebServer *ws = Object::cast_to<WebServer>(n);

		if (ws) {
			return ws;
		}

		n = n->get_parent();
	}

	return nullptr;
}

WebNode *WebNode::get_web_root() {
	WebServer *s = get_server();

	if (!s) {
		return nullptr;
	}

	return s->get_web_root();
}

WebNode *WebNode::get_parent_webnode() {
	return Object::cast_to<WebNode>(get_parent());
}

void WebNode::add_child_notify(Node *p_child) {
	if (_routing_enabled && is_inside_tree()) {
		build_handler_map();
	}
}
void WebNode::remove_child_notify(Node *p_child) {
	if (_routing_enabled && is_inside_tree()) {
		build_handler_map();
	}
}

void WebNode::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE: {
			if (_routing_enabled) {
				build_handler_map();
			}

#ifdef MODULE_DATABASE_ENABLED
			if (_migrations_enabled) {
				DatabaseManager::get_singleton()->connect("migration", this, "migrate");
			}
#endif
		} break;
		case NOTIFICATION_EXIT_TREE: {
#ifdef MODULE_DATABASE_ENABLED
			if (_migrations_enabled) {
				DatabaseManager::get_singleton()->disconnect("migration", this, "migrate");
			}
#endif
		} break;
		case NOTIFICATION_INTERNAL_PROCESS: {
			if (_write_lock_requested) {
				_rw_lock.write_lock();
				_write_lock_requested = false;
				notification(NOTIFICATION_WEB_NODE_WRITE_LOCKED);
				_rw_lock.write_unlock();
			}
		} break;
		default:
			break;
	}
}

WebNode::WebNode() {
	// should look this up in parents when parented (and node parenting is implemented)
	// should have an event later when a parent gets one
	//#ifdef MODULE_DATABASE_ENABLED
	//_database = nullptr;
	//#endif

	_routing_enabled = true;
	_index_node = nullptr;

#ifdef MODULE_DATABASE_ENABLED
	_migrations_enabled = false;
#endif

	_write_lock_requested = false;
	set_process_internal(true);
}

WebNode::~WebNode() {
}

void WebNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_uri_segment"), &WebNode::get_uri_segment);
	ClassDB::bind_method(D_METHOD("set_uri_segment", "val"), &WebNode::set_uri_segment);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "uri_segment"), "set_uri_segment", "get_uri_segment");

	ClassDB::bind_method(D_METHOD("get_full_uri", "slash_at_the_end "), &WebNode::get_full_uri, true);
	ClassDB::bind_method(D_METHOD("get_full_uri_parent", "slash_at_the_end "), &WebNode::get_full_uri_parent, true);

	ClassDB::bind_method(D_METHOD("get_web_permission"), &WebNode::get_web_permission);
	ClassDB::bind_method(D_METHOD("set_web_permission", "val"), &WebNode::set_web_permission);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "web_permission", PROPERTY_HINT_RESOURCE_TYPE, "WebPermission"), "set_web_permission", "get_web_permission");

	ClassDB::bind_method(D_METHOD("get_routing_enabled"), &WebNode::get_routing_enabled);
	ClassDB::bind_method(D_METHOD("set_routing_enabled", "val"), &WebNode::set_routing_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "routing_enabled"), "set_routing_enabled", "get_routing_enabled");

#ifdef MODULE_DATABASE_ENABLED
	ClassDB::bind_method(D_METHOD("get_database_table_name"), &WebNode::get_database_table_name);
	ClassDB::bind_method(D_METHOD("set_database_table_name", "val"), &WebNode::set_database_table_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "database_table_name"), "set_database_table_name", "get_database_table_name");

	ClassDB::bind_method(D_METHOD("get_database"), &WebNode::get_database);
	ClassDB::bind_method(D_METHOD("set_database", "val"), &WebNode::set_database);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "database", PROPERTY_HINT_RESOURCE_TYPE, "Database", 0), "set_database", "get_database");

	ClassDB::bind_method(D_METHOD("get_database_connection"), &WebNode::get_database_connection);
	ClassDB::bind_method(D_METHOD("get_table_builder"), &WebNode::get_table_builder);
	ClassDB::bind_method(D_METHOD("get_query_builder"), &WebNode::get_query_builder);

	ClassDB::bind_method(D_METHOD("get_migrations_enabled"), &WebNode::get_migrations_enabled);
	ClassDB::bind_method(D_METHOD("set_migrations_enabled", "val"), &WebNode::set_migrations_enabled);
#endif

	BIND_VMETHOD(MethodInfo("_handle_request_main", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));
	ClassDB::bind_method(D_METHOD("handle_request_main", "request"), &WebNode::handle_request_main);
	ClassDB::bind_method(D_METHOD("_handle_request_main", "request"), &WebNode::_handle_request_main);

	BIND_VMETHOD(MethodInfo("_handle_request", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));
	ClassDB::bind_method(D_METHOD("handle_request", "request"), &WebNode::handle_request);
	ClassDB::bind_method(D_METHOD("_handle_request", "request"), &WebNode::_handle_request);

	BIND_VMETHOD(MethodInfo("_handle_error_send_request", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest"), PropertyInfo(Variant::INT, "error_code")));
	ClassDB::bind_method(D_METHOD("handle_error_send_request", "request", "error_code"), &WebNode::handle_error_send_request);
	ClassDB::bind_method(D_METHOD("_handle_error_send_request", "request", "error_code"), &WebNode::_handle_error_send_request);

	BIND_VMETHOD(MethodInfo("_render_index", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));
	BIND_VMETHOD(MethodInfo("_render_preview", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));
	BIND_VMETHOD(MethodInfo("_render_menu", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));
	BIND_VMETHOD(MethodInfo("_render_main_menu", PropertyInfo(Variant::OBJECT, "request", PROPERTY_HINT_RESOURCE_TYPE, "WebServerRequest")));

	ClassDB::bind_method(D_METHOD("render_index", "request"), &WebNode::render_index);
	ClassDB::bind_method(D_METHOD("render_preview", "request"), &WebNode::render_preview);
	ClassDB::bind_method(D_METHOD("render_menu", "request"), &WebNode::render_menu);
	ClassDB::bind_method(D_METHOD("render_main_menu", "request"), &WebNode::render_main_menu);

	ClassDB::bind_method(D_METHOD("_render_index", "request"), &WebNode::_render_index);
	ClassDB::bind_method(D_METHOD("_render_preview", "request"), &WebNode::_render_preview);
	ClassDB::bind_method(D_METHOD("_render_menu", "request"), &WebNode::_render_menu);
	ClassDB::bind_method(D_METHOD("_render_main_menu", "request"), &WebNode::_render_main_menu);

	BIND_VMETHOD(MethodInfo("_create_table"));
	BIND_VMETHOD(MethodInfo("_drop_table"));
	BIND_VMETHOD(MethodInfo("_update_table", PropertyInfo(Variant::INT, "current_table_version")));
	BIND_VMETHOD(MethodInfo("_create_default_entries", PropertyInfo(Variant::INT, "pseed")));

	ClassDB::bind_method(D_METHOD("create_table"), &WebNode::create_table);
	ClassDB::bind_method(D_METHOD("drop_table"), &WebNode::drop_table);
	ClassDB::bind_method(D_METHOD("update_table", "current_table_version"), &WebNode::update_table);
	ClassDB::bind_method(D_METHOD("create_default_entries", "pseed"), &WebNode::create_default_entries);

	ClassDB::bind_method(D_METHOD("_create_table"), &WebNode::_create_table);
	ClassDB::bind_method(D_METHOD("_drop_table"), &WebNode::_drop_table);
	ClassDB::bind_method(D_METHOD("_update_table", "current_table_version"), &WebNode::_update_table);
	ClassDB::bind_method(D_METHOD("_create_default_entries", "pseed"), &WebNode::_create_default_entries);

	BIND_VMETHOD(MethodInfo("_migrate", PropertyInfo(Variant::BOOL, "clear"), PropertyInfo(Variant::BOOL, "should_seed"), PropertyInfo(Variant::INT, "pseed")));
	ClassDB::bind_method(D_METHOD("migrate", "clear", "pseed"), &WebNode::migrate);
	ClassDB::bind_method(D_METHOD("_migrate", "clear", "pseed"), &WebNode::_migrate);

	ClassDB::bind_method(D_METHOD("try_route_request_to_children", "request"), &WebNode::try_route_request_to_children);
	ClassDB::bind_method(D_METHOD("get_request_handler_child", "request"), &WebNode::get_request_handler_child);
	ClassDB::bind_method(D_METHOD("build_handler_map"), &WebNode::build_handler_map);
	ClassDB::bind_method(D_METHOD("clear_handlers"), &WebNode::clear_handlers);

	ClassDB::bind_method(D_METHOD("request_write_lock"), &WebNode::request_write_lock);

	ClassDB::bind_method(D_METHOD("get_server"), &WebNode::get_server);
	ClassDB::bind_method(D_METHOD("get_web_root"), &WebNode::get_web_root);
	ClassDB::bind_method(D_METHOD("get_parent_webnode"), &WebNode::get_parent_webnode);

	BIND_CONSTANT(NOTIFICATION_WEB_NODE_WRITE_LOCKED);
}
