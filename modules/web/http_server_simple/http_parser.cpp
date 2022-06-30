#include "http_parser.h"

#include "../http/web_server_request.h"

Ref<WebServerRequest> HTTPParser::get_request() {
	return _request;
}
void HTTPParser::set_request(const Ref<WebServerRequest> &val) {
	_request = val;
}

bool HTTPParser::is_ready() const {
	return _is_ready;
}

void HTTPParser::reset() {
	_partial_data = "";
	_is_ready = false;
}

//returns the index where processing was ended -> start of the next query if != data_length
int HTTPParser::read_from_buffer(const char *p_buffer, const int p_data_length) {
	ERR_FAIL_COND_V(!_request.is_valid(), p_data_length);

	int current_buffer_index = 0;

	_partial_data.resize(p_data_length - current_buffer_index);

	CharType *pdp = _partial_data.ptrw();
	for (int i = 0; i < current_buffer_index; ++i) {
		pdp[i] = p_buffer[i + current_buffer_index];
	}

	return current_buffer_index;
}

HTTPParser::HTTPParser() {
	_is_ready = false;
}

HTTPParser::~HTTPParser() {
}

void HTTPParser::_bind_methods() {
}
