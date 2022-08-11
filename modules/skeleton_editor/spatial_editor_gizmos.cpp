/*************************************************************************/
/*  spatial_editor_gizmos.cpp                                            */
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

#include "spatial_editor_gizmos.h"

#include "editor/editor_settings.h"
#include "scene/3d/skeleton.h"
#include "scene/resources/skin.h"
#include "scene/resources/surface_tool.h"

SkeletonSpatialGizmoPlugin::SkeletonSpatialGizmoPlugin() {
	skeleton_color = EDITOR_DEF("editors/3d_gizmos/gizmo_colors/skeleton", Color(1, 0.8, 0.4));
	selected_bone_color = EDITOR_DEF("editors/3d_gizmos/gizmo_colors/selected_bone", Color(1, 0, 0));
	bone_axis_length = EDITOR_DEF("editors/3d_gizmos/gizmo_settings/bone_axis_length", (float)0.015);
	create_material("skeleton_material", skeleton_color);
	selected_mat = Ref<ShaderMaterial>(memnew(ShaderMaterial));
	selected_sh = Ref<Shader>(memnew(Shader));
	selected_sh->set_code(" \
					shader_type spatial; \
					render_mode unshaded; \
					uniform vec4 albedo : hint_color = vec4(1,1,1,1); \
					uniform sampler2D texture_albedo : hint_albedo; \
					uniform float point_size : hint_range(0,128) = 32; \
					void vertex() { \
						if (!OUTPUT_IS_SRGB) { \
							COLOR.rgb = mix( pow((COLOR.rgb + vec3(0.055)) * (1.0 / (1.0 + 0.055)), vec3(2.4)), COLOR.rgb* (1.0 / 12.92), lessThan(COLOR.rgb,vec3(0.04045)) ); \
						} \
						VERTEX = VERTEX; \
						POSITION=PROJECTION_MATRIX*INV_CAMERA_MATRIX*WORLD_MATRIX*vec4(VERTEX.xyz,1.0); \
						POSITION.z = mix(POSITION.z, -POSITION.w, 0.998); \
					} \
					void fragment() { \
						vec2 base_uv = UV; \
						vec4 albedo_tex = texture(texture_albedo,base_uv); \
						albedo_tex *= COLOR; \
						if (albedo.a * albedo_tex.a < 0.5) { discard; } \
						ALBEDO = albedo.rgb * albedo_tex.rgb; \
					} \
				");
	selected_mat->set_shader(selected_sh);
}

bool SkeletonSpatialGizmoPlugin::has_gizmo(Spatial *p_spatial) {
	return Object::cast_to<Skeleton>(p_spatial) != NULL;
}

String SkeletonSpatialGizmoPlugin::get_gizmo_name() const {
	return "Skeleton";
}

int SkeletonSpatialGizmoPlugin::get_priority() const {
	return -1;
}

