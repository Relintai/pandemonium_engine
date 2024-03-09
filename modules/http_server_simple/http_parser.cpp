/*************************************************************************/
/*  http_parser.cpp                                                      */
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

#include "http_parser.h"

#include "./http_parser/http_parser.h"
#include "./multipart_parser_c/multipart_parser.h"
#include "core/log/logger.h"
#include "core/os/dir_access.h"
#include "core/os/os.h"

#include "modules/web/http/web_server_request.h"

#include "simple_web_server_request.h"

#define PROTOCOL_ERROR_LOGGING_ENABLED 0

Ref<SimpleWebServerRequest> HTTPParser::get_next_request() {
	ERR_FAIL_COND_V(_requests.size() == 0, Ref<SimpleWebServerRequest>());

	Ref<SimpleWebServerRequest> rn = _requests[0];

	_requests.remove(0);

	return rn;
}

int HTTPParser::get_request_count() const {
	return _requests.size();
}

bool HTTPParser::is_ready() const {
	return _is_ready;
}

bool HTTPParser::is_finished() const {
	return !_request.is_valid();
}

bool HTTPParser::has_error() const {
	return _error;
}

void HTTPParser::reset() {
	_partial_data = "";
	_is_ready = false;
	_content_type = REQUEST_CONTENT_URLENCODED;
	_error = false;
	_request.unref();
	_requests.clear();
}

//returns the index where processing was ended -> start of the next query if != data_length
int HTTPParser::read_from_buffer(const char *p_buffer, const int p_data_length) {
	int parsed_bytes = 0;

	parsed_bytes = static_cast<int>(http_parser_execute(parser, settings, p_buffer, p_data_length));

	_current_request_size += parsed_bytes;

	if (_current_request_size >= max_request_size) {
		_error = true;
#if PROTOCOL_ERROR_LOGGING_ENABLED
		PLOG_ERR("_current_request_size >= max_request_size");
#endif
	}

	return parsed_bytes;
}

HTTPParser::HTTPParser() {
	// Should always get set from the outside, if it remains 0 it's a bug.
	max_request_size = 0;
	request_max_file_upload_size = 0;
	upload_file_store_type = WebServerSimple::FILE_UPLOAD_STORE_TYPE_MEMORY;

	_upload_file_access = NULL;

	_current_request_size = 0;
	_current_upload_files_size = 0;

	_is_ready = false;
	_content_type = REQUEST_CONTENT_URLENCODED;
	_multipart_form_is_file = false;

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

	_multipart_parser_settings = memnew(multipart_parser_settings);

	_multipart_parser_settings->on_header_field = _on_multipart_header_field_cb;
	_multipart_parser_settings->on_header_value = _on_multipart_header_value_cb;
	_multipart_parser_settings->on_part_data = _on_multipart_part_data_cb;
	_multipart_parser_settings->on_part_data_begin = _on_multipart_part_data_begin_cb;
	_multipart_parser_settings->on_headers_complete = _on_multipart_headers_complete_cb;
	_multipart_parser_settings->on_part_data_end = _on_multipart_part_data_end_cb;
	_multipart_parser_settings->on_body_end = _on_multipart_body_end_cb;

	_multipart_parser = NULL;

	_error = false;
}

HTTPParser::~HTTPParser() {
	memdelete(parser);
	memdelete(settings);
	parser = nullptr;

	if (_upload_file_access) {
		String path = _upload_file_access->get_path();
		_upload_file_access->close();
		memdelete(_upload_file_access);
		_upload_file_access = NULL;

		DirAccess *d = DirAccess::create_for_path(path.get_base_dir());

		if (d) {
			d->remove(path);
			memdelete(d);
		}
	}
}

void HTTPParser::_bind_methods() {
}

int HTTPParser::HTTPParser::process_multipart_data(const char *at, size_t p_length) {
	ERR_FAIL_COND_V(!_multipart_parser, p_length);

	return multipart_parser_execute(_multipart_parser, at, p_length);
}

