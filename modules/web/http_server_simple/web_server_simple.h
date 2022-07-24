
#ifndef WEB_SERVER_SIMPLE_H
#define WEB_SERVER_SIMPLE_H

/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
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

#include "core/io/image_loader.h"
#include "core/io/json.h"
#include "core/io/stream_peer_ssl.h"
#include "core/io/tcp_server.h"
#include "core/io/zip_io.h"
#include "core/os/mutex.h"

#include "../http/web_server.h"

#include "core/project_settings.h"

class HTTPServerSimple;

class WebServerSimple : public WebServer {
	GDCLASS(WebServerSimple, WebServer);

public:
	int get_bind_port();
	void set_bind_port(const int val);

	String get_bind_host();
	void set_bind_host(const String &val);

	bool get_use_ssl();
	void set_use_ssl(const bool val);

	String get_ssl_key();
	void set_ssl_key(const String &val);

	String get_ssl_cert();
	void set_ssl_cert(const String &val);

	bool get_use_poll_thread();
	void set_use_poll_thread(const bool val);

	bool get_use_worker_threads();
	void set_use_worker_threads(const bool val);

	int get_worker_thread_count();
	void set_worker_thread_count(const int val);

	void add_mime_type(const String &file_extension, const String &mime_type);
	void remove_mime_type(const String &file_extension);

	bool is_running() const;

	void _start();
	void _stop();

	WebServerSimple();
	~WebServerSimple();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	int _bind_port;
	String _bind_host;

	//TODO add binds to set path
	bool _use_ssl;
	String _ssl_key;
	String _ssl_cert;

	bool _use_poll_thread;
	bool _use_worker_threads;
	int _worker_thread_count;

	bool _single_threaded_poll;

	Ref<HTTPServerSimple> _server;
	bool _server_quit;
	Mutex _server_lock;
	Thread *_poll_thread;
	bool _running;

	static void _server_thread_poll(void *data);
};

#endif
