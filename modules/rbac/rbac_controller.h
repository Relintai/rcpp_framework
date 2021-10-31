#ifndef RBAC_CONTROLLER_H
#define RBAC_CONTROLLER_H

#include "core/http/controller.h"

#include <string>

class Request;
class FormValidator;

class RBACController : public Controller {
	RCPP_OBJECT(RBACController, Controller);
	
public:
	void handle_request_main(Request *request);
	void create_validators();

	static RBACController *get_singleton();

	RBACController();
	~RBACController();

protected:
	static RBACController *_self;
};

#endif