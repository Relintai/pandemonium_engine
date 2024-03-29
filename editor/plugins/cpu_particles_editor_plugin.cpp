/*************************************************************************/
/*  cpu_particles_editor_plugin.cpp                                      */
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

#include "cpu_particles_editor_plugin.h"

#include "core/containers/pool_vector.h"
#include "core/math/vector3.h"
#include "core/object/class_db.h"
#include "core/os/memory.h"
#include "editor/editor_file_dialog.h"
#include "editor/editor_node.h"
#include "editor/plugins/spatial_editor_plugin.h"
#include "editor/scene_tree_editor.h"
#include "scene/3d/cpu_particles.h"
#include "scene/gui/box_container.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/popup_menu.h"
#include "scene/main/control.h"
#include "scene/main/node.h"

void CPUParticlesEditor::_node_removed(Node *p_node) {
	if (p_node == node) {
		node = nullptr;
		hide();
	}
}

void CPUParticlesEditor::_notification(int p_notification) {
	if (p_notification == NOTIFICATION_ENTER_TREE) {
		options->set_icon(options->get_popup()->get_theme_icon("CPUParticles", "EditorIcons"));
	}
}

void CPUParticlesEditor::_menu_option(int p_option) {
	switch (p_option) {
		case MENU_OPTION_CREATE_EMISSION_VOLUME_FROM_MESH: {
			emission_file_dialog->popup_centered_ratio();

		} break;

		case MENU_OPTION_CREATE_EMISSION_VOLUME_FROM_NODE: {
			emission_tree_dialog->popup_centered_ratio();

		} break;

		case MENU_OPTION_RESTART: {
			node->restart();

		} break;
	}
}

void CPUParticlesEditor::edit(CPUParticles *p_particles) {
	base_node = p_particles;
	node = p_particles;
}

void CPUParticlesEditor::_generate_emission_points() {
	/// hacer codigo aca
	PoolVector<Vector3> points;
	PoolVector<Vector3> normals;

	if (!_generate(points, normals)) {
		return;
	}

	if (normals.size() == 0) {
		node->set_emission_shape(CPUParticles::EMISSION_SHAPE_POINTS);
		node->set_emission_points(points);
	} else {
		node->set_emission_shape(CPUParticles::EMISSION_SHAPE_DIRECTED_POINTS);
		node->set_emission_points(points);
		node->set_emission_normals(normals);
	}
}

void CPUParticlesEditor::_bind_methods() {
	ClassDB::bind_method("_menu_option", &CPUParticlesEditor::_menu_option);
}

CPUParticlesEditor::CPUParticlesEditor() {
	particles_editor_hb = memnew(HBoxContainer);
	SpatialEditor::get_singleton()->add_control_to_menu_panel(particles_editor_hb);
	options = memnew(MenuButton);
	options->set_switch_on_hover(true);
	particles_editor_hb->add_child(options);
	particles_editor_hb->hide();

	options->set_text(TTR("CPUParticles"));
	options->get_popup()->add_item(TTR("Create Emission Points From Mesh"), MENU_OPTION_CREATE_EMISSION_VOLUME_FROM_MESH);
	options->get_popup()->add_item(TTR("Create Emission Points From Node"), MENU_OPTION_CREATE_EMISSION_VOLUME_FROM_NODE);
	options->get_popup()->add_separator();
	options->get_popup()->add_item(TTR("Restart"), MENU_OPTION_RESTART);
	options->get_popup()->connect("id_pressed", this, "_menu_option");
}

void CPUParticlesEditorPlugin::edit(Object *p_object) {
	particles_editor->edit(Object::cast_to<CPUParticles>(p_object));
}

bool CPUParticlesEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("CPUParticles");
}

void CPUParticlesEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		particles_editor->show();
		particles_editor->particles_editor_hb->show();
	} else {
		particles_editor->particles_editor_hb->hide();
		particles_editor->hide();
		particles_editor->edit(nullptr);
	}
}

CPUParticlesEditorPlugin::CPUParticlesEditorPlugin(EditorNode *p_node) {
	editor = p_node;
	particles_editor = memnew(CPUParticlesEditor);
	editor->get_viewport()->add_child(particles_editor);

	particles_editor->hide();
}

CPUParticlesEditorPlugin::~CPUParticlesEditorPlugin() {
}
