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

#include "web_editor_plugin.h"

#include "../http/web_node.h"
#include "web_editor.h"

#include "editor/editor_node.h"

void WebEditorPlugin::make_visible(const bool visible) {
	window->set_visible(visible);
}

String WebEditorPlugin::get_name() const {
	return "Web";
}

const Ref<Texture> WebEditorPlugin::get_icon() const {
	return _icon;
}

bool WebEditorPlugin::has_main_screen() const {
	return true;
}

void WebEditorPlugin::edit(Object *p_object) {
	//WebNode *wn = Object::cast_to<WebNode>(p_object);

	//if (wn) {
	//	window->set_node(wn);
	//}
}

bool WebEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("WebNode");
}

WebEditorPlugin::WebEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	window = memnew(WebEditor);

	get_editor_interface()->get_editor_viewport()->add_child(window);
	_icon = get_editor_interface()->get_base_control()->get_theme_icon("WebNodeEditor", "EditorIcons");

	make_visible(false);
}

WebEditorPlugin::~WebEditorPlugin() {
}

void WebEditorPlugin::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			get_editor_interface()->set_main_screen_editor_tab_button_visible("Web", false);
		} break;
	}
}

void WebEditorPlugin::_bind_methods() {
	//ClassDB::bind_method(D_METHOD(""), &WebEditorPlugin::);
}
