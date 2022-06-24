#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "core/nodes/node_tree.h"

class Request;
class WebNode;

class WebServer : public NodeTree {
	RCPP_OBJECT(WebServer, NodeTree);

public:
	WebNode *get_web_root();

	void set_root(Node *root);

	void handle_request(Request *request);

	float get_update_delta_time();

	WebServer();
	virtual ~WebServer();

protected:
	WebNode *_web_root;
	float _update_interval;
};

#endif