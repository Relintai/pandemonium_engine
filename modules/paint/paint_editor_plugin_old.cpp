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

#include "paint_editor_plugin_old.h"

#include "deprecated/paint_window.h"

void PaintEditorPluginOld::make_visible(const bool visible) {
	window->set_visible(visible);
}

String PaintEditorPluginOld::get_name() const {
	return "Paint";
}

const Ref<Texture> PaintEditorPluginOld::get_icon() const {
	return _icon;
}
bool PaintEditorPluginOld::has_main_screen() const {
	return true;
}

PaintEditorPluginOld::PaintEditorPluginOld(EditorNode *p_node) {
	editor = p_node;

	window = memnew(PaintWindow);

	get_editor_interface()->get_editor_viewport()->add_child(window);
	window->set_owner(get_editor_interface()->get_editor_viewport());
	make_visible(false);
	_icon = get_editor_interface()->get_base_control()->get_theme_icon("CanvasModulate", "EditorIcons");
}

PaintEditorPluginOld::~PaintEditorPluginOld() {
}

void PaintEditorPluginOld::_bind_methods() {
}
