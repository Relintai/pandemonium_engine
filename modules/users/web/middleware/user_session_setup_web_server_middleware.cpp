/*************************************************************************/
/*  user_session_setup_web_server_middleware.cpp                         */
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

#include "user_session_setup_web_server_middleware.h"

#include "../../singleton/user_db.h"
#include "../../users/user.h"
#include "modules/web/http/http_session.h"
#include "modules/web/http/web_server_request.h"

// returning true means handled, false means continue
bool UserSessionSetupWebServerMiddleware::_on_before_handle_request_main(Ref<WebServerRequest> request) {
	if (request->get_session().is_valid()) {
		int user_id = request->get_session()->get_int("user_id", -1);

		if (user_id != -1) {
			Ref<User> u = UserDB::get_singleton()->get_user(user_id);

			if (u.is_valid()) {
				request->set_meta("user", u);
			} else {
				// log
				request->get_session()->remove("user_id");
			}
		}
	}

	return false;
}

UserSessionSetupWebServerMiddleware::UserSessionSetupWebServerMiddleware() {
}
UserSessionSetupWebServerMiddleware::~UserSessionSetupWebServerMiddleware() {
}
