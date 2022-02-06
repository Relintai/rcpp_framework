#ifndef STATIC_PAGE_H
#define STATIC_PAGE_H

#include "core/string.h"

#include "web/http/web_node.h"

class StaticPage : public WebNode {
	RCPP_OBJECT(StaticPage, WebNode);

public:
	void _handle_request_main(Request *request);

	void load_file(const String &path);
	void load_and_process_file(const String &path);
	void load_md_file(const String &path);
	
	void set_data_md(const String &d);
	void set_data(const String &d);

	String data;
	bool should_render_menu;

	StaticPage();
	~StaticPage();
};

#endif