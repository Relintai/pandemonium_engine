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
	enum {
		NOTIFICATION_WEB_SERVER_STARTED = 2000,
		NOTIFICATION_WEB_SERVER_STOPPED = 2001,
		NOTIFICATION_WEB_SERVER_WRITE_LOCK_ACQUIRED = 2002,
	};

	bool get_is_running();

	WebNode *get_web_root();
	HTTPSessionManager *get_session_manager();

	void server_handle_request(Ref<WebServerRequest> request);

	// The server is expected to be running in a heavily multi threaded setting,
	// if you want to safely change to root, or to heavily change the tree
	// you need to lock the tree, however triggering a change like this using a web interface
	// from withing a server would cause a deadlock, so if you want to do something like this
	// you can call request_write_lock() while handling a request, then
	// do the desired change when your node receives NOTIFICATION_WEB_SERVER_WRITE_LOCK_ACQUIRED
	// Also note that every WebNode has a lock similar to this for the same reason,
	// if you want to only change a certain WebNode's children, only lock that particular node.
	void request_write_lock();

	void refresh_root();

	void start();
	void stop();

	virtual void _start();
	virtual void _stop();

	String get_configuration_warning() const;

	WebServer();
	~WebServer();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	bool _is_running;

	WebNode *_web_root;
	HTTPSessionManager *_session_manager;

	bool _write_lock_requested;
	RWLock _rw_lock;
};

#endif
