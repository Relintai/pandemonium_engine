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

#include "mdr_uv_rect_editor_popup.h"

#include "mdr_uv_rect_editor.h"
#include "scene/gui/button.h"

void MDRUVRectEditorPopup::on_ok_pressed() {
	_editor->ok_pressed();
}
void MDRUVRectEditorPopup::on_cancel_pressed() {
	_editor->cancel_pressed();
}

MDRUVRectEditorPopup::MDRUVRectEditorPopup() {
	_editor = memnew(MDRUVRectEditor);
	_editor->set_h_size_flags(SIZE_EXPAND_FILL);
	_editor->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(_editor);

	connect("confirmed", this, "on_ok_pressed");
	get_cancel()->connect("pressed", this, "on_cancel_pressed");

	set_size(Vector2(700, 500));
}

MDRUVRectEditorPopup::~MDRUVRectEditorPopup() {
}

void MDRUVRectEditorPopup::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_ok_pressed"), &MDRUVRectEditorPopup::on_ok_pressed);
	ClassDB::bind_method(D_METHOD("on_cancel_pressed"), &MDRUVRectEditorPopup::on_cancel_pressed);
}
