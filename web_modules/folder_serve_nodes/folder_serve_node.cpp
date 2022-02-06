#include "folder_serve_node.h"

#include "web/http/request.h"

void FolderServeNode::handle_request_main(Request *request) {
	const String &rp = request->get_current_path_segment();

	if (rp == "") {
		request->send_error(HTTP_STATUS_CODE_404_NOT_FOUND);
		return;
	}

	String file_name = "/" + request->get_path();

	if (file_cache->wwwroot_has_file(file_name)) {
		String fp = file_cache->wwwroot;
		fp.append_path(file_name);

		request->send_file(fp);
		return;
	}

	if (!try_route_request_to_children(request)) {
		_handle_request_main(request);
	}
}

void FolderServeNode::load() {
	file_cache->clear();

	if (serve_folder == "") {
		return;
	}

	serve_folder.path_clean_end_slash();

	file_cache->wwwroot = serve_folder;
	file_cache->wwwroot_refresh_cache();
}

void FolderServeNode::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

FolderServeNode::FolderServeNode() :
		WebNode() {

	file_cache = new FileCache();
}

FolderServeNode::~FolderServeNode() {
}
