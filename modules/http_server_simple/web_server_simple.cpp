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
	ERR_FAIL_COND(_running);

	_bind_port = val;
}

String WebServerSimple::get_bind_host() {
	return _bind_host;
}
void WebServerSimple::set_bind_host(const String &val) {
	ERR_FAIL_COND(_running);

	_bind_host = val;
}

bool WebServerSimple::get_start_on_ready() {
	return _start_on_ready;
}
void WebServerSimple::set_start_on_ready(const bool val) {
	_start_on_ready = val;
}

bool WebServerSimple::get_use_ssl() {
	return _use_ssl;
}
void WebServerSimple::set_use_ssl(const bool val) {
	ERR_FAIL_COND(_running);

	_use_ssl = val;
}

String WebServerSimple::get_ssl_key() {
	return _ssl_key;
}
void WebServerSimple::set_ssl_key(const String &val) {
	ERR_FAIL_COND(_running);

	_ssl_key = val;
}

String WebServerSimple::get_ssl_cert() {
	return _ssl_cert;
}
void WebServerSimple::set_ssl_cert(const String &val) {
	ERR_FAIL_COND(_running);

	_ssl_cert = val;
}

bool WebServerSimple::get_use_poll_thread() {
	return _use_poll_thread;
}
void WebServerSimple::set_use_poll_thread(const bool val) {
	ERR_FAIL_COND(_running);

	_use_poll_thread = val;
}

bool WebServerSimple::get_use_worker_threads() {
	return _use_worker_threads;
}
void WebServerSimple::set_use_worker_threads(const bool val) {
	ERR_FAIL_COND(_running);

	_use_worker_threads = val;
}

int WebServerSimple::get_worker_thread_count() {
	return _worker_thread_count;
}
void WebServerSimple::set_worker_thread_count(const int val) {
	ERR_FAIL_COND(_running);

	_worker_thread_count = val;
}

WebServerSimple::MaxRequestSizeTypes WebServerSimple::get_max_request_size_type() {
	return _max_request_size_type;
}
void WebServerSimple::set_max_request_size_type(const MaxRequestSizeTypes val) {
	_max_request_size_type = val;
	_apply_max_request_size_type();
}

int WebServerSimple::get_max_request_size() {
	return _max_request_size;
}
void WebServerSimple::set_max_request_size(const int val) {
	_max_request_size = val;
	_apply_max_request_size_type();
}

void WebServerSimple::add_mime_type(const String &file_extension, const String &mime_type) {
	_server->mimes[file_extension] = mime_type;
}
void WebServerSimple::remove_mime_type(const String &file_extension) {
	_server->mimes.erase(file_extension);
}

bool WebServerSimple::is_running() const {
	return _is_running;
}

void WebServerSimple::_start() {
	ERR_FAIL_COND(_running);

	WebServer::_start();

	if (!OS::get_singleton()->can_use_threads()) {
		_server->_use_worker_threads = false;
	} else {
		_server->_use_worker_threads = _use_worker_threads;
	}

	if (_worker_thread_count <= 0) {
		_server->_thread_count = OS::get_singleton()->get_processor_count();
	} else {
		_server->_thread_count = _worker_thread_count;
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
	_server_lock.lock();

	_server->stop();
	err = _server->listen(bind_port, bind_ip, use_ssl, ssl_key, ssl_cert);

	_server_lock.unlock();

	ERR_FAIL_COND_MSG(err != OK, "Error starting HTTP server:\n" + itos(err));

	_running = true;
	_server_quit = false;

	if (!_use_poll_thread || !OS::get_singleton()->can_use_threads()) {
		set_process_internal(true);

		_single_threaded_poll = true;
	} else {
		_poll_thread = memnew(Thread);
		_poll_thread->start(_server_thread_poll, this);

		_single_threaded_poll = false;
	}
}

void WebServerSimple::_stop() {
	ERR_FAIL_COND(!_running);

	WebServer::_stop();

	_server_lock.lock();
	_server_quit = true;

	_server->stop();

	if (_poll_thread) {
		_poll_thread->wait_to_finish();
		memdelete(_poll_thread);
		_poll_thread = nullptr;
	}

	set_process_internal(false);

	_running = false;
	_server_lock.unlock();
}

WebServerSimple::WebServerSimple() {
	_max_request_size_type = MAX_REQUEST_SIZE_TYPE_MEGA_BYTE;
	_max_request_size = 3;

	_bind_port = 8080;
	_bind_host = "0.0.0.0";

	_start_on_ready = false;

	_use_ssl = false;

	_use_worker_threads = true;
	_use_poll_thread = true;
	_poll_thread = nullptr;
	_worker_thread_count = 4;

	_running = false;

	_server_quit = false;

	_server.instance();
	_server->_web_server = this;

	_apply_max_request_size_type();
}

WebServerSimple::~WebServerSimple() {
	_server->stop();
	_server_quit = true;

	if (_poll_thread) {
		_poll_thread->wait_to_finish();
		memdelete(_poll_thread);
	}
}

void WebServerSimple::_apply_max_request_size_type() {
	_server->max_request_size = (static_cast<uint64_t>(1) << (10 * static_cast<uint64_t>(_max_request_size_type))) * static_cast<uint64_t>(_max_request_size);
}

void WebServerSimple::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_INTERNAL_PROCESS: {
			if (_single_threaded_poll) {
				_server_lock.lock();
				_server->poll();
				_server_lock.unlock();
			}
		} break;
		case NOTIFICATION_READY: {
			if (_start_on_ready) {
				start();
			}
		} break;
		default:
			break;
	}
}

