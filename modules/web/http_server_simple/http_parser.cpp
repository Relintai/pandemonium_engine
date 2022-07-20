#include "http_parser.h"

#include "../http/web_server_request.h"
#include "./http_parser/http_parser.h"

#include "simple_web_server_request.h"

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

void HTTPParser::reset() {
	_partial_data = "";
	_is_ready = false;
	_content_type = REQUEST_CONTENT_URLENCODED;
}

//returns the index where processing was ended -> start of the next query if != data_length
int HTTPParser::read_from_buffer(const char *p_buffer, const int p_data_length) {
	int parsed_bytes = 0;

	parsed_bytes = static_cast<int>(http_parser_execute(parser, settings, p_buffer, p_data_length));

	return parsed_bytes;
}

HTTPParser::HTTPParser() {
	_is_ready = false;
	_content_type = REQUEST_CONTENT_URLENCODED;
	_in_multipart_boundary = false;
	_in_boundary_header = false;
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

void HTTPParser::HTTPParser::process_multipart_data() {
	int iter = 0;
	//process one element per loop
	while (true) {
		//first boundary -> ignore, with everything before it
		if (!_in_multipart_boundary) {
			int boundary_index = _partial_data.find(_multipart_boundary);

			if (boundary_index == -1) {
				return;
			}

			boundary_index += _multipart_boundary.size();

			_partial_data = _partial_data.substr(boundary_index);
			_in_multipart_boundary = true;
			_in_boundary_header = true;
			continue;
		}

		//find the first \n\n -> process boundary_header
		//cut it out from the string.
		if (_in_boundary_header) {
			int header_end_index = _partial_data.find("\r\n\r\n");

			if (header_end_index != -1) {
				String header = _partial_data.substr_index(0, header_end_index);
				_partial_data = _partial_data.substr(header_end_index + 4);

				header = header.strip_edges();

				//ERR_PRINT("HEADER");
				//ERR_PRINT(header);

				_process_multipart_header(header);
				_in_boundary_header = false;
				continue;
			}

			//Boundary header has not yet fully arrived, return
			return;
		}

		//Multipart body
		int boundary_index = _partial_data.find(_multipart_boundary);

		if (boundary_index == -1) {
			//TODO
			//if file-> append everything to the HTTPTempFile, except the last boundary.size() - 1 characters from the string.
			//should probably only happen after a while to save on memory use like maybe a meg or two (should be configurable)
			// Should probably also be configurable whether it happens or not at all

			return;
		}

		//ERR_PRINT("BODY");
		String data = _partial_data.substr_index(0, boundary_index - 4); //to strip the 2 \r\n from before the boundary

		//ERR_PRINT(data);

		if (_multipart_form_is_file) {
			if (data == "") {
				_in_boundary_header = true;
				continue;
			}

			CharString cs = data.ascii();

			PoolByteArray file_data;
			file_data.resize(cs.length());
			PoolByteArray::Write w = file_data.write();

			for (int i = 0; i < cs.length(); i++) {
				w[i] = cs[i];
			}

			w.release();

			_request->add_file(_multipart_form_name, _multipart_form_filename, file_data);
		} else {
			_request->add_parameter(_multipart_form_name, data);
		}

		boundary_index += _multipart_boundary.size();
		_partial_data = _partial_data.substr(boundary_index);

		if (_partial_data.begins_with("--")) {
			//done
			return;
		}

		_in_boundary_header = true;

		//Safety for now
		++iter;
		ERR_FAIL_COND(iter == 10000);
	}
}

void HTTPParser::_process_multipart_header(const String &header) {
	_multipart_form_name = "";
	_multipart_form_filename = "";
	_multipart_form_content_type = "";
	_multipart_form_is_file = false;

	int nlc = header.get_slice_count("\r\n");

	for (int i = 0; i < nlc; ++i) {
		String l = header.get_slice("\r\n", i);

		int sc = l.get_slice_count(":");

		if (sc != 2) {
			continue;
		}

		String key = l.get_slicec(':', 0);
		String val = l.get_slicec(':', 1);

		if (key == "Content-Disposition") {
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
						_multipart_form_name.remove(_multipart_form_name.size() - 1);
					}
				} else if (kk == "filename") {
					_multipart_form_filename = vs.get_slicec('=', 1);
					_multipart_form_is_file = true;

					if (_multipart_form_name.length() >= 2 && _multipart_form_name.begins_with("\"") && _multipart_form_name.ends_with("\"")) {
						_multipart_form_name.remove(0);
						_multipart_form_name.remove(_multipart_form_name.size() - 1);
					}
				}
			}

		} else if (key == "Content-Type") {
			_multipart_form_content_type = val;
		} else {
			//Shouldn't happen, should probably close connection
		}
	}
}

