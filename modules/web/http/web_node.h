#ifndef WEB_NODE_H
#define WEB_NODE_H

#include "../../modules_enabled.gen.h"
#include "core/hash_map.h"
#include "core/os/rw_lock.h"
#include "core/reference.h"
#include "core/variant.h"
#include "scene/main/node.h"

class WebServerRequest;
class WebServer;
class WebPermission;

#ifdef MODULE_DATABASE_ENABLED
class Database;
class TableBuilder;
class QueryBuilder;
#endif

//note add an alterative queue_delete() -> (with different name) -> if called node tells parent to stop getting routed to, and when rwlock can be locked gets queue_deleted.
//Also add an rwlock just around the handler map (if it's not done already), and add api to lock and unlock it

class WebNode : public Node {
	GDCLASS(WebNode, Node);

public:
	enum {
		NOTIFICATION_WEB_TREE_WRITE_LOCKED = 2000,
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
	Ref<Database> get_database();
	void set_database(const Ref<Database> &db);
	Ref<TableBuilder> get_table_builder();
	Ref<QueryBuilder> get_query_builder();
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
	void udpate_table();
	void create_default_entries();

	virtual void _create_table();
	virtual void _drop_table();
	virtual void _udpate_table();
	virtual void _create_default_entries();

	void migrate(const bool clear, const bool seed);
	virtual void _migrate(const bool clear, const bool seed);

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
	Ref<Database> _database;
#endif

	bool _routing_enabled;
	WebNode *_index_node;
	HashMap<String, WebNode *> _node_route_map;
	RWLock _handler_map_lock;

	Ref<WebPermission> _web_permission;

	//TODO this should be atomic
	bool _write_lock_requested;
	RWLock _rw_lock;
};

#endif
