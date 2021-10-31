#ifndef RBAC_CONTROLLER_H
#define RBAC_CONTROLLER_H

#include "modules/admin_panel/admin_controller.h"

#include <string>

class Request;
class FormValidator;

class RBACController : public AdminController {
	RCPP_OBJECT(RBACController, AdminController);
	
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