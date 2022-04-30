#include "request.h"

#include "drogon_web_server.h"

#include "web/http/cookie.h"

const String DRequest::get_cookie(const String &key) {
	return request->getCookie(key);
}
void DRequest::add_cookie(const ::Cookie &cookie) {
	_added_cookies.push_back(cookie);
}
void DRequest::remove_cookie(const String &key) {
	_removed_cookies.push_back(key);
}

HTTPMethod DRequest::get_method() const {
	// the enums are in the same order

	return static_cast<HTTPMethod>(static_cast<int>(request->getMethod()));
}

void DRequest::parse_files() {
	if (_multipart_parser) {
		return;
	}

	_multipart_parser = new drogon::MultiPartParser();
	if (_multipart_parser->parse(request) != 0) {
		RLOG_ERR("asd2");
		delete _multipart_parser;
		_multipart_parser = nullptr;
	}
}
int DRequest::get_file_count() const {
	if (!_multipart_parser) {
		return 0;
	}

	return _multipart_parser->getFiles().size();
}
int DRequest::get_file_length(const int index) const {
	if (!_multipart_parser) {
		return 0;
	}

	ERR_FAIL_INDEX_V(index, _multipart_parser->getFiles().size(), 0);

	return static_cast<int>(_multipart_parser->getFiles()[index].fileLength());
}
const uint8_t *DRequest::get_file_data(const int index) const {
	if (!_multipart_parser) {
		return nullptr;
	}

	ERR_FAIL_INDEX_V(index, _multipart_parser->getFiles().size(), nullptr);

	return reinterpret_cast<const uint8_t*>(_multipart_parser->getFiles()[index].fileData());
}

const String DRequest::get_parameter(const String &key) const {
	return request->getParameter(key);
}

void DRequest::send_redirect(const String &location, const HTTPStatusCode status_code) {
	drogon::HttpResponsePtr response = drogon::HttpResponse::newRedirectionResponse(location, static_cast<const HttpStatusCode>(static_cast<const int>(status_code)));

	_response_additional_setup(response);

	response->setBody(body);

	response->setExpiredTime(0);
	callback(response);

	pool();
}

void DRequest::send() {
	// if (connection_closed) {
	//	DRequestPool::return_request(this);
	//	return;
	// }

	drogon::HttpResponsePtr response = drogon::HttpResponse::newHttpResponse();

	_response_additional_setup(response);

	response->setStatusCode(static_cast<const HttpStatusCode>(static_cast<const int>(_status_code)));
	response->setBody(compiled_body);

	response->setExpiredTime(0);
	callback(response);

	pool();
}

void DRequest::send_file(const String &p_file_path) {
	drogon::HttpResponsePtr response = drogon::HttpResponse::newFileResponse(p_file_path, "", drogon::getContentType(p_file_path));

	_response_additional_setup(response);

	callback(response);

	pool();
}

void DRequest::reset() {
	Request::reset();

	request.reset();

	_added_cookies.clear();
	_removed_cookies.clear();

	if (_multipart_parser) {
		delete _multipart_parser;
		_multipart_parser = nullptr;
	}

	// response = new HttpResponse();
}

String DRequest::parser_get_path() {
	return request->getPath();
}

String DRequest::get_host() const {
	// todo
	return "/";
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

	_multipart_parser = nullptr;

	// This value will need benchmarks, 2 MB seems to be just as fast for me as 4 MB, but 1MB is slower
	// It is a tradeoff on server memory though, as every active download will consume this amount of memory
	// where the file is bigger than this number
	file_chunk_size = 1 << 21; // 2MB

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
		// todo
		// c.setExpiresDate
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