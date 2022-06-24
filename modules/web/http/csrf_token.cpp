#include "csrf_token.h"

#include "crypto/hash/sha256.h"
#include "http_session.h"
#include "request.h"
#include <time.h>

bool CSRFTokenMiddleware::on_before_handle_request_main(Request *request) {
	switch (request->get_method()) {
		case HTTP_METHOD_POST:
		case HTTP_METHOD_DELETE:
		case HTTP_METHOD_PATCH:
		case HTTP_METHOD_PUT: {

			if (shold_ignore(request)) {
				return false;
			}

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

bool CSRFTokenMiddleware::shold_ignore(Request *request) {
	const String &path = request->get_path_full();

	for (int i = 0; i < ignored_urls.size(); ++i) {
		if (path.starts_with(ignored_urls[i])) {
			return true;
		}
	}

	return false;
}

String CSRFTokenMiddleware::create_token() {
	Ref<SHA256> h = SHA256::get();

	String s = h->compute(String::num(time(NULL)));

	return s.substr(0, 10);
}

CSRFTokenMiddleware::CSRFTokenMiddleware() {
}
CSRFTokenMiddleware::~CSRFTokenMiddleware() {
}
