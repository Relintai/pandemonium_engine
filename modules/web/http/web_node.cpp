
#include "web_node.h"

#include "http_enums.h"
#include "request.h"

#include "core/settings/settings.h"
#include "web/http/web_server.h"
#include "web_permission.h"

#ifdef DATABASES_ENABLED
#include "database/database.h"
#include "database/database_manager.h"
#include "database/query_builder.h"
#include "database/query_result.h"
#include "database/table_builder.h"
#endif

/*
void WebNode::update() {
	if (!_root_node) {
		return;
	}

	_root_node->notification(Node::NOTIFICATION_UPDATE);

	if (_write_lock_requested) {
		_rw_lock.write_lock();
		_root_node->notification(Node::NOTIFICATION_TREE_WRITE_LOCKED);
		_rw_lock.write_unlock();

		_write_lock_requested = false;
	}
}
*/

String WebNode::get_uri_segment() {
	return _uri_segment;
}
void WebNode::set_uri_segment(const String &val) {
	_uri_segment = val;
}

String WebNode::get_full_uri(const bool slash_at_the_end) {
	if (slash_at_the_end) {
		return get_full_uri_parent(true) + _uri_segment + '/';
	} else {
		return get_full_uri_parent(true) + _uri_segment;
	}
}
String WebNode::get_full_uri_parent(const bool slash_at_the_end) {
	String uri = "/";

	WebNode *n = get_parent_webnode();

	while (n) {
		if (n->_uri_segment == "" || n->_uri_segment == '/') {
			break;
		}

		uri = "/" + n->_uri_segment + uri;

		n = n->get_parent_webnode();
	}

	if (!slash_at_the_end) {
		uri.pop_back();
	}

	return uri;
}

Settings *WebNode::get_settings() {
	if (_settings) {
		return _settings;
	}

	return Settings::get_singleton();
}
void WebNode::set_settings(Settings *settings) {
	_settings = settings;

	// todo send event to children when it's implemented?
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
		if (is_in_tree()) {
			build_handler_map();
		}
	}
}

#ifdef DATABASES_ENABLED

Database *WebNode::get_database() {
	if (_database) {
		return _database;
	}

	return DatabaseManager::get_singleton()->ddb;
}

Ref<TableBuilder> WebNode::get_table_builder() {
	Database *db = get_database();

	ERR_FAIL_COND_V(!db, Ref<TableBuilder>());

	return db->get_table_builder();
}

Ref<QueryBuilder> WebNode::get_query_builder() {
	Database *db = get_database();

	ERR_FAIL_COND_V(!db, Ref<QueryBuilder>());

	return db->get_query_builder();
}

void WebNode::set_database(Database *db) {
	_database = db;

	// todo send event to children when it's implemented?
}

#endif

void WebNode::handle_request_main(Request *request) {
	if (_web_permission.is_valid()) {
		if (_web_permission->activate(request)) {
			return;
		}
	}

	if (!_routing_enabled) {
		_handle_request_main(request);
		return;
	}

	if (!try_route_request_to_children(request)) {
		_handle_request_main(request);
	}
}

void WebNode::_handle_request_main(Request *request) {
	request->send_error(HTTP_STATUS_CODE_404_NOT_FOUND);
}

void WebNode::handle_error_send_request(Request *request, const int error_code) {
	// this is a fallback error handler.
	// Webroot implements a proper one
	request->compiled_body = "<html><body>Internal server error!</body></html>";
	request->set_status_code(HTTP_STATUS_CODE_503_SERVICE_UNAVAILABLE);
	request->send();
}

void WebNode::render_index(Request *request) {
}
void WebNode::render_preview(Request *request) {
}

void WebNode::render_menu(Request *request) {
	WebNode *root = get_root();

	if (root) {
		root->_render_menu(request);
	}
}
void WebNode::_render_menu(Request *request) {
}

void WebNode::create_validators() {
}

void WebNode::create_table() {
}

void WebNode::drop_table() {
}

void WebNode::udpate_table() {
}

void WebNode::create_default_entries() {
}

void WebNode::migrate(const bool clear, const bool seed_db) {
	_migrate(clear, seed_db);

	for (int i = 0; i < _children.size(); ++i) {
		WebNode *c = Object::cast_to<WebNode>(_children[i]);
		c->migrate(clear, seed_db);
	}
}

void WebNode::_migrate(const bool clear, const bool seed_db) {
	if (clear) {
		drop_table();
		create_table();
	} else {
		udpate_table();
	}

	if (seed_db) {
		create_default_entries();
	}
}

bool WebNode::try_route_request_to_children(Request *request) {
	WebNode *handler = nullptr;

	// if (path == "/") {
	if (request->get_path_segment_count() == 0) {
		// quick shortcut
		handler = _index_node;
	} else {
		const String &main_route = request->get_current_path_segment();

		handler = _node_route_map[main_route];
	}

	if (!handler) {
		return false;
	}

	request->push_path();
	handler->handle_request_main(request);

	return true;
}

WebNode *WebNode::get_request_handler_child(Request *request) {
	WebNode *handler = nullptr;

	// if (path == "/") {
	if (request->get_path_segment_count() == 0) {
		// quick shortcut
		handler = _index_node;
	} else {
		const String &main_route = request->get_current_path_segment();
		handler = _node_route_map[main_route];
	}

	return handler;
}

void WebNode::build_handler_map() {
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
			if (_index_node) {
				RLOG_ERR("You have multiple root nodes!");
				continue;
			}

			_index_node = c;
		} else {
			if (_node_route_map[uri_segment]) {
				RLOG_ERR("You have multiple of the same uri! URI:" + uri_segment);
				continue;
			}

			_node_route_map[uri_segment] = c;
		}
	}
}

void WebNode::clear_handlers() {
	_index_node = nullptr;
	_node_route_map.clear();
}

WebServer *WebNode::get_server() {
	// todo this shoult probably be cached
	return Object::cast_to<WebServer>(get_tree());
}

WebNode *WebNode::get_root() {
	WebServer *s = get_server();

	if (!s) {
		return nullptr;
	}

	return s->get_web_root();
}

WebNode *WebNode::get_parent_webnode() {
	return Object::cast_to<WebNode>(get_parent());
}

void WebNode::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			if (_routing_enabled) {
				build_handler_map();
			}
			break;
		case NOTIFICATION_CHILD_ADDED:
			if (_routing_enabled && is_in_tree()) {
				build_handler_map();
			}
			break;
		case NOTIFICATION_CHILD_REMOVED:
			if (_routing_enabled && is_in_tree()) {
				build_handler_map();
			}
			break;
		default:
			break;
	}
}

WebNode::WebNode() :
		Node() {
	// should look this up in parents when parented (and node parenting is implemented)
	// should have an event later when a parent gets one
#ifdef DATABASES_ENABLED
	_database = nullptr;
#endif

	// same for this
	_settings = nullptr;

	_routing_enabled = true;
	_index_node = nullptr;

	create_validators();
}

WebNode::~WebNode() {
}
