#include "application.h"

#include <functional>
#include <string>

#include "request.h"

#include "file_cache.h"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <trantor/utils/Logger.h>

#include "core/listener_manager.h"

using namespace std::placeholders;

void Application::load_settings() {
}

void Application::setup_routes() {
	default_error_handler_func = Application::default_fallback_error_handler;

	error_handler_map[404] = Application::default_404_error_handler;
}

void Application::setup_middleware() {
	middlewares.push_back(HandlerInstance(Application::default_routing_middleware));
}

void Application::default_routing_middleware(Object *instance, Request *request) {
	std::string path = request->http_parser->getPath();

	if (FileCache::get_singleton()->wwwroot_has_file(path)) {
		send_file(path, request);

		return;
	}

	HandlerInstance handler_data;

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

void Application::default_fallback_error_handler(int error_code, Request *request) {
	request->response->setBody(default_generic_error_body);
	request->send();
}

void Application::default_404_error_handler(int error_code, Request *request) {
	request->response->setBody(default_error_404_body);
	request->send();
}

void Application::handle_request(Request *request) {
	request->middleware_stack = &middlewares;

	//note that middlewares handle the routing -> Application::default_routing_middleware by default
	request->next_stage();
}

void Application::send_error(int error_code, Request *request) {
	std::function<void(int, Request *)> func = error_handler_map[error_code];

	if (!func) {
		default_error_handler_func(error_code, request);
		return;
	}

	func(error_code, request);
}

void Application::send_file(const std::string &path, Request *request) {
	std::string fp = FileCache::get_singleton()->wwwroot + path;

	FILE *f = fopen(fp.c_str(), "rb");

	if (!f) {
		printf("Error: Registered file doesn't exists anymore! %s\n", path.c_str());

		send_error(404, request);
		return;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); /* same as rewind(f); */

	std::string body;
	body.resize(fsize);

	fread(&body[0], 1, fsize, f);
	fclose(f);

	//TODO set mimetype?

	request->response->setBody(body);
	request->send();
}

void Application::migrate() {
}

//drogon -> void HttpAppFrameworkImpl::run()
void Application::run() {

	if (!get_loop()->isInLoopThread()) {
		get_loop()->moveToCurrentThread();
	}

	LOG_TRACE << "Start to run...";

	/*
	trantor::AsyncFileLogger asyncFileLogger;

	// Create dirs for cache files
	for (int i = 0; i < 256; ++i) {
		char dirName[4];
		snprintf(dirName, sizeof(dirName), "%02x", i);
		std::transform(dirName, dirName + 2, dirName, toupper);
		utils::createPath(getUploadPath() + "/tmp/" + dirName);
	}
	*/

	/*
	if (runAsDaemon_) {
		// go daemon!
		godaemon();
#ifdef __linux__
		get_loop()->resetTimerQueue();
#endif
		get_loop()->resetAfterFork();
	}
	*/

	/*
	// set relaunching
	if (relaunchOnError_) {
#ifndef _WIN32
		while (true) {
			int child_status = 0;
			auto child_pid = fork();
			if (child_pid < 0) {
				LOG_ERROR << "fork error";
				abort();
			} else if (child_pid == 0) {
				// child
				break;
			}
			waitpid(child_pid, &child_status, 0);
			sleep(1);
			LOG_INFO << "start new process";
		}
		get_loop()->resetAfterFork();
#endif
	}*/

	//signal(SIGTERM, TERMFunction);

	/*
	// set logger
	if (!logPath_.empty()) {
#ifdef _WIN32
		if (_access(logPath_.c_str(), 06) != 0)
#else
		if (access(logPath_.c_str(), R_OK | W_OK) != 0)
#endif
		{
			LOG_ERROR << "log file path not exist";
			abort();
		} else {
			std::string baseName = logfileBaseName_;
			if (baseName == "") {
				baseName = "drogon";
			}
			asyncFileLogger.setFileName(baseName, ".log", logPath_);
			asyncFileLogger.startLogging();
			trantor::Logger::setOutputFunction(
					[&](const char *msg, const uint64_t len) {
						asyncFileLogger.output(msg, len);
					},
					[&]() { asyncFileLogger.flush(); });
			asyncFileLogger.setFileSizeLimit(logfileSize_);
		}
	}
*/

	/*
	if (relaunchOnError_) {
		LOG_INFO << "Start child process";
	}
*/

	// now start runing!!

	_running = true;

	/*
#ifndef _WIN32
	if (!libFilePaths_.empty()) {
		sharedLibManagerPtr_ = std::unique_ptr<SharedLibManager>(
				new SharedLibManager(libFilePaths_, libFileOutputPath_));
	}
#endif
*/

	// Create all listeners.
	/*
	auto ioLoops = listenerManagerPtr_->createListeners(
			std::bind(&Application::onAsyncRequest, this, _1, _2),
			std::bind(&Application::onNewWebsockRequest, this, _1, _2, _3),
			std::bind(&Application::onConnection, this, _1),
			idleConnectionTimeout_,
			sslCertPath_,
			sslKeyPath_,
			threadNum_,
			syncAdvices_);

	assert(ioLoops.size() == threadNum_);
	for (size_t i = 0; i < threadNum_; ++i) {
		ioLoops[i]->setIndex(i);
	}

	get_loop()->setIndex(threadNum_);
	*/

	// A fast database client instance should be created in the main event
	// loop, so put the main loop into ioLoops.

	//ioLoops.push_back(get_loop());

	/*
	dbClientManagerPtr_->createDbClients(ioLoops);
	httpCtrlsRouterPtr_->init(ioLoops);
	httpSimpleCtrlsRouterPtr_->init(ioLoops);
	staticFileRouterPtr_->init(ioLoops);
	websockCtrlsRouterPtr_->init();
	*/

/*
	get_loop()->queueInLoop([this]() {
		// Let listener event loops run when everything is ready.
		listenerManagerPtr_->startListening();

		for (auto &adv : beginningAdvices_) {
			adv();
		}

		beginningAdvices_.clear();
	});
*/
	get_loop()->loop();
}

trantor::EventLoop *Application::get_loop() const {
	static trantor::EventLoop loop;
	return &loop;
}

void Application::onAsyncRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
	/*
	LOG_TRACE << "new request:" << req->peerAddr().toIpPort() << "->"
			  << req->localAddr().toIpPort();
	LOG_TRACE << "Headers " << req->methodString() << " " << req->path();
	LOG_TRACE << "http path=" << req->path();
	if (req->method() == Options && (req->path() == "*" || req->path() == "/*")) {
		auto resp = HttpResponse::newHttpResponse();
		resp->setContentTypeCode(ContentType::CT_TEXT_PLAIN);
		resp->addHeader("ALLOW", "GET,HEAD,POST,PUT,DELETE,OPTIONS,PATCH");
		resp->setExpiredTime(0);
		callback(resp);
		return;
	}
	findSessionForRequest(req);
	// Route to controller
	if (!preRoutingObservers_.empty()) {
		for (auto &observer : preRoutingObservers_) {
			observer(req);
		}
	}
	if (preRoutingAdvices_.empty()) {
		httpSimpleCtrlsRouterPtr_->route(req, std::move(callback));
	} else {
		auto callbackPtr =
				std::make_shared<std::function<void(const HttpResponsePtr &)> >(
						std::move(callback));
		doAdvicesChain(
				preRoutingAdvices_,
				0,
				req,
				std::make_shared<std::function<void(const HttpResponsePtr &)> >(
						[req, callbackPtr, this](const HttpResponsePtr &resp) {
							callCallback(req, resp, *callbackPtr);
						}),
				[this, callbackPtr, req]() {
					httpSimpleCtrlsRouterPtr_->route(req, std::move(*callbackPtr));
				});
	}*/
}

