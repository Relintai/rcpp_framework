#ifndef APPLICATION_H
#define APPLICATION_H

#include "object.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <trantor/net/TcpServer.h>
#include <trantor/net/callbacks.h>
#include <trantor/utils/NonCopyable.h>

#include "handler_instance.h"

#include "core/http_server_callbacks.h"

#include "core/listener_manager.h"

using namespace drogon;

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

	virtual void run();

	trantor::EventLoop *get_loop() const;

	void onAsyncRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
	void onNewWebsockRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
			const WebSocketConnectionPtr &wsConnPtr);
	void onConnection(const trantor::TcpConnectionPtr &conn);

	Application();
	virtual ~Application();

	static Application *get_instance();

	std::unique_ptr<ListenerManager> listenerManagerPtr_;

	size_t getClientMaxBodySize() const {
		return clientMaxBodySize_;
	}
	size_t getClientMaxMemoryBodySize() const {
		return clientMaxMemoryBodySize_;
	}
	size_t getClientMaxWebSocketMessageSize() const {
		return clientMaxWebSocketMessageSize_;
	}
	size_t keepaliveRequestsNumber() const {
		return keepaliveRequestsNumber_;
	}
	size_t pipeliningRequestsNumber() const {
		return pipeliningRequestsNumber_;
	}

	bool sendDateHeader() const {
		return enableDateHeader_;
	}

	bool sendServerHeader() const {
		return enableServerHeader_;
	}

	const std::string &getServerHeaderString() const {
		return serverHeader_;
	}

	virtual const std::string &getUploadPath() const {
		return uploadPath_;
	}

	virtual size_t getCurrentThreadIndex() const {
		auto *loop = trantor::EventLoop::getEventLoopOfCurrentThread();
		if (loop) {
			return loop->index();
		}
#ifdef _WIN32
		return size_t(-1);
#else
		return std::numeric_limits<size_t>::max();
#endif
	}

public:
	static HandlerInstance index_func;
	static std::map<std::string, HandlerInstance> main_route_map;
	static std::vector<HandlerInstance> middlewares;

	static std::map<int, std::function<void(int, Request *)> > error_handler_map;
	static std::function<void(int, Request *)> default_error_handler_func;

	size_t idleConnectionTimeout_;
	size_t threadNum_;
	std::string sslCertPath_;
	std::string sslKeyPath_;
	std::vector<std::function<HttpResponsePtr(const HttpRequestPtr &)> > syncAdvices_;

private:
	static Application *_instance;
	bool _running;

	size_t clientMaxBodySize_{ 1024 * 1024 };
	size_t clientMaxMemoryBodySize_{ 64 * 1024 };
	size_t clientMaxWebSocketMessageSize_{ 128 * 1024 };
	size_t keepaliveRequestsNumber_{ 0 };
	size_t pipeliningRequestsNumber_{ 0 };
	std::string uploadPath_;
	bool enableServerHeader_{ false };
	std::string serverHeader_{ "Server: rcpp_cms\r\n" };
	bool enableDateHeader_{ true };
};

#endif