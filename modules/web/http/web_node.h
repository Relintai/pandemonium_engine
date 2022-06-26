#ifndef WEB_NODE_H
#define WEB_NODE_H

#include "core/nodes/node.h"
#include "core/reference.h"
#include "core/variant.h"
#include <map>

class Request;
class Settings;
class WebServer;
class WebPermission;

#ifdef DATABASES_ENABLED
class DataBase;
class TableBuilder;
class QueryBuilder;
#endif

//note add an alterative queue_delete() -> (with different name) -> if called node stops getting routed to, and when rwlock can be locked gets queue_deleted.

class WebNode : public Node {
	RCPP_OBJECT(WebNode, Node);

public:
	enum {
		//todo likely needs a different constant number
		NOTIFICATION_SERVER_TREE_WRITE_LOCKED = 2000,
	};

	String get_uri_segment();
	void set_uri_segment(const String &val);

	virtual String get_full_uri(const bool slash_at_the_end = true);
	virtual String get_full_uri_parent(const bool slash_at_the_end = true);

	Settings *get_settings();
	void set_settings(Settings *settings);

	Ref<WebPermission> get_web_permission();
	void set_web_permission(const Ref<WebPermission> &wp);

	virtual bool get_routing_enabled();
	virtual void set_routing_enabled(const bool value);

#ifdef DATABASES_ENABLED
	Database *get_database();
	Ref<TableBuilder> get_table_builder();
	Ref<QueryBuilder> get_query_builder();
	void set_database(Database *db);
#endif

	virtual void handle_request_main(Request *request);
	virtual void _handle_request_main(Request *request);
	virtual void handle_error_send_request(Request *request, const int error_code);

	virtual void render_index(Request *request);
	virtual void render_preview(Request *request);

	virtual void render_menu(Request *request);
	virtual void _render_menu(Request *request);

	virtual void create_validators();

	virtual void create_table();
	virtual void drop_table();
	virtual void udpate_table();
	virtual void create_default_entries();

	virtual void migrate(const bool clear, const bool seed);
	virtual void _migrate(const bool clear, const bool seed);

	bool try_route_request_to_children(Request *request);
	WebNode *get_request_handler_child(Request *request);
	void build_handler_map();
	void clear_handlers();

	WebServer *get_server();
	WebNode *get_root();
	WebNode *get_parent_webnode();

	void _notification(const int what);

	WebNode();
	~WebNode();

protected:
	String _uri_segment;

	Settings *_settings;

#ifdef DATABASES_ENABLED
	Database *_database;
#endif

	bool _routing_enabled;
	WebNode *_index_node;
	std::map<String, WebNode *> _node_route_map;

	Ref<WebPermission> _web_permission;
};

#endif
