#ifndef ADMIN_PANEL_H
#define ADMIN_PANEL_H

#include "core/http/controller.h"

#include "core/string.h"
#include "core/containers/vector.h"

class Request;
class FormValidator;
class AdminController;

class AdminPanel : public Controller {
	RCPP_OBJECT(AdminPanel, Controller);
	
public:
	void handle_request_main(Request *request);

	virtual void render_admin_panel_list(Request *request);
	virtual void render_controller_panel(Request *request, AdminController *controller);

	void register_admin_controller(const String &section, AdminController *controller);
	void clear();

	virtual void render_headers(Request *request);
	virtual void render_footer(Request *request);

	void set_default_header(const String &val);
	void set_default_footer(const String &val);

	static AdminPanel *get_singleton();

	AdminPanel();
	~AdminPanel();

protected:
	struct AdminPanelSection {
		String section_url;
		String name;
		AdminController *controller;
	};

	static AdminPanel *_self;

	Vector<AdminPanelSection> _controllers; 

	String _default_headers;
	String _default_footer;
};

#endif