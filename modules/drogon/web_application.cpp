#include "web_application.h"

#include <functional>
#include <string>

#include "request.h"

#include "core/file_cache.h"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <trantor/net/TcpConnection.h>
#include <trantor/utils/AsyncFileLogger.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifndef _WIN32
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>
#include <uuid.h>
#else
#include <io.h>
#endif

void DWebApplication::load_settings() {
}

void DWebApplication::setup_routes() {
	default_error_handler_func = WebApplication::default_fallback_error_handler;

	error_handler_map[404] = WebApplication::default_404_error_handler;
}

void DWebApplication::setup_middleware() {
	middlewares.push_back(HandlerInstance([this](Object *instance, Request *request) { this->default_routing_middleware(instance, request); }));
}

void DWebApplication::migrate() {
}

void DWebApplication::add_listener(const std::string &ip, uint16_t port, bool useSSL, const std::string &certFile, const std::string &keyFile, bool useOldTLS, const std::vector<std::pair<std::string, std::string> > &sslConfCmds) {
	assert(!_running);

	_listener_manager->addListener(ip, port, useSSL, certFile, keyFile, useOldTLS, sslConfCmds);
}

void DWebApplication::set_thread_num(size_t thread_num) {
	if (_thread_num == 0) {
		_thread_num = std::thread::hardware_concurrency();
	}

	_thread_num = _thread_num;
}
size_t DWebApplication::get_thread_num() const {
	return _thread_num;
}

void DWebApplication::set_ssl_config_commands(const std::vector<std::pair<std::string, std::string> > &sslConfCmds) {
	_ssl_conf_cmds = sslConfCmds;
}
void DWebApplication::set_ssl_files(const std::string &certPath, const std::string &keyPath) {
	_ssl_cert_path = certPath;
	_ssl_key_path = keyPath;
}

void DWebApplication::run() {
	if (!get_loop()->isInLoopThread()) {
		get_loop()->moveToCurrentThread();
	}

	LOG_TRACE << "Start to run...";

	trantor::AsyncFileLogger asyncFileLogger;

	// Create dirs for cache files
	for (int i = 0; i < 256; ++i) {
		char dirName[4];
		snprintf(dirName, sizeof(dirName), "%02x", i);

		std::transform(dirName, dirName + 2, dirName, toupper);

		utils::createPath(get_upload_path() + "/tmp/" + dirName);
	}

	/*
	if (runAsDaemon_) {
		// go daemon!
		godaemon();
#ifdef __linux__
		get_loop()->resetTimerQueue();
#endif
		get_loop()->resetAfterFork();
	}

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
	}

	if (handleSigterm_) {
		signal(SIGTERM, TERMFunction);
	}
*/

	// set logger
	if (!_log_path.empty()) {
#ifdef _WIN32
		if (_access(_log_path.c_str(), 06) != 0)
#else
		if (access(_log_path.c_str(), R_OK | W_OK) != 0)
#endif
		{
			LOG_ERROR << "log file path not exist";
			abort();
		} else {
			std::string baseName = _logfile_base_name;

			if (baseName.empty()) {
				baseName = "rcpp_fw";
			}

			asyncFileLogger.setFileName(baseName, ".log", _log_path);
			asyncFileLogger.startLogging();

			trantor::Logger::setOutputFunction(
					[&](const char *msg, const uint64_t len) {
						asyncFileLogger.output(msg, len);
					},
					[&]() { asyncFileLogger.flush(); });

			asyncFileLogger.setFileSizeLimit(_logfile_size);
		}
	}

	/*
	if (relaunchOnError_) {
		LOG_INFO << "Start child process";
	}
*/

	/*
#ifndef _WIN32
	if (!libFilePaths_.empty()) {
		sharedLibManagerPtr_ =
				std::make_unique<SharedLibManager>(libFilePaths_,
						libFileOutputPath_);
	}
#endif
*/

	// Create all listeners.
	auto ioLoops = _listener_manager->createListeners(get_loop(),
			std::bind(&DWebApplication::on_async_request, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DWebApplication::on_new_websock_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&DWebApplication::on_connection, this, std::placeholders::_1),
			_idle_connection_timeout, _ssl_cert_path, _ssl_key_path, _ssl_conf_cmds, _thread_num, _sync_advices, _pre_sending_advices);

	assert(ioLoops.size() == _thread_num);

	for (size_t i = 0; i < _thread_num; ++i) {
		ioLoops[i]->setIndex(i);
	}

	get_loop()->setIndex(_thread_num);

	// A fast database client instance should be created in the main event
	// loop, so put the main loop into ioLoops.
	ioLoops.push_back(get_loop());

	if (_use_session) {
		_session_manager = std::make_unique<SessionManager>(get_loop(), _session_timeout);
	}

	// now start runing!!
	_running = true;

	/*
	// Initialize plugins
	const auto &pluginConfig = jsonConfig_["plugins"];
	if (!pluginConfig.isNull()) {
		pluginsManagerPtr_->initializeAllPlugins(pluginConfig,
				[](PluginBase *plugin) {
					LOG_TRACE
							<< "new plugin:";
						//	<< plugin->className();
					// TODO: new plugin
				});
	}*/

	//httpCtrlsRouterPtr_->init(ioLoops);
	//httpSimpleCtrlsRouterPtr_->init(ioLoops);

	//staticFileRouterPtr_->init(ioLoops);
	//websockCtrlsRouterPtr_->init();

	get_loop()->queueInLoop([this]() {
		// Let listener event loops run when everything is ready.
		_listener_manager->startListening();

		for (auto &adv : _beginning_advices) {
			adv();
		}

		_beginning_advices.clear();
	});

	get_loop()->loop();
}

