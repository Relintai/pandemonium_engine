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

#include "../mesh_data_resource.h"
#include "./uv_editor/mdr_ed_uv_previewer.h"
#include "./uv_editor/mdr_uv_rect_editor.h"
#include "./uv_editor/mdr_uv_rect_editor_popup.h"
#include "core/os/keyboard.h"
#include "mdi_ed_plugin.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"

void MDIEd::set_plugin(MDIEdPlugin *plugin) {
	_plugin = plugin;
	uv_editor->set_plugin(plugin);
}
void MDIEd::set_mesh_data_resource(Ref<MeshDataResource> a) {
	uv_preview->set_mesh_data_resource(a);
	uv_editor->get_editor()->set_mesh_data_resource(a);
}
void MDIEd::set_mesh_data_instance(MeshDataInstance *a) {
	uv_preview->set_mesh_data_instance(a);
	uv_editor->get_editor()->set_mesh_data_instance(a);
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
	_edit_mode_translate_button->set_pressed(true);
}
void MDIEd::set_edit_mode_rotate() {
	_edit_mode_rotate_button->set_pressed(true);
}
void MDIEd::set_edit_mode_scale() {
	_edit_mode_scale_button->set_pressed(true);
}

void MDIEd::on_edit_mode_translate_toggled(bool on) {
	if (on) {
		_plugin->set_translate();
	}
}
void MDIEd::on_edit_mode_rotate_toggled(bool on) {
	if (on) {
		_plugin->set_rotate();
	}
}
void MDIEd::on_edit_mode_scale_toggled(bool on) {
	if (on) {
		_plugin->set_scale();
	}
}

//axis locks
bool MDIEd::get_axis_x() {
	return _axis_lock_x_button->is_pressed();
}
bool MDIEd::get_axis_y() {
	return _axis_lock_y_button->is_pressed();
}
bool MDIEd::get_axis_z() {
	return _axis_lock_z_button->is_pressed();
}
void MDIEd::set_axis_x(bool on) {
	_axis_lock_x_button->set_pressed(on);
}
void MDIEd::set_axis_y(bool on) {
	_axis_lock_y_button->set_pressed(on);
}
void MDIEd::set_axis_z(bool on) {
	_axis_lock_z_button->set_pressed(on);
}

void MDIEd::on_axis_x_toggled(bool on) {
	_plugin->set_axis_x(on);
}
void MDIEd::on_axis_y_toggled(bool on) {
	_plugin->set_axis_y(on);
}
void MDIEd::on_axis_z_toggled(bool on) {
	_plugin->set_axis_z(on);
}

//selection modes
void MDIEd::on_selection_mode_vertex_toggled(bool on) {
	if (on) {
		_plugin->set_selection_mode_vertex();

		vertex_operations_container->show();
		edge_operations_container->hide();
		face_operations_container->hide();
	}
}
void MDIEd::on_selection_mode_edge_toggled(bool on) {
	if (on) {
		_plugin->set_selection_mode_edge();

		vertex_operations_container->hide();
		edge_operations_container->show();
		face_operations_container->hide();
	}
}
void MDIEd::on_selection_mode_face_toggled(bool on) {
	if (on) {
		_plugin->set_selection_mode_face();

		vertex_operations_container->hide();
		edge_operations_container->hide();
		face_operations_container->show();
	}
}

void MDIEd::set_selection_mode_vertex() {
	_selection_mode_vertex_button->set_pressed(true);
}
void MDIEd::set_selection_mode_edge() {
	_selection_mode_edge_button->set_pressed(true);
}
void MDIEd::set_selection_mode_face() {
	_selection_mode_face_button->set_pressed(true);
}

void MDIEd::on_handle_selection_type_front_toggled(bool on) {
	if (on) {
		_plugin->handle_selection_type_front();
	}
}
void MDIEd::on_handle_selection_type_back_toggled(bool on) {
	if (on) {
		_plugin->handle_selection_type_back();
	}
}
void MDIEd::on_handle_selection_type_all_toggled(bool on) {
	if (on) {
		_plugin->handle_selection_type_all();
	}
}

void MDIEd::on_select_all_pressed() {
	_plugin->select_all();
}

void MDIEd::on_create_face_pressed() {
	_plugin->create_face();
}
void MDIEd::on_split_pressed() {
	_plugin->split();
}
void MDIEd::on_connect_to_first_selected_pressed() {
	_plugin->connect_to_first_selected();
}
void MDIEd::on_connect_to_avg_pressed() {
	_plugin->connect_to_avg();
}
void MDIEd::on_connect_to_last_selected_pressed() {
	_plugin->connect_to_last_selected();
}

