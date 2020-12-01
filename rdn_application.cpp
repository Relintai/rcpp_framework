#include "rdn_application.h"

#include "core/request.h"

#include <iostream>

#include "core/file_cache.h"

#include "core/handler_instance.h"

void RDNApplication::index(Object *instance, Request *request) {
	std::string body;

	if (FileCache::get_singleton()->get_cached_body("index", &body)) {
		request->response->setBody(body);

		return;
	}

	SiteTheme::default_theme->render_index_page(request, &body);

	FileCache::get_singleton()->set_cached_body("index", body);

	request->response->setBody(body);
	request->send();
}

void RDNApplication::session_middleware_func(Object* instance, Request *request) {
	std::cout << "test: session_middleware_func called" << std::endl;

	//if fail
	//request->send(); in middleware

	request->next_stage();
}

void RDNApplication::setup_routes() {
	Application::setup_routes();

	index_func = HandlerInstance(index);

	main_route_map["asd"] = HandlerInstance(index);
}

void RDNApplication::setup_middleware() {
	Application::setup_middleware();

	//middlewares.push_back(RDNApplication::session_middleware_func);
}

RDNApplication::RDNApplication() :	Application() {

	SiteTheme *t = new TestSiteTheme();
	t->register_theme();
	t->set_theme_as_default();

	themes.push_back(t);

	t = new TestSiteTheme2();
	t->register_theme();

	themes.push_back(t);
}

RDNApplication::~RDNApplication() {
	for (uint32_t i = 0; i < themes.size(); ++i) {
		delete themes[i];
	}

	themes.clear();
}