void DWebApplication::stop() {
}

void DWebApplication::enable_session(const size_t timeout) {
	_use_session = true;
	_session_timeout = timeout;
}

void DWebApplication::disable_session() {
	_use_session = false;
}

//todo think about it
const std::string &DWebApplication::get_document_root() const {
	return _root_path;
}
void DWebApplication::set_document_root(const std::string &rootPath) {
	_root_path = rootPath;
}

void DWebApplication::set_static_file_headers(const std::vector<std::pair<std::string, std::string> > &headers) {
	//	staticFileRouterPtr_->setStaticFileHeaders(headers);
}

const std::string &DWebApplication::get_upload_path() const {
	return _upload_path;
}

const std::shared_ptr<trantor::Resolver> &DWebApplication::get_resolver() const {
	static std::shared_ptr<trantor::Resolver> resolver = trantor::Resolver::newResolver(get_loop());
	return resolver;
}

void DWebApplication::set_upload_path(const std::string &uploadPath) {
	_upload_path = uploadPath;
}
void DWebApplication::set_file_types(const std::vector<std::string> &types) {
	//staticFileRouterPtr_->setFileTypes(types);
	//return *this;
}

void DWebApplication::set_max_connection_num(size_t maxConnections) {
	_max_connection_num = maxConnections;
}
void DWebApplication::set_max_connection_num_per_ip(size_t maxConnectionsPerIP) {
	_max_connection_num_per_ip = maxConnectionsPerIP;
}

void DWebApplication::set_log_path(const std::string &logPath, const std::string &logfileBaseName, size_t logfileSize) {
	if (logPath.empty())
		return;

#ifdef _WIN32
	if (_access(logPath.c_str(), 0) != 0)
#else
	if (access(logPath.c_str(), 0) != 0)
#endif
	{
		std::cerr << "Log path does not exist!\n";
		exit(1);
	}
#ifdef _WIN32
	if (_access(logPath.c_str(), 06) != 0)
#else
	if (access(logPath.c_str(), R_OK | W_OK) != 0)
#endif
	{
		std::cerr << "Unable to access log path!\n";
		exit(1);
	}
	_log_path = logPath;
	_logfile_base_name = logfileBaseName;
	_logfile_size = logfileSize;
}
void DWebApplication::set_log_level(trantor::Logger::LogLevel level) {
	trantor::Logger::setLogLevel(level);
}

void DWebApplication::enable_sendfile(bool sendFile) {
	_use_sendfile = sendFile;
}

void DWebApplication::enable_gzip(bool useGzip) {
	_use_gzip = useGzip;
}

bool DWebApplication::is_gzip_enabled() const {
	return _use_gzip;
}

void DWebApplication::enable_brotli(bool useBrotli) {
	_use_brotli = useBrotli;
}

bool DWebApplication::is_brotli_enabled() const {
	return _use_brotli;
}

void DWebApplication::set_static_files_cache_time(int cacheTime) {
	//staticFileRouterPtr_->setStaticFilesCacheTime(cacheTime);
}

int DWebApplication::static_files_cache_time() const {
	//return staticFileRouterPtr_->staticFilesCacheTime();
	return 0;
}

void DWebApplication::set_idle_connection_timeout(size_t timeout) {
	_idle_connection_timeout = timeout;
}
void DWebApplication::set_keepalive_requests_number(const size_t number) {
	_keepalive_requests_number = number;
}
void DWebApplication::set_pipelining_requests_number(const size_t number) {
	_pipelining_requests_number = number;
}

