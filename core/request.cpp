#include "request.h"

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

	response->setBody(compiled_body);
}

void Request::compile_and_send_body() {
	compile_body();
	send();
}

void Request::next_stage() {
	if (current_middleware_index == (*middleware_stack).size()) {
		handler_instance.handler_func(handler_instance.instance, this);
		return;
	}

	const HandlerInstance &hi = (*middleware_stack)[current_middleware_index++];

	hi.handler_func(hi.instance, this);
}

void Request::send() {
	if (http_parser->isKeepAlive()) {
		response->addHeadValue("Connection", "Keep-Alive");

		std::string result = response->getResult();

		(*session)->send(result.c_str(), result.size());
	} else {
		response->addHeadValue("Connection", "Close");

		std::string result = response->getResult();

		const brynet::net::http::HttpSession::Ptr lsession = (*session);

		(*session)->send(result.c_str(), result.size(), [lsession]() { lsession->postShutdown(); });
	}

	RequestPool::return_request(this);
}

void Request::send_file(const std::string &p_file_path) {
	file_path = p_file_path;

	FILE *f = fopen(file_path.c_str(), "rb");

	if (!f) {
		printf("send_file: Error: Download: file doesn't exists! %s\n", file_path.c_str());

		return;
	}

	fseek(f, 0, SEEK_END);
	file_size = ftell(f);
	fclose(f);

	response->addHeadValue("Connection", "Close");
	std::string result = "HTTP/1.1 200 OK\r\nConnection: Close\r\n\r\n";
	const brynet::net::http::HttpSession::Ptr lsession = (*session);

	(*session)->send(result.c_str(), result.size(), [this]() { this->_progress_send_file(); });
}

void Request::reset() {
	http_parser = nullptr;
	session = nullptr;
	current_middleware_index = 0;
	middleware_stack = nullptr;
	_path_stack.clear();
	_path_stack_pointer = 0;
	file_size = 0;
	current_file_progress = 0;

	head.clear();
	body.clear();
	footer.clear();
	compiled_body.clear();

	if (response)
		delete response;

	response = new brynet::net::http::HttpResponse();
}

void Request::setup_url_stack() {
	std::string path = http_parser->getPath();

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
	return http_parser->getPath();
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

Request::Request() {
	response = nullptr;
	//file_chunk_size = 1 << 20; //1MB
	file_chunk_size = 1 << 23;

	reset();
}

Request::~Request() {
	delete response;
}

void Request::_progress_send_file() {
	const brynet::net::http::HttpSession::Ptr lsession = (*session);

	if (current_file_progress >= file_size) {
		lsession->postShutdown();

		RequestPool::return_request(this);

		return;
	}

	FILE *f = fopen(file_path.c_str(), "rb");

	if (!f) {
		printf("Error: Download: In progress file doesn't exists anymore! %s\n", file_path.c_str());

		lsession->postShutdown();

		RequestPool::return_request(this);

		return;
	}

	fseek(f, current_file_progress, SEEK_SET);

	long nfp = current_file_progress + file_chunk_size;

	long csize = file_chunk_size;
	if (nfp >= file_size) {
		csize = (file_size - current_file_progress);
	}

	body.resize(csize);

	fread(&body[0], 1, csize, f);
	fclose(f);

	current_file_progress = nfp;

	(*session)->send(body.c_str(), body.size(), [this]() { this->_progress_send_file(); });
}

Request *RequestPool::get_request() {
	_mutex.lock();

	Request *request;

	if (_requests.size() == 0) {
		_mutex.unlock();

		request = new Request();

		return request;
	}

	request = _requests[_requests.size() - 1];
	_requests.pop_back();

	_mutex.unlock();

	return request;
}

void RequestPool::return_request(Request *request) {
	request->reset();

	_mutex.lock();
	_requests.push_back(request);
	_mutex.unlock();
}

RequestPool::RequestPool() {
}

RequestPool::~RequestPool() {
	for (uint32_t i = 0; i < _requests.size(); ++i) {
		delete _requests[i];
	}

	_requests.clear();
}

std::mutex RequestPool::_mutex;
std::vector<Request *> RequestPool::_requests;
