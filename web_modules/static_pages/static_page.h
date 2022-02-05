#ifndef STATIC_PAGE_H
#define STATIC_PAGE_H

#include "core/string.h"

#include "web/http/web_node.h"

/*

StaticPage -> string data (serve it)
load_md(path) + parse_md(data)
load_bbcode(path) + ...
load_html(path)
load_file(path)

bool add menu -> handle


*/

class StaticPage : public WebNode {
	RCPP_OBJECT(StaticPage, WebNode);

public:
	void handle_request_main(Request *request);

	void load();

	StaticPage();
	~StaticPage();
};

#endif