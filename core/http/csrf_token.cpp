#include "csrf_token.h"

#include "core/hash/sha256.h"
#include "http_session.h"
#include "request.h"

bool CSRFTokenMiddleware::on_before_handle_request_main(Request *request) {
	switch (request->get_method()) {
		case HTTP_METHOD_POST:
		case HTTP_METHOD_DELETE:
		case HTTP_METHOD_PATCH:
		case HTTP_METHOD_PUT: {

			if (!request->session.is_valid()) {
				request->send_error(HTTP_STATUS_CODE_401_UNAUTHORIZED);
				return true;
			}

			if (!request->validate_csrf_token()) {
				request->send_error(HTTP_STATUS_CODE_401_UNAUTHORIZED);
				return true;
			}

			break;
		}
		default:
			break;
	}

	// don't create the session itself
	if (!request->session.is_valid()) {
		return false;
	}

	if (!request->has_csrf_token()) {
		request->set_csrf_token(create_token());
	}

	return false;
}

String CSRFTokenMiddleware::create_token() {
	return "test";
}

CSRFTokenMiddleware::CSRFTokenMiddleware() {
}
CSRFTokenMiddleware::~CSRFTokenMiddleware() {
}
