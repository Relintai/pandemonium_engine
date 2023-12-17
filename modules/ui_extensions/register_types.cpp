

#include "register_types.h"

#include "core/input/shortcut.h"
#include "input_map_editor.h"
#include "touch_button.h"

void register_ui_extensions_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<TouchButton>();
		ClassDB::register_class<InputMapEditor>();
	}
}

void unregister_ui_extensions_types(ModuleRegistrationLevel p_level) {
}
