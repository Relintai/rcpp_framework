#include "request.h"

#include "core/http/cookie.h"
#include "web_application.h"

#include "http_session.h"

#include "session_manager.h"

HTTPSession *Request::get_or_create_session() {
	if (session) {
		return session;
	}

	session = SessionManager::get_singleton()->create_session();

	return session;
}

const std::string &Request::get_cookie(const std::string &key) {
}

void Request::add_cookie(const ::Cookie &cookie) {
}

void Request::remove_cookie(const std::string &key) {
}

HTTPMethod Request::get_method() const {
	return HTTP_METHOD_GET;
}

const std::string &Request::get_parameter(const std::string &key) const {
	return "";
}

void Request::compile_body() {
	compiled_body.reserve(body.size() + head.size() + 13 + 14 + 15);

	//13
	compiled_body += "<html>"
					 "<head>";

	compiled_body += head;

	//14
	compiled_body += "</head>"
					 "<body>";

	compiled_body += body;
	compiled_body += footer;

	//15
	compiled_body += "</body>"
					 "</html>";

	//response->setBody(compiled_body);
}

void Request::compile_and_send_body() {
	compile_body();
	send();
}

void Request::next_stage() {
	//if (middleware_stack == nullptr) {
	//	printf("Error Request::next_stage-> middleware_stack == nullptr\n");
	//}

	if (current_middleware_index == (*middleware_stack).size()) {
		handler_instance.handler_func(handler_instance.instance, this);
		return;
	}

	const HandlerInstance &hi = (*middleware_stack)[current_middleware_index++];

	hi.handler_func(hi.instance, this);
}

void Request::send() {
	//if (connection_closed) {
	//	RequestPool::return_request(this);
	//	return;
	//}

	//RequestPool::return_request(this);
}

void Request::send_file(const std::string &p_file_path) {
	//RequestPool::return_request(this);
}

void Request::send_error(int error_code) {
	application->send_error(error_code, this);
}

void Request::reset() {
	session = nullptr;
	application = nullptr;
	current_middleware_index = 0;
	middleware_stack = nullptr;
	_path_stack.clear();
	_path_stack_pointer = 0;
	file_size = 0;
	current_file_progress = 0;
	connection_closed = false;
	_full_path = "";

	head.clear();
	body.clear();
	footer.clear();
	compiled_body.clear();

	data.clear();
}

std::string Request::parser_get_path() {
	return "";
}

void Request::setup_url_stack() {
	_full_path = parser_get_path();
	std::string path = parser_get_path();

	size_t pos = 0;
	std::string st;
	while ((pos = path.find("/")) != std::string::npos) {
		st = path.substr(0, pos);

		if (st.size() != 0)
			_path_stack.push_back(st);

		path.erase(0, pos + 1);
	}

	if (path.size() != 0)
		_path_stack.push_back(path);
}

std::string Request::get_path() const {
	std::string path = "";

	for (uint32_t i = _path_stack_pointer; i < _path_stack.size(); ++i) {
		path += _path_stack[i];
		path += "/";
	}

	return path;
}

const std::string &Request::get_path_full() const {
	return _full_path;
}

const std::string &Request::get_path_segment(const uint32_t i) const {
	return _path_stack[i];
}

const std::string &Request::get_current_path_segment() const {
	if (_path_stack_pointer >= _path_stack.size()) {
		//for convenience
		static const std::string e_str = "";
		return e_str;
	}

	return _path_stack[_path_stack_pointer];
}

uint32_t Request::get_path_segment_count() const {
	return _path_stack.size();
}

uint32_t Request::get_current_segment_index() const {
	return _path_stack_pointer;
}

uint32_t Request::get_remaining_segment_count() const {
	if (_path_stack_pointer > _path_stack.size()) {
		return 0;
	}

	return _path_stack.size() - _path_stack_pointer;
}

void Request::pop_path() {
	_path_stack_pointer -= 1;
}

void Request::push_path() {
	_path_stack_pointer += 1;
}

void Request::update() {
}

void Request::pool() {
}

Request::Request() {
	//This value will need benchmarks, 2 MB seems to be just as fast for me as 4 MB, but 1MB is slower
	//It is a tradeoff on server memory though, as every active download will consume this amount of memory
	//where the file is bigger than this number
	file_chunk_size = 1 << 21; //2MB
}

Request::~Request() {
}
