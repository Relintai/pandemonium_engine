#ifndef SIMPLE_WEB_SERVER_REQUEST_H
#define SIMPLE_WEB_SERVER_REQUEST_H

/*************************************************************************/
/*  simple_web_server_request.h                                          */
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
#include "core/os/file_access.h"
#include "core/string/ustring.h"
#include "core/variant/dictionary.h"

#include "modules/web/http/web_server_request.h"

#include "modules/web/http/http_server_enums.h"

class WebServer;
class WebServerCookie;
class HTTPSession;
class WebPermission;
class WebNode;
class HTTPServerSimple;
class HTTPServerConnection;

class SimpleWebServerRequest : public WebServerRequest {
	GDCLASS(SimpleWebServerRequest, WebServerRequest);

public:
	virtual String get_cookie(const String &key);

	virtual HTTPServerEnums::HTTPMethod get_method() const;

	virtual int get_file_count() const;
	virtual String get_file_file_name(const int index) const;
	virtual String get_file_key(const int index) const;
	virtual uint64_t get_file_length(const int index) const;
	virtual PoolByteArray get_file_data(const int index) const;
	virtual String get_file_data_str(const int index) const;
	virtual Error move_file(const int index, const String &p_dest_file, const bool p_force = false);
	virtual bool is_file_moved(const int index) const;

	virtual String get_parameter(const String &key) const;
	virtual String get_post_parameter(const String &key) const;
	virtual String get_get_parameter(const String &key) const;

	virtual void set_post_parameter(const String &key, const String &value);
	virtual void set_get_parameter(const String &key, const String &value);

	virtual String get_header_parameter(const String &key) const;
	virtual void set_header_parameter(const String &key, const String &value);

	virtual PoolStringArray get_header_parameter_keys() const;
	virtual PoolStringArray get_parameter_keys() const;
	virtual PoolStringArray get_post_parameter_keys() const;
	virtual PoolStringArray get_get_parameter_keys() const;

	virtual void send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code = HTTPServerEnums::HTTP_STATUS_CODE_302_FOUND);
	virtual void send();
	virtual void send_file(const String &p_file_path);
	virtual String parser_get_path();
	virtual String get_host() const;

	void add_post_parameter(const String &key, const String &value);
	void add_get_parameter(const String &key, const String &value);
	void add_header_parameter(const String &key, const String &value);
	void set_parser_path(const String &value);
	void set_host(const String &value);

	void add_file_memory(const String &key, const String &file_name, const PoolByteArray &data);
	void add_file_temp_file(const String &key, const String &file_name, const String &path);

	void add_cookie_data(const String &key, const String &value);

	void set_method(const HTTPServerEnums::HTTPMethod method);

	//virtual String get_path_full() const;

	bool sent();

	SimpleWebServerRequest();
	~SimpleWebServerRequest();

	HTTPServerSimple *_server;
	Ref<HTTPServerConnection> _connection;

	FileAccess *_sending_file_fa;

protected:
	static void _bind_methods();

	HashMap<String, String> _post_parameters;
	HashMap<String, String> _get_parameters;
	HashMap<String, String> _header_parameters;
	String _parser_path;
	String _host;

	struct FileEntry {
		enum FileEntryType {
			FILE_ENTRY_TYPE_MEMORY = 0,
			FILE_ENTRY_TYPE_TEMP_FILE,
		};

		mutable bool moved;

		FileEntryType type;
		String file_name;
		PoolByteArray data;
		String key; //form name key
		mutable String path;

		FileEntry() {
			moved = false;
		}
	};

	Vector<FileEntry> _files;

	struct CookieData {
		String key;
		String value;
	};

	Vector<CookieData> _cookies;
	HTTPServerEnums::HTTPMethod _method;
};

#endif
