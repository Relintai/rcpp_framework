#ifndef ADMIN_PANEL_H
#define ADMIN_PANEL_H

#include "core/http/controller.h"

#include <string>

class Request;
class FormValidator;

class AdminPanel : public Controller {
	RCPP_OBJECT(AdminPanel, Controller);
	
public:
	void handle_request_main(Request *request);
	void create_validators();

	static AdminPanel *get_singleton();

	AdminPanel();
	~AdminPanel();

protected:
	static AdminPanel *_self;
};

#endif