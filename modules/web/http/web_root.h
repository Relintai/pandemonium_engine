#ifndef WEB_ROOT_H
#define WEB_ROOT_H

/*************************************************************************/
/*  web_root.h                                                           */
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

#include "core/containers/hash_map.h"
#include "core/containers/vector.h"
#include "core/os/mutex.h"
#include "core/string/ustring.h"

#include "web_node.h"

#include "web_server_middleware.h"

class WebServerRequest;
class FileCache;

class WebRoot : public WebNode {
	GDCLASS(WebRoot, WebNode);

public:
	String get_www_root_path();
	void set_www_root_path(const String &val);

	Ref<FileCache> get_www_root_file_cache();

	Vector<Variant> get_middlewares();
	void set_middlewares(const Vector<Variant> &data);

	void _handle_request_main(Ref<WebServerRequest> request);

	void _handle_error_send_request(Ref<WebServerRequest> request, const int error_code);

	bool process_middlewares(Ref<WebServerRequest> request);
	bool try_send_wwwroot_file(Ref<WebServerRequest> request);
	void send_file(const String &path, Ref<WebServerRequest> request);

	void register_request_update(Ref<WebServerRequest> request);
	void unregister_request_update(Ref<WebServerRequest> request);

	WebRoot();
	~WebRoot();

	Vector<Ref<WebServerMiddleware>> _middlewares;

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Mutex _update_registered_requests_mutex;
	Vector<Ref<WebServerRequest>> _update_registered_requests;

	String _www_root_path;
	Ref<FileCache> _www_root_file_cache;
};

#endif
