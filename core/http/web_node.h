#ifndef WEB_NODE_H
#define WEB_NODE_H

#include "core/object.h"

class Request;

class WebNode : public Object {
public:
	virtual void handle_request_main(Request *request);
	virtual void create_validators();

	virtual void create_table();
	virtual void drop_table();
	virtual void migrate();

	WebNode();
	~WebNode();

protected:
};

#endif