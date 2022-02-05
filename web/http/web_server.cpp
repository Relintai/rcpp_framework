#include "web_server.h"

#include "request.h"
#include "web/http/web_node.h"

WebNode *WebServer::get_web_root() {
	return _web_root;
}

void WebServer::set_root(Node *root) {
	WebNode *web_root = Object::cast_to<WebNode>(root);

	ERR_FAIL_COND(!web_root);

	_web_root = web_root;

	NodeTree::set_root(root);
}

void WebServer::handle_request(Request *request) {
	ERR_FAIL_COND(!_web_root);

	_web_root->handle_request_main(request);
}


WebServer::WebServer() : NodeTree() {
	_web_root = nullptr;
}

WebServer::~WebServer() {
}
