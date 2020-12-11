#include "rdn_application.h"

#include "core/request.h"

#include <iostream>

#include "core/file_cache.h"

#include "core/handler_instance.h"

#include "core/database_manager.h"

#include "core/html_builder.h"

void RDNApplication::index(Object *instance, Request *request) {
	std::string body;

	if (FileCache::get_singleton()->get_cached_body("index", &body)) {
		request->response->setBody(body);

		return;
	}

	HTMLBuilder b;

	b.h1();
	b.w("Testh1");
	b.ch1();

	b.h2()->cls("tcls")->id("tid");
	b.w("Testh2");
	b.ch2();

	b.br();

	b.p();
	b.w("Test HTML Body HTMLBuilder");
	b.cp();

	b.form()->method("post")->href("/");

	//->str("/") is a temp hack
	b.input()->type("text")->str("/");
	b.input()->type("submit")->str("/");
	b.cform();

	request->body = b.result;

	//request->body.append("<p>Test HTML Body</p>");
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