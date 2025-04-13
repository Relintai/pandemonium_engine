/*************************************************************************/
/*  prop_editor_plugin.cpp                                               */
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

#include "prop_editor_plugin.h"

#include "../props/prop_data.h"
#include "../singleton/prop_utils.h"
#include "core/os/keyboard.h"

#include "core/input/input.h"

#include "editor/editor_settings.h"
#include "scene/gui/box_container.h"
#include "scene/gui/separator.h"

#include "../tiled_wall/tiled_wall.h"

#include "../prop_instance.h"

#include "scene/resources/mesh/mesh.h"

void PropEditorPlugin::convert_active_scene_to_prop_data() {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		Node *scene = st->get_edited_scene_root();

		if (scene) {
			EditorData &ed = EditorNode::get_editor_data();
			String path = ed.get_scene_path(ed.get_edited_scene());

			convert_scene(scene, path.get_basename() + ".tres");
		}
	}
}
void PropEditorPlugin::convert_selected_scene_to_prop_data() {
}

void PropEditorPlugin::convert_scene(Node *root, const String &path) {
	Ref<PropData> data = PropUtils::get_singleton()->convert_tree(root);

	ERR_FAIL_COND(!data.is_valid());

	ResourceLoader l;
	if (l.exists(path)) {
		Ref<PropData> res = l.load(path, "PropData");

		ERR_FAIL_COND(!res.is_valid());

		res->copy_from(data);

		ResourceSaver s;
		s.save(path, res);

		res.unref();
	} else {
		ResourceSaver s;
		s.save(path, data);
	}
}

void PropEditorPlugin::find_room_points(Variant param) {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		Node *scene = st->get_edited_scene_root();

		if (scene) {
			PropUtils::get_singleton()->generate_room_points_node(scene);
		}
	}
}

void PropEditorPlugin::_quick_convert_button_pressed() {
	convert_active_scene_to_prop_data();
}

void PropEditorPlugin::_convert_active_scene_to_prop_data(Variant param) {
	convert_active_scene_to_prop_data();
}
void PropEditorPlugin::_convert_selected_scene_to_prop_data(Variant param) {
	convert_selected_scene_to_prop_data();
}

PropEditorPlugin::PropEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	editor->add_tool_menu_item("Convert active scene to PropData", this, "convert_active_scene_to_prop_data");
	editor->add_tool_menu_item("Convert selected scene(s) to PropData", this, "convert_selected_scene_to_prop_data");
	editor->add_tool_menu_item("(Prop) Find room points.", this, "find_room_points");

	HBoxContainer *bc = memnew(HBoxContainer);
	bc->add_child(memnew(VSeparator));

	Button *b = memnew(Button);
	bc->add_child(b);
	b->set_flat(true);

	b->connect("pressed", this, "_quick_convert_button_pressed");
	b->set_icon(editor->get_scene_tree_dock()->get_theme_icon("AssetLib", "EditorIcons"));
	//b->set_text("To Prop");
	b->set_tooltip(TTR("Quick convert this scene to a Prop Resource."));
	b->set_shortcut(ED_SHORTCUT("spatial_editor/quick_prop_convert", "Quick convert scene to Prop.", KEY_MASK_ALT + KEY_U));

	add_control_to_container(EditorPlugin::CONTAINER_SPATIAL_EDITOR_MENU, bc);

	prop_instance_gizmo_plugin.instance();
	add_spatial_gizmo_plugin(prop_instance_gizmo_plugin);

	tiled_wall_gizmo_plugin.instance();
	add_spatial_gizmo_plugin(tiled_wall_gizmo_plugin);
}

PropEditorPlugin::~PropEditorPlugin() {
}

void PropEditorPlugin::_bind_methods() {
	ClassDB::bind_method(D_METHOD("convert_active_scene_to_prop_data"), &PropEditorPlugin::_convert_active_scene_to_prop_data);
	ClassDB::bind_method(D_METHOD("convert_selected_scene_to_prop_data"), &PropEditorPlugin::_convert_active_scene_to_prop_data);

	ClassDB::bind_method(D_METHOD("find_room_points"), &PropEditorPlugin::find_room_points);

	ClassDB::bind_method(D_METHOD("_quick_convert_button_pressed"), &PropEditorPlugin::_quick_convert_button_pressed);
}

////////

PropInstanceSpatialGizmoPlugin::PropInstanceSpatialGizmoPlugin() {
	const Color gizmo_color = EDITOR_DEF("editors/props/gizmo_colors/outline", Color(0.5, 0.5, 1));
	create_material("outline_material", gizmo_color);
}

bool PropInstanceSpatialGizmoPlugin::has_gizmo(Spatial *p_spatial) {
	return Object::cast_to<PropInstance>(p_spatial) != NULL;
}

String PropInstanceSpatialGizmoPlugin::get_gizmo_name() const {
	return "PropInstance";
}

int PropInstanceSpatialGizmoPlugin::get_priority() const {
	return -1;
}

bool PropInstanceSpatialGizmoPlugin::can_be_hidden() const {
	return true;
}

void PropInstanceSpatialGizmoPlugin::redraw(EditorSpatialGizmo *p_gizmo) {
	PropInstance *prop = Object::cast_to<PropInstance>(p_gizmo->get_spatial_node());

	p_gizmo->clear();

	AABB aabb = prop->get_aabb();

	if (aabb.has_no_volume()) {
		return; //none
	}

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
}

////////

TiledWallSpatialGizmoPlugin::TiledWallSpatialGizmoPlugin() {
	const Color gizmo_color = EDITOR_DEF("editors/props/gizmo_colors/tiled_wall_outline", Color(0.7, 0.5, 1));
	create_material("tiled_wall_outline_material", gizmo_color);
}

bool TiledWallSpatialGizmoPlugin::has_gizmo(Spatial *p_spatial) {
	return Object::cast_to<TiledWall>(p_spatial) != NULL;
}

String TiledWallSpatialGizmoPlugin::get_gizmo_name() const {
	return "TiledWall";
}

int TiledWallSpatialGizmoPlugin::get_priority() const {
	return -1;
}

bool TiledWallSpatialGizmoPlugin::can_be_hidden() const {
	return true;
}

void TiledWallSpatialGizmoPlugin::redraw(EditorSpatialGizmo *p_gizmo) {
	TiledWall *tw = Object::cast_to<TiledWall>(p_gizmo->get_spatial_node());

	p_gizmo->clear();

	AABB aabb = tw->get_aabb();

	if (aabb.has_no_surface()) {
		return; //none
	}

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

	const Ref<Material> material = get_material("tiled_wall_outline_material");

	p_gizmo->add_mesh(cm, material);
}
