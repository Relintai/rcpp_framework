#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include "core/http/controller.h"

#include "core/containers/vector.h"
#include "core/string.h"

class Request;
class FormValidator;

struct AdminSectionLinkInfo {
	String name;
	String link;

	AdminSectionLinkInfo() {
	}

	AdminSectionLinkInfo(const String &p_name, const String &p_link) {
		name = p_name;
		link = p_link;
	}
};

class AdminController : public Controller {
	RCPP_OBJECT(AdminController, Controller);

public:
	virtual void admin_handle_request_main(Request *request);
	virtual void admin_add_section_links(Vector<AdminSectionLinkInfo> *links);
	virtual bool admin_full_render();

	AdminController();
	~AdminController();

protected:
};

#endif