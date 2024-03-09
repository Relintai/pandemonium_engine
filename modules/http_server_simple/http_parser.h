#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

/*************************************************************************/
/*  http_parser.h                                                        */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/containers/vector.h"
#include "core/string/ustring.h"
#include "core/os/file_access.h"

#include "core/object/reference.h"

#include "web_server_simple.h"

class SimpleWebServerRequest;
struct http_parser;
struct http_parser_settings;
struct multipart_parser;
struct multipart_parser_settings;

class HTTPParser : public Reference {
	GDCLASS(HTTPParser, Reference);

public:
	enum HTTPRequestContentType {
		REQUEST_CONTENT_URLENCODED = 0,
		REQUEST_CONTENT_MULTIPART_FORM_DATA,
		REQUEST_CONTENT_TEXT_PLAIN,
	};

	uint64_t max_request_size;
	uint64_t request_max_file_upload_size;

	WebServerSimple::FileUploadStoreType upload_file_store_type;
	String upload_temp_file_store_path;

	Ref<SimpleWebServerRequest> get_next_request();
	int get_request_count() const;

	bool is_ready() const;
	bool is_finished() const;
	bool has_error() const;

	void reset();

	//returns the index where processing was ended -> start of the next query if != data_length
	int read_from_buffer(const char *p_buffer, const int p_data_length);

	HTTPParser();
	~HTTPParser();

protected:
	static void _bind_methods();

	String _partial_data;

	uint64_t _current_request_size;
	uint64_t _current_upload_files_size;
	
	String _upload_file_full_path;
	FileAccess *_upload_file_access;

	Ref<SimpleWebServerRequest> _request;
	Vector<Ref<SimpleWebServerRequest>> _requests;

	bool _is_ready;

	HTTPRequestContentType _content_type;
	bool _in_header;
	String _queued_header_field;

	String _multipart_boundary;

	String _queued_multipart_header_field;
	Vector<char> _queued_multipart_form_data;

	String _multipart_form_name;
	String _multipart_form_filename;
	String _multipart_form_content_type;
	bool _multipart_form_is_file;
	Vector<char> _multipart_form_data;

	bool _error;

private:
	int process_multipart_data(const char *at, size_t length);
	void _process_multipart_header_value(const String &val);
	void process_urlenc_data();
	bool is_boundary_at(const char *at, size_t length);

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

	int on_multipart_header_field_cb(const char *at, size_t length);
	int on_multipart_header_value_cb(const char *at, size_t length);
	int on_multipart_part_data_cb(const char *at, size_t length);
	int on_multipart_part_data_begin_cb();
	int on_multipart_headers_complete_cb();
	int on_multipart_part_data_end_cb();
	int on_multipart_body_end_cb();

	static int _on_multipart_header_field_cb(multipart_parser *parser, const char *at, size_t length);
	static int _on_multipart_header_value_cb(multipart_parser *parser, const char *at, size_t length);
	static int _on_multipart_part_data_cb(multipart_parser *parser, const char *at, size_t length);
	static int _on_multipart_part_data_begin_cb(multipart_parser *parser);
	static int _on_multipart_headers_complete_cb(multipart_parser *parser);
	static int _on_multipart_part_data_end_cb(multipart_parser *parser);
	static int _on_multipart_body_end_cb(multipart_parser *parser);

	http_parser *parser;
	http_parser_settings *settings;

	multipart_parser *_multipart_parser;
	multipart_parser_settings *_multipart_parser_settings;
};

#endif