void HTTPParser::_process_multipart_header_value(const String &val) {
	if (_queued_multipart_header_field == "content-disposition") {
		int c = val.get_slice_count(";");

		for (int j = 0; j < c; ++j) {
			String vs = val.get_slicec(';', j).strip_edges();

			if (vs.get_slice_count("=") != 2) {
				continue;
			}

			String kk = vs.get_slicec('=', 0);

			if (kk == "name") {
				_multipart_form_name = vs.get_slicec('=', 1);

				if (_multipart_form_name.length() >= 2 && _multipart_form_name.begins_with("\"") && _multipart_form_name.ends_with("\"")) {
					_multipart_form_name.remove(0);
					//TODO check if this should be _multipart_form_name.remove(_multipart_form_name.length() - 1);
					_multipart_form_name.remove(_multipart_form_name.size() - 1);
				}
			} else if (kk == "filename") {
				_multipart_form_filename = vs.get_slicec('=', 1);
				_multipart_form_filename = _multipart_form_filename.replace("\"", "");
				_multipart_form_is_file = true;

				if (_multipart_form_name.length() >= 2 && _multipart_form_name.begins_with("\"") && _multipart_form_name.ends_with("\"")) {
					_multipart_form_name.remove(0);
					//TODO check if this should be _multipart_form_name.remove(_multipart_form_name.length() - 1);
					_multipart_form_name.remove(_multipart_form_name.size() - 1);
				}

				if (upload_file_store_type == WebServerSimple::FILE_UPLOAD_STORE_TYPE_TEMP_FILES) {
					if (_upload_file_access) {
						ERR_PRINT("BUG! if (_upload_file_access) is true!");
						_upload_file_access->close();
						memdelete(_upload_file_access);
						_upload_file_access = NULL;
					}

					DirAccess *da = DirAccess::create_for_path(upload_temp_file_store_path);

					if (!da) {
						// NO fallback!
						ERR_PRINT("upload_file_store_type == FILE_UPLOAD_STORE_TYPE_TEMP_FILES, but temp file path cannot be opened! Sending Error!");
						_error = true;
						return;
					}

					// Just use OS::Time for now. These names are internal, and if the file is not copied out it will get deleted automatically.
					// If filename exists just add values
					String fbase_name = upload_temp_file_store_path + itos(OS::get_singleton()->get_unix_time()) + "_" + itos(OS::get_singleton()->get_ticks_usec());

					String fname = fbase_name;
					int fcounter = 0;

					while (da->file_exists(fname) && fcounter < 100) {
						fname = fbase_name + "_" + itos(fcounter);
						++fcounter;
					}

					memdelete(da);

					Error err;
					_upload_file_access = FileAccess::open(fname, FileAccess::WRITE, &err);

					if (err != OK) {
						ERR_PRINT(vformat("upload_file_store_type == FILE_UPLOAD_STORE_TYPE_TEMP_FILES, but temp file cannot be opened! Sending Error! Error: %d, FileName: %s", itos(err), fname));
						_error = true;

						if (_upload_file_access) {
							memdelete(_upload_file_access);
						}
					}

					_upload_file_full_path = fname;
				}
			}
		}

	} else if (_queued_multipart_header_field == "content-type") {
		_multipart_form_content_type = val;
	} else {
		//Shouldn't happen, should probably close connection
	}

	_queued_multipart_header_field = "";
}

void HTTPParser::process_urlenc_data() {
	if (_partial_data == "") {
		return;
	}

	Vector<String> params = _partial_data.split("&", false);

	for (int i = 0; i < params.size(); ++i) {
		String p = params[i];

		//Ignore if it has no, or more than one =
		const CharType *c = p.ptr();
		int eqc = 0;
		for (int j = 0; j < p.length(); ++j) {
			if (c[j] == '=') {
				++eqc;

				if (eqc > 1) {
					break;
				}
			}
		}

		if (eqc > 1 || eqc == 0) {
			continue;
		}

		String key = p.get_slicec('=', 0).replace("+", " ").percent_decode();
		String value = p.get_slicec('=', 1).replace("+", " ").percent_decode();

		_request->add_post_parameter(key, value);
	}
}

bool HTTPParser::is_boundary_at(const char *at, size_t length) {
	return false;
}

#define MESSAGE_DEBUG 0

