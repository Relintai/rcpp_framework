#ifndef DWEB_APPLICATION_H
#define DWEB_APPLICATION_H

#include "core/object.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <mutex>

#include "handler_instance.h"

#include <trantor/net/InetAddress.h>
#include <trantor/net/Resolver.h>
#include <trantor/utils/Logger.h>

#include "http/HttpRequestImpl.h"
#include "http/HttpResponse.h"

#include "./drogon/lib/src/ListenerManager.h"
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

	//-------

	void add_listener(const std::string &ip, uint16_t port, 
						bool useSSL = false, const std::string &certFile = "", const std::string &keyFile = "", bool useOldTLS = false, 
						const std::vector<std::pair<std::string, std::string> > &sslConfCmds = {});

	void set_thread_num(size_t thread_num);
	size_t get_thread_num() const;

	void set_ssl_config_commands(const std::vector<std::pair<std::string, std::string> > &sslConfCmds);
	void set_ssl_files(const std::string &certPath, const std::string &keyPath);

	void run();
	void stop();

	void enable_session(const size_t timeout);
	void disable_session();

	//todo think about it
	const std::string &get_document_root() const;
	void set_document_root(const std::string &rootPath);

	void set_static_file_headers(const std::vector<std::pair<std::string, std::string> > &headers);

	const std::string &get_upload_path() const;

	const std::shared_ptr<trantor::Resolver> &get_resolver() const;

	void set_upload_path(const std::string &uploadPath);
	void set_file_types(const std::vector<std::string> &types);

	void set_max_connection_num(size_t maxConnections);
	void set_max_connection_num_per_ip(size_t maxConnectionsPerIP);

	void set_log_path(const std::string &logPath, const std::string &logfileBaseName, size_t logfileSize);
	void set_log_level(trantor::Logger::LogLevel level);
	void enable_sendfile(bool sendFile);
	void enable_gzip(bool useGzip);
	bool is_gzip_enabled() const;
	void enable_brotli(bool useBrotli);
	bool is_brotli_enabled() const;

	void set_static_files_cache_time(int cacheTime);
	int static_files_cache_time() const;

	void set_idle_connection_timeout(size_t timeout);
	void set_keepalive_requests_number(const size_t number);
	void set_pipelining_requests_number(const size_t number);

	void set_gzip_static(bool useGzipStatic);
	void set_br_static(bool useGzipStatic);

	void set_client_max_body_size(size_t maxSize);
	void set_client_max_memory_body_size(size_t maxSize);
	void set_client_max_web_socket_message_size(size_t maxSize);

	void set_home_page(const std::string &homePageFile);
	const std::string &get_home_page() const;

	void set_implicit_page_enable(bool useImplicitPage);
	bool is_implicit_page_enabled() const;
	void set_implicit_page(const std::string &implicitPageFile);

	const std::string &get_implicit_page() const;
	size_t get_client_max_body_size() const;
	size_t get_client_max_memory_body_size() const;
	size_t get_client_max_web_socket_message_size() const;

	size_t keepalive_requests_number() const;
	size_t pipelining_requests_number() const;

	bool is_running();

	void set_unicode_escaping_in_json(bool enable);

	bool is_unicode_escaping_used_in_json() const;
	void set_float_precision_in_json(unsigned int precision, const std::string &precisionType);

	const std::pair<unsigned int, std::string> &get_float_precision_in_json() const;

	trantor::EventLoop *get_loop() const;
	trantor::EventLoop *get_io_loop(size_t id) const;

	void set_server_header_field(const std::string &server);

	void enable_server_header(bool flag);
	void enable_date_header(bool flag);

	bool send_server_header() const;
	bool send_date_header() const;

	const std::string &get_server_header_string() const;

	std::vector<trantor::InetAddress> get_listeners() const;

	bool use_sendfile() const;

	bool support_ssl() const;

	size_t get_current_thread_index() const;

	void enable_reuse_port(bool enable);

	bool reuse_port() const;

	void on_async_request(const HttpRequestImplPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
	void on_new_websock_request(const HttpRequestImplPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const WebSocketConnectionImplPtr &wsConnPtr);
	void on_connection(const trantor::TcpConnectionPtr &conn);

	void find_session_for_request(const HttpRequestImplPtr &req);

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
	std::string _server_header{ "server: rcpp_framework\r\n" };

	std::string _root_path{ "./" };
	std::string _upload_path;
	std::atomic_bool _running{ false };

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

	std::pair<unsigned int, std::string> _float_precision_in_json{ 0, "significant" };

	size_t _client_max_body_size{ 1024 * 1024 };
	size_t _client_max_memory_body_size{ 64 * 1024 };
	size_t _client_max_web_socket_message_size{ 128 * 1024 };
	std::string _home_page_file{ "index.html" };

	const std::unique_ptr<ListenerManager> _listener_manager;
	std::unique_ptr<SessionManager> _session_manager;

	bool _enable_server_header{ true };
	bool _enable_date_header{ true };
	bool _reuse_port{ false };

	trantor::EventLoop *_loop;

	std::vector<std::function<void()> > _beginning_advices;
	std::vector<std::function<bool(const trantor::InetAddress &, const trantor::InetAddress &)> > _new_connection_advices;
	std::vector<std::function<HttpResponsePtr(const HttpRequestPtr &)> > _sync_advices;
	std::vector<std::function<void(const HttpRequestPtr &, const HttpResponsePtr &)> > _pre_sending_advices;
};

#endif