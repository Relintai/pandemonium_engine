#ifndef SIMPLE_WEB_SERVER_REQUEST_H
#define SIMPLE_WEB_SERVER_REQUEST_H

#include "core/dictionary.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "../http/web_server_request.h"

#include "../http/http_server_enums.h"

class WebServer;
class WebServerCookie;
class HTTPSession;
class WebPermission;
class WebNode;

class SimpleWebServerRequest : public WebServerRequest {
	GDCLASS(SimpleWebServerRequest, WebServerRequest);

public:
	virtual String get_cookie(const String &key);
	virtual void add_cookie(const Ref<WebServerCookie> &cookie);
	virtual void remove_cookie(const String &key);

	virtual HTTPServerEnums::HTTPMethod get_method() const;

	virtual void parse_files();
	virtual int get_file_count() const;
	virtual int get_file_length(const int index) const;
	virtual const uint8_t *get_file_data(const int index) const;
	virtual String get_file_data_str(const int index) const;

	virtual String get_parameter(const String &key) const;

	virtual void send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code = HTTPServerEnums::HTTP_STATUS_CODE_302_FOUND);
	virtual void send();
	virtual void send_file(const String &p_file_path);
	virtual String parser_get_path();
	virtual String get_host() const;

	//virtual String get_path_full() const;

	SimpleWebServerRequest();
	~SimpleWebServerRequest();

protected:
	static void _bind_methods();
};

#endif
