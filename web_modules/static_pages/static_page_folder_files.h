#ifndef STATIC_PAGE_FOLDER_FILES_H
#define STATIC_PAGE_FOLDER_FILES_H

#include "core/string.h"

#include "web/http/web_node.h"

class StaticPageFolderFiles : public WebNode {
	RCPP_OBJECT(StaticPageFolderFiles, WebNode);

public:
	void handle_request_main(Request *request);

	void load();

	StaticPageFolderFiles();
	~StaticPageFolderFiles();
};

#endif