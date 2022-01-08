#ifndef WEB_ROUTER_NODE_H
#define WEB_ROUTER_NODE_H

#include "web_node.h"

class WebRouterNode : public WebNode {
	RCPP_OBJECT(WebRouterNode, WebNode);

public:
	void handle_request_main(Request *request);

	WebRouterNode();
	~WebRouterNode();
};

#endif