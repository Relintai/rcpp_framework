#include "web_root.h"

#include <functional>
#include <string>

#include "request.h"

#include "core/file_cache.h"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include "core/http/web_server.h"

void WebRoot::load_settings() {
}

void WebRoot::setup_routes() {
	default_error_handler_func = WebRoot::default_fallback_error_handler;

	error_handler_map[404] = WebRoot::default_404_error_handler;
}

void WebRoot::setup_middleware() {
	// If you want sessions add this to your inherited class. Should probably be the first one.
	// middlewares.push_back(HandlerInstance(::SessionManager::session_setup_middleware));

	middlewares.push_back(HandlerInstance([this](Object *instance, Request *request) { this->default_routing_middleware(instance, request); }));
}

void WebRoot::default_routing_middleware(Object *instance, Request *request) {
	// handle default phase 1
	std::string path = request->get_path_full();

	if (FileCache::get_singleton()->wwwroot_has_file(path)) {
		send_file(path, request);

		return;
	}

	// call parent handle default

	// from this this will be handled by web router node by default
	HandlerInstance handler_data;

	// std::function<void(Object *, Request *)> func;

	// if (path == "/") {
	if (request->get_path_segment_count() == 0) {
		// quick shortcut
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

void WebRoot::default_fallback_error_handler(Request *request, int error_code) {
	request->compiled_body = default_generic_error_body;

	request->send();
}

void WebRoot::default_404_error_handler(Request *request, int error_code) {
	request->compiled_body = default_error_404_body;
	request->send();
}

void WebRoot::handle_request_main(Request *request) {
	// request->middleware_stack = &middlewares;
	// note that middlewares handle the routing -> WebRoot::default_routing_middleware by default
	// request->next_stage();

	// handle files first
	if (try_send_wwwroot_file(request)) {
		return;
	}

	// normal routing
	WebRouterNode::handle_request_main(request);
}

void WebRoot::handle_error_send_request(Request *request, const int error_code) {
	std::function<void(Request *, int)> func = error_handler_map[error_code];

	if (!func) {
		if (!default_error_handler_func) {
			WebNode::handle_error_send_request(request, error_code);
			return;
		}

		default_error_handler_func(request, error_code);
		return;
	}

	func(request, error_code);
}

bool WebRoot::try_send_wwwroot_file(Request *request) {
	const String &path = request->get_path_full();

	if (FileCache::get_singleton()->wwwroot_has_file(path)) {
		send_file(path, request);

		return true;
	}

	return false;
}

void WebRoot::send_error(int error_code, Request *request) {

}

void WebRoot::send_file(const std::string &path, Request *request) {
	std::string fp = FileCache::get_singleton()->wwwroot + path;

	request->send_file(fp);
}

void WebRoot::migrate() {
}

void WebRoot::register_request_update(Request *request) {
	std::lock_guard<std::mutex> lock(_update_registered_requests_mutex);

	_update_registered_requests.push_back(request);
}
void WebRoot::unregister_request_update(Request *request) {
	std::lock_guard<std::mutex> lock(_update_registered_requests_mutex);

	std::size_t s = _update_registered_requests.size();
	for (std::size_t i = 0; i < s; ++i) {
		Request *r = _update_registered_requests[i];

		if (r == request) {
			_update_registered_requests[i] = _update_registered_requests[s - 1];

			_update_registered_requests.pop_back();

			return;
		}
	}
}

void WebRoot::update() {
	for (std::size_t i = 0; i < _update_registered_requests.size(); ++i) {
		Request *r = _update_registered_requests[i];

		r->update();
	}
}

WebServer *WebRoot::get_server() {
	// todo this shoult probably be cached
	return Object::cast_to<WebServer>(get_tree());
}

WebRoot::WebRoot() :
		WebRouterNode() {
}

WebRoot::~WebRoot() {
	main_route_map.clear();
	error_handler_map.clear();
	middlewares.clear();
}

std::string WebRoot::default_error_404_body = "<html><body>404 :(</body></html>";
std::string WebRoot::default_generic_error_body = "<html><body>Internal server error! :(</body></html>";
