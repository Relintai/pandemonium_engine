/*************************************************************************/
/*  mdi_ed_plugin.cpp                                                    */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "mdi_ed_plugin.h"

#include "../nodes/mesh_data_instance.h"
#include "editor/editor_node.h"
#include "mdi_ed.h"
#include "mdi_gizmo.h"
#include "mdi_gizmo_plugin.h"

bool MDIEdPlugin::handles(Object *object) const {
	if (object->is_class("MeshDataInstance")) {
		return true;
	}

	return false;
}
void MDIEdPlugin::edit(Object *object) {
	MeshDataInstance *mdi = Object::cast_to<MeshDataInstance>(object);

	if (mdi) {
		if (current_mesh_data_instance) {
			Ref<MDIGizmo> g = get_gizmo_from(mdi);
			Ref<MDIGizmo> c = get_gizmo_from(current_mesh_data_instance);

			if (g.is_valid() && c.is_valid()) {
				g->transfer_state_from(c);
			}
		}

		mdi_ed_gui->set_mesh_data_resource(mdi->get_mesh_data());
		mdi_ed_gui->set_mesh_data_instance(mdi);
	}

	current_mesh_data_instance = mdi;
}
void MDIEdPlugin::make_visible(bool visible) {
	mdi_ed_gui->make_visible(visible);
}

String MDIEdPlugin::get_name() const {
	return "MeshDataResourceEditor";
}

void MDIEdPlugin::set_gizmo_visible(const bool visible) {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);

		if (g.is_valid()) {
			g->set_visible(visible);
			current_mesh_data_instance->set_transform_gizmo_visible(!visible);
		}
	}
}

void MDIEdPlugin::set_translate() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		g->set_translate();
	}
}
void MDIEdPlugin::set_scale() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		g->set_scale();
	}
}
void MDIEdPlugin::set_rotate() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		g->set_rotate();
	}
}

void MDIEdPlugin::set_axis_x(bool on) {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		g->set_axis_x(on);
	}
}
void MDIEdPlugin::set_axis_y(bool on) {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		g->set_axis_y(on);
	}
}
void MDIEdPlugin::set_axis_z(bool on) {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		g->set_axis_z(on);
	}
}

void MDIEdPlugin::set_selection_mode_vertex() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		g->set_selection_mode_vertex();
	}
}
void MDIEdPlugin::set_selection_mode_edge() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		g->set_selection_mode_edge();
	}
}
void MDIEdPlugin::set_selection_mode_face() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		g->set_selection_mode_face();
	}
}

Ref<MeshDataResource> MDIEdPlugin::get_mdr() {
	if (current_mesh_data_instance) {
		return current_mesh_data_instance->get_mesh_data();
	}

	return Ref<MeshDataResource>();
}

EditorPlugin::AfterGUIInput MDIEdPlugin::forward_spatial_gui_input(Camera *camera, const Ref<InputEvent> &p_event) {
	if (!ObjectDB::instance_validate(current_mesh_data_instance)) {
		current_mesh_data_instance = nullptr;
	}

	if (current_mesh_data_instance) {
		if (mdi_ed_gui->is_editing()) {
			Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);

			if (g.is_valid()) {
				return g->forward_spatial_gui_input(camera, p_event);
			}

			return EditorPlugin::AFTER_GUI_INPUT_NO_DESELECT;
		}
	}

	return EditorPlugin::AFTER_GUI_INPUT_PASS;
}

void MDIEdPlugin::add_box() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->add_box();
	}
}
void MDIEdPlugin::add_triangle() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->add_triangle();
	}
}
void MDIEdPlugin::add_quad() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->add_quad();
	}
}

void MDIEdPlugin::add_triangle_at() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->add_triangle_at();
	}
}
void MDIEdPlugin::add_quad_at() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->add_quad_at();
	}
}

void MDIEdPlugin::split() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->split();
	}
}

void MDIEdPlugin::disconnect_action() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->disconnect_action();
	}
}

