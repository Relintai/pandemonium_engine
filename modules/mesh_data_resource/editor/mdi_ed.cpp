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

#include "mdi_ed.h"

#include "./uv_editor/mdr_ed_uv_previewer.h"
#include "./uv_editor/mdr_uv_rect_editor_popup.h"
#include "core/os/keyboard.h"
#include "mdi_ed_plugin.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"

void MDIEd::_enter_tree() {
	/*
	uv_preview = get_node(uv_preview_path)
	uv_editor = get_node(uv_editor_path)

	if _plugin && uv_editor:
		uv_editor.set_plugin(_plugin)
	*/
}
void MDIEd::set_plugin(EditorPlugin *plugin) {
	/*
	_plugin = plugin

	if uv_editor:
		uv_editor.set_plugin(plugin)
	*/
}
void MDIEd::set_mesh_data_resource(Ref<MeshDataResource> a) {
	/*
	if uv_preview:
		uv_preview.set_mesh_data_resource(a)

	if uv_editor:
		uv_editor.set_mesh_data_resource(a)
	*/
}
void MDIEd::set_mesh_data_instance(MeshDataInstance *a) {
	/*
	if uv_preview:
		uv_preview.set_mesh_data_instance(a)

	if uv_editor:
		uv_editor.set_mesh_data_instance(a)
	*/
}
void MDIEd::_unhandled_key_input(Ref<InputEventKey> event) {
	if (event->is_echo()) {
		return;
	}

	if (event->get_alt() || event->get_shift() || event->get_control() || event->get_metakey() || event->get_command()) {
		return;
	}

	uint32_t scancode = event->get_scancode();

	if (scancode == KEY_G) {
		set_edit_mode_translate();
	} else if (scancode == KEY_H) {
		set_edit_mode_rotate();
	} else if (scancode == KEY_J) {
		set_edit_mode_scale();
	} else if (scancode == KEY_V) {
		set_axis_x(!get_axis_x());
	} else if (scancode == KEY_B) {
		set_axis_y(!get_axis_y());
	} else if (scancode == KEY_N) {
		set_axis_z(!get_axis_z());
	} else if (scancode == KEY_K) {
		set_selection_mode_vertex();
	} else if (scancode == KEY_L) {
		set_selection_mode_edge();
	} else if (scancode == KEY_SEMICOLON) {
		set_selection_mode_face();
	}
}

//Edit modes
void MDIEd::set_edit_mode_translate() {
	/*
	$VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer/Translate.pressed = true
	*/
}
void MDIEd::set_edit_mode_rotate() {
	/*
	$VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer/Rotate.pressed = true
	*/
}
void MDIEd::set_edit_mode_scale() {
	/*
	$VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer/Scale.pressed = true
	*/
}

void MDIEd::on_edit_mode_translate_toggled(bool on) {
	if (on) {
		if (_plugin) {
			_plugin->set_translate();
		}
	}
}
void MDIEd::on_edit_mode_rotate_toggled(bool on) {
	if (on) {
		if (_plugin) {
			_plugin->set_rotate();
		}
	}
}
void MDIEd::on_edit_mode_scale_toggled(bool on) {
	if (on) {
		if (_plugin) {
			_plugin->set_scale();
		}
	}
}

//axis locks
bool MDIEd::get_axis_x() {
	/*
	return $VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer2/AxisX.pressed
	*/
	return false;
}
bool MDIEd::get_axis_y() {
	/*
	return $VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer2/AxisY.pressed
	*/
	return false;
}
bool MDIEd::get_axis_z() {
	/*
	return $VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer2/AxisZ.pressed
	*/
	return false;
}
void MDIEd::set_axis_x(bool on) {
	/*
	$VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer2/AxisX.pressed = on
	*/
}
void MDIEd::set_axis_y(bool on) {
	/*
	$VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer2/AxisY.pressed = on
	*/
}
void MDIEd::set_axis_z(bool on) {
	/*
	$VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer2/AxisZ.pressed = on
	*/
}

void MDIEd::on_axis_x_toggled(bool on) {
	if (on) {
		if (_plugin) {
			_plugin->set_axis_x(on);
		}
	}
}
void MDIEd::on_axis_y_toggled(bool on) {
	if (on) {
		if (_plugin) {
			_plugin->set_axis_y(on);
		}
	}
}
void MDIEd::on_axis_z_toggled(bool on) {
	if (on) {
		if (_plugin) {
			_plugin->set_axis_z(on);
		}
	}
}

