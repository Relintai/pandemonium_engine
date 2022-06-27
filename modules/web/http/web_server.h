#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "core/os/rw_lock.h"
#include "scene/main/node.h"

class WebServerRequest;
class WebNode;
class WebRoot;
class HTTPSessionManager;

class WebServer : public Node {
	GDCLASS(WebServer, Node);

public:
	NodePath get_web_root_path() const;
	void set_web_root_path(const NodePath &path);

	WebNode *get_web_root();
	void set_web_root(WebNode *root);

	Node *get_web_root_bind();
	void set_web_root_bind(Node *root);

	HTTPSessionManager *get_session_manager();
	void set_session_manager(HTTPSessionManager *sess_man);

	Node *get_session_manager_bind();
	void set_session_manager_bind(Node *sess_man);

	void server_handle_request(Ref<WebServerRequest> request);

	void request_write_lock();

	void start();
	void stop();

	virtual void _start();
	virtual void _stop();

	WebServer();
	~WebServer();

protected:
	static void _bind_methods();

	NodePath _web_root_path;
	WebNode *_web_root;

	HTTPSessionManager *_session_manager;

	bool _write_lock_requested;
	RWLock _rw_lock;
};

#endif
