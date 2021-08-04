#include "request.h"

#include "web_application.h"

#include "core/http/cookie.h"

const std::string &DRequest::get_cookie(const std::string &key) {
	return request->getCookie(key);
}
void DRequest::add_cookie(const ::Cookie &cookie) {
	_added_cookies.push_back(cookie);
}
void DRequest::remove_cookie(const std::string &key) {
	_removed_cookies.push_back(key);
}

void DRequest::send() {
	//if (connection_closed) {
	//	DRequestPool::return_request(this);
	//	return;
	//}

	drogon::HttpResponsePtr response = drogon::HttpResponse::newHttpResponse();

	_response_additional_setup(response);

	response->setBody(compiled_body);

	response->setExpiredTime(0);
	callback(response);

	pool();
}

void DRequest::send_file(const std::string &p_file_path) {
	drogon::HttpResponsePtr response = drogon::HttpResponse::newFileResponse(p_file_path, "", drogon::getContentType(p_file_path));

	_response_additional_setup(response);

	callback(response);

	pool();
}

void DRequest::reset() {
	Request::reset();

	request.reset();

	//response = new HttpResponse();
}

std::string DRequest::parser_get_path() {
	return request->getPath();
}

void DRequest::update() {
	if (file_next) {
		file_next = false;
		_progress_send_file();
	}
}

DRequest *DRequest::get() {
	return _request_pool.get_request();
}
void DRequest::pool(DRequest *request) {
	return _request_pool.return_request(request);
}
void DRequest::pool() {
	DRequest::pool(this);
}

DRequest::DRequest() :
		Request() {

	//This value will need benchmarks, 2 MB seems to be just as fast for me as 4 MB, but 1MB is slower
	//It is a tradeoff on server memory though, as every active download will consume this amount of memory
	//where the file is bigger than this number
	file_chunk_size = 1 << 21; //2MB

	reset();
}

DRequest::~DRequest() {
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

void DRequest::_response_additional_setup(const drogon::HttpResponsePtr &req) {
	for (int i = 0; i < _added_cookies.size(); ++i) {
		::Cookie &co = _added_cookies[i];
		drogon::Cookie c;
		c.setDomain(co.domain);
		//todo
		//c.setExpiresDate
		c.setHttpOnly(co.http_only);
		c.setKey(co.key);
		c.setPath(co.path);
		c.setSecure(co.secure);
		c.setValue(co.value);

		req->addCookie(c);
	}

	for (int i = 0; i < _removed_cookies.size(); ++i) {
		req->removeCookie(_removed_cookies[i]);
	}
}

RequestPool<DRequest> DRequest::_request_pool;