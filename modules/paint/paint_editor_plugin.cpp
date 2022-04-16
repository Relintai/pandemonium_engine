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

#include "paint_editor_plugin.h"

void PaintEditorPlugin::make_visible(const bool visible) {
	/*
if editor_scene:
		editor_scene.visible = visible
	*/
}

String PaintEditorPlugin::get_name() const {
	return "Paint";
}

const Ref<Texture> PaintEditorPlugin::get_icon() const {
	// Must return some kind of Texture for the icon.
	//return get_editor_interface().get_base_control().get_icon("CanvasModulate", "EditorIcons")

	return Ref<Texture>();
}
bool PaintEditorPlugin::has_main_screen() const {
	return true;
}

PaintEditorPlugin::PaintEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	/*
	editor_scene.name = "Editor"
	if get_editor_interface().get_editor_viewport().has_node("Editor"):
		var n = get_editor_interface().get_editor_viewport().get_node("Editor")
		n.name = "EditorDel"
		n.queue_free()
	get_editor_interface().get_editor_viewport().add_child(editor_scene, true)
	editor_scene.owner = get_editor_interface().get_editor_viewport()
	make_visible(false)
	*/
}

PaintEditorPlugin::~PaintEditorPlugin() {
}

void PaintEditorPlugin::_bind_methods() {
}
