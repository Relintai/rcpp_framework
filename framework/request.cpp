#include "request.h"

void Request::reset() {
	http_parser = nullptr;
	session = nullptr;
    finalized = false;

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
