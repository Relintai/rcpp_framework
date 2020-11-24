#include "rdn_application.h"

#include "framework/request.h"

#include <iostream>

void RDNApplication::index(Request *request) {
	//std::string body = "<html>hello world aaaaa </html>";

    std::string body = "<html><script>var blocked = false; document.addEventListener(\"visibilitychange\", function() { if (!blocked && document.hidden) {blocked = true;alert('Blocked once');}});</script></html>";

	request->response->setBody(body);
}

void RDNApplication::session_middleware_func(Request *request) {
    std::cout << "dddd" << std::endl;
}

void RDNApplication::setup_routes() {
	Application::setup_routes();

	index_func = index;

	main_route_map["asd"] = index;
}

void RDNApplication::setup_middleware() {
	Application::setup_middleware();

	middlewares.push_back(RDNApplication::session_middleware_func);
}

RDNApplication::RDNApplication() :
		Application() {
}

RDNApplication::~RDNApplication() {
}