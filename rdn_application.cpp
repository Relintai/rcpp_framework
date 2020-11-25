#include "rdn_application.h"

#include "core/request.h"

#include <iostream>

#include "core/file_cache.h"

void RDNApplication::index(Request *request) {
	std::string body;

	if (FileCache::get_singleton()->get_cached_body("index", &body)) {
		std::string body = "<html>hello world aaaaa </html>";

		request->response->setBody(body);

        return;
	}

	body = "<html>hello world aaaaa </html>";

    FileCache::get_singleton()->set_cached_body("index", body);

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