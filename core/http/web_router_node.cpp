
#include "web_router_node.h"

void WebRouterNode::handle_request_main(Request *request) {
}

void WebRouterNode::build_handler_map() {
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