void Application::onNewWebsockRequest(const HttpRequestPtr &req,
		std::function<void(const HttpResponsePtr &)> &&callback,
		const WebSocketConnectionPtr &wsConnPtr) {
/*
	findSessionForRequest(req);
	// Route to controller
	if (!preRoutingObservers_.empty()) {
		for (auto &observer : preRoutingObservers_) {
			observer(req);
		}
	}
	if (preRoutingAdvices_.empty()) {
		websockCtrlsRouterPtr_->route(req, std::move(callback), wsConnPtr);
	} else {
		auto callbackPtr =
				std::make_shared<std::function<void(const HttpResponsePtr &)> >(
						std::move(callback));
		doAdvicesChain(
				preRoutingAdvices_,
				0,
				req,
				std::make_shared<std::function<void(const HttpResponsePtr &)> >(
						[req, callbackPtr, this](const HttpResponsePtr &resp) {
							callCallback(req, resp, *callbackPtr);
						}),
				[this, callbackPtr, req, wsConnPtr]() {
					websockCtrlsRouterPtr_->route(req,
							std::move(*callbackPtr),
							wsConnPtr);
				});
	}*/
}

void Application::onConnection(const trantor::TcpConnectionPtr &conn)
{
	/*
    static std::mutex mtx;
    LOG_TRACE << "connect!!!" << maxConnectionNum_
              << " num=" << connectionNum_.load();
    if (conn->connected())
    {
        if (connectionNum_.fetch_add(1, std::memory_order_relaxed) >=
            maxConnectionNum_)
        {
            LOG_ERROR << "too much connections!force close!";
            conn->forceClose();
            return;
        }
        else if (maxConnectionNumPerIP_ > 0)
        {
            {
                std::lock_guard<std::mutex> lock(mtx);
                auto iter = connectionsNumMap_.find(conn->peerAddr().toIp());
                if (iter == connectionsNumMap_.end())
                {
                    connectionsNumMap_[conn->peerAddr().toIp()] = 1;
                }
                else if (iter->second++ > maxConnectionNumPerIP_)
                {
                    conn->getLoop()->queueInLoop(
                        [conn]() { conn->forceClose(); });
                    return;
                }
            }
        }
        for (auto &advice : newConnectionAdvices_)
        {
            if (!advice(conn->peerAddr(), conn->localAddr()))
            {
                conn->forceClose();
                return;
            }
        }
    }
    else
    {
        if (!conn->hasContext())
        {
            // If the connection is connected to the SSL port and then
            // disconnected before the SSL handshake.
            return;
        }
        connectionNum_.fetch_sub(1, std::memory_order_relaxed);
        if (maxConnectionNumPerIP_ > 0)
        {
            std::lock_guard<std::mutex> lock(mtx);
            auto iter = connectionsNumMap_.find(conn->peerAddr().toIp());
            if (iter != connectionsNumMap_.end())
            {
                --iter->second;
                if (iter->second <= 0)
                {
                    connectionsNumMap_.erase(iter);
                }
            }
        }
    }
	*/
}


Application::Application() {
	_instance = this;
	threadNum_ = 1;
	idleConnectionTimeout_ = 60;
}

Application::~Application() {
	main_route_map.clear();
	error_handler_map.clear();
	middlewares.clear();
}

Application *Application::get_instance() {
	return _instance;
}

HandlerInstance Application::index_func;
std::map<std::string, HandlerInstance> Application::main_route_map;
std::vector<HandlerInstance> Application::middlewares;

std::map<int, std::function<void(int, Request *)> > Application::error_handler_map;
std::function<void(int, Request *)> Application::default_error_handler_func = nullptr;

Application *Application::_instance = nullptr;

std::string Application::default_error_404_body = "<html><body>404 :(</body></html>";
std::string Application::default_generic_error_body = "<html><body>Internal server error! :(</body></html>";
