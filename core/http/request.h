#ifndef REQUEST_H
#define REQUEST_H

#include <mutex>
#include <vector>
#include <map>

#include "core/object.h"

#include "handler_instance.h"

class WebApplication;
class Cookie;

class Request {
public:
	WebApplication *application;

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

	std::map<std::string, Object*> data;

	virtual const std::string &get_cookie(const std::string &key);
	virtual void add_cookie(const ::Cookie &cookie);
	virtual void remove_cookie(const std::string &key);

	virtual void compile_body();
	virtual void compile_and_send_body();
	virtual void next_stage();
	virtual void send();
	virtual void send_file(const std::string &p_file_path);
	virtual void send_error(int error_code);
	virtual void reset();
	virtual std::string parser_get_path();

	void setup_url_stack();
	std::string get_path() const;
	virtual const std::string &get_path_full() const;
	const std::string &get_path_segment(const uint32_t i) const;
	const std::string &get_current_path_segment() const;
	uint32_t get_path_segment_count() const;
	uint32_t get_current_segment_index() const;
	uint32_t get_remaining_segment_count() const;
	void pop_path();
	void push_path();

	virtual void update();
	virtual void pool();

	Request();
	virtual ~Request();

protected:
	std::string _full_path;
	std::vector<std::string> _path_stack;
	uint32_t _path_stack_pointer;
};

template <class T>
class RequestPool {
public:
	T *get_request();
	void return_request(T *request);

	RequestPool();
	~RequestPool();

protected:
	std::mutex _mutex;
	std::vector<T *> _requests;
};

template <class T>
T *RequestPool<T>::get_request() {
	_mutex.lock();

	T *request;

	if (_requests.size() == 0) {
		_mutex.unlock();

		request = new T();

		return request;
	}

	request = _requests[_requests.size() - 1];
	_requests.pop_back();

	_mutex.unlock();

	request->reset();

	return request;
}

template <class T>
void RequestPool<T>::return_request(T *request) {
	_mutex.lock();
	_requests.push_back(request);
	_mutex.unlock();
}

template <class T>
RequestPool<T>::RequestPool() {
}

template <class T>
RequestPool<T>::~RequestPool() {
	for (uint32_t i = 0; i < _requests.size(); ++i) {
		delete _requests[i];
	}

	_requests.clear();
}

#endif