void DWebApplication::set_gzip_static(bool useGzipStatic) {
	//staticFileRouterPtr_->setGzipStatic(useGzipStatic);
}
void DWebApplication::set_br_static(bool useGzipStatic) {
	//staticFileRouterPtr_->setBrStatic(useGzipStatic);
}

void DWebApplication::set_client_max_body_size(size_t maxSize) {
	_client_max_body_size = maxSize;
}
void DWebApplication::set_client_max_memory_body_size(size_t maxSize) {
	_client_max_memory_body_size = maxSize;
}
void DWebApplication::set_client_max_web_socket_message_size(size_t maxSize) {
	_client_max_web_socket_message_size = maxSize;
}

void DWebApplication::set_home_page(const std::string &homePageFile) {
	_home_page_file = homePageFile;
}
const std::string &DWebApplication::get_home_page() const {
	return _home_page_file;
}

void DWebApplication::set_implicit_page_enable(bool useImplicitPage) {
	//staticFileRouterPtr_->setImplicitPageEnable(useImplicitPage);
}
bool DWebApplication::is_implicit_page_enabled() const {
	//return staticFileRouterPtr_->isImplicitPageEnabled();
	return false;
}
void DWebApplication::set_implicit_page(const std::string &implicitPageFile) {
	//staticFileRouterPtr_->setImplicitPage(implicitPageFile);
}

const std::string &DWebApplication::get_implicit_page() const {
	//return staticFileRouterPtr_->getImplicitPage();
	static std::string s = "";
	return s;
}
size_t DWebApplication::get_client_max_body_size() const {
	return _client_max_body_size;
}
size_t DWebApplication::get_client_max_memory_body_size() const {
	return _client_max_memory_body_size;
}
size_t DWebApplication::get_client_max_web_socket_message_size() const {
	return _client_max_web_socket_message_size;
}

size_t DWebApplication::keepalive_requests_number() const {
	return _keepalive_requests_number;
}
size_t DWebApplication::pipelining_requests_number() const {
	return _pipelining_requests_number;
}

bool DWebApplication::is_running() {
	return _running;
}

void DWebApplication::set_unicode_escaping_in_json(bool enable) {
	_using_unicode_escaping = enable;
}

bool DWebApplication::is_unicode_escaping_used_in_json() const {
	return _using_unicode_escaping;
}
void DWebApplication::set_float_precision_in_json(unsigned int precision, const std::string &precisionType) {
	_float_precision_in_json = std::make_pair(precision, precisionType);
}

const std::pair<unsigned int, std::string> &DWebApplication::get_float_precision_in_json() const {
	return _float_precision_in_json;
}

trantor::EventLoop *DWebApplication::get_loop() const {
	return _loop;
}
trantor::EventLoop *DWebApplication::get_io_loop(size_t id) const {
	assert(_listener_manager);

	return _listener_manager->getIOLoop(id);
}

void DWebApplication::set_server_header_field(const std::string &server) {
	assert(!_running);
	assert(server.find("\r\n") == std::string::npos);
	_server_header = "server: " + server + "\r\n";
}

void DWebApplication::enable_server_header(bool flag) {
	_enable_server_header = flag;
}
void DWebApplication::enable_date_header(bool flag) {
	_enable_date_header = flag;
}

bool DWebApplication::send_server_header() const {
	return _enable_server_header;
}
bool DWebApplication::send_date_header() const {
	return _enable_date_header;
}

const std::string &DWebApplication::get_server_header_string() const {
	return _server_header;
}

std::vector<trantor::InetAddress> DWebApplication::get_listeners() const {
	return _listener_manager->getListeners();
}

bool DWebApplication::use_sendfile() const {
	return _use_sendfile;
}

bool DWebApplication::support_ssl() const {
#ifdef OPENSSL_FOUND
	return true;
#endif
	return false;
}

size_t DWebApplication::get_current_thread_index() const {
	trantor::EventLoop *loop = trantor::EventLoop::getEventLoopOfCurrentThread();

	if (loop) {
		return loop->index();
	}

	return (std::numeric_limits<size_t>::max)();
}

void DWebApplication::enable_reuse_port(bool enable) {
	_reuse_port = enable;
}

bool DWebApplication::reuse_port() const {
	return _reuse_port;
}

