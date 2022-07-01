#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "core/ustring.h"

#include "core/reference.h"

class SimpleWebServerRequest;
struct http_parser;
struct http_parser_settings;

class HTTPParser : public Reference {
	GDCLASS(HTTPParser, Reference);

public:
	Ref<SimpleWebServerRequest> get_next_request();
	int get_request_count() const;

	bool is_ready() const;
	bool is_finished() const;

	void reset();

	//returns the index where processing was ended -> start of the next query if != data_length
	int read_from_buffer(const char *p_buffer, const int p_data_length);

	HTTPParser();
	~HTTPParser();

protected:
	static void _bind_methods();

	String _partial_data;

	Ref<SimpleWebServerRequest> _request;
	Vector<Ref<SimpleWebServerRequest>> _requests;

	bool _is_ready;

private:
	String chr_len_to_str(const char *at, size_t length);

	int on_message_begin();
	int on_url(const char *at, size_t length);
	int on_status(const char *at, size_t length);
	int on_header_field(const char *at, size_t length);
	int on_header_value(const char *at, size_t length);
	int on_headers_complete();
	int on_body(const char *at, size_t length);
	int on_message_complete();
	int on_chunk_header();
	int on_chunk_complete();

	static int _on_message_begin_cb(http_parser *parser);
	static int _on_url_cb(http_parser *parser, const char *at, size_t length);
	static int _on_status_cb(http_parser *parser, const char *at, size_t length);
	static int _on_header_field_cb(http_parser *parser, const char *at, size_t length);
	static int _on_header_value_cb(http_parser *parser, const char *at, size_t length);
	static int _on_headers_complete_cb(http_parser *parser);
	static int _on_body_cb(http_parser *parser, const char *at, size_t length);
	static int _on_message_complete_cb(http_parser *parser);
	static int _on_chunk_header_cb(http_parser *parser);
	static int _on_chunk_complete_cb(http_parser *parser);

	http_parser *parser;
	http_parser_settings *settings;

	bool _in_header;
	String _queued_header_field;
};

#endif
