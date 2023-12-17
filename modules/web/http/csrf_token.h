#ifndef CSRF_TOKEN_MIDDLEWARE_H
#define CSRF_TOKEN_MIDDLEWARE_H

/*************************************************************************/
/*  csrf_token.h                                                         */
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

#include "web_server_middleware.h"

#include "core/string/ustring.h"
#include "core/variant/variant.h"

class WebServerRequest;

class CSRFTokenWebServerMiddleware : public WebServerMiddleware {
	GDCLASS(CSRFTokenWebServerMiddleware, WebServerMiddleware);

public:
	PoolStringArray get_ignored_urls();
	void set_ignored_urls(const PoolStringArray &val);

	//returnring true means handled, false means continue
	bool _on_before_handle_request_main(Ref<WebServerRequest> request);

	bool shold_ignore(Ref<WebServerRequest> request);

	virtual String create_token();

	CSRFTokenWebServerMiddleware();
	~CSRFTokenWebServerMiddleware();

	PoolStringArray ignored_urls;

protected:
	static void _bind_methods();
};

#endif
