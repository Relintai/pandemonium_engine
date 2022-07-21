/*************************************************************************/
/*  export.cpp                                                           */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           PANDEMONIUM ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "http_server_simple.h"

#include "../http/web_server_cookie.h"
#include "http_parser.h"
#include "simple_web_server_request.h"
#include "web_server_simple.h"

void HTTPServerSimple::stop() {
	server->stop();
	_clear_client();
}

Error HTTPServerSimple::listen(int p_port, IP_Address p_address, bool p_use_ssl, String p_ssl_key, String p_ssl_cert) {
	use_ssl = p_use_ssl;
	if (use_ssl) {
		Ref<Crypto> crypto = Crypto::create();
		if (crypto.is_null()) {
			return ERR_UNAVAILABLE;
		}
		if (!p_ssl_key.empty() && !p_ssl_cert.empty()) {
			key = Ref<CryptoKey>(CryptoKey::create());
			Error err = key->load(p_ssl_key);
			ERR_FAIL_COND_V(err != OK, err);
			cert = Ref<X509Certificate>(X509Certificate::create());
			err = cert->load(p_ssl_cert);
			ERR_FAIL_COND_V(err != OK, err);
		} else {
			_set_internal_certs(crypto);
		}
	}
	return server->listen(p_port, p_address);
}

bool HTTPServerSimple::is_listening() const {
	return server->is_listening();
}

void HTTPServerSimple::_send_response() {
	Vector<String> psa = String((char *)req_buf).split("\r\n");
	int len = psa.size();
	ERR_FAIL_COND_MSG(len < 4, "Not enough response headers, got: " + itos(len) + ", expected >= 4.");

	Vector<String> req = psa[0].split(" ", false);
	ERR_FAIL_COND_MSG(req.size() < 2, "Invalid protocol or status code.");

	// Wrong protocol
	ERR_FAIL_COND_MSG(req[0] != "GET" || req[2] != "HTTP/1.1", "Invalid method or HTTP version.");

	const int query_index = req[1].find_char('?');
	const String path = (query_index == -1) ? req[1] : req[1].substr(0, query_index);

	const String req_file = path.get_file();
	const String req_ext = path.get_extension();
	//const String cache_path = EditorSettings::get_singleton()->get_cache_dir().plus_file("web");
	//TODO
	const String cache_path = "./cache/web/";
	const String filepath = cache_path.plus_file(req_file);

	if (!mimes.has(req_ext) || !FileAccess::exists(filepath)) {
		String s = "HTTP/1.1 404 Not Found\r\n";
		s += "Connection: Close\r\n";
		s += "\r\n";
		CharString cs = s.utf8();
		peer->put_data((const uint8_t *)cs.get_data(), cs.size() - 1);
		return;
	}
	const String ctype = mimes[req_ext];

	FileAccess *f = FileAccess::open(filepath, FileAccess::READ);
	ERR_FAIL_COND(!f);
	String s = "HTTP/1.1 200 OK\r\n";
	s += "Connection: Close\r\n";
	s += "Content-Type: " + ctype + "\r\n";
	s += "Access-Control-Allow-Origin: *\r\n";
	s += "Cross-Origin-Opener-Policy: same-origin\r\n";
	s += "Cross-Origin-Embedder-Policy: require-corp\r\n";
	s += "Cache-Control: no-store, max-age=0\r\n";
	s += "\r\n";
	CharString cs = s.utf8();

	Error err = peer->put_data((const uint8_t *)cs.get_data(), cs.size() - 1);
	if (err != OK) {
		memdelete(f);
		ERR_FAIL();
	}

	while (true) {
		uint8_t bytes[4096];
		uint64_t read = f->get_buffer(bytes, 4096);
		if (read == 0) {
			break;
		}
		err = peer->put_data(bytes, read);
		if (err != OK) {
			memdelete(f);
			ERR_FAIL();
		}
	}

	memdelete(f);
}

