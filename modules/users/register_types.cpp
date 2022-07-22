/*
Copyright (c) 2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "register_types.h"

#include "core/engine.h"

#include "users/user.h"

#include "managers/user_manager.h"
#include "managers/user_manager_file.h"
#include "managers/user_manager_static.h"

#include "singleton/user_db.h"

#include "../modules_enabled.gen.h"

#ifdef MODULE_WEB_ENABLED
#include "web/middleware/user_session_setup_web_server_middleware.h"

#include "web/web_nodes/user_delete_web_page.cpp"
#include "web/web_nodes/user_login_web_page.cpp"
#include "web/web_nodes/user_logout_web_page.cpp"
#include "web/web_nodes/user_password_reset_web_page.cpp"
#include "web/web_nodes/user_register_web_page.cpp"
#include "web/web_nodes/user_settings_web_page.cpp"
#include "web/web_nodes/user_stats_web_page.cpp"
#include "web/web_nodes/user_web_page.cpp"
#endif

UserDB *_user_db = nullptr;

void register_users_types() {
	ClassDB::register_class<User>();

	ClassDB::register_class<UserManager>();
	ClassDB::register_class<UserManagerStatic>();
	ClassDB::register_class<UserManagerFile>();

	ClassDB::register_class<UserDB>();

	_user_db = memnew(UserDB);
	Engine::get_singleton()->add_singleton(Engine::Singleton("UserDB", UserDB::get_singleton()));

#ifdef MODULE_WEB_ENABLED
	ClassDB::register_class<UserSessionSetupWebServerMiddleware>();

	ClassDB::register_class<UserWebPage>();
	ClassDB::register_class<UserDeleteWebPage>();
	ClassDB::register_class<UserLoginWebPage>();
	ClassDB::register_class<UserLogoutWebPage>();
	ClassDB::register_class<UserPasswordResetWebPage>();
	ClassDB::register_class<UserRegisterWebPage>();
	ClassDB::register_class<UserSettingsWebPage>();
	ClassDB::register_class<UserStatsWebPage>();
#endif
}

void unregister_users_types() {
	if (_user_db) {
		memdelete(_user_db);
	}
}
