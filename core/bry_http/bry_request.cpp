#include "bry_request.h"

#include "web_application.h"

void BryRequest::send() {
	//if (connection_closed) {
	//	RequestPool::return_request(this);
	//	return;
	//}

	if (http_parser->isKeepAlive()) {
		response->addHeadValue("Connection", "Keep-Alive");

		response->setBody(compiled_body);

		std::string result = response->getResult();

		session->send(result.c_str(), result.size());
	} else {
		response->addHeadValue("Connection", "Close");

		response->setBody(compiled_body);

		std::string result = response->getResult();

		HttpSession::Ptr lsession = session;

		session->send(result.c_str(), result.size(), [lsession]() { lsession->postShutdown(); });
	}

	pool();
}

void BryRequest::send_file(const std::string &p_file_path) {
	//if (connection_closed) {
	//	RequestPool::return_request(this);
	//	return;
	//}

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
}

void BryRequest::reset() {
	application = nullptr;
	http_parser = nullptr;
	session = nullptr;
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

//todo fix
	//response = new HttpResponse();
}

void BryRequest::update() {
	if (file_next) {
		file_next = false;
		_progress_send_file();
	}
}

BryRequest *BryRequest::get() {
	return _request_pool.get_request();
}
void BryRequest::pool(BryRequest *request) {
	return _request_pool.return_request(request);
}
void BryRequest::pool() {
	BryRequest::pool(this);
}

BryRequest::BryRequest() :
		Request() {
	response = nullptr;
}

BryRequest::~BryRequest() {
	delete response;
}

void BryRequest::_progress_send_file() {
	if (connection_closed) {
		pool();
		return;
	}

	if (current_file_progress >= file_size) {
		session->postShutdown();

		pool();

		return;
	}

	FILE *f = fopen(file_path.c_str(), "rb");

	if (!f) {
		printf("Error: Download: In progress file doesn't exists anymore! %s\n", file_path.c_str());

		application->unregister_request_update(this);

		session->postShutdown();

		pool();

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
}

void BryRequest::_file_chunk_sent() {
	file_next = true;
}

RequestPool<BryRequest> BryRequest::_request_pool;