int HTTPParser::on_message_begin() {
	if (_request.is_valid()) {
		ERR_PRINT("Request was valid!");
	}

	_current_request_size = 0;

	_in_header = true;
	_content_type = REQUEST_CONTENT_URLENCODED;
	_multipart_form_is_file = false;

	_request.instance();

	switch (parser->method) {
		case http_method::HTTP_DELETE:
			_request->set_method(HTTPServerEnums::HTTP_METHOD_DELETE);
			break;
		case http_method::HTTP_GET:
			_request->set_method(HTTPServerEnums::HTTP_METHOD_GET);
			break;
		case http_method::HTTP_HEAD:
			_request->set_method(HTTPServerEnums::HTTP_METHOD_HEAD);
			break;
		case http_method::HTTP_POST:
			_request->set_method(HTTPServerEnums::HTTP_METHOD_POST);
			break;
		case http_method::HTTP_PUT:
			_request->set_method(HTTPServerEnums::HTTP_METHOD_PUT);
			break;
		case http_method::HTTP_OPTIONS:
			_request->set_method(HTTPServerEnums::HTTP_METHOD_OPTIONS);
			break;
		case http_method::HTTP_PATCH:
			_request->set_method(HTTPServerEnums::HTTP_METHOD_PATCH);
			break;
		default:
			_request->set_method(HTTPServerEnums::HTTP_METHOD_INVALID);
			_error = true;
#if PROTOCOL_ERROR_LOGGING_ENABLED
			PLOG_ERR("Unimplemented / invalid method!");
#endif
			break;
	}

#if MESSAGE_DEBUG
	ERR_PRINT("begin");
#endif

	return 0;
}
int HTTPParser::on_url(const char *at, size_t length) {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

	String s = String::utf8(at, length).uri_decode().strip_edges();

#if MESSAGE_DEBUG
	ERR_PRINT("url " + s);
#endif

	_request->set_parser_path(s);

	return 0;
}
int HTTPParser::on_status(const char *at, size_t length) {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

	String s = String::utf8(at, length);

#if MESSAGE_DEBUG
	ERR_PRINT("status " + s);
#endif

	return 0;
}
int HTTPParser::on_header_field(const char *at, size_t length) {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

	String s = String::utf8(at, length);

#if MESSAGE_DEBUG
	ERR_PRINT("header_field " + s);
#endif

	_queued_header_field = s.to_lower();

	return 0;
}
int HTTPParser::on_header_value(const char *at, size_t length) {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

	String s = String::utf8(at, length);

#if MESSAGE_DEBUG
	ERR_PRINT("header_val " + s);
#endif

	_request->add_header_parameter(_queued_header_field, s);

	if (_queued_header_field == "host") {
		_request->set_host(s);
	} else if (_queued_header_field == "content-type") {
		// It can be:
		// application/x-www-form-urlencoded (default) -> ignore, as its the default
		// text/plain -> useful only for debugging "They are not reliably interpretable by computer"
		// multipart/form-data

		if (s.begins_with("multipart/form-data")) {
			_content_type = REQUEST_CONTENT_MULTIPART_FORM_DATA;

			int bs = s.find("boundary=");

			if (bs == -1) {
				//Error! boundary must exist
				_error = true;
#if PROTOCOL_ERROR_LOGGING_ENABLED
				PLOG_ERR("Boundary must exist!");
#endif
				return 0;
			}

			bs += 9; //skip ahead to the end of "boundary="

			_multipart_boundary = "--" + s.substr(bs).strip_edges();
			//_multipart_boundary = _multipart_boundary.strip_edges();

			//TODO can be inside quoted
			//Append -- if it doesn't have it already
			//It shouldn't be longer that 70 chars
			//The CRLF preceeding could also be appended for simpler logic

			if (_multipart_boundary.empty()) {
#if PROTOCOL_ERROR_LOGGING_ENABLED
				PLOG_ERR("Empty boundary!");
#endif
				_error = true;
			}

		} else if (s.begins_with("text/plain")) {
			_content_type = REQUEST_CONTENT_TEXT_PLAIN;
			//maybe just close the connection?
		}
	} else if (_queued_header_field == "cookie") {
		Vector<String> cookies = s.split(";");

		for (int i = 0; i < cookies.size(); ++i) {
			String c = cookies[i].strip_edges();

			if (c.get_slice_count("=") != 2) {
				continue;
			}

			String key = c.get_slice("=", 0);
			String val = c.get_slice("=", 1);

			_request->add_cookie_data(key, val);
		}
	}

	return 0;
}
int HTTPParser::on_headers_complete() {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

#if MESSAGE_DEBUG
	ERR_PRINT("headers_complete");
#endif

	//Check content length, and send error if bigger than server limit (add)

	if (_content_type == REQUEST_CONTENT_MULTIPART_FORM_DATA) {
		if (_multipart_parser) {
			multipart_parser_free(_multipart_parser);
			_multipart_parser = NULL;
		}

		_multipart_parser = multipart_parser_init(_multipart_boundary.ascii().get_data(), _multipart_parser_settings);
		multipart_parser_set_data(_multipart_parser, this);
	}

	_in_header = false;
	_partial_data = "";

	return 0;
}
int HTTPParser::on_body(const char *at, size_t p_length) {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);
	int length = static_cast<int>(p_length);

	if (_content_type == REQUEST_CONTENT_MULTIPART_FORM_DATA) {
		int wofs = _queued_multipart_form_data.size();
		_queued_multipart_form_data.resize(_queued_multipart_form_data.size() + length);
		char *w = _queued_multipart_form_data.ptrw();
		for (int i = 0; i < length; ++i) {
			w[wofs++] = at[i];
		}

		int processed = process_multipart_data(_queued_multipart_form_data.ptr(), _queued_multipart_form_data.size());
		int size = _queued_multipart_form_data.size();
		wofs = 0;
		for (int i = processed; i < size; ++i) {
			w[wofs++] = w[i];
		}

		_queued_multipart_form_data.resize(_queued_multipart_form_data.size() - processed);

		return 0;
	}

	String s = String::utf8(at, length);

