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

#include "core/config/engine.h"

void PaintEditorPlugin::make_visible(const bool visible) {
}

String PaintEditorPlugin::get_name() const {
	return "Paint";
}

PaintEditorPlugin::PaintEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	make_visible(false);

	Engine::get_singleton()->add_global("PaintEditorPlugin", this);
}

PaintEditorPlugin::~PaintEditorPlugin() {
	Engine::get_singleton()->remove_global("PaintEditorPlugin");
}

void PaintEditorPlugin::_bind_methods() {
}
