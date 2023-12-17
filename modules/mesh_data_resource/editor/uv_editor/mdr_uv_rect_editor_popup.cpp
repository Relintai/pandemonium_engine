

#include "mdr_uv_rect_editor_popup.h"

#include "../mdi_ed_plugin.h"
#include "mdr_uv_rect_editor.h"
#include "scene/gui/button.h"

void MDRUVRectEditorPopup::on_ok_pressed() {
	_editor->ok_pressed();
}
void MDRUVRectEditorPopup::on_cancel_pressed() {
	_editor->cancel_pressed();
}

MDRUVRectEditor *MDRUVRectEditorPopup::get_editor() {
	return _editor;
}

void MDRUVRectEditorPopup::set_plugin(MDIEdPlugin *plugin) {
	_editor->set_plugin(plugin);
}

void MDRUVRectEditorPopup::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		connect("confirmed", this, "on_ok_pressed");
	}
}

MDRUVRectEditorPopup::MDRUVRectEditorPopup() {
	_editor = memnew(MDRUVRectEditor);
	_editor->set_h_size_flags(SIZE_EXPAND_FILL);
	_editor->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(_editor);

	get_cancel()->connect("pressed", this, "on_cancel_pressed");

	set_size(Vector2(700, 500));
}

MDRUVRectEditorPopup::~MDRUVRectEditorPopup() {
}

void MDRUVRectEditorPopup::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_ok_pressed"), &MDRUVRectEditorPopup::on_ok_pressed);
	ClassDB::bind_method(D_METHOD("on_cancel_pressed"), &MDRUVRectEditorPopup::on_cancel_pressed);
}
