#include "application.h"

#include <functional>
#include <string>

#include "request.h"

#include "file_cache.h"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

void Application::load_settings() {
}

void Application::setup_routes() {
	default_error_handler_func = Application::default_fallback_error_handler;

	error_handler_map[404] = Application::default_404_error_handler;
}

void Application::setup_middleware() {
	middlewares.push_back(HandlerInstance(Application::default_routing_middleware));
}

void Application::default_routing_middleware(Object *instance, Request *request) {
	std::string path = request->http_parser->getPath();

	if (FileCache::get_singleton()->wwwroot_has_file(path)) {
		send_file(path, request);

		return;
	}

	HandlerInstance handler_data;

	//std::function<void(Object *, Request *)> func;

	//if (path == "/") {
	if (request->get_path_segment_count() == 0) {
		//quick shortcut
		handler_data = index_func;
	} else {
		const std::string main_route = request->get_current_path_segment();

		handler_data = main_route_map[main_route];

		request->push_path();
	}

	if (!handler_data.handler_func) {
		send_error(404, request);

		return;
	}

	request->handler_instance = handler_data;
	request->next_stage();
}

void Application::default_fallback_error_handler(int error_code, Request *request) {
	request->response->setBody(default_generic_error_body);
	request->send();
}

void Application::default_404_error_handler(int error_code, Request *request) {
	request->response->setBody(default_error_404_body);
	request->send();
}

void Application::handle_request(Request *request) {
	request->middleware_stack = &middlewares;

	//note that middlewares handle the routing -> Application::default_routing_middleware by default
	request->next_stage();
}

void Application::send_error(int error_code, Request *request) {
	std::function<void(int, Request *)> func = error_handler_map[error_code];

	if (!func) {
		default_error_handler_func(error_code, request);
		return;
	}

	func(error_code, request);
}

void Application::send_file(const std::string &path, Request *request) {
	std::string fp = FileCache::get_singleton()->wwwroot + path;

	FILE *f = fopen(fp.c_str(), "rb");

	if (!f) {
		printf("Error: Registered file doesn't exists anymore! %s\n", path.c_str());

		send_error(404, request);
		return;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); /* same as rewind(f); */

	std::string body;
	body.resize(fsize);

	fread(&body[0], 1, fsize, f);
	fclose(f);

	//TODO set mimetype?

	request->response->setBody(body);
	request->send();
}

void Application::migrate() {
}

Application::Application() {
	_instance = this;
}

Application::~Application() {
	main_route_map.clear();
	error_handler_map.clear();
	middlewares.clear();
}

Application *Application::get_instance() {
	return _instance;
}

HandlerInstance Application::index_func;
std::map<std::string, HandlerInstance> Application::main_route_map;
std::vector<HandlerInstance> Application::middlewares;

std::map<int, std::function<void(int, Request *)> > Application::error_handler_map;
std::function<void(int, Request *)> Application::default_error_handler_func = nullptr;

Application *Application::_instance = nullptr;

std::string Application::default_error_404_body = "<html><body>404 :(</body></html>";
std::string Application::default_generic_error_body = "<html><body>Internal server error! :(</body></html>";