//Edge ops
void MDIEd::on_extrude_pressed() {
	_plugin->extrude();
}
void MDIEd::on_add_triangle_at_pressed() {
	_plugin->add_triangle_at();
}
void MDIEd::on_add_quad_at_pressed() {
	_plugin->add_quad_at();
}
void MDIEd::on_mark_seam_pressed() {
	_plugin->mark_seam();
}
void MDIEd::on_unmark_seam_pressed() {
	_plugin->unmark_seam();
}

//Face ops
void MDIEd::on_delete_pressed() {
	_plugin->delete_selected();
}
void MDIEd::on_flip_face_pressed() {
	_plugin->flip_selected_faces();
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

void MDIEd::on_generate_normals_pressed() {
	_plugin->generate_normals();
}
void MDIEd::on_generate_tangents_pressed() {
	_plugin->generate_tangents();
}

void MDIEd::on_remove_doubles_pressed() {
	_plugin->remove_doubles();
}
void MDIEd::on_merge_optimize_pressed() {
	_plugin->merge_optimize();
}
void MDIEd::on_clean_mesh_pressed() {
	_plugin->clean_mesh();
}
void MDIEd::on_uv_edit_pressed() {
	uv_editor->popup_centered();
}
void MDIEd::on_apply_seams_pressed() {
	_plugin->apply_seam();
}
void MDIEd::on_unwrap_button_pressed() {
	_plugin->uv_unwrap();
}

void MDIEd::on_add_box_pressed() {
	_plugin->add_box();
}
void MDIEd::on_add_triangle_pressed() {
	_plugin->add_triangle();
}
void MDIEd::on_add_quad_pressed() {
	_plugin->add_quad();
}

void MDIEd::_on_disconnect_pressed() {
	_plugin->disconnect_action();
}

MDIEd::MDIEd() {
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
	_edit_mode_translate_button = action_button;

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
	_edit_mode_rotate_button = action_button;

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
	_edit_mode_scale_button = action_button;

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
	_axis_lock_x_button = action_button;

	action_button = memnew(Button);
	action_button->set_text("Y");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_axis_y_toggled");
	action_buttons_container->add_child(action_button);
	_axis_lock_y_button = action_button;

	action_button = memnew(Button);
	action_button->set_text("Z");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_axis_z_toggled");
	action_buttons_container->add_child(action_button);
	_axis_lock_z_button = action_button;

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
	_selection_mode_vertex_button = action_button;

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
	_selection_mode_edge_button = action_button;

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
	_selection_mode_face_button = action_button;

	// -- Main separator
	main_cont->add_child(memnew(HSeparator));

	// -- Main ScrollContainer
	ScrollContainer *main_scroll_container = memnew(ScrollContainer);
	main_scroll_container->set_v_size_flags(SIZE_EXPAND_FILL);
	main_scroll_container->set_h_size_flags(SIZE_EXPAND_FILL);
	main_scroll_container->set_enable_h_scroll(false);
	main_cont->add_child(main_scroll_container);

	VBoxContainer *scroll_container_content = memnew(VBoxContainer);
	scroll_container_content->set_h_size_flags(SIZE_EXPAND_FILL);
	main_scroll_container->add_child(scroll_container_content);

	// -- Scroll -- Handle Selection type
	action_buttons_container = memnew(HBoxContainer);
	scroll_container_content->add_child(action_buttons_container);

	action_label = memnew(Label);
	action_label->set_text("H");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	action_label->set_tooltip("Handle Selection Type");
	action_label->set_mouse_filter(Control::MOUSE_FILTER_STOP);
	action_label->set_v_size_flags(SIZE_EXPAND_FILL | SIZE_SHRINK_CENTER);
	action_buttons_container->add_child(action_label);

	action_button_group.instance();

	action_button = memnew(Button);
	action_button->set_text("F");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Front");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_handle_selection_type_front_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("B");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(false);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Back");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_handle_selection_type_back_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("A");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(false);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("All");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_handle_selection_type_all_toggled");
	action_buttons_container->add_child(action_button);

	// -- Scroll -- Pivot
	action_buttons_container = memnew(HBoxContainer);
	scroll_container_content->add_child(action_buttons_container);

	action_label = memnew(Label);
	action_label->set_text("P");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	action_label->set_tooltip("Pivot");
	action_label->set_mouse_filter(Control::MOUSE_FILTER_STOP);
	action_label->set_v_size_flags(SIZE_EXPAND_FILL | SIZE_SHRINK_CENTER);
	action_buttons_container->add_child(action_label);

	action_button_group.instance();

	action_button = memnew(Button);
	action_button->set_text("A");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Average");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_pivot_average_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("M");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(false);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Mesh Data Instance Origin");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_pivot_mdi_origin_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("W");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(false);
	action_button->set_button_group(action_button_group);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("World Origin");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_pivot_world_origin_toggled");
	action_buttons_container->add_child(action_button);

	// -- Scroll -- Visual indicators
	action_buttons_container = memnew(HBoxContainer);
	scroll_container_content->add_child(action_buttons_container);

	action_label = memnew(Label);
	action_label->set_text("V");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	action_label->set_tooltip("Visual indicators");
	action_label->set_mouse_filter(Control::MOUSE_FILTER_STOP);
	action_label->set_v_size_flags(SIZE_EXPAND_FILL | SIZE_SHRINK_CENTER);
	action_buttons_container->add_child(action_label);

	action_button = memnew(Button);
	action_button->set_text("O");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Outline");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_handle_selection_type_front_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("S");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Seam");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_handle_selection_type_back_toggled");
	action_buttons_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("H");
	action_button->set_toggle_mode(true);
	action_button->set_pressed(true);
	action_button->set_custom_minimum_size(Size2(25, 20));
	action_button->set_tooltip("Handle");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("toggled", this, "on_handle_selection_type_all_toggled");
	action_buttons_container->add_child(action_button);

	// -- Scroll -- Separator
	scroll_container_content->add_child(memnew(HSeparator));

	// -- Scroll -- Select All Button
	action_button = memnew(Button);
	action_button->set_text("Select All");
	action_button->connect("pressed", this, "on_select_all_pressed");
	scroll_container_content->add_child(action_button);

	// -- Scroll -- Separator
	scroll_container_content->add_child(memnew(HSeparator));

	// -- Scroll -- Vertex Operations
	vertex_operations_container = memnew(VBoxContainer);
	scroll_container_content->add_child(vertex_operations_container);

	action_label = memnew(Label);
	action_label->set_text("= Vertex =");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	vertex_operations_container->add_child(action_label);

	action_button = memnew(Button);
	action_button->set_text("Create Face");
	action_button->connect("pressed", this, "on_create_face_pressed");
	vertex_operations_container->add_child(action_button);

	action_label = memnew(Label);
	action_label->set_text("Connect");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	vertex_operations_container->add_child(action_label);

	HBoxContainer *vertex_connect_operations_container = memnew(HBoxContainer);
	vertex_operations_container->add_child(vertex_connect_operations_container);

	action_button = memnew(Button);
	action_button->set_text("x<");
	action_button->set_tooltip("Move all vertices to the first one that was selected.");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("pressed", this, "on_connect_to_first_selected_pressed");
	vertex_connect_operations_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text(">x<");
	action_button->set_tooltip("Move all selected vertices to their average.");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("pressed", this, "on_connect_to_avg_pressed");
	vertex_connect_operations_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text(">x");
	action_button->set_tooltip("Move all selected vertices to the last that was selected.");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("pressed", this, "on_connect_to_last_selected_pressed");
	vertex_connect_operations_container->add_child(action_button);

	// -- Scroll -- Edge Operations
	edge_operations_container = memnew(VBoxContainer);
	edge_operations_container->hide();
	scroll_container_content->add_child(edge_operations_container);

	action_label = memnew(Label);
	action_label->set_text("= Edge =");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	edge_operations_container->add_child(action_label);

	action_button = memnew(Button);
	action_button->set_text("Extrude");
	action_button->connect("pressed", this, "on_extrude_pressed");
	edge_operations_container->add_child(action_button);

	action_label = memnew(Label);
	action_label->set_text("Append");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	edge_operations_container->add_child(action_label);

	HBoxContainer *edge_operations_sub_container = memnew(HBoxContainer);
	edge_operations_container->add_child(edge_operations_sub_container);

	action_button = memnew(Button);
	action_button->set_text("Tri");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("pressed", this, "on_add_triangle_at_pressed");
	edge_operations_sub_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Quad");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("pressed", this, "on_add_quad_at_pressed");
	edge_operations_sub_container->add_child(action_button);

	action_label = memnew(Label);
	action_label->set_text("Seam");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	edge_operations_container->add_child(action_label);

	edge_operations_sub_container = memnew(HBoxContainer);
	edge_operations_container->add_child(edge_operations_sub_container);

	action_button = memnew(Button);
	action_button->set_text("Mark");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("pressed", this, "on_mark_seam_pressed");
	edge_operations_sub_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Unmark");
	action_button->set_h_size_flags(SIZE_EXPAND_FILL);
	action_button->connect("pressed", this, "on_unmark_seam_pressed");
	edge_operations_sub_container->add_child(action_button);

	// -- Scroll -- Face Operations
	face_operations_container = memnew(VBoxContainer);
	face_operations_container->hide();
	scroll_container_content->add_child(face_operations_container);

	action_label = memnew(Label);
	action_label->set_text("= Face =");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	face_operations_container->add_child(action_label);

	action_button = memnew(Button);
	action_button->set_text("Delete");
	action_button->connect("pressed", this, "on_delete_pressed");
	face_operations_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Flip");
	action_button->connect("pressed", this, "on_flip_face_pressed");
	face_operations_container->add_child(action_button);

	// -- Scroll -- Separator
	scroll_container_content->add_child(memnew(HSeparator));

	// -- Scroll -- Operations
	VBoxContainer *operations_container = memnew(VBoxContainer);
	scroll_container_content->add_child(operations_container);

	action_label = memnew(Label);
	action_label->set_text("= Operations =");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	operations_container->add_child(action_label);

	action_label = memnew(Label);
	action_label->set_text("Generate");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	operations_container->add_child(action_label);

	action_button = memnew(Button);
	action_button->set_text("Normals");
	action_button->connect("pressed", this, "on_generate_normals_pressed");
	operations_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Tangents");
	action_button->connect("pressed", this, "on_remove_doubles_pressed");
	operations_container->add_child(action_button);

	action_label = memnew(Label);
	action_label->set_text("Optimizations");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	operations_container->add_child(action_label);

	action_button = memnew(Button);
	action_button->set_text("Rem Doubles");
	action_button->connect("pressed", this, "on_remove_doubles_pressed");
	operations_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Full Merge");
	action_button->connect("pressed", this, "on_merge_optimize_pressed");
	operations_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Clean");
	action_button->set_tooltip("Clean mesh for example for unused verts.");
	action_button->connect("pressed", this, "on_clean_mesh_pressed");
	operations_container->add_child(action_button);

	action_label = memnew(Label);
	action_label->set_text("UV");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	operations_container->add_child(action_label);

	uv_preview = memnew(MDREdUVPreviewer);
	uv_preview->set_custom_minimum_size(Point2(100, 100));
	operations_container->add_child(uv_preview);

	action_button = memnew(Button);
	action_button->set_text("Edit");
	action_button->connect("pressed", this, "on_uv_edit_pressed");
	operations_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Apply seams");
	action_button->connect("pressed", this, "on_apply_seams_pressed");
	operations_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Unwrap");
	action_button->connect("pressed", this, "on_unwrap_button_pressed");
	operations_container->add_child(action_button);

	// -- Scroll -- Separator
	scroll_container_content->add_child(memnew(HSeparator));

	// -- Scroll -- Add
	VBoxContainer *add_operations_container = memnew(VBoxContainer);
	scroll_container_content->add_child(add_operations_container);

	action_label = memnew(Label);
	action_label->set_text("= Add =");
	action_label->set_align(Label::ALIGN_CENTER);
	action_label->set_valign(Label::VALIGN_CENTER);
	add_operations_container->add_child(action_label);

	action_button = memnew(Button);
	action_button->set_text("Box");
	action_button->connect("pressed", this, "on_add_box_pressed");
	add_operations_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Tri");
	action_button->connect("pressed", this, "on_add_triangle_pressed");
	add_operations_container->add_child(action_button);

	action_button = memnew(Button);
	action_button->set_text("Quad");
	action_button->connect("pressed", this, "on_add_quad_pressed");
	add_operations_container->add_child(action_button);

	// -- Scroll -- Separator
	scroll_container_content->add_child(memnew(HSeparator));

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

	ClassDB::bind_method(D_METHOD("on_handle_selection_type_front_toggled"), &MDIEd::on_handle_selection_type_front_toggled);
	ClassDB::bind_method(D_METHOD("on_handle_selection_type_back_toggled"), &MDIEd::on_handle_selection_type_back_toggled);
	ClassDB::bind_method(D_METHOD("on_handle_selection_type_all_toggled"), &MDIEd::on_handle_selection_type_all_toggled);

	ClassDB::bind_method(D_METHOD("on_pivot_average_toggled"), &MDIEd::on_pivot_average_toggled);
	ClassDB::bind_method(D_METHOD("on_pivot_mdi_origin_toggled"), &MDIEd::on_pivot_mdi_origin_toggled);
	ClassDB::bind_method(D_METHOD("on_pivot_world_origin_toggled"), &MDIEd::on_pivot_world_origin_toggled);

	ClassDB::bind_method(D_METHOD("on_visual_indicator_outline_toggled"), &MDIEd::on_visual_indicator_outline_toggled);
	ClassDB::bind_method(D_METHOD("on_visual_indicator_seam_toggled"), &MDIEd::on_visual_indicator_seam_toggled);
	ClassDB::bind_method(D_METHOD("on_visual_indicator_handle_toggled"), &MDIEd::on_visual_indicator_handle_toggled);

	ClassDB::bind_method(D_METHOD("on_select_all_pressed"), &MDIEd::on_select_all_pressed);

	//Vertex
	ClassDB::bind_method(D_METHOD("on_create_face_pressed"), &MDIEd::on_create_face_pressed);
	//ClassDB::bind_method(D_METHOD("on_split_pressed"), &MDIEd::on_split_pressed);
	ClassDB::bind_method(D_METHOD("on_connect_to_first_selected_pressed"), &MDIEd::on_connect_to_first_selected_pressed);
	ClassDB::bind_method(D_METHOD("on_connect_to_avg_pressed"), &MDIEd::on_connect_to_avg_pressed);
	ClassDB::bind_method(D_METHOD("on_connect_to_last_selected_pressed"), &MDIEd::on_connect_to_last_selected_pressed);

	//Edge
	ClassDB::bind_method(D_METHOD("on_extrude_pressed"), &MDIEd::on_extrude_pressed);
	ClassDB::bind_method(D_METHOD("on_add_triangle_at_pressed"), &MDIEd::on_add_triangle_at_pressed);
	ClassDB::bind_method(D_METHOD("on_add_quad_at_pressed"), &MDIEd::on_add_quad_at_pressed);
	ClassDB::bind_method(D_METHOD("on_mark_seam_pressed"), &MDIEd::on_mark_seam_pressed);
	ClassDB::bind_method(D_METHOD("on_unmark_seam_pressed"), &MDIEd::on_unmark_seam_pressed);

	//Face
	ClassDB::bind_method(D_METHOD("on_delete_pressed"), &MDIEd::on_delete_pressed);
	ClassDB::bind_method(D_METHOD("on_flip_face_pressed"), &MDIEd::on_flip_face_pressed);

	//Operations
	ClassDB::bind_method(D_METHOD("on_generate_normals_pressed"), &MDIEd::on_generate_normals_pressed);
	ClassDB::bind_method(D_METHOD("on_generate_tangents_pressed"), &MDIEd::on_generate_tangents_pressed);

	ClassDB::bind_method(D_METHOD("on_remove_doubles_pressed"), &MDIEd::on_remove_doubles_pressed);
	ClassDB::bind_method(D_METHOD("on_merge_optimize_pressed"), &MDIEd::on_merge_optimize_pressed);
	ClassDB::bind_method(D_METHOD("on_clean_mesh_pressed"), &MDIEd::on_clean_mesh_pressed);

	ClassDB::bind_method(D_METHOD("on_uv_edit_pressed"), &MDIEd::on_uv_edit_pressed);
	ClassDB::bind_method(D_METHOD("on_apply_seams_pressed"), &MDIEd::on_apply_seams_pressed);
	ClassDB::bind_method(D_METHOD("on_unwrap_button_pressed"), &MDIEd::on_unwrap_button_pressed);

	ClassDB::bind_method(D_METHOD("on_add_box_pressed"), &MDIEd::on_add_box_pressed);
	ClassDB::bind_method(D_METHOD("on_add_triangle_pressed"), &MDIEd::on_add_triangle_pressed);
	ClassDB::bind_method(D_METHOD("on_add_quad_pressed"), &MDIEd::on_add_quad_pressed);
}
