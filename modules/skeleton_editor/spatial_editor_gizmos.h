/*************************************************************************/
/*  spatial_editor_gizmos.h                                              */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
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

#ifndef MODULE_SPATIAL_EDITOR_GIZMOS_H
#define MODULE_SPATIAL_EDITOR_GIZMOS_H

#include "editor/plugins/spatial_editor_plugin.h"
#include "scene/3d/camera.h"

class Camera;

class ModuleSkeletonSpatialGizmoPlugin : public EditorSpatialGizmoPlugin {

	GDCLASS(ModuleSkeletonSpatialGizmoPlugin, EditorSpatialGizmoPlugin);
	Color skeleton_color = Color(1, 0.8, 0.4);
	Color selected_bone_color = Color(1, 0, 0);
	float bone_axis_length = 0.015;
	Ref<ShaderMaterial> selected_mat;
	Ref<Shader> selected_sh;

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_name() const;
	int get_priority() const;
	void redraw(EditorSpatialGizmo *p_gizmo);

	ModuleSkeletonSpatialGizmoPlugin();
};

#endif // SPATIAL_EDITOR_GIZMOS_H
