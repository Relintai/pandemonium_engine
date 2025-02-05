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

#include "editor/editor_node.h"
#include "scene/3d/camera.h"

void TerrainWorldGizmo::set_visible(const bool visible) {
	_visible = visible;
	redraw();
}

void TerrainWorldGizmo::set_editor_plugin(EditorPlugin *editor_plugin) {
	_editor_plugin = editor_plugin;

	_undo_redo = EditorNode::get_undo_redo();
}

void TerrainWorldGizmo::set_handle(int index, bool secondary, Camera *camera, const Point2 &point) {
}

void TerrainWorldGizmo::redraw() {
	clear();

	if (!_visible) {
		return;
	}

	if (!get_plugin().is_valid()) {
		return;
	}
	/*

	Ref<SpatialMaterial> handles_material = get_plugin()->get_material("handles", Ref<EditorSpatialGizmo>(this));
	Ref<SpatialMaterial> material = get_plugin()->get_material("main", Ref<EditorSpatialGizmo>(this));
	Ref<SpatialMaterial> seam_material = get_plugin()->get_material("seam", Ref<EditorSpatialGizmo>(this));

	_mesh_outline_generator->setup(_mdr);

	if (selection_mode == SELECTION_MODE_EDGE) {
		_mesh_outline_generator->generate_mark_edges(visual_indicator_outline, visual_indicator_handle);
	} else if (selection_mode == SELECTION_MODE_FACE) {
		_mesh_outline_generator->generate_mark_faces(visual_indicator_outline, visual_indicator_handle);
	} else {
		_mesh_outline_generator->generate(visual_indicator_outline, visual_indicator_handle);
	}

	if (visual_indicator_outline || visual_indicator_handle) {
		add_lines(_mesh_outline_generator->lines, material, false);
	}

	if (visual_indicator_seam) {
		add_lines(_mesh_outline_generator->seam_lines, seam_material, false);
	}
	*/
}
void TerrainWorldGizmo::apply() {
}

TerrainWorldGizmo::TerrainWorldGizmo() {
	_editor_plugin = nullptr;
	_undo_redo = nullptr;

	_visible = false;
}

TerrainWorldGizmo::~TerrainWorldGizmo() {
}

void TerrainWorldGizmo::_bind_methods() {
}
