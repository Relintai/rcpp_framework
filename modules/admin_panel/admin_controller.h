#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include "core/http/controller.h"

#include <string>

class Request;
class FormValidator;

class AdminController : public Controller {
	RCPP_OBJECT(AdminController, Controller);
	
public:
	AdminController();
	~AdminController();

protected:
};

#endif