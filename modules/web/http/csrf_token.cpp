/*************************************************************************/
/*  csrf_token.cpp                                                       */
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

#include "csrf_token.h"

#include "core/crypto/crypto_core.h"
#include "core/os/os.h"
#include "http_server_enums.h"
#include "http_session.h"
#include "web_server_request.h"

PoolStringArray CSRFTokenWebServerMiddleware::get_ignored_urls() {
	return ignored_urls;
}
void CSRFTokenWebServerMiddleware::set_ignored_urls(const PoolStringArray &val) {
	ignored_urls = val;
}

bool CSRFTokenWebServerMiddleware::_on_before_handle_request_main(Ref<WebServerRequest> request) {
	switch (request->get_method()) {
		case HTTPServerEnums::HTTP_METHOD_POST:
		case HTTPServerEnums::HTTP_METHOD_DELETE:
		case HTTPServerEnums::HTTP_METHOD_PATCH:
		case HTTPServerEnums::HTTP_METHOD_PUT: {
			if (shold_ignore(request)) {
				return false;
			}

			if (!request->get_session().is_valid()) {
				request->send_error(HTTPServerEnums::HTTP_STATUS_CODE_401_UNAUTHORIZED);
				return true;
			}

			if (!request->validate_csrf_token()) {
				request->send_error(HTTPServerEnums::HTTP_STATUS_CODE_401_UNAUTHORIZED);
				return true;
			}

			break;
		}
		default:
			break;
	}

	// don't create the session itself
	if (!request->get_session().is_valid()) {
		return false;
	}

	if (!request->has_csrf_token()) {
		request->set_csrf_token(create_token());
	}

	return false;
}

bool CSRFTokenWebServerMiddleware::shold_ignore(Ref<WebServerRequest> request) {
	String path = request->get_path_full();

	for (int i = 0; i < ignored_urls.size(); ++i) {
		if (path.begins_with(ignored_urls[i])) {
			return true;
		}
	}

	return false;
}

String CSRFTokenWebServerMiddleware::create_token() {
	String s = String::num(OS::get_singleton()->get_unix_time());

	return s.sha256_text().substr(0, 10);
}

CSRFTokenWebServerMiddleware::CSRFTokenWebServerMiddleware() {
}
CSRFTokenWebServerMiddleware::~CSRFTokenWebServerMiddleware() {
}

void CSRFTokenWebServerMiddleware::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_ignored_urls"), &CSRFTokenWebServerMiddleware::get_ignored_urls);
	ClassDB::bind_method(D_METHOD("set_ignored_urls", "val"), &CSRFTokenWebServerMiddleware::set_ignored_urls);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "ignored_urls"), "set_ignored_urls", "get_ignored_urls");

	ClassDB::bind_method(D_METHOD("create_token"), &CSRFTokenWebServerMiddleware::create_token);
}
