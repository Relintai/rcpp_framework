#include "rdn_application.h"

#include "core/request.h"

#include <iostream>

#include "core/file_cache.h"

#include "core/handler_instance.h"

#include "core/database_manager.h"

void RDNApplication::index(Object *instance, Request *request) {
	std::string body;

	if (FileCache::get_singleton()->get_cached_body("index", &body)) {
		request->response->setBody(body);

		return;
	}

	request->body.append("<p>Test HTML Body</p>");
	request->compile_body();

	FileCache::get_singleton()->set_cached_body("index", request->compiled_body);

	request->send();
}

void RDNApplication::session_middleware_func(Object *instance, Request *request) {
	std::cout << "test: session_middleware_func called" << std::endl;

	//if fail
	//request->send(); in middleware

	request->next_stage();
}

void RDNApplication::message_page_func(Object *instance, Request *request) {
	dynamic_cast<MessagePage *>(instance)->index(request);
}

void RDNApplication::setup_routes() {
	Application::setup_routes();

	index_func = HandlerInstance(index);

	main_route_map["asd"] = HandlerInstance(index);
	main_route_map["message_page"] = HandlerInstance(message_page_func, message_page);
}

void RDNApplication::setup_middleware() {
	Application::setup_middleware();

	//middlewares.push_back(RDNApplication::session_middleware_func);
}

void RDNApplication::migrate() {
	message_page->migrate();
}

RDNApplication::RDNApplication() :
		Application() {

	message_page = new MessagePage();
	message_page->db = DatabaseManager::get_singleton()->databases[0];
}

RDNApplication::~RDNApplication() {
	delete message_page;
}