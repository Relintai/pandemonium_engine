#ifndef WEB_NODE_EDITOR_WEB_SERVER_REQUEST_H
#define WEB_NODE_EDITOR_WEB_SERVER_REQUEST_H

/*************************************************************************/
/*  web_node_editor_web_server_request.h                                 */
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

class WebNodeEditorWebServerRequest : public WebServerRequest {
	GDCLASS(WebNodeEditorWebServerRequest, WebServerRequest);

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

	WebNodeEditorWebServerRequest();
	~WebNodeEditorWebServerRequest();

	ResponseType _response_type;
	String _sent_message;
	bool _error_handler_called;

protected:
	static void _bind_methods();
};

#endif