void HTTPServerSimple::poll() {
	if (!server->is_listening()) {
		return;
	}
	if (tcp.is_null()) {
		if (!server->is_connection_available()) {
			return;
		}
		tcp = server->take_connection();
		peer = tcp;
		time = OS::get_singleton()->get_ticks_usec();
	}
	if (OS::get_singleton()->get_ticks_usec() - time > 1000000) {
		_clear_client();
		return;
	}
	if (tcp->get_status() != StreamPeerTCP::STATUS_CONNECTED) {
		return;
	}

	if (use_ssl) {
		if (ssl.is_null()) {
			ssl = Ref<StreamPeerSSL>(StreamPeerSSL::create());
			peer = ssl;
			ssl->set_blocking_handshake_enabled(false);
			if (ssl->accept_stream(tcp, key, cert) != OK) {
				_clear_client();
				return;
			}
		}
		ssl->poll();
		if (ssl->get_status() == StreamPeerSSL::STATUS_HANDSHAKING) {
			// Still handshaking, keep waiting.
			return;
		}
		if (ssl->get_status() != StreamPeerSSL::STATUS_CONNECTED) {
			_clear_client();
			return;
		}
	}

	//char *r = (char *)req_buf;
	//int l = req_pos - 1;
	//if (l > 3 && r[l] == '\n' && r[l - 1] == '\r' && r[l - 2] == '\n' && r[l - 3] == '\r') {
	//		_send_response();
	//		_clear_client();
	//		return;
	//	}

	int read = 0;
	Error err = peer->get_partial_data(req_buf, 4096, read);

	if (err != OK) {
		// Got an error
		_clear_client();
		return;
	}

	if (read > 0) {
		int buffer_start_index = 0;
		while (true) {
			char *rb = reinterpret_cast<char *>(&req_buf[buffer_start_index]);
			buffer_start_index += _http_parser->read_from_buffer(rb, read);

			if (buffer_start_index >= read) {
				break;
			}
		}
	}

	if (_http_parser->get_request_count() > 0) {
		Ref<SimpleWebServerRequest> request = _http_parser->get_next_request();

		request->_server = this;
		request->setup_url_stack();

		_web_server->server_handle_request(request);

		if (_http_parser->get_request_count() == 0 && _http_parser->is_finished()) {
			_clear_client();
		}
	}
}

