#include "web_server.h"

#include "request.h"
#include "core/http/web_root.h"

WebRoot *WebServer::get_web_root() {
	return _web_root;
}

void WebServer::set_root(Node *root) {
	WebRoot *web_root = Object::cast_to<WebRoot>(root);

	ERR_FAIL_COND(!web_root);

	_web_root = web_root;

	NodeTree::set_root(root);
}

void WebServer::handle_request(Request *request) {
	ERR_FAIL_COND(!_web_root);

	_web_root->handle_request_main(request);
}


WebServer::WebServer() {
}

WebServer::~WebServer() {
}
