#ifndef WEB_SERVER_REQUEST_SCRIPTABLE_H
#define WEB_SERVER_REQUEST_SCRIPTABLE_H

#include "core/containers/vector.h"
#include "core/string/ustring.h"
#include "core/variant/dictionary.h"

#include "core/object/object.h"
#include "core/object/reference.h"

#include "http_server_enums.h"

#include "web_server_request.h"

class WebServer;
class WebServerCookie;
class HTTPSession;
class WebPermission;
class WebNode;

class WebServerRequestScriptable : public WebServerRequest {
	GDCLASS(WebServerRequestScriptable, WebServerRequest);

public:
	virtual String get_cookie(const String &key);

	virtual HTTPServerEnums::HTTPMethod get_method() const;

	virtual void parse_files();
	virtual int get_file_count() const;
	virtual String get_file_file_name(const int index) const;
	virtual String get_file_key(const int index) const;
	virtual int get_file_length(const int index) const;
	virtual PoolByteArray get_file_data(const int index) const;
	virtual String get_file_data_str(const int index) const;

	virtual String get_parameter(const String &key) const;
	virtual String get_post_parameter(const String &key) const;
	virtual String get_get_parameter(const String &key) const;

	virtual void set_post_parameter(const String &key, const String &value);
	virtual void set_get_parameter(const String &key, const String &value);

	virtual void send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code = HTTPServerEnums::HTTP_STATUS_CODE_302_FOUND);
	virtual void compile_body();
	virtual void compile_and_send_body();
	virtual void send();
	virtual void send_file(const String &p_file_path);
	virtual void send_error(int error_code);
	virtual String parser_get_path();
	virtual String get_host() const;

	virtual String get_path_full() const;

	virtual void update();

	// script virtuals
	virtual String _get_cookie(const String &key);

	virtual HTTPServerEnums::HTTPMethod _get_method() const;

	virtual void _parse_files();
	virtual int _get_file_count() const;
	virtual String _get_file_file_name(const int index) const;
	virtual String _get_file_key(const int index) const;
	virtual int _get_file_length(const int index) const;
	virtual PoolByteArray _get_file_data(const int index) const;
	virtual String _get_file_data_str(const int index) const;

	virtual String _get_parameter(const String &key) const;
	virtual String _get_post_parameter(const String &key) const;
	virtual String _get_get_parameter(const String &key) const;

	virtual void _set_post_parameter(const String &key, const String &value);
	virtual void _set_get_parameter(const String &key, const String &value);

	void _send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code);
	void _compile_body();
	void _compile_and_send_body();
	void _send();
	void _send_file(const String &p_file_path);
	void _send_error(int error_code);
	String _parser_get_path();
	String _get_host() const;

	String _get_path_full() const;

	void _update();

	WebServerRequestScriptable();
	~WebServerRequestScriptable();

	void _set_server_bind(Node *v);
	void _set_web_root_bind(Node *v);

protected:
	static void _bind_methods();
};

#endif
