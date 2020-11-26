#include "request.h"

void Request::next_stage() {
	if (current_middleware_index == (*middleware_stack).size()) {
		handler_func(this);
		return;
	}

	(*middleware_stack)[current_middleware_index++](this);
}

void Request::send() {
	if (http_parser->isKeepAlive()) {
		response->addHeadValue("Connection", "Keep-Alive");

		std::string result = response->getResult();

		(*session)->send(result.c_str(), result.size());
	} else {
		response->addHeadValue("Connection", "Close");

		std::string result = response->getResult();

		const HttpSession::Ptr lsession = (*session);

		(*session)->send(result.c_str(), result.size(), [lsession]() { lsession->postShutdown(); });
	}

	RequestPool::return_request(this);
}

void Request::reset() {
	http_parser = nullptr;
	session = nullptr;
	current_middleware_index = 0;
	middleware_stack = nullptr;

	if (response)
		delete response;

	response = new HttpResponse();
}

Request::Request() {
	response = nullptr;

	reset();
}

Request::~Request() {
	delete response;
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
