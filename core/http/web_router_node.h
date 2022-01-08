#ifndef WEB_ROUTER_NODE_H
#define WEB_ROUTER_NODE_H

#include "web_node.h"

#include <map>
#include "core/string.h"

class WebRouterNode : public WebNode {
	RCPP_OBJECT(WebRouterNode, WebNode);

public:
	void handle_request_main(Request *request);
	bool try_route_request_to_children(Request *request);

	void build_handler_map();

	void _notification(const int what);

	WebRouterNode();
	~WebRouterNode();

protected:
	WebNode *_index_node;
	std::map<String, WebNode *> _node_route_map;
};

#endif