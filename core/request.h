#ifndef REQUEST_H
#define REQUEST_H

#include <mutex>
#include <vector>

#include <brynet/net/http/HttpFormat.hpp>
#include <brynet/net/http/HttpService.hpp>

#include "handler_instance.h"

class BryWebApplication;

class Request {
public:
	HTTPParser::Ptr http_parser;
	HttpSession::Ptr session;
	HttpResponse *response;
	BryWebApplication *application;

	uint32_t current_middleware_index;
	HandlerInstance handler_instance;
	std::vector<HandlerInstance> *middleware_stack;

	std::string head;
	std::string body;
	std::string footer;
	std::string compiled_body;

	std::string file_path;
	long file_size;
	long current_file_progress;
	long file_chunk_size;
	bool file_next;

	bool connection_closed;

	void compile_body();
	void compile_and_send_body();
	void next_stage();
	void send();
	void send_file(const std::string &p_file_path);
	void send_error(int error_code);
	void reset();

	void setup_url_stack();
	std::string get_path() const;
	const std::string &get_path_full() const;
	const std::string &get_path_segment(const uint32_t i) const;
	const std::string &get_current_path_segment() const;
	uint32_t get_path_segment_count() const;
	uint32_t get_current_segment_index() const;
	uint32_t get_remaining_segment_count() const;
	void pop_path();
	void push_path();

	void update();

	Request();
	~Request();

protected:
	void _progress_send_file();
	void _file_chunk_sent();

	std::vector<std::string> _path_stack;
	uint32_t _path_stack_pointer;
};

class RequestPool {
public:
	static Request *get_request();
	static void return_request(Request *request);

	RequestPool();
	~RequestPool();

protected:
	static std::mutex _mutex;
	static std::vector<Request *> _requests;
};

#endif