//selection modes
void MDIEd::on_selection_mode_vertex_toggled(bool on) {
	if (on) {
		if (_plugin) {
			_plugin->set_selection_mode_vertex();
		}
	}
}
void MDIEd::on_selection_mode_edge_toggled(bool on) {
	if (on) {
		if (_plugin) {
			_plugin->set_selection_mode_edge();
		}
	}
}
void MDIEd::on_selection_mode_face_toggled(bool on) {
	if (on) {
		if (_plugin) {
			_plugin->set_selection_mode_face();
		}
	}
}

void MDIEd::set_selection_mode_vertex() {
	/*
	$VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer3/Vertex.pressed = true
	*/
}
void MDIEd::set_selection_mode_edge() {
	/*
	$VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer3/Edge.pressed = true
	*/
}
void MDIEd::set_selection_mode_face() {
	/*
	$VBoxContainer/Actions/Actions/VBoxContainer2/HBoxContainer3/Face.pressed = true
	*/
}

void MDIEd::_on_Extrude_pressed() {
	_plugin->extrude();
}
void MDIEd::_on_AddBox_pressed() {
	_plugin->add_box();
}
void MDIEd::_on_UnwrapButton_pressed() {
	_plugin->uv_unwrap();
}
void MDIEd::_on_add_triangle_pressed() {
	_plugin->add_triangle();
}
void MDIEd::_on_add_quad_pressed() {
	_plugin->add_quad();
}
void MDIEd::_on_split_pressed() {
	_plugin->split();
}
void MDIEd::_on_connect_to_first_selected_pressed() {
	_plugin->connect_to_first_selected();
}
void MDIEd::_on_connect_to_avg_pressed() {
	_plugin->connect_to_avg();
}
void MDIEd::_on_connect_to_last_selected_pressed() {
	_plugin->connect_to_last_selected();
}
void MDIEd::_on_disconnect_pressed() {
	_plugin->disconnect_action();
}
void MDIEd::_on_add_triangle_at_pressed() {
	_plugin->add_triangle_at();
}
void MDIEd::_on_add_auad_at_pressed() {
	_plugin->add_quad_at();
}
void MDIEd::_oncreate_face_pressed() {
	_plugin->create_face();
}
void MDIEd::_on_delete_pressed() {
	_plugin->delete_selected();
}
void MDIEd::_on_GenNormals_pressed() {
	_plugin->generate_normals();
}
void MDIEd::_on_RemDoubles_pressed() {
	_plugin->remove_doubles();
}
void MDIEd::_on_MergeOptimize_pressed() {
	_plugin->merge_optimize();
}
void MDIEd::_on_GenTangents_pressed() {
	_plugin->generate_tangents();
}
void MDIEd::_on_mark_seam_pressed() {
	_plugin->mark_seam();
}
void MDIEd::_on_unmark_seam_pressed() {
	_plugin->unmark_seam();
}
void MDIEd::_on_apply_seams_pressed() {
	_plugin->apply_seam();
}
void MDIEd::_on_uv_edit_pressed() {
	/*
	$Popups/UVEditorPopup.popup_centered()
	*/
}

void MDIEd::on_pivot_average_toggled(bool on) {
	if (on) {
		_plugin->set_pivot_averaged();
	}
}
void MDIEd::on_pivot_mdi_origin_toggled(bool on) {
	if (on) {
		_plugin->set_pivot_mdi_origin();
	}
}
void MDIEd::on_pivot_world_origin_toggled(bool on) {
	if (on) {
		_plugin->set_pivot_world_origin();
	}
}
void MDIEd::on_visual_indicator_outline_toggled(bool on) {
	_plugin->visual_indicator_outline_set(on);
}
void MDIEd::on_visual_indicator_seam_toggled(bool on) {
	_plugin->visual_indicator_seam_set(on);
}
void MDIEd::on_visual_indicator_handle_toggled(bool on) {
	_plugin->visual_indicator_handle_set(on);
}
void MDIEd::_on_select_all_pressed() {
	_plugin->select_all();
}

void MDIEd::onhandle_selection_type_front_toggled(bool on) {
	_plugin->handle_selection_type_front();
}
void MDIEd::onhandle_selection_type_back_toggled(bool on) {
	_plugin->handle_selection_type_back();
}
void MDIEd::onhandle_selection_type_all_toggled(bool on) {
	_plugin->handle_selection_type_all();
}

void MDIEd::_on_clean_mesh_pressed() {
	_plugin->clean_mesh();
}
void MDIEd::_on_flip_face_pressed() {
	_plugin->flip_selected_faces();
}

