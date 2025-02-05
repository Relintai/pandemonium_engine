/*************************************************************************/
/*  terrain_world_gizmo_plugin.cpp                                       */
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

#include "terrain_world_gizmo_plugin.h"

#include "../world/terrain_world.h"
#include "terrain_world_editor.h"
#include "terrain_world_gizmo.h"

String TerrainWorldGizmoPlugin::get_gizmo_name() const {
	return "TerrainWorldGizmo";
}
int TerrainWorldGizmoPlugin::get_priority() const {
	return 100;
}
bool TerrainWorldGizmoPlugin::is_handle_highlighted(const EditorSpatialGizmo *p_gizmo, int p_idx, bool p_secondary) const {
	return EditorSpatialGizmoPlugin::is_handle_highlighted(p_gizmo, p_idx, p_secondary);
}

Ref<EditorSpatialGizmo> TerrainWorldGizmoPlugin::create_gizmo(Spatial *p_spatial) {
	TerrainWorld *world = Object::cast_to<TerrainWorld>(p_spatial);

	if (world) {
		Ref<TerrainWorldGizmo> gizmo;
		gizmo.instance();

		gizmo->set_editor_plugin(plugin);
		gizmo->set_spatial_node(p_spatial);

		return gizmo;
	} else {
		return Ref<EditorSpatialGizmo>();
	}
}

TerrainWorldGizmoPlugin::TerrainWorldGizmoPlugin() {
	plugin = nullptr;

	create_material("main", Color(0.7, 0.7, 0.7));
	create_material("seam", Color(1, 0, 0), false, true);
	create_handle_material("handles");
}

TerrainWorldGizmoPlugin::~TerrainWorldGizmoPlugin() {
}

void TerrainWorldGizmoPlugin::_bind_methods() {
}
