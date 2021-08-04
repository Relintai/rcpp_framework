
#include "http_session.h"

void HTTPSession::add_object(const std::string &key, Object *obj) {
	std::lock_guard<std::mutex> lock(_mutex);

	_data[key] = obj;
}
void HTTPSession::remove_object(const std::string &key) {
	std::lock_guard<std::mutex> lock(_mutex);

	_data.erase(key);
}
Object *HTTPSession::get_object(const std::string &key) {
	//don't lock here

	return _data[key];
}

void HTTPSession::add_int(const std::string &key, const int val) {
	std::lock_guard<std::mutex> lock(_mutex);

	_int_data[key] = val;
}
void HTTPSession::remove_int(const std::string &key) {
	std::lock_guard<std::mutex> lock(_mutex);

	_int_data.erase(key);
}
int HTTPSession::get_int(const std::string &key) {
	//don't lock here

	return _int_data[key];
}

void HTTPSession::clear() {
	_data.clear();
	_int_data.clear();
}

void HTTPSession::reset() {
	clear();
	session_id = "";
}

HTTPSession::HTTPSession() {
}

HTTPSession::~HTTPSession() {
	clear();
}