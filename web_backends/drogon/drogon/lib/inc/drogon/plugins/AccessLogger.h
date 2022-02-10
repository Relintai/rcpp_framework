/**
 *
 *  AccessLogger.h
 *
 */

#pragma once

#include <http/HttpRequest.h>
#include <http/HttpResponse.h>
#include <drogon/plugins/Plugin.h>
#include "core/log/async_file_logger.h"
#include <vector>

namespace drogon {
namespace plugin {
/**
 * @brief This plugin is used to print all requests to the log.
 *
 * The json configuration is as follows:
 *
 * @code
   {
      "name": "drogon::plugin::AccessLogger",
      "dependencies": [],
      "config": {
            "log_path": "./",
            "log_format": "",
            "log_file": "access.log",
            "log_size_limit": 0,
            "use_local_time": true,
            "log_index": 0
      }
   }
   @endcode
 *
 * log_format: a format string for access logging, there are several
 * placeholders that represent particular data.
 *     $date: the time when the log was printed.
 *     $request_date: the time when the request was created.
 *     $request_path|$path: the path of the request.
 *     $request_query|$query: the query string of the request.
 *     $request_url|$url: the URL of the request, equals to
 *                   $request_path+"?"+$request_query.
 *     $remote_addr: the remote address
 *     $local_addr: the local address
 *     $request_len|$body_bytes_received: the content length of the request.
 *     $method: the HTTP method of the request.
 *     $thread: the current thread number.
 *     $response_len|$body_bytes_sent: the content length of the response.
 *     $http_[header_name]: the header of the request.
 *     $cookie_[cookie_name]: the cookie of the request.
 *     $upstream_http_[header_name]: the header of the response sent to the
 *                                   client.
 *     $status_code: the status code of the response.
 *     $status: the status code and string of the response.
 *     $processing_time: request processing time in seconds with a microseconds
 *                       resolution; time elapsed between the request object was
 *                       created and response object was created.
 * @note If the format string is empty or not configured, a default value of
 * "$request_date $method $url [$body_bytes_received] ($remote_addr -
 * $local_addr) $status $body_bytes_sent $processing_time" is applied.
 *
 * log_path: Log file path, empty by default,in which case,logs are output to
 * the regular log file (or stdout based on the log configuration).
 *
 * log_file: The access log file name, 'access.log' by default. if the file name
 * does not contain a extension, the .log extension is used.
 *
 * log_size_limit: 0 bytes by default, when the log file size reaches
 * "log_size_limit", the log file is switched. Zero value means never switch
 *
 * log_index: The index of log output, 0 by default.
 *
 * Enable the plugin by adding the configuration to the list of plugins in the
 * configuration file.
 *
 */
class AccessLogger : public drogon::Plugin<AccessLogger> {
public:
	AccessLogger() {
	}
	void initAndStart(const Json::Value &config) override;
	void shutdown() override;

private:
	AsyncFileLogger asyncFileLogger_;
	int logIndex_{ 0 };
	bool useLocalTime_{ true };
	using LogFunction = std::function<void(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &)>;
	std::vector<LogFunction> logFunctions_;
	void logging(LogStream &stream,
			const drogon::HttpRequestPtr &req,
			const drogon::HttpResponsePtr &resp);
	void createLogFunctions(std::string format);
	LogFunction newLogFunction(const std::string &placeholder);
	std::map<std::string, LogFunction> logFunctionMap_;
	//$request_path
	static void outputReqPath(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$request_query
	static void outputReqQuery(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$request_url
	static void outputReqURL(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$date
	void outputDate(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &) const;
	//$request_date
	void outputReqDate(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &) const;
	//$remote_addr
	static void outputRemoteAddr(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$local_addr
	static void outputLocalAddr(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$request_len $body_bytes_received
	static void outputReqLength(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$response_len $body_bytes_sent
	static void outputRespLength(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$method
	static void outputMethod(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$thread
	static void outputThreadNumber(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$http_[header_name]
	static void outputReqHeader(LogStream &stream,
			const drogon::HttpRequestPtr &req,
			const std::string &headerName);
	//$cookie_[cookie_name]
	static void outputReqCookie(LogStream &stream,
			const drogon::HttpRequestPtr &req,
			const std::string &cookie);
	//$upstream_http_[header_name]
	static void outputRespHeader(LogStream &stream,
			const drogon::HttpResponsePtr &resp,
			const std::string &headerName);
	//$status
	static void outputStatusString(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$status_code
	static void outputStatusCode(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$processing_time
	static void outputProcessingTime(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
	//$upstream_http_content-type $upstream_http_content_type
	static void outputRespContentType(LogStream &,
			const drogon::HttpRequestPtr &,
			const drogon::HttpResponsePtr &);
};
} // namespace plugin
} // namespace drogon
