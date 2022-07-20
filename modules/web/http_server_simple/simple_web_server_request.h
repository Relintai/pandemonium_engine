#ifndef SIMPLE_WEB_SERVER_REQUEST_H
#define SIMPLE_WEB_SERVER_REQUEST_H

#include "core/dictionary.h"
#include "core/hash_map.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "../http/web_server_request.h"

#include "../http/http_server_enums.h"

class WebServer;
class WebServerCookie;
class HTTPSession;
class WebPermission;
class WebNode;
class HTTPServerSimple;

class SimpleWebServerRequest : public WebServerRequest {
	GDCLASS(SimpleWebServerRequest, WebServerRequest);

public:
	virtual String get_cookie(const String &key);
	virtual void add_cookie(const Ref<WebServerCookie> &cookie);
	virtual void remove_cookie(const String &key);

	virtual HTTPServerEnums::HTTPMethod get_method() const;

	virtual void parse_files();
	virtual int get_file_count() const;
	virtual String get_file_file_name(const int index) const;
	virtual String get_file_key(const int index) const;
	virtual int get_file_length(const int index) const;
	virtual PoolByteArray get_file_data(const int index) const;
	virtual String get_file_data_str(const int index) const;

	virtual String get_parameter(const String &key) const;

	virtual void send_redirect(const String &location, const HTTPServerEnums::HTTPStatusCode status_code = HTTPServerEnums::HTTP_STATUS_CODE_302_FOUND);
	virtual void send();
	virtual void send_file(const String &p_file_path);
	virtual String parser_get_path();
	virtual String get_host() const;

	void add_parameter(const String &key, const String &value);
	void set_parser_path(const String &value);
	void set_host(const String &value);

	void add_file(const String &key, const String &file_name, const PoolByteArray &data);

	//virtual String get_path_full() const;

	SimpleWebServerRequest();
	~SimpleWebServerRequest();

	HTTPServerSimple *_server;

protected:
	static void _bind_methods();

	HashMap<String, String> _parameters;
	String _parser_path;
	String _host;

	struct FileEntry {
		String file_name;
		PoolByteArray data;
		String key; //form name key
	};

	Vector<FileEntry> _files;
};

#endif
