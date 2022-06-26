#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "core/os/rw_lock.h"
#include "scene/main/node.h"

class WebServerRequest;
class WebNode;

class WebServer : public Node {
	GDCLASS(WebServer, Node);

public:
	NodePath get_web_root_path() const;
	void set_web_root_path(const NodePath &path);

	WebNode *get_web_root();
	void set_web_root(WebNode *root);

	Node *get_web_root_bind();
	void set_web_root_bind(Node *root);

	void server_handle_request(Ref<WebServerRequest> request);

	void request_write_lock();

	WebServer();
	~WebServer();

protected:
	static void _bind_methods();

	NodePath _web_root_path;
	WebNode *_web_root;

	bool _write_lock_requested;
	RWLock _rw_lock;
};

#endif
