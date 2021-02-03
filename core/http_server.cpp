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

void HTTPServer::httpEnterCallbackDefault(const brynet::net::http::HTTPParser &httpParser, const brynet::net::http::HttpSession::Ptr &session) {
	Request *request = RequestPool::get_request();

	request->http_parser = &httpParser;
	request->session = &session;

	request->setup_url_stack();

#if LOG_VERBOSE
	std::cout << "method:" << http_method_str(static_cast<http_method>(httpParser.method())) << std::endl;
#endif

	http_callback_handler(request);
}

void HTTPServer::wsEnterCallbackDefault(const brynet::net::http::HttpSession::Ptr &httpSession, brynet::net::http::WebSocketFormat::WebSocketFrameType opcode, const std::string &payload) {

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

	service = brynet::net::TcpService::Create();
	service->startWorkerThread(threads);

	int p_port = port;

	//!
	if (listenBuilder)
		delete listenBuilder;

	listenBuilder = new brynet::net::wrapper::HttpListenerBuilder();
	listenBuilder->configureService(service);

	listenBuilder->configureSocketOptions({
			[](brynet::net::TcpSocket &socket) {
				socket.setNodelay();
			},
	});

	listenBuilder->configureConnectionOptions({ brynet::net::AddSocketOption::WithMaxRecvBufferSize(1024) });

	listenBuilder->configureListen([p_port](brynet::net::wrapper::BuildListenConfig builder) {
		builder.setAddr(false, "0.0.0.0", p_port);
	});

	listenBuilder->configureEnterCallback([](const brynet::net::http::HttpSession::Ptr &httpSession, brynet::net::http::HttpSessionHandlers &handlers) {
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
	/*
	auto ioLoops = listenerManagerPtr_->createListeners(
			std::bind(&Application::onAsyncRequest, this, _1, _2),
			std::bind(&Application::onNewWebsockRequest, this, _1, _2, _3),
			std::bind(&Application::onConnection, this, _1),
			Application::get_instance()->idleConnectionTimeout_,
			Application::get_instance()->sslCertPath_,
			Application::get_instance()->sslKeyPath_,
			threads,
			syncAdvices_);


	assert(ioLoops.size() == threads);
	for (size_t i = 0; i < threads; ++i) {
		ioLoops[i]->setIndex(i);
	}


	getLoop()->setIndex(threads);
*/
	// A fast database client instance should be created in the main event
	// loop, so put the main loop into ioLoops.

	//ioLoops.push_back(getLoop());

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

static void defaultHttpAsyncCallback(
    const HttpRequestPtr &,
    std::function<void(const HttpResponsePtr &resp)> &&callback)
{
   // auto resp = HttpResponse::newNotFoundResponse();
    //resp->setCloseConnection(true);
   // callback(resp);
}

static void defaultWebSockAsyncCallback(
    const HttpRequestPtr &,
    std::function<void(const HttpResponsePtr &resp)> &&callback,
    const WebSocketConnectionPtr &)
{
    //auto resp = HttpResponse::newNotFoundResponse();
    //resp->setCloseConnection(true);
    //callback(resp);
}

static void defaultConnectionCallback(const trantor::TcpConnectionPtr &)
{
    return;
}

HTTPServer::HTTPServer() {
	port = 80;
	threads = 4;
	listenBuilder = nullptr;
	_running = false;
}

HTTPServer::HTTPServer(trantor::EventLoop *loop,
		const trantor::InetAddress &listenAddr,
		const std::string &name,
		const std::vector<std::function<HttpResponsePtr(const HttpRequestPtr &)> >
				&syncAdvices)
#ifdef __linux__
		:
		server_(loop, listenAddr, name.c_str()),
#else
		:
		server_(loop, listenAddr, name.c_str(), true, false),
#endif
		httpAsyncCallback_(defaultHttpAsyncCallback),
		newWebsocketCallback_(defaultWebSockAsyncCallback),
		connectionCallback_(defaultConnectionCallback),
		syncAdvices_(syncAdvices) {
	port = 80;
	threads = 4;
	listenBuilder = nullptr;
	_running = false;

	server_.setConnectionCallback(std::bind(&HTTPServer::onConnection, this, _1));
	server_.setRecvMessageCallback(std::bind(&HTTPServer::onMessage, this, _1, _2));
}

HTTPServer::~HTTPServer() {
	delete listenBuilder;
}


void HTTPServer::onConnection(const trantor::TcpConnectionPtr &conn)
{
	/*
    if (conn->connected())
    {
        auto parser = std::make_shared<HttpRequestParser>(conn);
        parser->reset();
        conn->setContext(parser);
        connectionCallback_(conn);
    }
    else if (conn->disconnected())
    {
        LOG_TRACE << "conn disconnected!";
        connectionCallback_(conn);
        auto requestParser = conn->getContext<HttpRequestParser>();
        if (requestParser)
        {
            if (requestParser->webSocketConn())
            {
                requestParser->webSocketConn()->onClose();
            }
            conn->clearContext();
        }
    }
	*/
}

void HTTPServer::onMessage(const trantor::TcpConnectionPtr &conn, trantor::MsgBuffer *buf)
{
	/*
    if (!conn->hasContext())
        return;
    auto requestParser = conn->getContext<HttpRequestParser>();
    if (!requestParser)
        return;
    // With the pipelining feature or web socket, it is possible to receice
    // multiple messages at once, so
    // the while loop is necessary
    if (requestParser->webSocketConn())
    {
        // Websocket payload
        requestParser->webSocketConn()->onNewMessage(conn, buf);
    }
    else
    {
        auto &requests = requestParser->getRequestBuffer();
        while (buf->readableBytes() > 0)
        {
            if (requestParser->isStop())
            {
                // The number of requests has reached the limit.
                buf->retrieveAll();
                return;
            }
            if (!requestParser->parseRequest(buf))
            {
                requestParser->reset();
                conn->forceClose();
                return;
            }
            if (requestParser->gotAll())
            {
                requestParser->requestImpl()->setPeerAddr(conn->peerAddr());
                requestParser->requestImpl()->setLocalAddr(conn->localAddr());
                requestParser->requestImpl()->setCreationDate(
                    trantor::Date::date());
                requestParser->requestImpl()->setSecure(
                    conn->isSSLConnection());
                if (requestParser->firstReq() &&
                    isWebSocket(requestParser->requestImpl()))
                {
                    auto wsConn =
                        std::make_shared<WebSocketConnectionImpl>(conn);
                    newWebsocketCallback_(
                        requestParser->requestImpl(),
                        [conn, wsConn, requestParser](
                            const HttpResponsePtr &resp) mutable {
                            if (conn->connected())
                            {
                                if (resp->statusCode() ==
                                    k101SwitchingProtocols)
                                {
                                    requestParser->setWebsockConnection(wsConn);
                                }
                                auto httpString =
                                    ((HttpResponseImpl *)resp.get())
                                        ->renderToBuffer();
                                conn->send(httpString);
                                COZ_PROGRESS
                            }
                        },
                        wsConn);
                }
                else
                    requests.push_back(requestParser->requestImpl());
                requestParser->reset();
            }
            else
            {
                break;
            }
        }
        onRequests(conn, requests, requestParser);
        requests.clear();
    }
	*/
}

void HTTPServer::onRequests(
    const trantor::TcpConnectionPtr &conn,
    const std::vector<HttpRequestPtr> &requests,
    const std::shared_ptr<HttpRequestParser> &requestParser)
{
	/*
    if (requests.empty())
        return;
    if (HttpAppFrameworkImpl::instance().keepaliveRequestsNumber() > 0 &&
        requestParser->numberOfRequestsParsed() >=
            HttpAppFrameworkImpl::instance().keepaliveRequestsNumber())
    {
        requestParser->stop();
        conn->shutdown();
        return;
    }
    else if (HttpAppFrameworkImpl::instance().pipeliningRequestsNumber() > 0 &&
             requestParser->numberOfRequestsInPipelining() + requests.size() >=
                 HttpAppFrameworkImpl::instance().pipeliningRequestsNumber())
    {
        requestParser->stop();
        conn->shutdown();
        return;
    }
    if (!conn->connected())
    {
        return;
    }
    auto loopFlagPtr = std::make_shared<bool>(true);

    for (auto &req : requests)
    {
        bool close_ = (!req->keepAlive());
        bool isHeadMethod = (req->method() == Head);
        if (isHeadMethod)
        {
            req->setMethod(Get);
        }
        bool syncFlag = false;
        if (!requestParser->emptyPipelining())
        {
            requestParser->pushRequestToPipelining(req);
            syncFlag = true;
        }
        if (!syncAdvices_.empty())
        {
            bool adviceFlag = false;
            for (auto &advice : syncAdvices_)
            {
                auto resp = advice(req);
                if (resp)
                {
                    resp->setVersion(req->getVersion());
                    resp->setCloseConnection(close_);
                    if (!syncFlag)
                    {
                        requestParser->getResponseBuffer().emplace_back(
                            getCompressedResponse(req, resp, isHeadMethod),
                            isHeadMethod);
                    }
                    else
                    {
                        requestParser->pushResponseToPipelining(
                            req,
                            getCompressedResponse(req, resp, isHeadMethod),
                            isHeadMethod);
                    }

                    adviceFlag = true;
                    break;
                }
            }
            if (adviceFlag)
                continue;
        }
        httpAsyncCallback_(
            req,
            [conn,
             close_,
             req,
             loopFlagPtr,
             &syncFlag,
             isHeadMethod,
             this,
             requestParser](const HttpResponsePtr &response) {
                if (!response)
                    return;
                if (!conn->connected())
                    return;

                response->setVersion(req->getVersion());
                response->setCloseConnection(close_);
                auto newResp =
                    getCompressedResponse(req, response, isHeadMethod);
                if (conn->getLoop()->isInLoopThread())
                {
                    //*
                    // * A client that supports persistent connections MAY
                    // * "pipeline" its requests (i.e., send multiple requests
                    // * without waiting for each response). A server MUST send
                    // * its responses to those requests in the same order that
                    // * the requests were received. rfc2616-8.1.1.2
                    // *
                    if (!conn->connected())
                        return;
                    if (*loopFlagPtr)
                    {
                        syncFlag = true;
                        if (requestParser->emptyPipelining())
                        {
                            requestParser->getResponseBuffer().emplace_back(
                                newResp, isHeadMethod);
                        }
                        else
                        {
                            // some earlier requests are waiting for responses;
                            requestParser->pushResponseToPipelining(
                                req, newResp, isHeadMethod);
                        }
                    }
                    else if (requestParser->getFirstRequest() == req)
                    {
                        requestParser->popFirstRequest();

                        std::vector<std::pair<HttpResponsePtr, bool>> resps;
                        resps.emplace_back(newResp, isHeadMethod);
                        while (!requestParser->emptyPipelining())
                        {
                            auto resp = requestParser->getFirstResponse();
                            if (resp.first)
                            {
                                requestParser->popFirstRequest();
                                resps.push_back(std::move(resp));
                            }
                            else
                                break;
                        }
                        sendResponses(conn, resps, requestParser->getBuffer());
                    }
                    else
                    {
                        // some earlier requests are waiting for responses;
                        requestParser->pushResponseToPipelining(req,
                                                                newResp,
                                                                isHeadMethod);
                    }
                }
                else
                {
                    conn->getLoop()->queueInLoop([conn,
                                                  req,
                                                  newResp,
                                                  this,
                                                  isHeadMethod,
                                                  requestParser]() {
                        if (conn->connected())
                        {
                            if (requestParser->getFirstRequest() == req)
                            {
                                requestParser->popFirstRequest();
                                std::vector<std::pair<HttpResponsePtr, bool>>
                                    resps;
                                resps.emplace_back(newResp, isHeadMethod);
                                while (!requestParser->emptyPipelining())
                                {
                                    auto resp =
                                        requestParser->getFirstResponse();
                                    if (resp.first)
                                    {
                                        requestParser->popFirstRequest();
                                        resps.push_back(std::move(resp));
                                    }
                                    else
                                        break;
                                }
                                sendResponses(conn,
                                              resps,
                                              requestParser->getBuffer());
                            }
                            else
                            {
                                // some earlier requests are waiting for
                                // responses;
                                requestParser->pushResponseToPipelining(
                                    req, newResp, isHeadMethod);
                            }
                        }
                    });
                }
            });
        if (syncFlag == false)
        {
            requestParser->pushRequestToPipelining(req);
        }
    }
    *loopFlagPtr = false;
    if (conn->connected() && !requestParser->getResponseBuffer().empty())
    {
        sendResponses(conn,
                      requestParser->getResponseBuffer(),
                      requestParser->getBuffer());
        requestParser->getResponseBuffer().clear();
    }
	*/
}

void HTTPServer::sendResponse(const trantor::TcpConnectionPtr &conn,
                              const HttpResponsePtr &response,
                              bool isHeadMethod)
{
	/*
    conn->getLoop()->assertInLoopThread();
    auto respImplPtr = static_cast<HttpResponseImpl *>(response.get());
    if (!isHeadMethod)
    {
        auto httpString = respImplPtr->renderToBuffer();
        conn->send(httpString);
        auto &sendfileName = respImplPtr->sendfileName();
        if (!sendfileName.empty())
        {
            conn->sendFile(sendfileName.c_str());
        }
        COZ_PROGRESS
    }
    else
    {
        auto httpString = respImplPtr->renderHeaderForHeadMethod();
        conn->send(std::move(*httpString));
        COZ_PROGRESS
    }

    if (response->ifCloseConnection())
    {
        conn->shutdown();
        COZ_PROGRESS
    }
	*/
}

void HTTPServer::sendResponses(
    const trantor::TcpConnectionPtr &conn,
    const std::vector<std::pair<HttpResponsePtr, bool>> &responses,
    trantor::MsgBuffer &buffer)
{
	/*
    conn->getLoop()->assertInLoopThread();
    if (responses.empty())
        return;
    if (responses.size() == 1)
    {
        sendResponse(conn, responses[0].first, responses[0].second);
        return;
    }
    for (auto const &resp : responses)
    {
        auto respImplPtr = static_cast<HttpResponseImpl *>(resp.first.get());
        if (!resp.second)
        {
            // Not HEAD method
            respImplPtr->renderToBuffer(buffer);
            auto &sendfileName = respImplPtr->sendfileName();
            if (!sendfileName.empty())
            {
                conn->send(buffer);
                buffer.retrieveAll();
                conn->sendFile(sendfileName.c_str());
                COZ_PROGRESS
            }
        }
        else
        {
            auto httpString = respImplPtr->renderHeaderForHeadMethod();
            buffer.append(httpString->peek(), httpString->readableBytes());
        }
        if (respImplPtr->ifCloseConnection())
        {
            if (buffer.readableBytes() > 0)
            {
                conn->send(buffer);
                buffer.retrieveAll();
                COZ_PROGRESS
            }
            conn->shutdown();
            return;
        }
    }
    if (conn->connected() && buffer.readableBytes() > 0)
    {
        conn->send(buffer);
        COZ_PROGRESS
    }
    buffer.retrieveAll();
	*/
}