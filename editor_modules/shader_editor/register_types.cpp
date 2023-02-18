
#include "register_types.h"

#include "shader_editor_plugin.h"

void register_shader_editor_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		//ClassDB::register_class<>();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<ShaderEditorPlugin>();
	}
#endif
}

void unregister_shader_editor_types(ModuleRegistrationLevel p_level) {
}