void WebServerSimple::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_bind_port"), &WebServerSimple::get_bind_port);
	ClassDB::bind_method(D_METHOD("set_bind_port", "val"), &WebServerSimple::set_bind_port);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "bind_port"), "set_bind_port", "get_bind_port");

	ClassDB::bind_method(D_METHOD("get_bind_host"), &WebServerSimple::get_bind_host);
	ClassDB::bind_method(D_METHOD("set_bind_host", "val"), &WebServerSimple::set_bind_host);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "bind_host"), "set_bind_host", "get_bind_host");

	ClassDB::bind_method(D_METHOD("get_start_on_ready"), &WebServerSimple::get_start_on_ready);
	ClassDB::bind_method(D_METHOD("set_start_on_ready", "val"), &WebServerSimple::set_start_on_ready);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "start_on_ready"), "set_start_on_ready", "get_start_on_ready");

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

	ClassDB::bind_method(D_METHOD("get_use_worker_threads"), &WebServerSimple::get_use_worker_threads);
	ClassDB::bind_method(D_METHOD("set_use_worker_threads", "val"), &WebServerSimple::set_use_worker_threads);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_worker_threads"), "set_use_worker_threads", "get_use_worker_threads");

	ClassDB::bind_method(D_METHOD("get_worker_thread_count"), &WebServerSimple::get_worker_thread_count);
	ClassDB::bind_method(D_METHOD("set_worker_thread_count", "val"), &WebServerSimple::set_worker_thread_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "worker_thread_count"), "set_worker_thread_count", "get_worker_thread_count");

	ClassDB::bind_method(D_METHOD("get_max_request_size_type"), &WebServerSimple::get_max_request_size_type);
	ClassDB::bind_method(D_METHOD("set_max_request_size_type", "val"), &WebServerSimple::set_max_request_size_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_request_size_type", PROPERTY_HINT_ENUM, "B,KB,MB,GB"), "set_max_request_size_type", "get_max_request_size_type");

	ClassDB::bind_method(D_METHOD("get_max_request_size"), &WebServerSimple::get_max_request_size);
	ClassDB::bind_method(D_METHOD("set_max_request_size", "val"), &WebServerSimple::set_max_request_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_request_size"), "set_max_request_size", "get_max_request_size");

	ClassDB::bind_method(D_METHOD("add_mime_type", "file_extension", "mime_type"), &WebServerSimple::add_mime_type);
	ClassDB::bind_method(D_METHOD("remove_mime_type", "file_extension"), &WebServerSimple::remove_mime_type);

	ClassDB::bind_method(D_METHOD("is_running"), &WebServerSimple::is_running);

	BIND_ENUM_CONSTANT(MAX_REQUEST_SIZE_TYPE_BYTE);
	BIND_ENUM_CONSTANT(MAX_REQUEST_SIZE_TYPE_KILO_BYTE);
	BIND_ENUM_CONSTANT(MAX_REQUEST_SIZE_TYPE_MEGA_BYTE);
	BIND_ENUM_CONSTANT(MAX_REQUEST_SIZE_TYPE_GIGA_BYTE);
}

void WebServerSimple::_server_thread_poll(void *data) {
	WebServerSimple *ej = (WebServerSimple *)data;
	while (!ej->_server_quit) {
		OS::get_singleton()->delay_usec(1000);

		ej->_server_lock.lock();
		ej->_server->poll();
		ej->_server_lock.unlock();
	}
}