#define MESSAGE_DEBUG 0

int HTTPParser::on_message_begin() {
	if (_request.is_valid()) {
		ERR_PRINT("Request was valid!");
	}

	_in_header = true;
	_content_type = REQUEST_CONTENT_URLENCODED;
	_multipart_boundary = "";
	_in_multipart_boundary = false;
	_in_multipart_boundary = false;
	_in_boundary_header = false;
	_multipart_form_is_file = false;

	_request.instance();

#if MESSAGE_DEBUG
	ERR_PRINT("begin");
#endif

	return 0;
}
int HTTPParser::on_url(const char *at, size_t length) {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

	String s = chr_len_to_str(at, length);

#if MESSAGE_DEBUG
	ERR_PRINT("url " + s);
#endif

	_request->set_parser_path(s);

	return 0;
}
int HTTPParser::on_status(const char *at, size_t length) {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

	String s = chr_len_to_str(at, length);

#if MESSAGE_DEBUG
	ERR_PRINT("status " + s);
#endif

	return 0;
}
int HTTPParser::on_header_field(const char *at, size_t length) {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

	String s = chr_len_to_str(at, length);

#if MESSAGE_DEBUG
	ERR_PRINT("header_field " + s);
#endif

	_queued_header_field = s;

	return 0;
}
int HTTPParser::on_header_value(const char *at, size_t length) {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

	String s = chr_len_to_str(at, length);

#if MESSAGE_DEBUG
	ERR_PRINT("header_val " + s);
#endif

	_request->add_parameter(_queued_header_field, s);

	if (_queued_header_field == "Host") {
		_request->set_host(s);
	} else if (_queued_header_field == "Content-Type") {
		// It can be:
		// application/x-www-form-urlencoded (default) -> ignore, as its the default
		// text/plain -> useful only for debugging "They are not reliably interpretable by computer"
		// multipart/form-data

		if (s.begins_with("multipart/form-data")) {
			_content_type = REQUEST_CONTENT_MULTIPART_FORM_DATA;

			int bs = s.find("boundary=");

			if (bs == -1) {
				//Error! boundary must exist TODO set an error variable and close the connection
				return 0;
			}

			bs += 9; //skip ahead to the end of "boundary="

			_multipart_boundary = s.substr(bs);
			_multipart_boundary = _multipart_boundary.strip_edges();

			//TODO can be inside quoted
			//Append -- if it doesn't have it already
			//It shouldn't be longer that 70 chars
			//The CRLF preceeding could also be appended for simpler logic

			if (_multipart_boundary == "") {
				//Error!  TODO set an error variable and close the connection
			}

		} else if (s.begins_with("text/plain")) {
			_content_type = REQUEST_CONTENT_TEXT_PLAIN;
			//maybe just close the connection?
		}
	}

	//TODO close connection on chunked connection (for now)

	return 0;
}
int HTTPParser::on_headers_complete() {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

#if MESSAGE_DEBUG
	ERR_PRINT("headers_complete");
#endif

	//Check content length, and send error if bigger than server limit (add)

	_in_header = false;
	_partial_data = "";

	return 0;
}
int HTTPParser::on_body(const char *at, size_t length) {
	ERR_FAIL_COND_V(!_request.is_valid(), 0);

	String s = chr_len_to_str(at, length);

#if MESSAGE_DEBUG
	ERR_PRINT("on_body " + s);
#endif

	_partial_data += s;

	if (_content_type == REQUEST_CONTENT_MULTIPART_FORM_DATA) {
		process_multipart_data();
	}

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
		//Parse the content into the request
		//Also add content body
	}

	_requests.push_back(_request);
	_request.unref();

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
