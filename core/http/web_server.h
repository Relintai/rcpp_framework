#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "core/nodes/node_tree.h"

class Request;
class WebRoot;

class WebServer : public NodeTree {
	RCPP_OBJECT(WebServer, NodeTree);

public:
	WebRoot *get_web_root();

	void set_root(Node *root);

	void handle_request(Request *request);

	WebServer();
	virtual ~WebServer();

protected:
	WebRoot *_web_root;

};

#endif