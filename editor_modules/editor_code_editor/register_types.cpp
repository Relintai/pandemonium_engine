
#include "register_types.h"

#include "editor_script_editor.h"
#include "script_editor_plugin.h"

#include "editor_script_editor_base.h"
//#include "script_text_editor.h"
//#include "text_editor.h"
//#include "code_text_editor.h"

void register_editor_code_editor_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		//ClassDB::register_class<>();
		ClassDB::register_virtual_class<EditorScriptEditor>();

		ClassDB::register_virtual_class<EditorScriptEditorBase>();

		ClassDB::register_class<EditorSyntaxHighlighter>();

		//ClassDB::register_class<TextEditor>();
		//ClassDB::register_class<EditorScriptTextEditor>();
		//ClassDB::register_class<EditorCodeTextEditor>();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type_front<EditorScriptEditorPlugin>();
	}
#endif
}

void unregister_editor_code_editor_types(ModuleRegistrationLevel p_level) {
}
