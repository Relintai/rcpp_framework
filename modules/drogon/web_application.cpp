#include "web_application.h"

#include <functional>
#include <string>

#include "request.h"

#include "core/file_cache.h"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

void DWebApplication::load_settings() {
}

void DWebApplication::setup_routes() {
	default_error_handler_func = DWebApplication::default_fallback_error_handler;

	error_handler_map[404] = DWebApplication::default_404_error_handler;
}

void DWebApplication::setup_middleware() {
	middlewares.push_back(DHandlerInstance([this](Object *instance, DRequest *request){ this->default_routing_middleware(instance, request); }));
}

void DWebApplication::default_routing_middleware(Object *instance, DRequest *request) {
	std::string path = request->http_parser->getPath();

	if (FileCache::get_singleton()->wwwroot_has_file(path)) {
		send_file(path, request);

		return;
	}

	DHandlerInstance handler_data;

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

void DWebApplication::default_fallback_error_handler(int error_code, DRequest *request) {
	request->response->setBody(default_generic_error_body);
	request->send();
}

void DWebApplication::default_404_error_handler(int error_code, DRequest *request) {
	request->response->setBody(default_error_404_body);
	request->send();
}

void DWebApplication::handle_request(DRequest *request) {
	request->middleware_stack = &middlewares;

	//note that middlewares handle the routing -> DWebApplication::default_routing_middleware by default
	request->next_stage();
}

void DWebApplication::send_error(int error_code, DRequest *request) {
	std::function<void(int, DRequest *)> func = error_handler_map[error_code];

	if (!func) {
		default_error_handler_func(error_code, request);
		return;
	}

	func(error_code, request);
}

void DWebApplication::send_file(const std::string &path, DRequest *request) {
	std::string fp = FileCache::get_singleton()->wwwroot + path;

	request->send_file(fp);
}

void DWebApplication::migrate() {
}

void DWebApplication::register_request_update(DRequest *request) {
	std::lock_guard<std::mutex> lock(_update_registered_requests_mutex);

	_update_registered_requests.push_back(request);
}
void DWebApplication::unregister_request_update(DRequest *request) {
	std::lock_guard<std::mutex> lock(_update_registered_requests_mutex);

	std::size_t s = _update_registered_requests.size();
	for (std::size_t i = 0; i < s; ++i) {
		DRequest *r = _update_registered_requests[i];

		if (r == request) {
			_update_registered_requests[i] = _update_registered_requests[s - 1];

			_update_registered_requests.pop_back();

			return;
		}
	}
}

void DWebApplication::update() {
	for (std::size_t i = 0; i < _update_registered_requests.size(); ++i) {
		DRequest *r = _update_registered_requests[i];

		r->update();
	}
}

DWebApplication::DWebApplication() {
}

DWebApplication::~DWebApplication() {
	main_route_map.clear();
	error_handler_map.clear();
	middlewares.clear();
}

std::string DWebApplication::default_error_404_body = "<html><body>404 :(</body></html>";
std::string DWebApplication::default_generic_error_body = "<html><body>Internal server error! :(</body></html>";
