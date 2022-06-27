#ifndef WEB_SERVER_REQUEST_H
#define WEB_SERVER_REQUEST_H

#include "core/dictionary.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "core/object.h"
#include "core/reference.h"

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

	virtual const String get_cookie(const String &key);
	virtual void add_cookie(const ::WebServerCookie &cookie);
	virtual void remove_cookie(const String &key);

	virtual HTTPServerEnums::HTTPMethod get_method() const;

	virtual void parse_files();
	virtual int get_file_count() const;
	virtual int get_file_length(const int index) const;
	virtual const uint8_t *get_file_data(const int index) const;

	virtual const String get_parameter(const String &key) const;

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
	String get_url_site_add(const String &add) const;

	virtual void update();

	WebServer *get_server();
	Node *get_server_bind();
	WebNode *get_web_root();
	Node *get_web_root_bind();

	WebServerRequest();
	~WebServerRequest();

	WebServer *server;
	WebNode *web_root;

	String head;
	String body;
	String footer;
	String compiled_body;

	bool connection_closed;

	Ref<HTTPSession> session;
	Dictionary data;

	Ref<WebPermission> active_permission;
	int permissions;

protected:
	static void _bind_methods();

	HTTPServerEnums::HTTPStatusCode _status_code;
	String _full_path;
	Vector<String> _path_stack;
	uint32_t _path_stack_pointer;
};

#endif
