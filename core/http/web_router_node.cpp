
#include "web_router_node.h"

#include "request.h"

void WebRouterNode::handle_request_main(Request *request) {
	if (!try_route_request_to_children(request)) {
		_handle_request_main(request);
	}
}

bool WebRouterNode::try_route_request_to_children(Request *request) {
	WebNode *handler = nullptr;

	// if (path == "/") {
	if (request->get_path_segment_count() == 0) {
		// quick shortcut
		handler = _index_node;
	} else {
		const String &main_route = request->get_current_path_segment();

		handler = _node_route_map[main_route];

		request->push_path();
	}

	if (!handler) {
		return false;
	}

	handler->handle_request_main(request);

	return true;
}

WebNode *WebRouterNode::get_request_handler_child(Request *request) {
	WebNode *handler = nullptr;

	// if (path == "/") {
	if (request->get_path_segment_count() == 0) {
		// quick shortcut
		handler = _index_node;
	} else {
		const String &main_route = request->get_current_path_segment();
		handler = _node_route_map[main_route];

		request->push_path();
	}

	return handler;
}

void WebRouterNode::build_handler_map() {
	_index_node = nullptr;
	_node_route_map.clear();

	for (int i = 0; i < get_child_count(); ++i) {
		WebNode *c = Object::cast_to<WebNode>(get_child(i));

		if (!c) {
			continue;
		}

		String uri_segment = c->get_uri_segment();

		if (uri_segment == "") {
			// ignore
			continue;
		} else if (uri_segment == "/") {
			if (_index_node) {
				RLOG_ERR("You have multiple root nodes!");
				continue;
			}

			_index_node = c;
		} else {
			if (_node_route_map[uri_segment]) {
				RLOG_ERR("You have multiple of the same uri! URI:" + uri_segment);
				continue;
			}

			_node_route_map[uri_segment] = c;
		}
	}
}

void WebRouterNode::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			build_handler_map();
			break;
		case NOTIFICATION_CHILD_ADDED:
			if (is_in_tree()) {
				build_handler_map();
			}
			break;
		case NOTIFICATION_CHILD_REMOVED:
			if (is_in_tree()) {
				build_handler_map();
			}
			break;
		default:
			break;
	}
}

WebRouterNode::WebRouterNode() :
		WebNode() {
}

WebRouterNode::~WebRouterNode() {
}