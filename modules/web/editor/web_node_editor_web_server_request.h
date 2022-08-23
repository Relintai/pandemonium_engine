#ifndef WEB_NODE_EDITOR_WEB_SERVER_REQUEST_H
#define WEB_NODE_EDITOR_WEB_SERVER_REQUEST_H

#include "core/containers/vector.h"
#include "core/string/ustring.h"
#include "core/variant/dictionary.h"

#include "../http/web_server_request.h"

#include "../http/http_server_enums.h"

class WebServer;
class WebServerCookie;
class HTTPSession;
class WebPermission;
class WebNode;

class WebEditorWebServerRequest : public WebServerRequest {
	GDCLASS(WebEditorWebServerRequest, WebServerRequest);

public:
	enum ResponseType {
		RESPONSE_TYPE_NONE = 0,
		RESPONSE_TYPE_NORMAL,
		RESPONSE_TYPE_FILE,
		RESPONSE_TYPE_REDIRECT,
	};

	String get_cookie(const String &key);

	HTTPServerEnums::HTTPMethod get_method() const;

	void parse_files();
	int get_file_count() const;
	String get_file_file_name(const int index) const;
	String get_file_key(const int index) const;
	int get_file_length(const int index) const;
	PoolByteArray get_file_data(const int index) const;
	String get_file_data_str(const int index) const;

	String get_parameter(const String &key) const;

	void send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code = HTTPServerEnums::HTTP_STATUS_CODE_302_FOUND);
	void send();
	void send_file(const String &p_file_path);
	void send_error(int error_code);
	String parser_get_path();
	String get_host() const;

	void update();

	WebEditorWebServerRequest();
	~WebEditorWebServerRequest();

	ResponseType _response_type;
	String _sent_message;
	bool _error_handler_called;

protected:
	static void _bind_methods();
};

#endif
