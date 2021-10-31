#ifndef HTTP_CONTROLLER_H
#define HTTP_CONTROLLER_H

#include "core/object.h"

class Request;

class Controller : public Object {
public:
	virtual void handle_request_main(Request *request);
	virtual void create_validators();

	Controller();
	~Controller();

protected:
};

#endif