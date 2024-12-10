#ifndef WEB_SERVER_REQUEST_H
#define WEB_SERVER_REQUEST_H

/*************************************************************************/
/*  web_server_request.h                                                 */
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
#include "core/string/ustring.h"
#include "core/variant/dictionary.h"

#include "core/object/object.h"
#include "core/object/reference.h"

#include "http_server_enums.h"

class WebServer;
class WebServerCookie;
class HTTPSession;
class WebPermission;
class WebNode;

class WebServerRequest : public Reference {
	GDCLASS(WebServerRequest, Reference);

public:
	String get_head();
	void set_head(const String &val);

	String get_body();
	void set_body(const String &val);

	String get_footer();
	void set_footer(const String &val);

	String get_compiled_body();
	void set_compiled_body(const String &val);

	bool get_connection_closed();
	void set_connection_closed(const bool &val);

	Ref<HTTPSession> get_session();
	void set_session(const Ref<HTTPSession> &val);

	Dictionary get_data();
	void set_data(const Dictionary &val);

	Ref<WebPermission> get_active_permission();
	void set_active_permission(const Ref<WebPermission> &val);

	int get_permissions();
	void set_permissions(const int &val);

	Ref<HTTPSession> get_or_create_session();

	bool can_view() const;
	bool can_create() const;
	bool can_edit() const;
	bool can_delete() const;

	bool has_csrf_token();
	String get_csrf_token();
	void set_csrf_token(const String &value);
	bool validate_csrf_token();

	bool has_locale() const;
	StringName get_locale() const;
	void set_locale(const StringName &p_locale);

	void save_locale_as_cookie();
	void load_locale_from_cookie();
	void clear_locale_from_cookie();

	virtual String get_cookie(const String &key);

	void response_add_cookie(const Ref<WebServerCookie> &cookie);
	int response_get_cookie_count();
	Ref<WebServerCookie> response_get_cookie(const int index);
	void response_remove_cookie(const int index);
	void response_remove_cookie_simple(const String &key);

	void custom_response_header_set(const StringName &key, const String &value);
	String custom_response_header_get(const StringName &key);
	bool custom_response_header_has(const StringName &key);
	HashMap<StringName, String> custom_response_headers_get();
	Dictionary custom_response_headers_get_bind();

	String get_content_type();
	void set_content_type(const String &content_type);

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

	HTTPServerEnums::HTTPStatusCode get_status_code() const;
	void set_status_code(const HTTPServerEnums::HTTPStatusCode status_code);

	virtual void send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code = HTTPServerEnums::HTTP_STATUS_CODE_302_FOUND);
	virtual void compile_body();
	virtual void compile_and_send_body();
	virtual void send();
	virtual void send_file(const String &p_file_path);
	virtual void send_error(int error_code);
	virtual String parser_get_path();
	virtual String get_host() const;

	void setup_url_stack();
	String get_path(const bool beginning_slash = false, const bool end_slash = true) const;
	virtual String get_path_full() const;
	String get_path_segment(const uint32_t i) const;
	String get_current_path_segment() const;
	String get_next_path_segment() const;
	uint32_t get_path_segment_count() const;
	uint32_t get_current_segment_index() const;
	uint32_t get_remaining_segment_count() const;
	void pop_path();
	void push_path();

	String get_url_root_parent(const int parent = 1) const;
	String get_url_root() const;
	String get_url_root_current() const;
	String get_url_site() const;

	String get_url_root_parent_add(const String &add) const;
	String get_url_root_add(const String &add) const;
	String get_url_root_current_add(const String &add) const;
	String get_url_site_add(const String &add) const;

	virtual void update();

	WebServer *get_server();
	WebNode *get_web_root();

	WebServerRequest();
	~WebServerRequest();

	String head;
	String body;
	String footer;
	String compiled_body;

	void _set_server(WebServer *v);
	void _set_web_root(WebNode *v);

protected:
	static void _bind_methods();

	HTTPServerEnums::HTTPStatusCode _status_code;
	String _full_path;
	Vector<String> _path_stack;
	int _path_stack_pointer;
	Vector<Ref<WebServerCookie>> _cookies;

	HashMap<StringName, String> _custom_response_headers;

	Ref<WebPermission> _active_permission;
	int _permissions;

	Ref<HTTPSession> _session;

	StringName _locale;

	bool _connection_closed;

	WebServer *_server;
	WebNode *_web_root;
};

#endif
