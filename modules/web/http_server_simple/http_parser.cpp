#include "http_parser.h"

#include "../http/web_server_request.h"

#include "./http_parser/http_parser.h"

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

	int parsed_bytes = 0;

	parsed_bytes = static_cast<int>(http_parser_execute(parser, settings, p_buffer, p_data_length));

	return parsed_bytes;
}

HTTPParser::HTTPParser() {
	_is_ready = false;

	settings = memnew(http_parser_settings);

	settings->on_message_begin = _on_message_begin_cb;
	settings->on_url = _on_url_cb;
	settings->on_status = _on_status_cb;
	settings->on_header_field = _on_header_field_cb;
	settings->on_header_value = _on_header_value_cb;
	settings->on_headers_complete = _on_headers_complete_cb;
	settings->on_body = _on_body_cb;
	settings->on_message_complete = _on_message_complete_cb;
	settings->on_chunk_header = _on_chunk_header_cb;
	settings->on_chunk_complete = _on_chunk_complete_cb;

	//parser = malloc(sizeof(http_parser));
	parser = memnew(http_parser);
	http_parser_init(parser, HTTP_REQUEST);

	parser->data = this;
}

HTTPParser::~HTTPParser() {
	memdelete(parser);
	memdelete(settings);
	parser = nullptr;
}

void HTTPParser::_bind_methods() {
}

String HTTPParser::chr_len_to_str(const char *at, size_t length) {
	String ret;
	ret.resize(length + 1);

	CharType *p = ret.ptrw();

	for (size_t i = 0; i <= length; ++i) {
		p[i] = at[i];
	}

	return ret;
}

int HTTPParser::on_message_begin() {
	print_error("begin");
	return 0;
}
int HTTPParser::on_url(const char *at, size_t length) {
	String s = chr_len_to_str(at, length);

	print_error("url " + s);
	return 0;
}
int HTTPParser::on_status(const char *at, size_t length) {
	String s = chr_len_to_str(at, length);
	print_error("status " + s);
	return 0;
}
int HTTPParser::on_header_field(const char *at, size_t length) {
	String s = chr_len_to_str(at, length);
	print_error("header_field " + s);
	return 0;
}
int HTTPParser::on_header_value(const char *at, size_t length) {
	String s = chr_len_to_str(at, length);
	print_error("header_val " + s);
	return 0;
}
int HTTPParser::on_headers_complete() {
	print_error("headers_complete");
	return 0;
}
int HTTPParser::on_body(const char *at, size_t length) {
	String s = chr_len_to_str(at, length);
	print_error("on_body " + s);
	return 0;
}
int HTTPParser::on_message_complete() {
	print_error("msg_copmlete");
	return 0;
}
int HTTPParser::on_chunk_header() {
	print_error("chunk_header");
	return 0;
}
int HTTPParser::on_chunk_complete() {
	print_error("chunk_complete");
	return 0;
}

int HTTPParser::_on_message_begin_cb(http_parser *parser) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(parser->data);
	return p->on_message_begin();
}
int HTTPParser::_on_url_cb(http_parser *parser, const char *at, size_t length) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(parser->data);
	return p->on_url(at, length);
}
int HTTPParser::_on_status_cb(http_parser *parser, const char *at, size_t length) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(parser->data);
	return p->on_status(at, length);
}
int HTTPParser::_on_header_field_cb(http_parser *parser, const char *at, size_t length) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(parser->data);
	return p->on_header_field(at, length);
}
int HTTPParser::_on_header_value_cb(http_parser *parser, const char *at, size_t length) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(parser->data);
	return p->on_header_value(at, length);
}
int HTTPParser::_on_headers_complete_cb(http_parser *parser) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(parser->data);
	return p->on_headers_complete();
}
int HTTPParser::_on_body_cb(http_parser *parser, const char *at, size_t length) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(parser->data);
	return p->on_body(at, length);
}
int HTTPParser::_on_message_complete_cb(http_parser *parser) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(parser->data);
	return p->on_message_complete();
}
int HTTPParser::_on_chunk_header_cb(http_parser *parser) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(parser->data);
	return p->on_chunk_header();
}
int HTTPParser::_on_chunk_complete_cb(http_parser *parser) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(parser->data);
	return p->on_chunk_complete();
}
