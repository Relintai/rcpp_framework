#ifndef WEB_ROOT_H
#define WEB_ROOT_H

#include "web_router_node.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "mutex"

#include "handler_instance.h"

class Request;

//Middleware turn into a class (reference)
//add them to a folder
//method should return bool -> true to continue, false if done

//FileCache -> set up, for this webroot, don't use singleton

//clean up unused things here
//remove handler instances!

//Update the rest of the modules to the new systems

//remove middleware stack from request

class WebRoot : public WebRouterNode {
	RCPP_OBJECT(WebRoot, WebRouterNode);

public:
	static std::string default_error_404_body;
	static std::string default_generic_error_body;

	void handle_request_main(Request *request);
	void handle_error_send_request(Request *request, const int error_code);

	bool try_send_wwwroot_file(Request *request);
	void send_error(int error_code, Request *request);
	void send_file(const std::string &path, Request *request);

	static void default_fallback_error_handler(Request *request, int error_code);
	static void default_404_error_handler(Request *request, int error_code);

	virtual void load_settings();
	virtual void setup_routes();
	virtual void setup_middleware();

	void default_routing_middleware(Object *instance, Request *request);

	virtual void migrate();

	void register_request_update(Request *request);
	void unregister_request_update(Request *request);
	void update();

	WebRoot();
	virtual ~WebRoot();

public:
	HandlerInstance index_func;
	std::map<std::string, HandlerInstance> main_route_map;
	std::vector<HandlerInstance> middlewares;

	std::map<int, std::function<void(Request *, int)> > error_handler_map;
	std::function<void(Request *, int)> default_error_handler_func;

protected:
	std::mutex _update_registered_requests_mutex;
	std::vector<Request *> _update_registered_requests;
};

#endif