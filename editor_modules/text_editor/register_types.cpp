/*
Copyright (c) 2019-2023 Péter Magyar

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