void MDIEdPlugin::create_face() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->create_face();
	}
}
void MDIEdPlugin::delete_selected() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->delete_selected();
	}
}

void MDIEdPlugin::generate_normals() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->generate_normals();
	}
}
void MDIEdPlugin::remove_doubles() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->remove_doubles();
	}
}
void MDIEdPlugin::merge_optimize() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->merge_optimize();
	}
}
void MDIEdPlugin::generate_tangents() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->generate_tangents();
	}
}

void MDIEdPlugin::connect_to_first_selected() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->connect_to_first_selected();
	}
}
void MDIEdPlugin::connect_to_avg() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->connect_to_avg();
	}
}
void MDIEdPlugin::connect_to_last_selected() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->connect_to_last_selected();
	}
}

void MDIEdPlugin::mark_seam() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->mark_seam();
	}
}
void MDIEdPlugin::unmark_seam() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->unmark_seam();
	}
}
void MDIEdPlugin::apply_seam() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->apply_seam();
	}
}

void MDIEdPlugin::uv_unwrap() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->uv_unwrap();
	}
}

void MDIEdPlugin::set_pivot_averaged() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->set_pivot_averaged();
	}
}
void MDIEdPlugin::set_pivot_mdi_origin() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->set_translate();
	}
}
void MDIEdPlugin::set_pivot_world_origin() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->set_pivot_world_origin();
	}
}

void MDIEdPlugin::visual_indicator_outline_set(bool on) {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->visual_indicator_outline_set(on);
	}
}
void MDIEdPlugin::visual_indicator_seam_set(bool on) {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->visual_indicator_seam_set(on);
	}
}
void MDIEdPlugin::visual_indicator_handle_set(bool on) {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->visual_indicator_handle_set(on);
	}
}

void MDIEdPlugin::select_all() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->select_all();
	}
}

void MDIEdPlugin::handle_selection_type_front() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->handle_selection_type_front();
	}
}
void MDIEdPlugin::handle_selection_type_back() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->handle_selection_type_back();
	}
}
void MDIEdPlugin::handle_selection_type_all() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->handle_selection_type_all();
	}
}

void MDIEdPlugin::extrude() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->extrude();
	}
}
void MDIEdPlugin::clean_mesh() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->clean_mesh();
	}
}

void MDIEdPlugin::flip_selected_faces() {
	if (current_mesh_data_instance) {
		Ref<MDIGizmo> g = get_gizmo_from(current_mesh_data_instance);
		if (!g.is_valid()) {
			return;
		}
		g->flip_selected_faces();
	}
}

Ref<MDIGizmo> MDIEdPlugin::get_gizmo_from(MeshDataInstance *mdi) {
	if (!mdi) {
		return Ref<MDIGizmo>();
	}

	Vector<Ref<SpatialGizmo>> gizmos = mdi->get_gizmos();

	for (int i = 0; i < gizmos.size(); ++i) {
		Ref<SpatialGizmo> g = gizmos[i];

		if (g.is_valid()) {
			return g;
		}
	}

	return Ref<MDIGizmo>();
}

MDIEdPlugin::MDIEdPlugin(EditorNode *p_node) {
	editor = p_node;

	current_mesh_data_instance = nullptr;

	mdi_ed_gui = memnew(MDIEd);
	mdi_ed_gui->set_plugin(this);
	add_control_to_container(EditorPlugin::CONTAINER_SPATIAL_EDITOR_SIDE_RIGHT, mdi_ed_gui);
	mdi_ed_gui->hide();

	gizmo_plugin.instance();
	gizmo_plugin->plugin = this;
	add_spatial_gizmo_plugin(gizmo_plugin);

	editor->get_scene_tree_dock()->add_custom_scene_root_class("Mesh", "MeshDataInstance");
}

MDIEdPlugin::~MDIEdPlugin() {
}

void MDIEdPlugin::_bind_methods() {
}
