#include "http_server.h"

#include <functional>

#include "application.h"
#include "request.h"

#include <trantor/utils/Logger.h>

#define LOG_VERBOSE 0

using namespace std::placeholders;

void HTTPServer::http_callback_handler(Request *request) {
	Application::handle_request(request);
}

void HTTPServer::httpEnterCallbackDefault(const HTTPParser &httpParser, const HttpSession::Ptr &session) {
	Request *request = RequestPool::get_request();

	request->http_parser = &httpParser;
	request->session = &session;

	request->setup_url_stack();

#if LOG_VERBOSE
	std::cout << "method:" << http_method_str(static_cast<http_method>(httpParser.method())) << std::endl;
#endif

	http_callback_handler(request);
}

void HTTPServer::wsEnterCallbackDefault(const HttpSession::Ptr &httpSession, WebSocketFormat::WebSocketFrameType opcode, const std::string &payload) {

	std::cout << "frame enter of type:" << int(opcode) << std::endl;
	std::cout << "payload is:" << payload << std::endl;
	// echo frame
	//auto frame = std::make_shared<std::string>();

	//WebSocketFormat::wsFrameBuild(payload.c_str(), payload.size(), *frame, WebSocketFormat::WebSocketFrameType::TEXT_FRAME, true, false);

	//httpSession->send(frame);
}

void HTTPServer::configure_old() {
}

void HTTPServer::initialize_old() {
	if (service)
		return;

	configure_old();

	service = TcpService::Create();
	service->startWorkerThread(threads);

	int p_port = port;

	//!
	if (listenBuilder)
		delete listenBuilder;

	listenBuilder = new wrapper::HttpListenerBuilder();
	listenBuilder->configureService(service);

	listenBuilder->configureSocketOptions({
			[](TcpSocket &socket) {
				socket.setNodelay();
			},
	});

	listenBuilder->configureConnectionOptions({ AddSocketOption::WithMaxRecvBufferSize(1024) });

	listenBuilder->configureListen([p_port](wrapper::BuildListenConfig builder) {
		builder.setAddr(false, "0.0.0.0", p_port);
	});

	listenBuilder->configureEnterCallback([](const HttpSession::Ptr &httpSession, HttpSessionHandlers &handlers) {
		handlers.setHttpCallback(HTTPServer::httpEnterCallbackDefault);
		handlers.setWSCallback(HTTPServer::wsEnterCallbackDefault);
	});

	listenBuilder->asyncRun();
}

void HTTPServer::main_loop_old() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));

		if (brynet::base::app_kbhit()) {
			break;
		}
	}
}

void HTTPServer::configure() {
}
void HTTPServer::initialize() {
}

//drogon -> void HttpAppFrameworkImpl::run()
void HTTPServer::main_loop() {

	
	if (!getLoop()->isInLoopThread()) {
		getLoop()->moveToCurrentThread();
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
		getLoop()->resetTimerQueue();
#endif
		getLoop()->resetAfterFork();
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
		getLoop()->resetAfterFork();
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
	auto ioLoops = listenerManagerPtr_->createListeners(
			std::bind(&HttpAppFrameworkImpl::onAsyncRequest, this, _1, _2),
			std::bind(&HttpAppFrameworkImpl::onNewWebsockRequest, this, _1, _2, _3),
			std::bind(&HttpAppFrameworkImpl::onConnection, this, _1),
			idleConnectionTimeout_,
			sslCertPath_,
			sslKeyPath_,
			threads,
			syncAdvices_);


	assert(ioLoops.size() == threads);
	for (size_t i = 0; i < threads; ++i) {
		ioLoops[i]->setIndex(i);
	}


	getLoop()->setIndex(threads);

	// A fast database client instance should be created in the main event
	// loop, so put the main loop into ioLoops.

	ioLoops.push_back(getLoop());

	/*
	dbClientManagerPtr_->createDbClients(ioLoops);
	httpCtrlsRouterPtr_->init(ioLoops);
	httpSimpleCtrlsRouterPtr_->init(ioLoops);
	staticFileRouterPtr_->init(ioLoops);
	websockCtrlsRouterPtr_->init();
	*/

	getLoop()->queueInLoop([this]() {
		// Let listener event loops run when everything is ready.
		listenerManagerPtr_->startListening();

		for (auto &adv : beginningAdvices_) {
			adv();
		}

		beginningAdvices_.clear();
	});

	getLoop()->loop();
}

trantor::EventLoop *HttpAppFrameworkImpl::get_loop() const
{
    static trantor::EventLoop loop;
    return &loop;
}

HTTPServer::HTTPServer() {
	port = 80;
	threads = 4;
	listenBuilder = nullptr;
	_running = false;
}

HttpServer::HTTPServer(
    EventLoop *loop,
    const InetAddress &listenAddr,
    const std::string &name,
    const std::vector<std::function<HttpResponsePtr(const HttpRequestPtr &)>>
        &syncAdvices)
#ifdef __linux__
    : server_(loop, listenAddr, name.c_str()),
#else
    : server_(loop, listenAddr, name.c_str(), true, false),
#endif
      httpAsyncCallback_(defaultHttpAsyncCallback),
      newWebsocketCallback_(defaultWebSockAsyncCallback),
      connectionCallback_(defaultConnectionCallback),
      syncAdvices_(syncAdvices)
{
    server_.setConnectionCallback(
        std::bind(&HttpServer::onConnection, this, _1));
    server_.setRecvMessageCallback(
        std::bind(&HttpServer::onMessage, this, _1, _2));
}

HTTPServer::~HTTPServer() {
	delete listenBuilder;
}