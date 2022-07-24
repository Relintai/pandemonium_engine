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

	WebServer::_start();

	const uint16_t bind_port = 8080;
	// Resolve host if needed.
	const String bind_host = "127.0.0.1";
	IP_Address bind_ip;

	if (bind_host.is_valid_ip_address()) {
		bind_ip = bind_host;
	} else {
		bind_ip = IP::get_singleton()->resolve_hostname(bind_host);
	}

	ERR_FAIL_COND_MSG(!bind_ip.is_valid(), "Invalid editor setting 'export/web/http_host': '" + bind_host + "'. Try using '127.0.0.1'.");

	const bool use_ssl = false;
	const String ssl_key = "";
	const String ssl_cert = "";

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
	server_quit = false;
	_use_worker_threads = true;
	_use_poll_thread = true;
	_poll_thread_count = 1;
	_worker_thread_count = 4;

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