MDIEd::MDIEd() {
	uv_preview = nullptr;

	//Main
	VBoxContainer *main_cont = memnew(VBoxContainer);
	add_child(main_cont);

	// -- Actions
	VBoxContainer *main_actions_container = memnew(VBoxContainer);
	main_cont->add_child(main_actions_container);

	HBoxContainer *actions_inner_container = memnew(HBoxContainer);
	actions_inner_container->set_alignment(BoxContainer::ALIGN_CENTER);
	main_actions_container->add_child(actions_inner_container);

	VBoxContainer *left_actions_container = memnew(VBoxContainer);
	actions_inner_container->add_child(left_actions_container);

	VBoxContainer *right_actions_container = memnew(VBoxContainer);
	actions_inner_container->add_child(right_actions_container);

	// -- Actions -- Edit Mode
	Label *action_label = memnew(Label);
	action_label->set_text("E");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	action_label->set_tooltip("Edit Mode");
	action_label->set_mouse_filter(Control::MOUSE_FILTER_STOP);
	action_label->set_v_size_flags(SIZE_EXPAND_FILL | SIZE_SHRINK_CENTER);
	left_actions_container->add_child(action_label);

	Ref<ButtonGroup> action_button_group;
	action_button_group.instance();

	HBoxContainer *action_buttons_container = memnew(HBoxContainer);
	right_actions_container->add_child(action_buttons_container);

	Button *action_button = memnew(Button);
	action_button->set_text("T");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Translate");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_edit_mode_translate_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("R");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(false);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Rotate");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_edit_mode_rotate_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("S");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(false);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Scale");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_edit_mode_scale_toggled");
	action_buttons_container->add_child(action_button);

	// -- Actions -- Active Axis
	action_label = memnew(Label);
	action_label->set_text("A");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	action_label->set_tooltip("Active Axis");
	action_label->set_mouse_filter(Control::MOUSE_FILTER_STOP);
	action_label->set_v_size_flags(SIZE_EXPAND_FILL | SIZE_SHRINK_CENTER);
	left_actions_container->add_child(action_label);

	action_buttons_container = memnew(HBoxContainer);
	right_actions_container->add_child(action_buttons_container);

	action_button = memnew(Button);
	action_button->set_text("X");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_axis_x_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Y");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_axis_y_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Z");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_axis_z_toggled");
	action_buttons_container->add_child(action_button);

	// -- Actions -- Selection Mode
	action_label = memnew(Label);
	action_label->set_text("S");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	action_label->set_tooltip("Selection Mode");
	action_label->set_mouse_filter(Control::MOUSE_FILTER_STOP);
	action_label->set_v_size_flags(SIZE_EXPAND_FILL | SIZE_SHRINK_CENTER);
	left_actions_container->add_child(action_label);

	action_button_group.instance();

	action_buttons_container = memnew(HBoxContainer);
	right_actions_container->add_child(action_buttons_container);

	action_button = memnew(Button);
	action_button->set_text("V");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Vertex");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_selection_mode_vertex_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("E");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(false);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Edge");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_selection_mode_edge_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("F");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(false);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Face");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_selection_mode_face_toggled");
	action_buttons_container->add_child(action_button);

	// -- Main separator
	main_cont->add_child(memnew(HSeparator));

	// -- Main ScrollContainer
	ScrollContainer *main_scroll_container = memnew(ScrollContainer);
	main_cont->add_child(main_scroll_container);

	//Popups
	Control *popups_node = memnew(Control);
	popups_node->set_mouse_filter(MOUSE_FILTER_IGNORE);
	add_child(popups_node);

	uv_editor = memnew(MDRUVRectEditorPopup);
	uv_editor->set_title("UV Editor");
	uv_editor->set_resizable(true);
	uv_editor->hide();
	popups_node->add_child(uv_editor);
}

MDIEd::~MDIEd() {
}

void MDIEd::_bind_methods() {
	ClassDB::bind_method(D_METHOD("on_edit_mode_translate_toggled"), &MDIEd::on_edit_mode_translate_toggled);
	ClassDB::bind_method(D_METHOD("on_edit_mode_rotate_toggled"), &MDIEd::on_edit_mode_rotate_toggled);
	ClassDB::bind_method(D_METHOD("on_edit_mode_scale_toggled"), &MDIEd::on_edit_mode_scale_toggled);

	ClassDB::bind_method(D_METHOD("on_axis_x_toggled"), &MDIEd::on_axis_x_toggled);
	ClassDB::bind_method(D_METHOD("on_axis_y_toggled"), &MDIEd::on_axis_y_toggled);
	ClassDB::bind_method(D_METHOD("on_axis_z_toggled"), &MDIEd::on_axis_z_toggled);

	ClassDB::bind_method(D_METHOD("on_selection_mode_vertex_toggled"), &MDIEd::on_selection_mode_vertex_toggled);
	ClassDB::bind_method(D_METHOD("on_selection_mode_edge_toggled"), &MDIEd::on_selection_mode_edge_toggled);
	ClassDB::bind_method(D_METHOD("on_selection_mode_face_toggled"), &MDIEd::on_selection_mode_face_toggled);
}
