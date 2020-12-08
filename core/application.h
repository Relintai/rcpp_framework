#ifndef APPLICATION_H
#define APPLICATION_H

#include "object.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "handler_instance.h"

class Request;

class Application {
public:
	static std::string default_error_404_body;
	static std::string default_generic_error_body;

	static void handle_request(Request *request);
	static void send_error(int error_code, Request *request);
	static void send_file(const std::string &path, Request *request);

	static void default_fallback_error_handler(int error_code, Request *request);
	static void default_404_error_handler(int error_code, Request *request);

	virtual void load_settings();
	virtual void setup_routes();
	virtual void setup_middleware();

	static void default_routing_middleware(Object *instance, Request *request);

	virtual void migrate();

	Application();
	virtual ~Application();

	static Application *get_instance();

public:
	static HandlerInstance index_func;
	static std::map<std::string, HandlerInstance> main_route_map;
	static std::vector<HandlerInstance> middlewares;

	static std::map<int, std::function<void(int, Request *)> > error_handler_map;
	static std::function<void(int, Request *)> default_error_handler_func;

private:
	static Application *_instance;
};

#endif