void SkeletonSpatialGizmoPlugin::redraw(EditorSpatialGizmo *p_gizmo) {
	Skeleton *skel = Object::cast_to<Skeleton>(p_gizmo->get_spatial_node());

	skel->force_update_all_bone_transforms();

	p_gizmo->clear();

	Ref<Material> material;
	if (p_gizmo->is_selected()) {
		material = selected_mat;
	} else {
		material = get_material("skeleton_material", p_gizmo);
	}

	Ref<SurfaceTool> surface_tool(memnew(SurfaceTool));

	surface_tool->begin(Mesh::PRIMITIVE_LINES);
	surface_tool->set_material(material);
	Vector<Transform> grests;
	grests.resize(skel->get_bone_count());

	Vector<int> bones;
	Vector<float> weights;
	bones.resize(4);
	weights.resize(4);

	for (int i = 0; i < 4; i++) {
		bones.write[i] = 0;
		weights.write[i] = 0;
	}

	weights.write[0] = 1;

	Color bone_color;
	AABB aabb;

	//LocalVector<int> bones_to_process = skel->get_parentless_bones();
	LocalVector<int> bones_to_process;
	bones_to_process = skel->get_parentless_bones();

	while (bones_to_process.size() > 0) {
		int current_bone_idx = bones_to_process[0];
		bones_to_process.erase(current_bone_idx);

		LocalVector<int> child_bones_vector;
		child_bones_vector = skel->get_bone_children(current_bone_idx);
		int child_bones_size = child_bones_vector.size();

		// You have children but no parent, then you must be a root/parentless bone.
		if (child_bones_size >= 0 && skel->get_bone_parent(current_bone_idx) <= 0) {
			grests.write[current_bone_idx] = skel->global_pose_to_local_pose(current_bone_idx, skel->get_bone_global_pose(current_bone_idx));
		}

		for (int i = 0; i < child_bones_size; i++) {
			int child_bone_idx = child_bones_vector[i];

			int parent = skel->get_bone_parent(child_bone_idx);
			if (parent == skel->get_selected_bone()) {
				bone_color = selected_bone_color;
			} else {
				bone_color = skeleton_color;
			}

			grests.write[child_bone_idx] = skel->global_pose_to_local_pose(child_bone_idx, skel->get_bone_global_pose(child_bone_idx));
			Vector3 v0 = grests[current_bone_idx].origin;
			Vector3 v1 = grests[child_bone_idx].origin;
			Vector3 d = skel->get_bone_rest(child_bone_idx).origin.normalized();
			real_t dist = skel->get_bone_rest(child_bone_idx).origin.length();

			// Find closest axis.
			int closest = -1;
			real_t closest_d = 0.0;
			for (int j = 0; j < 3; j++) {
				real_t dp = Math::abs(grests[current_bone_idx].basis[j].normalized().dot(d));
				if (j == 0 || dp > closest_d) {
					closest = j;
				}
			}

			//find closest other
			Vector3 first;
			Vector3 points[4];
			int pointidx = 0;
			//Color axis_color[3];
			//axis_color[0] = Color(1, 0, 0);
			//axis_color[1] = Color(0, 1, 0);
			//axis_color[2] = Color(0, 0, 1);
			for (int j = 0; j < 3; j++) {
				/*
				if (p_gizmo->is_selected()) {
					bones.write[0] = current_bone_idx;
					surface_tool->add_bones(bones);
					surface_tool->add_weights(weights);
					surface_tool->add_color(axis_color[j]);
					surface_tool->add_vertex(v1);
					surface_tool->add_bones(bones);
					surface_tool->add_weights(weights);
					surface_tool->add_color(axis_color[j]);
					surface_tool->add_vertex(v1 + (grests[current_bone_idx].basis.inverse())[j].normalized() * bone_axis_length);
				} else {
					bones.write[0] = current_bone_idx;
					surface_tool->add_bones(bones);
					surface_tool->add_weights(weights);
					surface_tool->add_color(axis_color[j]);
					surface_tool->add_vertex(v1 - (grests[current_bone_idx].basis.inverse())[j].normalized() * bone_axis_length * 0.5);
					surface_tool->add_bones(bones);
					surface_tool->add_weights(weights);
					surface_tool->add_color(axis_color[j]);
					surface_tool->add_vertex(v1 + (grests[current_bone_idx].basis.inverse())[j].normalized() * bone_axis_length * 0.5);
				}*/

				bones.write[0] = current_bone_idx;
				surface_tool->add_bones(bones);
				surface_tool->add_weights(weights);
				surface_tool->add_color(bone_color);
				surface_tool->add_vertex(v0 - grests[current_bone_idx].basis[j].normalized() * dist * 0.05);
				
				surface_tool->add_bones(bones);
				surface_tool->add_weights(weights);
				surface_tool->add_color(bone_color);
				surface_tool->add_vertex(v0 + grests[current_bone_idx].basis[j].normalized() * dist * 0.05);

				if (j == closest) {
					continue;
				}

				Vector3 axis;
				if (first == Vector3()) {
					axis = d.cross(d.cross(grests[current_bone_idx].basis[j])).normalized();
					first = axis;
				} else {
					axis = d.cross(first).normalized();
				}

				for (int k = 0; k < 2; k++) {
					if (k == 1) {
						axis = -axis;
					}
					Vector3 point = v0 + d * dist * 0.2;
					point += axis * dist * 0.1;

					bones.write[0] = current_bone_idx;
					surface_tool->add_bones(bones);
					surface_tool->add_weights(weights);
					surface_tool->add_color(bone_color);
					surface_tool->add_vertex(v0);

					surface_tool->add_bones(bones);
					surface_tool->add_weights(weights);
					surface_tool->add_color(bone_color);
					surface_tool->add_vertex(point);

					bones.write[0] = current_bone_idx;
					surface_tool->add_bones(bones);
					surface_tool->add_weights(weights);
					surface_tool->add_color(bone_color);
					surface_tool->add_vertex(point);

					bones.write[0] = child_bone_idx;
					surface_tool->add_bones(bones);
					surface_tool->add_weights(weights);
					surface_tool->add_color(bone_color);
					surface_tool->add_vertex(v1);
					points[pointidx++] = point;
				}
			}

			SWAP(points[1], points[2]);
			for (int j = 0; j < 4; j++) {
				bones.write[0] = current_bone_idx;
				surface_tool->add_bones(bones);
				surface_tool->add_weights(weights);
				surface_tool->add_color(bone_color);
				surface_tool->add_vertex(points[j]);
				surface_tool->add_bones(bones);
				surface_tool->add_weights(weights);
				surface_tool->add_color(bone_color);
				surface_tool->add_vertex(points[(j + 1) % 4]);
			}

			// Add the bone's children to the list of bones to be processed.
			bones_to_process.push_back(child_bones_vector[i]);
		}
	}

	Ref<ArrayMesh> m = surface_tool->commit();
	p_gizmo->add_mesh(m, Ref<Material>(), Transform(), skel->register_skin(Ref<Skin>()));
}
