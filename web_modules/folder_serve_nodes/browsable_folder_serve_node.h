#ifndef BROWSABLE_FOLDER_SERVE_NODE_H
#define BROWSABLE_FOLDER_SERVE_NODE_H

#include "core/string.h"
#include <map>

#include "folder_serve_node.h"

class BrowsableFolderServeNode : public FolderServeNode {
	RCPP_OBJECT(BrowsableFolderServeNode, FolderServeNode);

public:
	void _handle_request_main(Request *request);

	void render(Request *request);
	void render_preview(Request *request);

	virtual void load();

	void evaluate_dir(const String &path, const bool top_level = false);
	virtual void render_dir_page(const String &dir_uri, const Vector<String> &folders, const Vector<String> &files, const bool top_level);

	bool should_render_menu;

	BrowsableFolderServeNode();
	~BrowsableFolderServeNode();

protected:
	std::map<String, String *> _folder_indexes;
};

#endif