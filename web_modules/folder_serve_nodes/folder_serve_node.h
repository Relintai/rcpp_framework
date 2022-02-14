#ifndef FOLDER_SERVE_NODE_H
#define FOLDER_SERVE_NODE_H

#include "core/string.h"

#include "web/file_cache.h"
#include "web/http/web_node.h"

class FolderServeNode : public WebNode {
	RCPP_OBJECT(FolderServeNode, WebNode);

public:
	void handle_request_main(Request *request);

	virtual void load();

	void _notification(const int what);

	String serve_folder;

	FolderServeNode();
	~FolderServeNode();

protected:
	FileCache *file_cache;
};

#endif