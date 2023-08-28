#ifndef SIMPLE_WEB_SERVER_REQUEST_H
#define SIMPLE_WEB_SERVER_REQUEST_H

#include "core/containers/hash_map.h"
#include "core/containers/vector.h"
#include "core/string/ustring.h"
#include "core/variant/dictionary.h"
#include "core/os/file_access.h"

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
	virtual void send();
	virtual void send_file(const String &p_file_path);
	virtual String parser_get_path();
	virtual String get_host() const;

	void add_post_parameter(const String &key, const String &value);
	void add_get_parameter(const String &key, const String &value);
	void set_parser_path(const String &value);
	void set_host(const String &value);

	void add_file(const String &key, const String &file_name, const PoolByteArray &data);

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
	String _parser_path;
	String _host;

	struct FileEntry {
		String file_name;
		PoolByteArray data;
		String key; //form name key
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
