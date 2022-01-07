#ifndef WEB_NODE_H
#define WEB_NODE_H

#include "core/nodes/node.h"

class Request;

class WebNode : public Node {
public:
	virtual void handle_request_main(Request *request);
	virtual void create_validators();

	virtual void create_table();
	virtual void drop_table();
	virtual void migrate();
	virtual void create_default_entries();

	WebNode();
	~WebNode();

protected:
};

#endif