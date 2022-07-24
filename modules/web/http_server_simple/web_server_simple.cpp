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

#include "web_server_simple.h"

#include "core/os/os.h"
#include "http_server_simple.h"

int WebServerSimple::get_bind_port() {
	return _bind_port;
}
void WebServerSimple::set_bind_port(const int val) {
	_bind_port = val;
}

String WebServerSimple::get_bind_host() {
	return _bind_host;
}
void WebServerSimple::set_bind_host(const String &val) {
	_bind_host = val;
}

bool WebServerSimple::get_use_ssl() {
	return _use_ssl;
}
void WebServerSimple::set_use_ssl(const bool val) {
	_use_ssl = val;
}

String WebServerSimple::get_ssl_key() {
	return _ssl_key;
}
void WebServerSimple::set_ssl_key(const String &val) {
	_ssl_key = val;
}

String WebServerSimple::get_ssl_cert() {
	return _ssl_cert;
}
void WebServerSimple::set_ssl_cert(const String &val) {
	_ssl_cert = val;
}

bool WebServerSimple::get_use_poll_thread() {
	return _use_poll_thread;
}
void WebServerSimple::set_use_poll_thread(const bool val) {
	_use_poll_thread = val;
}

int WebServerSimple::get_poll_thread_count() {
	return _poll_thread_count;
}
void WebServerSimple::set_poll_thread_count(const int val) {
	_poll_thread_count = val;
}

bool WebServerSimple::get_use_worker_threads() {
	return _use_worker_threads;
}
void WebServerSimple::set_use_worker_threads(const bool val) {
	_use_worker_threads = val;
}

int WebServerSimple::get_worker_thread_count() {
	return _worker_thread_count;
}
void WebServerSimple::set_worker_thread_count(const int val) {
	_worker_thread_count = val;
}

void WebServerSimple::_start() {
	WebServer::_start();

	if (!OS::get_singleton()->can_use_threads()) {
		server->_use_worker_threads = false;
	} else {
		server->_use_worker_threads = _use_worker_threads;
	}

	if (_worker_thread_count < 0) {
		server->_thread_count = OS::get_singleton()->get_processor_count();
	} else {
		server->_thread_count = _worker_thread_count;
	}

	const uint16_t bind_port = _bind_port;
	// Resolve host if needed.
	const String bind_host = _bind_host;
	IP_Address bind_ip;

	if (bind_host.is_valid_ip_address()) {
		bind_ip = bind_host;
	} else {
		bind_ip = IP::get_singleton()->resolve_hostname(bind_host);
	}

	ERR_FAIL_COND(!bind_ip.is_valid());

	const bool use_ssl = _use_ssl;
	const String ssl_key = _ssl_key;
	const String ssl_cert = _ssl_cert;

	Error err;

	// Restart server.
	{
		MutexLock lock(server_lock);

		server->stop();
		err = server->listen(bind_port, bind_ip, use_ssl, ssl_key, ssl_cert);
	}

	ERR_FAIL_COND_MSG(err != OK, "Error starting HTTP server:\n" + itos(err));
}

void WebServerSimple::_stop() {
	WebServer::_stop();

	MutexLock lock(server_lock);
	server->stop();
}

WebServerSimple::WebServerSimple() {
	_bind_port = 8080;
	_bind_host = "127.0.0.1";

	_use_ssl = false;

	_use_worker_threads = true;
	_use_poll_thread = true;
	_poll_thread_count = 1;
	_worker_thread_count = 4;

	server_quit = false;

	server.instance();
	server->_web_server = this;
	server_thread.start(_server_thread_poll, this);
}

WebServerSimple::~WebServerSimple() {
	server->stop();
	server_quit = true;
	server_thread.wait_to_finish();
}

void WebServerSimple::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_bind_port"), &WebServerSimple::get_bind_port);
	ClassDB::bind_method(D_METHOD("set_bind_port", "val"), &WebServerSimple::set_bind_port);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "bind_port"), "set_bind_port", "get_bind_port");

	ClassDB::bind_method(D_METHOD("get_bind_host"), &WebServerSimple::get_bind_host);
	ClassDB::bind_method(D_METHOD("set_bind_host", "val"), &WebServerSimple::set_bind_host);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "bind_host"), "set_bind_host", "get_bind_host");

	ClassDB::bind_method(D_METHOD("get_use_ssl"), &WebServerSimple::get_use_ssl);
	ClassDB::bind_method(D_METHOD("set_use_ssl", "val"), &WebServerSimple::set_use_ssl);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_ssl"), "set_use_ssl", "get_use_ssl");

	ClassDB::bind_method(D_METHOD("get_ssl_key"), &WebServerSimple::get_ssl_key);
	ClassDB::bind_method(D_METHOD("set_ssl_key", "val"), &WebServerSimple::set_ssl_key);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "ssl_key"), "set_ssl_key", "get_ssl_key");

	ClassDB::bind_method(D_METHOD("get_ssl_cert"), &WebServerSimple::get_ssl_cert);
	ClassDB::bind_method(D_METHOD("set_ssl_cert", "val"), &WebServerSimple::set_ssl_cert);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "ssl_cert"), "set_ssl_cert", "get_ssl_cert");

	ClassDB::bind_method(D_METHOD("get_use_poll_thread"), &WebServerSimple::get_use_poll_thread);
	ClassDB::bind_method(D_METHOD("set_use_poll_thread", "val"), &WebServerSimple::set_use_poll_thread);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_poll_thread"), "set_use_poll_thread", "get_use_poll_thread");

	ClassDB::bind_method(D_METHOD("get_poll_thread_count"), &WebServerSimple::get_poll_thread_count);
	ClassDB::bind_method(D_METHOD("set_poll_thread_count", "val"), &WebServerSimple::set_poll_thread_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "poll_thread_count"), "set_poll_thread_count", "get_poll_thread_count");

	ClassDB::bind_method(D_METHOD("get_use_worker_threads"), &WebServerSimple::get_use_worker_threads);
	ClassDB::bind_method(D_METHOD("set_use_worker_threads", "val"), &WebServerSimple::set_use_worker_threads);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_worker_threads"), "set_use_worker_threads", "get_use_worker_threads");

	ClassDB::bind_method(D_METHOD("get_worker_thread_count"), &WebServerSimple::get_worker_thread_count);
	ClassDB::bind_method(D_METHOD("set_worker_thread_count", "val"), &WebServerSimple::set_worker_thread_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "worker_thread_count"), "set_worker_thread_count", "get_worker_thread_count");
}

void WebServerSimple::_server_thread_poll(void *data) {
	WebServerSimple *ej = (WebServerSimple *)data;
	while (!ej->server_quit) {
		OS::get_singleton()->delay_usec(1000);
		{
			MutexLock lock(ej->server_lock);
			ej->server->poll();
		}
	}
}
