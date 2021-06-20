#ifndef DWEB_APPLICATION_H
#define WEB_APPLICATION_H

#include "core/object.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <mutex>

#include "handler_instance.h"

class DRequest;

class DWebApplication {
public:
	static std::string default_error_404_body;
	static std::string default_generic_error_body;

	void handle_request(DRequest *request);
	void send_error(int error_code, DRequest *request);
	void send_file(const std::string &path, DRequest *request);

	static void default_fallback_error_handler(int error_code, DRequest *request);
	static void default_404_error_handler(int error_code, DRequest *request);

	virtual void load_settings();
	virtual void setup_routes();
	virtual void setup_middleware();

	void default_routing_middleware(Object *instance, DRequest *request);

	virtual void migrate();

	void register_request_update(DRequest *request);
	void unregister_request_update(DRequest *request);
	void update();

	DWebApplication();
	virtual ~DWebApplication();

public:
	DHandlerInstance index_func;
	std::map<std::string, DHandlerInstance> main_route_map;
	std::vector<DHandlerInstance> middlewares;

	std::map<int, std::function<void(int, DRequest *)> > error_handler_map;
	std::function<void(int, DRequest *)> default_error_handler_func;

protected:
	std::mutex _update_registered_requests_mutex;
	std::vector<DRequest *> _update_registered_requests;
};

#endif