void DWebApplication::on_async_request(const HttpRequestImplPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {

	LOG_INFO << "on_async_request";

	LOG_TRACE << "new request:" << req->peerAddr().toIpPort() << "->" << req->localAddr().toIpPort();
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

	find_session_for_request(req);

	/*
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
	}
	*/

	//void HttpSimpleControllersRouter::route(const HttpRequestImplPtr &req,std::function<void(const HttpResponsePtr &)> &&callback)

	//auto resp = HttpResponse::newHttpResponse();
	//resp->setBody("<p>Hello, world!</p>");
	//resp->setExpiredTime(0);
	//callback(resp);

	DRequest *request = DRequest::get();
	request->application = this;
	request->request = std::shared_ptr<drogon::HttpRequestImpl>(req);
	request->callback = callback; //std::move(callback);

	request->setup_url_stack();

	handle_request(request);
}

void DWebApplication::on_new_websock_request(const HttpRequestImplPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const WebSocketConnectionImplPtr &wsConnPtr) {
	LOG_INFO << "on_new_websock_request";

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
void DWebApplication::on_connection(const trantor::TcpConnectionPtr &conn) {
	static std::mutex mtx;
	LOG_TRACE << "connect!!!" << _max_connection_num
			  << " num=" << _connection_num.load();

	if (conn->connected()) {
		if (_connection_num.fetch_add(1, std::memory_order_relaxed) >= _max_connection_num) {
			LOG_ERROR << "too much connections!force close!";
			conn->forceClose();
			return;
		} else if (_max_connection_num_per_ip > 0) {
			{
				std::lock_guard<std::mutex> lock(mtx);
				auto iter = _connections_num_map.find(conn->peerAddr().toIp());

				if (iter == _connections_num_map.end()) {
					_connections_num_map[conn->peerAddr().toIp()] = 1;
				} else if (iter->second++ > _max_connection_num_per_ip) {
					conn->getLoop()->queueInLoop([conn]() { conn->forceClose(); });

					return;
				}
			}
		}

		for (auto &advice : _new_connection_advices) {
			if (!advice(conn->peerAddr(), conn->localAddr())) {
				conn->forceClose();
				return;
			}
		}

	} else {

		if (!conn->hasContext()) {
			// If the connection is connected to the SSL port and then
			// disconnected before the SSL handshake.
			return;
		}

		_connection_num.fetch_sub(1, std::memory_order_relaxed);

		if (_max_connection_num_per_ip > 0) {
			std::lock_guard<std::mutex> lock(mtx);

			auto iter = _connections_num_map.find(conn->peerAddr().toIp());

			if (iter != _connections_num_map.end()) {
				--iter->second;
				if (iter->second <= 0) {
					_connections_num_map.erase(iter);
				}
			}
		}
	}
}

void DWebApplication::find_session_for_request(const HttpRequestImplPtr &req) {
	if (_use_session) {
		std::string sessionId = req->getCookie("JSESSIONID");
		bool needSetJsessionid = false;

		if (sessionId.empty()) {
			sessionId = utils::getUuid();
			needSetJsessionid = true;
		}

		req->setSession(_session_manager->getSession(sessionId, needSetJsessionid));
	}
}

//to request
//void forward(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,const std::string &hostString,double timeout);
//void forward(const HttpRequestImplPtr &req,std::function<void(const HttpResponsePtr &)> &&callback,const std::string &hostString,double timeout = 0);

DWebApplication::DWebApplication() :
		_listener_manager(new ListenerManager()) {
	//staticFileRouterPtr_(new StaticFileRouter{}),
	/*
		httpCtrlsRouterPtr_(new HttpControllersRouter(*staticFileRouterPtr_,
				postRoutingAdvices_,
				postRoutingObservers_,
				preHandlingAdvices_,
				preHandlingObservers_,
				postHandlingAdvices_)),
		httpSimpleCtrlsRouterPtr_(
				new HttpSimpleControllersRouter(*httpCtrlsRouterPtr_,
						postRoutingAdvices_,
						postRoutingObservers_,
						preHandlingAdvices_,
						preHandlingObservers_,
						postHandlingAdvices_)),*/
	//websockCtrlsRouterPtr_(
	//		new WebsocketControllersRouter(postRoutingAdvices_,
	//				postRoutingObservers_)),
	//listenerManagerPtr_(new ListenerManager),
	//pluginsManagerPtr_(new PluginsManager),
	//uploadPath_(rootPath_ + "uploads") {

	_loop = new trantor::EventLoop();

	//_listener_manager = new ListenerManager();
	_upload_path = _root_path + "uploads";
}

DWebApplication::~DWebApplication() {
	main_route_map.clear();
	error_handler_map.clear();
	middlewares.clear();
}
