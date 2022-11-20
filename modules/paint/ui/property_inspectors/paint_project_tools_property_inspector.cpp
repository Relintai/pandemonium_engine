/*
Copyright (c) 2022 Péter Magyar

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

#include "paint_project_tools_property_inspector.h"

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/flow_container.h"
#include "scene/gui/scroll_container.h"

#include "core/os/dir_access.h"

#include "../../nodes/paint_node.h"
#include "../../nodes/paint_project.h"

#ifdef TOOLS_ENABLED
#include "core/config/engine.h"
#include "editor/editor_node.h"
#endif

void PaintProjectToolsPropertyInspector::add_action_button(const String &callback, const String &hint, const String &icon, const String &theme_type) {
	Button *button = memnew(Button);

	Ref<Texture> icon_tex;

#ifdef TOOLS_ENABLED
	if (EditorNode::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		icon_tex = EditorNode::get_singleton()->get_gui_base()->get_theme_icon(icon, theme_type);
	} else
#endif
	{
		icon_tex = get_theme_icon(icon, theme_type);
	}

	button->set_icon(icon_tex);
	button->set_tooltip(hint);
	button->connect("pressed", this, callback);

	_button_contianer->add_child(button);
}

void PaintProjectToolsPropertyInspector::_on_paint_node_selected(Node *p_paint_node) {
	PaintNode *paint_node = Object::cast_to<PaintNode>(p_paint_node);

	_current_paint_node = 0;
	_current_paint_project = 0;

	if (!paint_node) {
		return;
	}

	_current_paint_node = paint_node->get_instance_id();

	PaintProject *proj = paint_node->get_paint_project();

	if (!proj) {
		return;
	}

	_current_paint_project = proj->get_instance_id();
}

PaintProjectToolsPropertyInspector::PaintProjectToolsPropertyInspector() {
	_current_paint_node = 0;
	_current_paint_project = 0;
	_ignore_preset_changed_event = false;
	_ignore_color_event = false;

	Control *popups = memnew(Control);
	add_child(popups);
	popups->set_mouse_filter(MOUSE_FILTER_IGNORE);

	_export_as_file_dialog = memnew(FileDialog);
	popups->add_child(_export_as_file_dialog);
	_export_as_file_dialog->set_access(FileDialog::ACCESS_FILESYSTEM);
	_export_as_file_dialog->set_resizable(true);
	_export_as_file_dialog->set_mode(FileDialog::MODE_SAVE_FILE);
	_export_as_file_dialog->set_title("Export Image");
	_export_as_file_dialog->add_filter("*.png");
	_export_as_file_dialog->connect("file_selected", this, "_on_export_as_dialog_file_selected");

	VBoxContainer *main_container = memnew(VBoxContainer);
	add_child(main_container);

	_button_contianer = memnew(HFlowContainer);
	main_container->add_child(_button_contianer);
	//_button_contianer->set_h_size_flags(SIZE_EXPAND_FILL);
	_button_contianer->set_v_size_flags(SIZE_EXPAND_FILL);

	add_action_button("_on_export_pressed", "Export Image", "Save", "EditorIcons");
	add_action_button("_on_export_as_pressed", "Export Image As", "Duplicate", "EditorIcons");
	add_action_button("_on_set_colors_as_default_pressed", "Set Colors as Default", "Theme", "EditorIcons");
}

PaintProjectToolsPropertyInspector::~PaintProjectToolsPropertyInspector() {
}

void PaintProjectToolsPropertyInspector::_on_export_pressed() {
	PaintProject *paint_project = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	ERR_FAIL_COND(!paint_project);

	String fn = paint_project->get_save_file_name();

	if (!fn.empty()) {
		_on_export_as_dialog_file_selected(fn);
	} else {
		_on_export_as_pressed();
	}
}
void PaintProjectToolsPropertyInspector::_on_export_as_pressed() {
	_export_as_file_dialog->popup_centered_ratio();
}

void PaintProjectToolsPropertyInspector::_on_export_as_dialog_file_selected(const String &f) {
	PaintProject *paint_project = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	ERR_FAIL_COND(!paint_project);

	if (f.empty()) {
		return;
	}

	Ref<Image> img = paint_project->render_image();

	ERR_FAIL_COND(!img.is_valid());

	Error err = img->save_png(f);

	ERR_FAIL_COND_MSG(err != OK, "err != OK :" + itos(err));

	String res_path = DirAccess::get_full_path("res://", DirAccess::ACCESS_FILESYSTEM);
	String user_path = DirAccess::get_full_path("user://", DirAccess::ACCESS_FILESYSTEM);

	if (f.begins_with(res_path)) {
		paint_project->set_save_file_name(f.replace_first(res_path, "res:/"));
	} else if (f.begins_with(user_path)) {
		paint_project->set_save_file_name(f.replace_first(res_path, "user:/"));
	} else {
		paint_project->set_save_file_name(f);
	}
}

void PaintProjectToolsPropertyInspector::_on_set_colors_as_default_pressed() {
	PaintProject *paint_project = Object::cast_to<PaintProject>(ObjectDB::get_instance(_current_paint_project));

	ERR_FAIL_COND(!paint_project);

	paint_project->set_colors_as_default();
}

//void PaintProjectToolsPropertyInspector::_notification(int p_what) {
//if (p_what == NOTIFICATION_THEME_CHANGED || p_what == NOTIFICATION_ENTER_TREE) {
//_add_color_button->set_icon(get_theme_icon("Add", "EditorIcons"));
//}
//}

void PaintProjectToolsPropertyInspector::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_action_button", "callback", "hint", "icon", "theme_type"), &PaintProjectToolsPropertyInspector::add_action_button);

	ClassDB::bind_method(D_METHOD("_on_export_pressed"), &PaintProjectToolsPropertyInspector::_on_export_pressed);
	ClassDB::bind_method(D_METHOD("_on_export_as_pressed"), &PaintProjectToolsPropertyInspector::_on_export_as_pressed);
	ClassDB::bind_method(D_METHOD("_on_export_as_dialog_file_selected"), &PaintProjectToolsPropertyInspector::_on_export_as_dialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_set_colors_as_default_pressed"), &PaintProjectToolsPropertyInspector::_on_set_colors_as_default_pressed);
}
