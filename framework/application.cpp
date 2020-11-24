#include "application.h"

#include <functional>
#include <string>

#include "request.h"

#include "file_cache.h"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

void Application::setup_routes() {
	default_error_handler_func = Application::default_fallback_error_handler;

	error_handler_map[404] = Application::default_404_error_handler;
}

void Application::setup_middleware() {
}

void Application::default_fallback_error_handler(int error_code, Request *request) {
	std::string body = "<html>Internal server error! :(</html>";
	request->response->setBody(body);
	request->finalized = true;
}

void Application::default_404_error_handler(int error_code, Request *request) {
	std::string body = "<html>404 :(</html>";
	request->response->setBody(body);
	request->finalized = true;
}

void Application::handle_request(Request *request) {

	std::string path = request->http_parser->getPath();

	if (FileCache::get_instance()->has_file(path)) {
		send_file(path, request);

		return;
	}

	std::function<void(Request *)> func;

	if (path == "/") {
		//quick shortcut
		func = index_func;
	} else {
		std::string main_route = "";

		uint32_t endpos = 1;
		for (; endpos < path.size(); ++endpos) {
			if (path[endpos] == '/') {
				break;
			}
		}

		main_route = path.substr(1, endpos - 1);

		func = main_route_map[main_route];
	}

	if (!func) {
		send_error(404, request);

		return;
	}

	for (uint32_t i; i < middlewares.size(); ++i) {
		middlewares[i](request);

		if (request->finalized)
			return;
	}

	func(request);
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
	std::string fp = FileCache::get_instance()->wwwroot + path;
	
	FILE *f = fopen(fp.c_str(), "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

	std::string body;
	body.resize(fsize);

	fread(&body[0], 1, fsize, f);
	fclose(f);

	request->response->setBody(body);
	request->finalized = true;
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

std::function<void(Request *)> Application::index_func = nullptr;
std::map<std::string, std::function<void(Request *)> > Application::main_route_map;
std::map<int, std::function<void(int, Request *)> > Application::error_handler_map;
std::function<void(int, Request *)> Application::default_error_handler_func = nullptr;
std::vector<std::function<void(Request *)> > Application::middlewares;

Application *Application::_instance = nullptr;
