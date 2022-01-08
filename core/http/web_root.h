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
class WebServer;

class WebRoot : public WebRouterNode {
	RCPP_OBJECT(WebRoot, WebRouterNode);

public:
	static std::string default_error_404_body;
	static std::string default_generic_error_body;

	void handle_request_main(Request *request);
	void send_error(int error_code, Request *request);
	void send_file(const std::string &path, Request *request);

	static void default_fallback_error_handler(int error_code, Request *request);
	static void default_404_error_handler(int error_code, Request *request);

	virtual void load_settings();
	virtual void setup_routes();
	virtual void setup_middleware();

	void default_routing_middleware(Object *instance, Request *request);

	virtual void migrate();

	void register_request_update(Request *request);
	void unregister_request_update(Request *request);
	void update();

	WebServer *get_server();

	WebRoot();
	virtual ~WebRoot();

public:
	HandlerInstance index_func;
	std::map<std::string, HandlerInstance> main_route_map;
	std::vector<HandlerInstance> middlewares;

	std::map<int, std::function<void(int, Request *)> > error_handler_map;
	std::function<void(int, Request *)> default_error_handler_func;

protected:
	std::mutex _update_registered_requests_mutex;
	std::vector<Request *> _update_registered_requests;
};

#endif