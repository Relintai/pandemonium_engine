

#include "register_types.h"

#include "text_editor_file.h"
#include "text_editor_format_loader.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_plugin.h"
#include "text_editor_plugin.h"
#endif

Ref<TextEditorTextLoader> text_editor_text_resource_loader;

void register_text_editor_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_CORE) {
		text_editor_text_resource_loader.instance();
		ResourceLoader::add_resource_format_loader(text_editor_text_resource_loader);
	}

	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<TextEditorFile>();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<TextEditorEditorPlugin>();
	}
#endif
}

void unregister_text_editor_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_CORE) {
		ResourceLoader::remove_resource_format_loader(text_editor_text_resource_loader);
		text_editor_text_resource_loader.unref();
	}
}
