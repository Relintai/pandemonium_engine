/*************************************************************************/
/*  ess_editor_plugin.cpp                                                */
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

#include "ess_editor_plugin.h"

#include "editor/editor_settings.h"

#include "../world_spawners/ess_entity_world_spawner_3d.h"

#include "scene/resources/mesh/mesh.h"
#include "scene/resources/mesh/primitive_meshes.h"

void ESSEditorPlugin::fix_ids(Variant param) {
}

ESSEditorPlugin::ESSEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	//editor->add_tool_menu_item("ESS: make ids unique", this, "fix_ids");

	gizmo_plugin.instance();
	add_spatial_gizmo_plugin(gizmo_plugin);
}

ESSEditorPlugin::~ESSEditorPlugin() {
}

void ESSEditorPlugin::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("fix_ids"), &ESSEditorPlugin::fix_ids);
}

////////

WorldSpawner3DSpatialGizmoPlugin::WorldSpawner3DSpatialGizmoPlugin() {
	const Color gizmo_color = EDITOR_DEF("editors/props/gizmo_colors/outline", Color(0.5, 0.5, 1));

	create_material("outline_material", gizmo_color);
	create_material("text_material", gizmo_color);
}

bool WorldSpawner3DSpatialGizmoPlugin::has_gizmo(Spatial *p_spatial) {
	return Object::cast_to<ESSEntityWorldSpawner3D>(p_spatial) != NULL;
}

String WorldSpawner3DSpatialGizmoPlugin::get_gizmo_name() const {
	return "ESSEntityWorldSpawner3D";
}

int WorldSpawner3DSpatialGizmoPlugin::get_priority() const {
	return -1;
}

bool WorldSpawner3DSpatialGizmoPlugin::can_be_hidden() const {
	return true;
}

void WorldSpawner3DSpatialGizmoPlugin::redraw(EditorSpatialGizmo *p_gizmo) {
	ESSEntityWorldSpawner3D *ws = Object::cast_to<ESSEntityWorldSpawner3D>(p_gizmo->get_spatial_node());

	p_gizmo->clear();

	AABB aabb;
	aabb.size = Vector3(1, 2, 1);
	aabb.position = Vector3(-0.5, 0, -0.5);

	Ref<ArrayMesh> cm;
	cm.instance();

	//cm->set_size(aabb.size);

	Array mesh_arr;
	mesh_arr.resize(Mesh::ARRAY_MAX);

	// set our bounding box
	PoolVector<Vector3> points;

	points.push_back(aabb.position);
	points.push_back(aabb.position + Vector3(aabb.size.x, 0, 0));
	points.push_back(aabb.position + Vector3(aabb.size.x, 0, 0));
	points.push_back(aabb.position + Vector3(aabb.size.x, 0, aabb.size.z));
	points.push_back(aabb.position);
	points.push_back(aabb.position + Vector3(0, 0, aabb.size.z));
	points.push_back(aabb.position + Vector3(0, 0, aabb.size.z));
	points.push_back(aabb.position + Vector3(aabb.size.x, 0, aabb.size.z));
	points.push_back(aabb.position);
	points.push_back(aabb.position + Vector3(0, aabb.size.y, 0));
	points.push_back(aabb.position + Vector3(aabb.size.x, 0, 0));
	points.push_back(aabb.position + Vector3(aabb.size.x, aabb.size.y, 0));
	points.push_back(aabb.position + Vector3(0, 0, aabb.size.z));
	points.push_back(aabb.position + Vector3(0, aabb.size.y, aabb.size.z));
	points.push_back(aabb.position + Vector3(aabb.size.x, 0, aabb.size.z));
	points.push_back(aabb.position + aabb.size);
	points.push_back(aabb.position + Vector3(0, aabb.size.y, 0));
	points.push_back(aabb.position + Vector3(aabb.size.x, aabb.size.y, 0));
	points.push_back(aabb.position + Vector3(0, aabb.size.y, 0));
	points.push_back(aabb.position + Vector3(0, aabb.size.y, aabb.size.z));
	points.push_back(aabb.position + Vector3(0, aabb.size.y, aabb.size.z));
	points.push_back(aabb.position + aabb.size);
	points.push_back(aabb.position + Vector3(aabb.size.x, aabb.size.y, 0));
	points.push_back(aabb.position + aabb.size);

	mesh_arr[RS::ARRAY_VERTEX] = points;

	cm->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, mesh_arr);

	Ref<TriangleMesh> tm = cm->generate_triangle_mesh_from_aabb();

	if (tm.is_valid()) {
		p_gizmo->add_collision_triangles(tm);
	}

	const Ref<Material> material = get_material("outline_material");

	p_gizmo->add_mesh(cm, material);

	const Ref<Material> text_material = get_material("text_material");

	Ref<TextMesh> text_mesh;
	text_mesh.instance();
	text_mesh->set_depth(0.01);
	text_mesh->set_text(ws->get_name());
	text_mesh->set_font(EditorNode::get_singleton()->get_theme_base()->get_theme_font("main", "EditorFonts"));
	Transform tt;
	tt.set_origin(Vector3(0, 1.6, 0));
	// TODO this should be configurable
	tt.basis.rotate_local(Vector3(0, 1, 0), Math_PI);
	p_gizmo->add_mesh(text_mesh, text_material, tt);
}
