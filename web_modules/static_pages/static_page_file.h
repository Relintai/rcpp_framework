#ifndef STATIC_PAGE_FILE_H
#define STATIC_PAGE_FILE_H

#include "core/string.h"

#include "web/http/web_node.h"

class StaticPageFile : public WebNode {
	RCPP_OBJECT(StaticPageFile, WebNode);

public:
	void handle_request_main(Request *request);

	void load();

	StaticPageFile();
	~StaticPageFile();
};

#endif