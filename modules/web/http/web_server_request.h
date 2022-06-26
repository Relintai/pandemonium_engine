#ifndef WEB_SERVER_REQUEST_H
#define WEB_SERVER_REQUEST_H

#include "core/ustring.h"
#include "core/vector.h"

#include <map>
#include <mutex>
#include <vector>

#include "core/object.h"
#include "core/reference.h"

#include "http_enums.h"

class WebServer;
class Cookie;
class HTTPSession;
class WebPermission;

class WebServerRequest {
public:
	WebServer *server;

	String head;
	String body;
	String footer;
	String compiled_body;

	String file_path;
	long file_size;
	long current_file_progress;
	long file_chunk_size;
	bool file_next;

	bool connection_closed;

	Ref<HTTPSession> session;
	std::map<String, Object *> data;
	std::map<String, Ref<Reference>> reference_data;

	Ref<HTTPSession> get_or_create_session();

	Ref<WebPermission> active_permission;
	int permissions;

	bool can_view() const;
	bool can_create() const;
	bool can_edit() const;
	bool can_delete() const;

	bool has_csrf_token();
	String get_csrf_token();
	void set_csrf_token(const String &value);
	bool validate_csrf_token();

	virtual const String get_cookie(const String &key);
	virtual void add_cookie(const ::Cookie &cookie);
	virtual void remove_cookie(const String &key);

	virtual HTTPMethod get_method() const;

	virtual void parse_files();
	virtual int get_file_count() const;
	virtual int get_file_length(const int index) const;
	virtual const uint8_t *get_file_data(const int index) const;

	virtual const String get_parameter(const String &key) const;

	HTTPStatusCode get_status_code() const;
	void set_status_code(const HTTPStatusCode status_code);

	virtual void send_redirect(const String &location, const HTTPStatusCode status_code = HTTP_STATUS_CODE_302_FOUND);
	virtual void compile_body();
	virtual void compile_and_send_body();
	virtual void send();
	virtual void send_file(const String &p_file_path);
	virtual void send_error(int error_code);
	virtual void reset();
	virtual String parser_get_path();
	virtual String get_host() const;

	void setup_url_stack();
	String get_path(const bool beginning_slash = false, const bool end_slash = true) const;
	virtual const String &get_path_full() const;
	const String &get_path_segment(const uint32_t i) const;
	const String &get_current_path_segment() const;
	const String &get_next_path_segment() const;
	uint32_t get_path_segment_count() const;
	uint32_t get_current_segment_index() const;
	uint32_t get_remaining_segment_count() const;
	void pop_path();
	void push_path();

	String get_url_root_parent(const int parent = 1) const;
	String get_url_root() const;
	String get_url_root_current() const;
	String get_url_site() const;

	String get_url_root_parent(const String &add) const;
	String get_url_root(const String &add) const;
	String get_url_site(const String &add) const;

	virtual void update();
	virtual void pool();

	WebServerRequest();
	virtual ~WebServerRequest();

protected:
	HTTPStatusCode _status_code;
	String _full_path;
	Vector<String> _path_stack;
	uint32_t _path_stack_pointer;
};

#endif
