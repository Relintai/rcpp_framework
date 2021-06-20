#ifndef DWEB_APPLICATION_H
#define WEB_APPLICATION_H

#include "core/object.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <mutex>

#include "handler_instance.h"

#include <trantor/utils/Logger.h>
#include <trantor/net/InetAddress.h>

#include "http/HttpRequestImpl.h"
#include "http/HttpResponse.h"

#include "./drogon/lib/src/impl_forwards.h"

#include "http/SessionManager.h"

using namespace drogon;

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

	void addListener(const std::string &ip, uint16_t port, bool useSSL, const std::string &certFile, const std::string &keyFile, bool useOldTLS, const std::vector<std::pair<std::string, std::string> > &sslConfCmds);

	void setThreadNum(size_t threadNum);
	size_t getThreadNum() const;

	void setSSLConfigCommands(const std::vector<std::pair<std::string, std::string> > &sslConfCmds);
	void setSSLFiles(const std::string &certPath, const std::string &keyPath);

	void run();
	void stop();

	void enableSession(const size_t timeout);
	void disableSession();

	//todo think about it
	const std::string &getDocumentRoot() const;
	void setDocumentRoot(const std::string &rootPath);

	void setStaticFileHeaders(const std::vector<std::pair<std::string, std::string> > &headers);

	const std::string &getUploadPath() const;

	const std::shared_ptr<trantor::Resolver> &getResolver() const;

	void setUploadPath(const std::string &uploadPath);
	void setFileTypes(const std::vector<std::string> &types);

	void setMaxConnectionNum(size_t maxConnections);
	void setMaxConnectionNumPerIP(size_t maxConnectionsPerIP);

	void setLogPath(const std::string &logPath, const std::string &logfileBaseName, size_t logfileSize);
	void setLogLevel(trantor::Logger::LogLevel level);
	void enableSendfile(bool sendFile);
	void enableGzip(bool useGzip);
	bool isGzipEnabled() const;
	void enableBrotli(bool useBrotli);
	bool isBrotliEnabled() const;

	void setStaticFilesCacheTime(int cacheTime);
	int staticFilesCacheTime() const;

	void setIdleConnectionTimeout(size_t timeout);
	void setKeepaliveRequestsNumber(const size_t number);
	void setPipeliningRequestsNumber(const size_t number);

	void setGzipStatic(bool useGzipStatic);
	void setBrStatic(bool useGzipStatic);

	void setClientMaxBodySize(size_t maxSize);
	void setClientMaxMemoryBodySize(size_t maxSize);
	void setClientMaxWebSocketMessageSize(size_t maxSize);

	void setHomePage(const std::string &homePageFile);
	const std::string &getHomePage() const;

	void setImplicitPageEnable(bool useImplicitPage);
	bool isImplicitPageEnabled() const;
	void setImplicitPage(const std::string &implicitPageFile);

	const std::string &getImplicitPage() const;
	size_t getClientMaxBodySize() const;
	size_t getClientMaxMemoryBodySize() const;
	size_t getClientMaxWebSocketMessageSize() const;

	size_t keepaliveRequestsNumber() const;
	size_t pipeliningRequestsNumber() const;

	bool isRunning();

	void setUnicodeEscapingInJson(bool enable);

	bool isUnicodeEscapingUsedInJson() const;
	void setFloatPrecisionInJson(unsigned int precision, const std::string &precisionType);

	const std::pair<unsigned int, std::string> &getFloatPrecisionInJson() const;

	trantor::EventLoop *getLoop() const;
	trantor::EventLoop *getIOLoop(size_t id) const;

	void setServerHeaderField(const std::string &server);

	void enableServerHeader(bool flag);
	void enableDateHeader(bool flag);

	bool sendServerHeader() const;
	bool sendDateHeader() const;

	const std::string &getServerHeaderString() const;

	std::vector<trantor::InetAddress> getListeners() const;

	bool useSendfile() const;

	bool supportSSL() const;

	size_t getCurrentThreadIndex() const;

	void enableReusePort(bool enable);

	bool reusePort() const;

	void onAsyncRequest(const HttpRequestImplPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
	void onNewWebsockRequest(const HttpRequestImplPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const WebSocketConnectionImplPtr &wsConnPtr);
	void onConnection(const trantor::TcpConnectionPtr &conn);

	void findSessionForRequest(const HttpRequestImplPtr &req);

	//to request
	//void forward(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,const std::string &hostString,double timeout);
	//void forward(const HttpRequestImplPtr &req,std::function<void(const HttpResponsePtr &)> &&callback,const std::string &hostString,double timeout = 0);

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

	// We use a uuid string as session id;
	// set sessionTimeout_=0 to make location session valid forever based on
	// cookies;
	size_t _session_timeout{ 0 };
	size_t _idle_connection_timeout{ 60 };
	bool _use_session{ false };
	std::string _server_header { "server: rcpp_framework\r\n" };

	std::string _root_path { "./" };
	std::string _upload_path;
	std::atomic_bool _running { false };

	size_t _thread_num{ 1 };

	std::vector<std::pair<std::string, std::string> > _ssl_conf_cmds;
	std::string _ssl_cert_path;
	std::string _ssl_key_path;

	size_t _max_connection_num_per_ip{ 0 };
	std::unordered_map<std::string, size_t> _connections_num_map;

	int64_t _max_connection_num{ 100000 };
	std::atomic<int64_t> _connection_num{ 0 };

	std::string _log_path;
	std::string _logfile_base_name;
	size_t _logfile_size{ 100000000 };
	size_t _keepalive_requests_number{ 0 };
	size_t _pipelining_requests_number{ 0 };
	bool _use_sendfile{ true };
	bool _use_gzip{ true };
	bool _use_brotli{ false };
	bool _using_unicode_escaping{ true };

	size_t _client_max_body_size{ 1024 * 1024 };
	size_t _client_max_memory_body_size{ 64 * 1024 };
	size_t _client_max_web_socket_message_size{ 128 * 1024 };
	std::string _home_page_file{ "index.html" };

	std::unique_ptr<SessionManager> _sessionManagerPtr;

	bool _enable_server_header{ true };
	bool _enable_date_header{ true };
	bool _reuse_port{ false };
};

#endif