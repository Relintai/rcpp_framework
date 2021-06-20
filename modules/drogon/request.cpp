#include "request.h"

#include "web_application.h"

void DRequest::compile_body() {
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

void DRequest::compile_and_send_body() {
	compile_body();
	send();
}

void DRequest::next_stage() {
	if (current_middleware_index == (*middleware_stack).size()) {
		handler_instance.handler_func(handler_instance.instance, this);
		return;
	}

	const DHandlerInstance &hi = (*middleware_stack)[current_middleware_index++];

	hi.handler_func(hi.instance, this);
}

void DRequest::send() {
	//if (connection_closed) {
	//	DRequestPool::return_request(this);
	//	return;
	//}

/*
	if (http_parser->isKeepAlive()) {
		response->addHeadValue("Connection", "Keep-Alive");

		std::string result = response->getResult();

		session->send(result.c_str(), result.size());
	} else {
		response->addHeadValue("Connection", "Close");

		std::string result = response->getResult();

		HttpSession::Ptr lsession = session;

		session->send(result.c_str(), result.size(), [lsession]() { lsession->postShutdown(); });
	}
*/
	DRequestPool::return_request(this);
}

void DRequest::send_file(const std::string &p_file_path) {
	//if (connection_closed) {
	//	DRequestPool::return_request(this);
	//	return;
	//}
/*
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

	application->register_request_update(this);

	session->send(result.c_str(), result.size(), [this]() { this->_file_chunk_sent(); });
	*/
}

void DRequest::send_error(int error_code) {
	application->send_error(error_code, this);
}

void DRequest::reset() {
	application = nullptr;
	current_middleware_index = 0;
	middleware_stack = nullptr;
	_path_stack.clear();
	_path_stack_pointer = 0;
	file_size = 0;
	current_file_progress = 0;
	connection_closed = false;

	head.clear();
	body.clear();
	footer.clear();
	compiled_body.clear();

	if (response)
		delete response;

	//response = new HttpResponse();
}

void DRequest::setup_url_stack() {
	/*
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
		*/
}

std::string DRequest::get_path() const {
	std::string path = "";

	for (uint32_t i = _path_stack_pointer; i < _path_stack.size(); ++i) {
		path += _path_stack[i];
		path += "/";
	}

	return path;
}

const std::string &DRequest::get_path_full() const {
	static std::string a = "";
	return a;
	//return http_parser->getPath();
}

const std::string &DRequest::get_path_segment(const uint32_t i) const {
	return _path_stack[i];
}

const std::string &DRequest::get_current_path_segment() const {
	if (_path_stack_pointer >= _path_stack.size()) {
		//for convenience
		static const std::string e_str = "";
		return e_str;
	}

	return _path_stack[_path_stack_pointer];
}

uint32_t DRequest::get_path_segment_count() const {
	return _path_stack.size();
}

uint32_t DRequest::get_current_segment_index() const {
	return _path_stack_pointer;
}

uint32_t DRequest::get_remaining_segment_count() const {
	if (_path_stack_pointer > _path_stack.size()) {
		return 0;
	}

	return _path_stack.size() - _path_stack_pointer;
}

void DRequest::pop_path() {
	_path_stack_pointer -= 1;
}

void DRequest::push_path() {
	_path_stack_pointer += 1;
}

void DRequest::update() {
	if (file_next) {
		file_next = false;
		_progress_send_file();
	}
}

DRequest::DRequest() {
	response = nullptr;

	//This value will need benchmarks, 2 MB seems to be just as fast for me as 4 MB, but 1MB is slower
	//It is a tradeoff on server memory though, as every active download will consume this amount of memory
	//where the file is bigger than this number
	file_chunk_size = 1 << 21; //2MB

	reset();
}

DRequest::~DRequest() {
	delete response;
}

void DRequest::_progress_send_file() {
	/*
	if (connection_closed) {
		DRequestPool::return_request(this);
		return;
	}

	if (current_file_progress >= file_size) {
		session->postShutdown();

		DRequestPool::return_request(this);

		return;
	}

	FILE *f = fopen(file_path.c_str(), "rb");

	if (!f) {
		printf("Error: Download: In progress file doesn't exists anymore! %s\n", file_path.c_str());

		application->unregister_request_update(this);

		session->postShutdown();

		DRequestPool::return_request(this);

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

	session->send(body.c_str(), body.size(), [this]() { this->_file_chunk_sent(); });
	*/
}

void DRequest::_file_chunk_sent() {
	file_next = true;
}

DRequest *DRequestPool::get_request() {
	_mutex.lock();

	DRequest *request;

	if (_requests.size() == 0) {
		_mutex.unlock();

		request = new DRequest();

		return request;
	}

	request = _requests[_requests.size() - 1];
	_requests.pop_back();

	_mutex.unlock();

	request->reset();

	return request;
}

void DRequestPool::return_request(DRequest *request) {
	_mutex.lock();
	_requests.push_back(request);
	_mutex.unlock();
}

DRequestPool::DRequestPool() {
}

DRequestPool::~DRequestPool() {
	for (uint32_t i = 0; i < _requests.size(); ++i) {
		delete _requests[i];
	}

	_requests.clear();
}

std::mutex DRequestPool::_mutex;
std::vector<DRequest *> DRequestPool::_requests;