void HTTPServerSimple::send_redirect(Ref<WebServerRequest> request, const String &location, const HTTPServerEnums::HTTPStatusCode status_code) {
	//String s = "HTTP/1.1 " + itos(static_cast<int>(status_code)) + " Found\r\n";
	String s = "HTTP/1.1 " + HTTPServerEnums::get_status_code_header_string(status_code) + "\r\n";
	s += "Location: " + location + "\r\n";
	s += "Connection: Close\r\n";

	for (int i = 0; i < request->response_get_cookie_count(); ++i) {
		Ref<WebServerCookie> cookie = request->response_get_cookie(i);

		ERR_CONTINUE(!cookie.is_valid());

		String cookie_str = cookie->get_response_header_string();

		if (cookie_str != "") {
			s += cookie_str;
		}
	}

	s += "\r\n";
	CharString cs = s.utf8();
	peer->put_data((const uint8_t *)cs.get_data(), cs.size() - 1);
}
void HTTPServerSimple::send(Ref<WebServerRequest> request) {
	String body = request->get_compiled_body();

	String s = "HTTP/1.1 " + HTTPServerEnums::get_status_code_header_string(request->get_status_code()) + "\r\n";
	s += "Content-Length: " + itos(body.size()) + "\r\n";
	s += "Content-type: text/html\r\n";
	s += "Connection: Close\r\n";

	for (int i = 0; i < request->response_get_cookie_count(); ++i) {
		Ref<WebServerCookie> cookie = request->response_get_cookie(i);

		ERR_CONTINUE(!cookie.is_valid());

		String cookie_str = cookie->get_response_header_string();

		if (cookie_str != "") {
			s += cookie_str;
		}
	}

	s += "\r\n";
	s += body;

	CharString cs = s.utf8();
	peer->put_data((const uint8_t *)cs.get_data(), cs.size() - 1);
}
void HTTPServerSimple::send_file(Ref<WebServerRequest> request, const String &p_file_path) {
	if (!FileAccess::exists(p_file_path)) {
		String s = "HTTP/1.1 404 Not Found\r\n";
		s += "Connection: Close\r\n";

		for (int i = 0; i < request->response_get_cookie_count(); ++i) {
			Ref<WebServerCookie> cookie = request->response_get_cookie(i);

			ERR_CONTINUE(!cookie.is_valid());

			String cookie_str = cookie->get_response_header_string();

			if (cookie_str != "") {
				s += cookie_str;
			}
		}

		s += "\r\n";
		CharString cs = s.utf8();
		peer->put_data((const uint8_t *)cs.get_data(), cs.size() - 1);
		return;
	}

	String ctype;
	String req_ext = p_file_path.get_extension();

	if (!mimes.has(req_ext)) {
		ctype = mimes[req_ext];
	} else {
		ctype = "text/plain";
	}

	FileAccess *f = FileAccess::open(p_file_path, FileAccess::READ);
	ERR_FAIL_COND(!f);
	String s = "HTTP/1.1 200 OK\r\n";
	s += "Connection: Close\r\n";
	s += "Content-Type: " + ctype + "\r\n";

	for (int i = 0; i < request->response_get_cookie_count(); ++i) {
		Ref<WebServerCookie> cookie = request->response_get_cookie(i);

		ERR_CONTINUE(!cookie.is_valid());

		String cookie_str = cookie->get_response_header_string();

		if (cookie_str != "") {
			s += cookie_str;
		}
	}

	s += "Access-Control-Allow-Origin: *\r\n";
	s += "Cross-Origin-Opener-Policy: same-origin\r\n";
	s += "Cross-Origin-Embedder-Policy: require-corp\r\n";
	s += "Cache-Control: no-store, max-age=0\r\n";
	s += "\r\n";
	CharString cs = s.utf8();

	Error err = peer->put_data((const uint8_t *)cs.get_data(), cs.size() - 1);
	if (err != OK) {
		memdelete(f);
		ERR_FAIL();
	}

	while (true) {
		uint8_t bytes[4096];
		uint64_t read = f->get_buffer(bytes, 4096);
		if (read == 0) {
			break;
		}
		err = peer->put_data(bytes, read);
		if (err != OK) {
			memdelete(f);
			ERR_FAIL();
		}
	}

	memdelete(f);
}

HTTPServerSimple::HTTPServerSimple() {
	_web_server = nullptr;

	mimes["html"] = "text/html";
	mimes["js"] = "application/javascript";
	mimes["json"] = "application/json";
	mimes["pck"] = "application/octet-stream";
	mimes["png"] = "image/png";
	mimes["svg"] = "image/svg";
	mimes["wasm"] = "application/wasm";

	server.instance();
	_http_parser.instance();
	stop();
}

HTTPServerSimple::~HTTPServerSimple() {
}

void HTTPServerSimple::_clear_client() {
	peer = Ref<StreamPeer>();
	ssl = Ref<StreamPeerSSL>();
	tcp = Ref<StreamPeerTCP>();
	memset(req_buf, 0, sizeof(req_buf));
	time = 0;
}

void HTTPServerSimple::_set_internal_certs(Ref<Crypto> p_crypto) {
	//const String cache_path = EditorSettings::get_singleton()->get_cache_dir();
	//TODO
	const String cache_path = "./cache/web/";
	const String key_path = cache_path.plus_file("html5_server.key");
	const String crt_path = cache_path.plus_file("html5_server.crt");
	bool regen = !FileAccess::exists(key_path) || !FileAccess::exists(crt_path);
	if (!regen) {
		key = Ref<CryptoKey>(CryptoKey::create());
		cert = Ref<X509Certificate>(X509Certificate::create());
		if (key->load(key_path) != OK || cert->load(crt_path) != OK) {
			regen = true;
		}
	}
	if (regen) {
		key = p_crypto->generate_rsa(2048);
		key->save(key_path);
		cert = p_crypto->generate_self_signed_certificate(key, "CN=pandemonium-debug.local,O=A Game Dev,C=XXA", "20140101000000", "20340101000000");
		cert->save(crt_path);
	}
}
