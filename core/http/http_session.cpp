
#include "http_session.h"

void HTTPSession::add_object(const String &key, Object *obj) {
	std::lock_guard<std::mutex> lock(_mutex);

	_data[key] = obj;
}
void HTTPSession::remove_object(const String &key) {
	std::lock_guard<std::mutex> lock(_mutex);

	_data.erase(key);
}
Object *HTTPSession::get_object(const String &key) {
	//don't lock here

	return _data[key];
}

void HTTPSession::add_reference(const String &key, const Ref<Reference> &ref) {
	std::lock_guard<std::mutex> lock(_mutex);

	_reference_data[key] = ref;
}
void HTTPSession::remove_reference(const String &key) {
	std::lock_guard<std::mutex> lock(_mutex);

	_reference_data.erase(key);
}
Ref<Reference> HTTPSession::get_reference(const String &key) {
	//don't lock here

	return _reference_data[key];
}

void HTTPSession::add_int(const String &key, const int val) {
	std::lock_guard<std::mutex> lock(_mutex);

	_int_data[key] = val;
}
void HTTPSession::remove_int(const String &key) {
	std::lock_guard<std::mutex> lock(_mutex);

	_int_data.erase(key);
}
int HTTPSession::get_int(const String &key) {
	//don't lock here

	return _int_data[key];
}

void HTTPSession::clear() {
	_data.clear();
	_int_data.clear();
	_reference_data.clear();
}

void HTTPSession::reset() {
	clear();
	session_id = "";
}

std::map<String, int> HTTPSession::get_int_data() {
	return _int_data;
}

HTTPSession::HTTPSession() {
	id = 0;
}

HTTPSession::~HTTPSession() {
	clear();
}