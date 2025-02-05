/*************************************************************************/
/*  terrain_world_gizmo.cpp                                              */
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

#include "terrain_world_gizmo.h"

#include "core/math/math_defs.h"
#include "editor/editor_node.h"
#include "scene/3d/camera.h"
#include "scene/main/world.h"
#include "scene/resources/world_3d.h"
#include "servers/physics_server.h"

#include "../world/terrain_world.h"

void TerrainWorldGizmo::set_editor_plugin(EditorPlugin *editor_plugin) {
	_editor_plugin = editor_plugin;

	_undo_redo = EditorNode::get_undo_redo();
}

void TerrainWorldGizmo::set_handle(int index, bool secondary, Camera *camera, const Point2 &point) {
}

void TerrainWorldGizmo::redraw() {
	clear();

	if (!visible) {
		return;
	}

	if (!get_plugin().is_valid()) {
		return;
	}

	if (_lines.size() == 0) {
		return;
	}

	Ref<SpatialMaterial> material;

	if (drawing) {
		material = get_plugin()->get_material("drawing", Ref<EditorSpatialGizmo>(this));
	} else {
		material = get_plugin()->get_material("main", Ref<EditorSpatialGizmo>(this));
	}

	add_lines(_lines, material);
}
void TerrainWorldGizmo::apply() {
}

void TerrainWorldGizmo::refresh_lines(TerrainWorld *world) {
	//Transform local_xform = world->get_global_transform().affine_inverse();

	PhysicsDirectSpaceState *ss = world->get_world_3d()->get_direct_space_state();

	Vector3 from;
	Vector3 to;
	PhysicsDirectSpaceState::RayResult res;

	_lines.clear();
	_points.clear();

	float voxel_scale = world->get_voxel_scale();
	Vector3 vsv = Vector3(voxel_scale, voxel_scale, voxel_scale);

	for (int i = 0; i < 10; ++i) {
		from = Vector3(Math::sin((Math_PI * 2.0 / 10.0) * (float)i) * size, 1000, Math::cos((Math_PI * 2.0 / 10.0) * (float)i) * size) * vsv;
		to = Vector3(from.x, -from.y, from.z);

		from += position;
		to += position;

		//from = local_xform.xform(from);
		//to = local_xform.xform(to);

		Vector3 p;

		if (ss->intersect_ray(from, to, res)) {
			p = res.position + res.normal.limit_length(0.1);
		} else {
			p = Vector3(from.x, 0, from.z);
		}

		_points.push_back(p);
	}

	for (int i = 1; i < _points.size(); ++i) {
		_lines.push_back(_points[i - 1]);
		_lines.push_back(_points[i]);
	}

	_lines.push_back(_points[_points.size() - 1]);
	_lines.push_back(_points[0]);

	redraw();
}

TerrainWorldGizmo::TerrainWorldGizmo() {
	_editor_plugin = nullptr;
	_undo_redo = nullptr;

	visible = false;
	drawing = false;
	size = 10;
}

TerrainWorldGizmo::~TerrainWorldGizmo() {
}

void TerrainWorldGizmo::_bind_methods() {
}
