

#include "register_types.h"

#include "web_server_simple.h"

void register_http_server_simple_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<WebServerSimple>();
	}
}

void unregister_http_server_simple_types(ModuleRegistrationLevel p_level) {
}