#if MESSAGE_DEBUG
	ERR_PRINT("on_body " + s);
#endif

	_partial_data += s;

	return 0;
}

int HTTPParser::on_message_complete() {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

#if MESSAGE_DEBUG
	ERR_PRINT("msg_copmlete");
#endif

	//if (_content_type == REQUEST_CONTENT_MULTIPART_FORM_DATA) {
	//	process_multipart_data();
	//} else

	if (_content_type == REQUEST_CONTENT_URLENCODED) {
		process_urlenc_data();
	}

	if (!_error) {
		_requests.push_back(_request);
	}

	_request.unref();

	if (_multipart_parser) {
		multipart_parser_free(_multipart_parser);
		_multipart_parser = NULL;
	}

	_multipart_boundary = "";

	_queued_multipart_header_field = "";

	_multipart_form_name = "";
	_multipart_form_filename = "";
	_multipart_form_content_type = "";

	return 0;
}
int HTTPParser::on_chunk_header() {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

#if MESSAGE_DEBUG
	ERR_PRINT("chunk_header");
#endif

	return 0;
}
int HTTPParser::on_chunk_complete() {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

#if MESSAGE_DEBUG
	ERR_PRINT("chunk_complete");
#endif

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

#define MULTIPART_MESSAGE_DEBUG 0

int HTTPParser::on_multipart_header_field_cb(const char *at, size_t length) {
	String s = String::utf8(at, length);

	_queued_multipart_header_field = s.to_lower();

#if MULTIPART_MESSAGE_DEBUG
	ERR_PRINT("on_multipart_header_field_cb " + _queued_multipart_header_field);
#endif

	return 0;
}
int HTTPParser::on_multipart_header_value_cb(const char *at, size_t length) {
	String s = String::utf8(at, length);

	_process_multipart_header_value(s);

#if MULTIPART_MESSAGE_DEBUG
	ERR_PRINT("on_multipart_header_value_cb " + s);
#endif

	return 0;
}
int HTTPParser::on_multipart_part_data_cb(const char *at, size_t length) {
#if MULTIPART_MESSAGE_DEBUG
	ERR_PRINT("on_multipart_part_data_cb");
#endif

	if (_upload_file_access) {
		_upload_file_access->store_buffer((const uint8_t *)at, (uint64_t)length);

		_current_upload_files_size += length;

		if (_current_upload_files_size >= request_max_file_upload_size) {
			_error = true;

#if PROTOCOL_ERROR_LOGGING_ENABLED
			PLOG_ERR("_current_upload_files_size >= request_max_file_upload_size");
#endif

			_upload_file_access->close();
			memdelete(_upload_file_access);
			_upload_file_access = NULL;

			DirAccess *d = DirAccess::create_for_path(_upload_file_full_path.get_base_dir());

			if (d) {
				d->remove(_upload_file_full_path);
				memdelete(d);
			}
		}

	} else {
		int l = static_cast<int>(length);
		int mfdofs = _multipart_form_data.size();
		_multipart_form_data.resize(mfdofs + length);
		char *w = _multipart_form_data.ptrw();
		for (int i = 0; i < l; ++i) {
			w[mfdofs++] = at[i];
		}
	}

	return 0;
}
int HTTPParser::on_multipart_part_data_begin_cb() {
#if MULTIPART_MESSAGE_DEBUG
	ERR_PRINT("on_multipart_part_data_begin_cb");
#endif

	return 0;
}
int HTTPParser::on_multipart_headers_complete_cb() {
#if MULTIPART_MESSAGE_DEBUG
	ERR_PRINT("on_multipart_headers_complete_cb");
#endif

	return 0;
}
int HTTPParser::on_multipart_part_data_end_cb() {
#if MULTIPART_MESSAGE_DEBUG
	ERR_PRINT("on_multipart_part_data_end_cb");
#endif

	if (_multipart_form_is_file) {
		if (_upload_file_access) {
			_upload_file_access->close();
			memdelete(_upload_file_access);
			_upload_file_access = NULL;

			_request->add_file_temp_file(_multipart_form_name, _multipart_form_filename, _upload_file_full_path);
		} else {
			if (_multipart_form_data.size() > 0) {
				PoolByteArray file_data;
				int len = _multipart_form_data.size();
				file_data.resize(len);
				PoolByteArray::Write w = file_data.write();
				const char *r = _multipart_form_data.ptr();
				for (int i = 0; i < len; i++) {
					w[i] = r[i];
				}

				w.release();

				_request->add_file_memory(_multipart_form_name, _multipart_form_filename, file_data);
			}
		}
	} else {
		String s = String::utf8(_multipart_form_data.ptr(), _multipart_form_data.size());
		_request->add_post_parameter(_multipart_form_name, s);
	}

	_multipart_form_is_file = false;
	_multipart_form_data.clear();

	return 0;
}
int HTTPParser::on_multipart_body_end_cb() {
#if MULTIPART_MESSAGE_DEBUG
	ERR_PRINT("on_multipart_body_end_cb");
#endif

	return 0;
}
int HTTPParser::_on_multipart_header_field_cb(multipart_parser *parser, const char *at, size_t length) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(multipart_parser_get_data(parser));
	return p->on_multipart_header_field_cb(at, length);
}
int HTTPParser::_on_multipart_header_value_cb(multipart_parser *parser, const char *at, size_t length) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(multipart_parser_get_data(parser));
	return p->on_multipart_header_value_cb(at, length);
}
int HTTPParser::_on_multipart_part_data_cb(multipart_parser *parser, const char *at, size_t length) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(multipart_parser_get_data(parser));
	return p->on_multipart_part_data_cb(at, length);
}
int HTTPParser::_on_multipart_part_data_begin_cb(multipart_parser *parser) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(multipart_parser_get_data(parser));
	return p->on_multipart_part_data_begin_cb();
}
int HTTPParser::_on_multipart_headers_complete_cb(multipart_parser *parser) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(multipart_parser_get_data(parser));
	return p->on_multipart_headers_complete_cb();
}
int HTTPParser::_on_multipart_part_data_end_cb(multipart_parser *parser) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(multipart_parser_get_data(parser));
	return p->on_multipart_part_data_end_cb();
}
int HTTPParser::_on_multipart_body_end_cb(multipart_parser *parser) {
	HTTPParser *p = reinterpret_cast<HTTPParser *>(multipart_parser_get_data(parser));
	return p->on_multipart_body_end_cb();
}
