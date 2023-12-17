

#include "register_types.h"

#include "core/config/engine.h"

#include "users/user.h"
#include "users/user_module.h"

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

#ifdef MODULE_DATABASE_ENABLED
#include "managers/user_manager_db.h"
#endif

UserDB *_user_db = nullptr;

void register_users_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		_user_db = memnew(UserDB);
		ClassDB::register_class<UserDB>();
		Engine::get_singleton()->add_singleton(Engine::Singleton("UserDB", UserDB::get_singleton()));
	}

	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<User>();
		ClassDB::register_class<UserModule>();

		ClassDB::register_class<UserManager>();
		ClassDB::register_class<UserManagerStatic>();
		ClassDB::register_class<UserManagerFile>();

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

#ifdef MODULE_DATABASE_ENABLED
		ClassDB::register_class<UserManagerDB>();
#endif
	}
}

void unregister_users_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		if (_user_db) {
			memdelete(_user_db);
		}
	}
}
