/*
Copyright (c) 2019-2022 Péter Magyar

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

#include "text_editor_plugin.h"

#include "text_editor_file.h"
#include "text_file_editor.h"
#include "editor/editor_node.h"

#include "editor/filesystem_dock.h"

void TextEditorEditorPlugin::make_visible(const bool visible) {
	window->set_visible(visible);
}

String TextEditorEditorPlugin::get_name() const {
	return "Text";
}

const Ref<Texture> TextEditorEditorPlugin::get_icon() const {
	return _icon;
}

bool TextEditorEditorPlugin::has_main_screen() const {
	return true;
}

void TextEditorEditorPlugin::edit(Object *p_object) {
	Ref<TextEditorFile> f = Object::cast_to<TextEditorFile>(p_object);

	if (f.is_valid()) {
		window->open_file(f->get_path());
	}
}

bool TextEditorEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("TextEditorFile");
}

TextEditorEditorPlugin::TextEditorEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	window = memnew(TextFileEditor);

	get_editor_interface()->get_editor_viewport()->add_child(window);

	_filesystem_dock_entry_id = get_editor_interface()->get_file_system_dock()->add_custom_popup_creation_entry("New Textfile...", "TextEditorFile", "EditorIcons");
	get_editor_interface()->get_file_system_dock()->connect("custom_popup_creation_entry_pressed", this, "_on_filesystem_dock_entry_pressed");

	make_visible(false);
	_icon = get_editor_interface()->get_base_control()->get_theme_icon("File", "EditorIcons");
}

TextEditorEditorPlugin::~TextEditorEditorPlugin() {
}

void TextEditorEditorPlugin::_on_filesystem_dock_entry_pressed(int id) {
	if (_filesystem_dock_entry_id == id) {
		editor->select_editor_by_name(get_name());
		window->create_selected_file();
	}
}

void TextEditorEditorPlugin::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_filesystem_dock_entry_pressed"), &TextEditorEditorPlugin::_on_filesystem_dock_entry_pressed);
}
