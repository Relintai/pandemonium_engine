/*************************************************************************/
/*  mesh_instance.cpp                                                    */
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

#include "mesh_instance.h"

#include "collision_shape.h"
#include "core/config/project_settings.h"
#include "core/core_string_names.h"
#include "physics_body.h"
#include "scene/main/scene_string_names.h"
#include "scene/resources/material/material.h"
#include "scene/resources/material/spatial_material.h"
#include "scene/resources/mesh/merging_tool.h"
#include "scene/resources/mesh/mesh.h"
#include "servers/rendering/rendering_server_globals.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_SKELETON_3D_ENABLED
#include "modules/skeleton_3d/nodes/skeleton.h"
#include "modules/skeleton_3d/resources/skin.h"
#endif

bool MeshInstance::_set(const StringName &p_name, const Variant &p_value) {
	//this is not _too_ bad performance wise, really. it only arrives here if the property was not set anywhere else.
	//add to it that it's probably found on first call to _set anyway.

	if (!get_instance().is_valid()) {
		return false;
	}

	RBMap<StringName, BlendShapeTrack>::Element *E = blend_shape_tracks.find(p_name);
	if (E) {
		E->get().value = p_value;
		RenderingServer::get_singleton()->instance_set_blend_shape_weight(get_instance(), E->get().idx, E->get().value);
		return true;
	}

	if (p_name.operator String().begins_with("material/")) {
		int idx = p_name.operator String().get_slicec('/', 1).to_int();
		if (idx >= materials.size() || idx < 0) {
			return false;
		}

		set_surface_material(idx, p_value);
		return true;
	}

	return false;
}

bool MeshInstance::_get(const StringName &p_name, Variant &r_ret) const {
	if (!get_instance().is_valid()) {
		return false;
	}

	const RBMap<StringName, BlendShapeTrack>::Element *E = blend_shape_tracks.find(p_name);
	if (E) {
		r_ret = E->get().value;
		return true;
	}

	if (p_name.operator String().begins_with("material/")) {
		int idx = p_name.operator String().get_slicec('/', 1).to_int();
		if (idx >= materials.size() || idx < 0) {
			return false;
		}
		r_ret = materials[idx];
		return true;
	}
	return false;
}

void MeshInstance::_get_property_list(List<PropertyInfo> *p_list) const {
	List<String> ls;
	for (const RBMap<StringName, BlendShapeTrack>::Element *E = blend_shape_tracks.front(); E; E = E->next()) {
		ls.push_back(E->key());
	}

	ls.sort();

	for (List<String>::Element *E = ls.front(); E; E = E->next()) {
		p_list->push_back(PropertyInfo(Variant::REAL, E->get(), PROPERTY_HINT_RANGE, "-1,1,0.00001"));
	}

	if (mesh.is_valid()) {
		for (int i = 0; i < mesh->get_surface_count(); i++) {
			p_list->push_back(PropertyInfo(Variant::OBJECT, "material/" + itos(i), PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial,SpatialMaterial"));
		}
	}
}

void MeshInstance::set_mesh(const Ref<Mesh> &p_mesh) {
	if (mesh == p_mesh) {
		return;
	}

	if (mesh.is_valid()) {
		mesh->disconnect(CoreStringNames::get_singleton()->changed, this, SceneStringNames::get_singleton()->_mesh_changed);
	}

#ifdef MODULE_SKELETON_3D_ENABLED
	if (skin_ref.is_valid() && mesh.is_valid() && _is_software_skinning_enabled() && is_visible_in_tree()) {
		ERR_FAIL_COND(!skin_ref->get_skeleton_node());
		skin_ref->get_skeleton_node()->disconnect("pose_updated", this, "_update_skinning");
	}
#endif

#ifdef MODULE_SKELETON_3D_ENABLED
	if (software_skinning) {
		memdelete(software_skinning);
		software_skinning = nullptr;
	}
#endif

	mesh = p_mesh;

	blend_shape_tracks.clear();
	if (mesh.is_valid()) {
		for (int i = 0; i < mesh->get_blend_shape_count(); i++) {
			BlendShapeTrack mt;
			mt.idx = i;
			mt.value = 0;
			blend_shape_tracks["blend_shapes/" + String(mesh->get_blend_shape_name(i))] = mt;
		}

		mesh->connect(CoreStringNames::get_singleton()->changed, this, SceneStringNames::get_singleton()->_mesh_changed);
		materials.resize(mesh->get_surface_count());

#ifdef MODULE_SKELETON_3D_ENABLED
		_initialize_skinning(false, true);
#endif
	} else {
		set_base(RID());
	}

	update_gizmos();

	_change_notify();
}
Ref<Mesh> MeshInstance::get_mesh() const {
	return mesh;
}

#ifdef MODULE_SKELETON_3D_ENABLED
void MeshInstance::_resolve_skeleton_path() {
	Ref<SkinReference> new_skin_reference;

	if (!skeleton_path.is_empty()) {
		Skeleton *skeleton = Object::cast_to<Skeleton>(get_node(skeleton_path));
		if (skeleton) {
			if (skin_internal.is_null()) {
				new_skin_reference = skeleton->register_skin(skeleton->create_skin_from_rest_transforms());
				//a skin was created for us
				skin_internal = new_skin_reference->get_skin();
				_change_notify();
			} else {
				new_skin_reference = skeleton->register_skin(skin_internal);
			}
		}
	}

	if (skin_ref.is_valid() && mesh.is_valid() && _is_software_skinning_enabled() && is_visible_in_tree()) {
		ERR_FAIL_COND(!skin_ref->get_skeleton_node());
		skin_ref->get_skeleton_node()->disconnect("pose_updated", this, "_update_skinning");
	}

	skin_ref = new_skin_reference;

	software_skinning_flags &= ~SoftwareSkinning::FLAG_BONES_READY;

	_initialize_skinning();
}

bool MeshInstance::_is_global_software_skinning_enabled() {
	// Check if forced in project settings.
	GLOBAL_CACHED(force_software_skinning, bool, "rendering/quality/skinning/force_software_skinning");
	if (force_software_skinning) {
		return true;
	}

	// Check if enabled in project settings.
	GLOBAL_CACHED(software_skinning_fallback, bool, "rendering/quality/skinning/software_skinning_fallback");
	if (!software_skinning_fallback) {
		return false;
	}

	// Check if requested by renderer settings.
	return RSG::storage->has_os_feature("skinning_fallback");
}

bool MeshInstance::_is_software_skinning_enabled() const {
	// Using static local variable which will be initialized only once,
	// so _is_global_software_skinning_enabled can be only called once on first use.
	static bool global_software_skinning = _is_global_software_skinning_enabled();
	return global_software_skinning;
}

void MeshInstance::_initialize_skinning(bool p_force_reset, bool p_call_attach_skeleton) {
	if (mesh.is_null()) {
		return;
	}

	RenderingServer *rendering_server = RenderingServer::get_singleton();

	bool update_mesh = false;

	if (skin_ref.is_valid()) {
		if (_is_software_skinning_enabled()) {
			if (is_visible_in_tree()) {
				ERR_FAIL_COND(!skin_ref->get_skeleton_node());
				if (!skin_ref->get_skeleton_node()->is_connected("pose_updated", this, "_update_skinning")) {
					skin_ref->get_skeleton_node()->connect("pose_updated", this, "_update_skinning");
				}
			}

			if (p_force_reset && software_skinning) {
				memdelete(software_skinning);
				software_skinning = nullptr;
			}

			if (!software_skinning) {
				software_skinning = memnew(SoftwareSkinning);

				if (mesh->get_blend_shape_count() > 0) {
					ERR_PRINT("Blend shapes are not supported for software skinning.");
				}

				Ref<ArrayMesh> software_mesh;
				software_mesh.instance();
				RID mesh_rid = software_mesh->get_rid();

				// Initialize mesh for dynamic update.
				int surface_count = mesh->get_surface_count();
				software_skinning->surface_data.resize(surface_count);
				for (int surface_index = 0; surface_index < surface_count; ++surface_index) {
					ERR_CONTINUE(Mesh::PRIMITIVE_TRIANGLES != mesh->surface_get_primitive_type(surface_index));

					SoftwareSkinning::SurfaceData &surface_data = software_skinning->surface_data[surface_index];
					surface_data.transform_tangents = false;
					surface_data.ensure_correct_normals = false;

					uint32_t format = mesh->surface_get_format(surface_index);
					ERR_CONTINUE(0 == (format & Mesh::ARRAY_FORMAT_VERTEX));
					ERR_CONTINUE(0 == (format & Mesh::ARRAY_FORMAT_BONES));
					ERR_CONTINUE(0 == (format & Mesh::ARRAY_FORMAT_WEIGHTS));

					format |= Mesh::ARRAY_FLAG_USE_DYNAMIC_UPDATE;
					format &= ~Mesh::ARRAY_COMPRESS_VERTEX;
					format &= ~Mesh::ARRAY_COMPRESS_WEIGHTS;
					format &= ~Mesh::ARRAY_FLAG_USE_16_BIT_BONES;

					Array write_arrays = mesh->surface_get_arrays(surface_index);
					Array read_arrays;
					read_arrays.resize(Mesh::ARRAY_MAX);

					read_arrays[Mesh::ARRAY_VERTEX] = write_arrays[Mesh::ARRAY_VERTEX];
					read_arrays[Mesh::ARRAY_BONES] = write_arrays[Mesh::ARRAY_BONES];
					read_arrays[Mesh::ARRAY_WEIGHTS] = write_arrays[Mesh::ARRAY_WEIGHTS];

					write_arrays[Mesh::ARRAY_BONES] = Variant();
					write_arrays[Mesh::ARRAY_WEIGHTS] = Variant();

					if (software_skinning_flags & SoftwareSkinning::FLAG_TRANSFORM_NORMALS) {
						ERR_CONTINUE(0 == (format & Mesh::ARRAY_FORMAT_NORMAL));
						format &= ~Mesh::ARRAY_COMPRESS_NORMAL;

						read_arrays[Mesh::ARRAY_NORMAL] = write_arrays[Mesh::ARRAY_NORMAL];

						Ref<Material> mat = get_active_material(surface_index);
						if (mat.is_valid()) {
							Ref<SpatialMaterial> spatial_mat = mat;
							if (spatial_mat.is_valid()) {
								// Spatial material, check from material settings.
								surface_data.transform_tangents = spatial_mat->get_feature(SpatialMaterial::FEATURE_NORMAL_MAPPING);
								surface_data.ensure_correct_normals = spatial_mat->get_flag(SpatialMaterial::FLAG_ENSURE_CORRECT_NORMALS);
							} else {
								// Custom shader, must check for compiled flags.
								surface_data.transform_tangents = RSG::storage->material_uses_tangents(mat->get_rid());
								surface_data.ensure_correct_normals = RSG::storage->material_uses_ensure_correct_normals(mat->get_rid());
							}
						}

						if (surface_data.transform_tangents) {
							ERR_CONTINUE(0 == (format & Mesh::ARRAY_FORMAT_TANGENT));
							format &= ~Mesh::ARRAY_COMPRESS_TANGENT;

							read_arrays[Mesh::ARRAY_TANGENT] = write_arrays[Mesh::ARRAY_TANGENT];
						}
					}

					// 1. Temporarily add surface with bone data to create the read buffer.
					software_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, read_arrays, Array(), format);

					PoolByteArray buffer_read = rendering_server->mesh_surface_get_array(mesh_rid, surface_index);
					surface_data.source_buffer.append_array(buffer_read);
					surface_data.source_format = software_mesh->surface_get_format(surface_index);

					software_mesh->surface_remove(surface_index);

					// 2. Create the surface again without the bone data for the write buffer.
					software_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, write_arrays, Array(), format);

					Ref<Material> material = mesh->surface_get_material(surface_index);
					software_mesh->surface_set_material(surface_index, material);

					surface_data.buffer = rendering_server->mesh_surface_get_array(mesh_rid, surface_index);
					surface_data.buffer_write = surface_data.buffer.write();
				}

				software_skinning->mesh_instance = software_mesh;
				update_mesh = true;
			}

			if (p_call_attach_skeleton) {
				rendering_server->instance_attach_skeleton(get_instance(), RID());
			}

			if (is_visible_in_tree() && (software_skinning_flags & SoftwareSkinning::FLAG_BONES_READY)) {
				// Initialize from current skeleton pose.
				_update_skinning();
			}
		} else {
			ERR_FAIL_COND(!skin_ref->get_skeleton_node());
			if (skin_ref->get_skeleton_node()->is_connected("pose_updated", this, "_update_skinning")) {
				skin_ref->get_skeleton_node()->disconnect("pose_updated", this, "_update_skinning");
			}

			if (p_call_attach_skeleton) {
				rendering_server->instance_attach_skeleton(get_instance(), skin_ref->get_skeleton());
			}

			if (software_skinning) {
				memdelete(software_skinning);
				software_skinning = nullptr;
				update_mesh = true;
			}
		}
	} else {
		if (p_call_attach_skeleton) {
			rendering_server->instance_attach_skeleton(get_instance(), RID());
		}
		if (software_skinning) {
			memdelete(software_skinning);
			software_skinning = nullptr;
			update_mesh = true;
		}
	}

	RID render_mesh = software_skinning ? software_skinning->mesh_instance->get_rid() : mesh->get_rid();
	if (update_mesh || (render_mesh != get_base())) {
		set_base(render_mesh);

		// Update instance materials after switching mesh.
		int surface_count = mesh->get_surface_count();
		for (int surface_index = 0; surface_index < surface_count; ++surface_index) {
			if (materials[surface_index].is_valid()) {
				rendering_server->instance_set_surface_material(get_instance(), surface_index, materials[surface_index]->get_rid());
			}
		}
	}
}

void MeshInstance::_update_skinning() {
	ERR_FAIL_COND(!_is_software_skinning_enabled());
#if defined(TOOLS_ENABLED) && defined(DEBUG_ENABLED)
	ERR_FAIL_COND(!is_visible_in_tree());
#else
	ERR_FAIL_COND(!is_visible());
#endif

	ERR_FAIL_COND(!software_skinning);
	Ref<Mesh> software_skinning_mesh = software_skinning->mesh_instance;
	ERR_FAIL_COND(!software_skinning_mesh.is_valid());
	RID mesh_rid = software_skinning_mesh->get_rid();
	ERR_FAIL_COND(!mesh_rid.is_valid());

	ERR_FAIL_COND(!mesh.is_valid());
	RID source_mesh_rid = mesh->get_rid();
	ERR_FAIL_COND(!source_mesh_rid.is_valid());

	ERR_FAIL_COND(skin_ref.is_null());
	RID skeleton = skin_ref->get_skeleton();
	ERR_FAIL_COND(!skeleton.is_valid());

	Vector3 aabb_min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3 aabb_max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	RenderingServer *rendering_server = RenderingServer::get_singleton();

	// Prepare bone transforms.
	const int num_bones = rendering_server->skeleton_get_bone_count(skeleton);
	ERR_FAIL_COND(num_bones <= 0);
	Transform *bone_transforms = (Transform *)alloca(sizeof(Transform) * num_bones);
	for (int bone_index = 0; bone_index < num_bones; ++bone_index) {
		bone_transforms[bone_index] = rendering_server->skeleton_bone_get_transform(skeleton, bone_index);
	}

	// Apply skinning.
	int surface_count = software_skinning_mesh->get_surface_count();
	for (int surface_index = 0; surface_index < surface_count; ++surface_index) {
		ERR_CONTINUE((uint32_t)surface_index >= software_skinning->surface_data.size());
		const SoftwareSkinning::SurfaceData &surface_data = software_skinning->surface_data[surface_index];
		const bool transform_tangents = surface_data.transform_tangents;
		const bool ensure_correct_normals = surface_data.ensure_correct_normals;

		const uint32_t format_write = software_skinning_mesh->surface_get_format(surface_index);

		const int vertex_count_write = software_skinning_mesh->surface_get_array_len(surface_index);
		const int index_count_write = software_skinning_mesh->surface_get_array_index_len(surface_index);

		uint32_t array_offsets_write[Mesh::ARRAY_MAX];
		uint32_t array_strides_write[Mesh::ARRAY_MAX];
		rendering_server->mesh_surface_make_offsets_from_format(format_write, vertex_count_write, index_count_write, array_offsets_write, array_strides_write);
		ERR_FAIL_COND(array_strides_write[Mesh::ARRAY_VERTEX] != array_strides_write[Mesh::ARRAY_NORMAL]);
		const uint32_t stride_write = array_strides_write[Mesh::ARRAY_VERTEX];
		const uint32_t offset_vertices_write = array_offsets_write[Mesh::ARRAY_VERTEX];
		const uint32_t offset_normals_write = array_offsets_write[Mesh::ARRAY_NORMAL];
		const uint32_t offset_tangents_write = array_offsets_write[Mesh::ARRAY_TANGENT];

		PoolByteArray buffer_source = surface_data.source_buffer;
		PoolByteArray::Read buffer_read = buffer_source.read();

		const uint32_t format_read = surface_data.source_format;

		ERR_CONTINUE(0 == (format_read & Mesh::ARRAY_FORMAT_BONES));
		ERR_CONTINUE(0 == (format_read & Mesh::ARRAY_FORMAT_WEIGHTS));

		const int vertex_count = mesh->surface_get_array_len(surface_index);
		const int index_count = mesh->surface_get_array_index_len(surface_index);

		ERR_CONTINUE(vertex_count != vertex_count_write);

		uint32_t array_offsets[Mesh::ARRAY_MAX];
		uint32_t array_strides[Mesh::ARRAY_MAX];
		rendering_server->mesh_surface_make_offsets_from_format(format_read, vertex_count, index_count, array_offsets, array_strides);
		ERR_FAIL_COND(array_strides[Mesh::ARRAY_VERTEX] != array_strides[Mesh::ARRAY_NORMAL]);
		const uint32_t stride = array_strides[Mesh::ARRAY_VERTEX];
		const uint32_t offset_vertices = array_offsets[Mesh::ARRAY_VERTEX];
		const uint32_t offset_normals = array_offsets[Mesh::ARRAY_NORMAL];
		const uint32_t offset_tangents = array_offsets[Mesh::ARRAY_TANGENT];
		const uint32_t offset_bones = array_offsets[Mesh::ARRAY_BONES];
		const uint32_t offset_weights = array_offsets[Mesh::ARRAY_WEIGHTS];

		PoolByteArray buffer = surface_data.buffer;
		PoolByteArray::Write buffer_write = surface_data.buffer_write;

		for (int vertex_index = 0; vertex_index < vertex_count; ++vertex_index) {
			const uint32_t vertex_offset = vertex_index * stride;
			const uint32_t vertex_offset_write = vertex_index * stride_write;

			float bone_weights[4];
			const float *weight_ptr = (const float *)(buffer_read.ptr() + offset_weights + vertex_offset);
			bone_weights[0] = weight_ptr[0];
			bone_weights[1] = weight_ptr[1];
			bone_weights[2] = weight_ptr[2];
			bone_weights[3] = weight_ptr[3];

			const uint8_t *bones_ptr = buffer_read.ptr() + offset_bones + vertex_offset;
			const int b0 = bones_ptr[0];
			const int b1 = bones_ptr[1];
			const int b2 = bones_ptr[2];
			const int b3 = bones_ptr[3];

			Transform transform;
			transform.origin =
					bone_weights[0] * bone_transforms[b0].origin +
					bone_weights[1] * bone_transforms[b1].origin +
					bone_weights[2] * bone_transforms[b2].origin +
					bone_weights[3] * bone_transforms[b3].origin;

			transform.basis =
					bone_transforms[b0].basis * bone_weights[0] +
					bone_transforms[b1].basis * bone_weights[1] +
					bone_transforms[b2].basis * bone_weights[2] +
					bone_transforms[b3].basis * bone_weights[3];

			const Vector3 &vertex_read = (const Vector3 &)buffer_read[vertex_offset + offset_vertices];
			Vector3 &vertex = (Vector3 &)buffer_write[vertex_offset_write + offset_vertices_write];
			vertex = transform.xform(vertex_read);

			if (software_skinning_flags & SoftwareSkinning::FLAG_TRANSFORM_NORMALS) {
				if (ensure_correct_normals) {
					transform.basis.invert();
					transform.basis.transpose();
				}

				const Vector3 &normal_read = (const Vector3 &)buffer_read[vertex_offset + offset_normals];
				Vector3 &normal = (Vector3 &)buffer_write[vertex_offset_write + offset_normals_write];
				normal = transform.basis.xform(normal_read);

				if (transform_tangents) {
					const Vector3 &tangent_read = (const Vector3 &)buffer_read[vertex_offset + offset_tangents];
					Vector3 &tangent = (Vector3 &)buffer_write[vertex_offset_write + offset_tangents_write];
					tangent = transform.basis.xform(tangent_read);
				}
			}

			aabb_min.x = MIN(aabb_min.x, vertex.x);
			aabb_min.y = MIN(aabb_min.y, vertex.y);
			aabb_min.z = MIN(aabb_min.z, vertex.z);
			aabb_max.x = MAX(aabb_max.x, vertex.x);
			aabb_max.y = MAX(aabb_max.y, vertex.y);
			aabb_max.z = MAX(aabb_max.z, vertex.z);
		}

		rendering_server->mesh_surface_update_region(mesh_rid, surface_index, 0, buffer);
	}

	rendering_server->mesh_set_custom_aabb(mesh_rid, AABB(aabb_min, aabb_max - aabb_min));

	software_skinning_flags |= SoftwareSkinning::FLAG_BONES_READY;
}
#endif

#ifdef MODULE_SKELETON_3D_ENABLED
void MeshInstance::set_skin(const Ref<Skin> &p_skin) {
	skin_internal = p_skin;
	skin = p_skin;
	if (!is_inside_tree()) {
		return;
	}
	_resolve_skeleton_path();
}

Ref<Skin> MeshInstance::get_skin() const {
	return skin;
}

void MeshInstance::set_skeleton_path(const NodePath &p_skeleton) {
	skeleton_path = p_skeleton;
	if (!is_inside_tree()) {
		return;
	}
	_resolve_skeleton_path();
}

NodePath MeshInstance::get_skeleton_path() {
	return skeleton_path;
}
#endif

AABB MeshInstance::get_aabb() const {
	if (!mesh.is_null()) {
		return mesh->get_aabb();
	}

	return AABB();
}

PoolVector<Face3> MeshInstance::get_faces(uint32_t p_usage_flags) const {
	if (!(p_usage_flags & (FACES_SOLID | FACES_ENCLOSING))) {
		return PoolVector<Face3>();
	}

	if (mesh.is_null()) {
		return PoolVector<Face3>();
	}

	return mesh->get_faces();
}

Node *MeshInstance::create_trimesh_collision_node() {
	if (mesh.is_null()) {
		return nullptr;
	}

	Ref<Shape> shape = mesh->create_trimesh_shape();
	if (shape.is_null()) {
		return nullptr;
	}

	StaticBody *static_body = memnew(StaticBody);
	CollisionShape *cshape = memnew(CollisionShape);
	cshape->set_shape(shape);
	static_body->add_child(cshape);
	return static_body;
}

void MeshInstance::create_trimesh_collision() {
	StaticBody *static_body = Object::cast_to<StaticBody>(create_trimesh_collision_node());
	ERR_FAIL_COND(!static_body);
	static_body->set_name(String(get_name()) + "_col");

	add_child(static_body);
	if (get_owner()) {
		CollisionShape *cshape = Object::cast_to<CollisionShape>(static_body->get_child(0));
		static_body->set_owner(get_owner());
		cshape->set_owner(get_owner());
	}
}

Node *MeshInstance::create_multiple_convex_collisions_node() {
	if (mesh.is_null()) {
		return nullptr;
	}

	Vector<Ref<Shape>> shapes = mesh->convex_decompose();
	if (!shapes.size()) {
		return nullptr;
	}

	StaticBody *static_body = memnew(StaticBody);
	for (int i = 0; i < shapes.size(); i++) {
		CollisionShape *cshape = memnew(CollisionShape);
		cshape->set_shape(shapes[i]);
		static_body->add_child(cshape);
	}
	return static_body;
}

void MeshInstance::create_multiple_convex_collisions() {
	StaticBody *static_body = Object::cast_to<StaticBody>(create_multiple_convex_collisions_node());
	ERR_FAIL_COND(!static_body);
	static_body->set_name(String(get_name()) + "_col");

	add_child(static_body);
	if (get_owner()) {
		static_body->set_owner(get_owner());
		int count = static_body->get_child_count();
		for (int i = 0; i < count; i++) {
			CollisionShape *cshape = Object::cast_to<CollisionShape>(static_body->get_child(i));
			cshape->set_owner(get_owner());
		}
	}
}

Node *MeshInstance::create_convex_collision_node(bool p_clean, bool p_simplify) {
	if (mesh.is_null()) {
		return nullptr;
	}

	Ref<Shape> shape = mesh->create_convex_shape(p_clean, p_simplify);
	if (shape.is_null()) {
		return nullptr;
	}

	StaticBody *static_body = memnew(StaticBody);
	CollisionShape *cshape = memnew(CollisionShape);
	cshape->set_shape(shape);
	static_body->add_child(cshape);
	return static_body;
}

void MeshInstance::create_convex_collision(bool p_clean, bool p_simplify) {
	StaticBody *static_body = Object::cast_to<StaticBody>(create_convex_collision_node(p_clean, p_simplify));
	ERR_FAIL_COND(!static_body);
	static_body->set_name(String(get_name()) + "_col");

	add_child(static_body);
	if (get_owner()) {
		CollisionShape *cshape = Object::cast_to<CollisionShape>(static_body->get_child(0));
		static_body->set_owner(get_owner());
		cshape->set_owner(get_owner());
	}
}

void MeshInstance::_notification(int p_what) {
#ifdef MODULE_SKELETON_3D_ENABLED
	if (p_what == NOTIFICATION_ENTER_TREE) {
		_resolve_skeleton_path();
	}

	if (p_what == NOTIFICATION_TRANSLATION_CHANGED) {
		if (mesh.is_valid()) {
			mesh->notification(NOTIFICATION_TRANSLATION_CHANGED);
		}
	}

	if (p_what == NOTIFICATION_VISIBILITY_CHANGED) {
		if (skin_ref.is_valid() && mesh.is_valid() && _is_software_skinning_enabled()) {
			ERR_FAIL_COND(!skin_ref->get_skeleton_node());
			if (is_visible_in_tree()) {
				skin_ref->get_skeleton_node()->connect("pose_updated", this, "_update_skinning");
			} else {
				skin_ref->get_skeleton_node()->disconnect("pose_updated", this, "_update_skinning");
			}
		}
	}
#endif
}

int MeshInstance::get_surface_material_count() const {
	return materials.size();
}

void MeshInstance::set_surface_material(int p_surface, const Ref<Material> &p_material) {
	ERR_FAIL_INDEX(p_surface, materials.size());

	materials.write[p_surface] = p_material;

	if (materials[p_surface].is_valid()) {
		RS::get_singleton()->instance_set_surface_material(get_instance(), p_surface, materials[p_surface]->get_rid());
	} else {
		RS::get_singleton()->instance_set_surface_material(get_instance(), p_surface, RID());
	}

#ifdef MODULE_SKELETON_3D_ENABLED
	if (software_skinning) {
		_initialize_skinning(true);
	}
#endif
}

Ref<Material> MeshInstance::get_surface_material(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, materials.size(), Ref<Material>());

	return materials[p_surface];
}

Ref<Material> MeshInstance::get_active_material(int p_surface) const {
	Ref<Material> material_override = get_material_override();
	if (material_override.is_valid()) {
		return material_override;
	}

	Ref<Material> surface_material = get_surface_material(p_surface);
	if (surface_material.is_valid()) {
		return surface_material;
	}

	Ref<Mesh> mesh = get_mesh();
	if (mesh.is_valid()) {
		return mesh->surface_get_material(p_surface);
	}

	return Ref<Material>();
}

void MeshInstance::set_material_override(const Ref<Material> &p_material) {
	if (p_material == get_material_override()) {
		return;
	}

	GeometryInstance::set_material_override(p_material);

#ifdef MODULE_SKELETON_3D_ENABLED
	if (software_skinning) {
		_initialize_skinning(true);
	}
#endif
}

void MeshInstance::set_material_overlay(const Ref<Material> &p_material) {
	if (p_material == get_material_overlay()) {
		return;
	}

	GeometryInstance::set_material_overlay(p_material);
}

#ifdef MODULE_SKELETON_3D_ENABLED
void MeshInstance::set_software_skinning_transform_normals(bool p_enabled) {
	if (p_enabled == is_software_skinning_transform_normals_enabled()) {
		return;
	}

	if (p_enabled) {
		software_skinning_flags |= SoftwareSkinning::FLAG_TRANSFORM_NORMALS;
	} else {
		software_skinning_flags &= ~SoftwareSkinning::FLAG_TRANSFORM_NORMALS;
	}

	if (software_skinning) {
		_initialize_skinning(true);
	}
}

bool MeshInstance::is_software_skinning_transform_normals_enabled() const {
	return 0 != (software_skinning_flags & SoftwareSkinning::FLAG_TRANSFORM_NORMALS);
}
#endif

void MeshInstance::_mesh_changed() {
	ERR_FAIL_COND(mesh.is_null());
	materials.resize(mesh->get_surface_count());

#ifdef MODULE_SKELETON_3D_ENABLED
	if (software_skinning) {
		_initialize_skinning(true);
	}
#endif
}

void MeshInstance::create_debug_tangents() {
	Vector<Vector3> lines;
	Vector<Color> colors;

	Ref<Mesh> mesh = get_mesh();
	if (!mesh.is_valid()) {
		return;
	}

	for (int i = 0; i < mesh->get_surface_count(); i++) {
		Array arrays = mesh->surface_get_arrays(i);
		Vector<Vector3> verts = arrays[Mesh::ARRAY_VERTEX];
		Vector<Vector3> norms = arrays[Mesh::ARRAY_NORMAL];
		if (norms.size() == 0) {
			continue;
		}
		Vector<float> tangents = arrays[Mesh::ARRAY_TANGENT];
		if (tangents.size() == 0) {
			continue;
		}

		for (int j = 0; j < verts.size(); j++) {
			Vector3 v = verts[j];
			Vector3 n = norms[j];
			Vector3 t = Vector3(tangents[j * 4 + 0], tangents[j * 4 + 1], tangents[j * 4 + 2]);
			Vector3 b = (n.cross(t)).normalized() * tangents[j * 4 + 3];

			lines.push_back(v); //normal
			colors.push_back(Color(0, 0, 1)); //color
			lines.push_back(v + n * 0.04); //normal
			colors.push_back(Color(0, 0, 1)); //color

			lines.push_back(v); //tangent
			colors.push_back(Color(1, 0, 0)); //color
			lines.push_back(v + t * 0.04); //tangent
			colors.push_back(Color(1, 0, 0)); //color

			lines.push_back(v); //binormal
			colors.push_back(Color(0, 1, 0)); //color
			lines.push_back(v + b * 0.04); //binormal
			colors.push_back(Color(0, 1, 0)); //color
		}
	}

	if (lines.size()) {
		Ref<SpatialMaterial> sm;
		sm.instance();

		sm->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
		sm->set_flag(SpatialMaterial::FLAG_SRGB_VERTEX_COLOR, true);
		sm->set_flag(SpatialMaterial::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

		Ref<ArrayMesh> am;
		am.instance();
		Array a;
		a.resize(Mesh::ARRAY_MAX);
		a[Mesh::ARRAY_VERTEX] = lines;
		a[Mesh::ARRAY_COLOR] = colors;

		am->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, a);
		am->surface_set_material(0, sm);

		MeshInstance *mi = memnew(MeshInstance);
		mi->set_mesh(am);
		mi->set_name("DebugTangents");
		add_child(mi);
#ifdef TOOLS_ENABLED

		if (is_inside_tree() && this == get_tree()->get_edited_scene_root()) {
			mi->set_owner(this);
		} else {
			mi->set_owner(get_owner());
		}
#endif
	}
}

bool MeshInstance::merge_meshes(Vector<Variant> p_list, bool p_use_global_space, bool p_check_compatibility, bool p_shadows_only) {
	return MergingTool::wrapped_merge_meshes(*this, p_list, p_use_global_space, p_check_compatibility, p_shadows_only, Mesh::STORAGE_MODE_GPU);
}

bool MeshInstance::is_mergeable_with(Node *p_other, bool p_shadows_only) const {
	const MeshInstance *mi = Object::cast_to<MeshInstance>(p_other);

	if (mi) {
		if (p_shadows_only) {
			return MergingTool::is_shadow_mergeable_with(*this, *mi);
		} else {
			return MergingTool::is_mergeable_with(*this, *mi, true);
		}
	}

	return false;
}

void MeshInstance::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &MeshInstance::set_mesh);
	ClassDB::bind_method(D_METHOD("get_mesh"), &MeshInstance::get_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "Mesh"), "set_mesh", "get_mesh");

#ifdef MODULE_SKELETON_3D_ENABLED
	ClassDB::bind_method(D_METHOD("set_skeleton_path", "skeleton_path"), &MeshInstance::set_skeleton_path);
	ClassDB::bind_method(D_METHOD("get_skeleton_path"), &MeshInstance::get_skeleton_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "skeleton", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Skeleton"), "set_skeleton_path", "get_skeleton_path");

	ClassDB::bind_method(D_METHOD("set_skin", "skin"), &MeshInstance::set_skin);
	ClassDB::bind_method(D_METHOD("get_skin"), &MeshInstance::get_skin);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "skin", PROPERTY_HINT_RESOURCE_TYPE, "Skin"), "set_skin", "get_skin");
#endif

	ClassDB::bind_method(D_METHOD("get_surface_material_count"), &MeshInstance::get_surface_material_count);
	ClassDB::bind_method(D_METHOD("set_surface_material", "surface", "material"), &MeshInstance::set_surface_material);
	ClassDB::bind_method(D_METHOD("get_surface_material", "surface"), &MeshInstance::get_surface_material);
	ClassDB::bind_method(D_METHOD("get_active_material", "surface"), &MeshInstance::get_active_material);

#ifdef MODULE_SKELETON_3D_ENABLED
	ADD_GROUP("Software Skinning", "software_skinning");
	ClassDB::bind_method(D_METHOD("set_software_skinning_transform_normals", "enabled"), &MeshInstance::set_software_skinning_transform_normals);
	ClassDB::bind_method(D_METHOD("is_software_skinning_transform_normals_enabled"), &MeshInstance::is_software_skinning_transform_normals_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "software_skinning_transform_normals"), "set_software_skinning_transform_normals", "is_software_skinning_transform_normals_enabled");
#endif

	ClassDB::bind_method(D_METHOD("create_trimesh_collision"), &MeshInstance::create_trimesh_collision);
	ClassDB::set_method_flags("MeshInstance", "create_trimesh_collision", METHOD_FLAGS_DEFAULT);
	ClassDB::bind_method(D_METHOD("create_multiple_convex_collisions"), &MeshInstance::create_multiple_convex_collisions);
	ClassDB::set_method_flags("MeshInstance", "create_multiple_convex_collisions", METHOD_FLAGS_DEFAULT);
	ClassDB::bind_method(D_METHOD("create_convex_collision", "clean", "simplify"), &MeshInstance::create_convex_collision, DEFVAL(true), DEFVAL(false));
	ClassDB::set_method_flags("MeshInstance", "create_convex_collision", METHOD_FLAGS_DEFAULT);
	ClassDB::bind_method(D_METHOD("_mesh_changed"), &MeshInstance::_mesh_changed);

#ifdef MODULE_SKELETON_3D_ENABLED
	ClassDB::bind_method(D_METHOD("_update_skinning"), &MeshInstance::_update_skinning);
#endif

	ClassDB::bind_method(D_METHOD("create_debug_tangents"), &MeshInstance::create_debug_tangents);
	ClassDB::set_method_flags("MeshInstance", "create_debug_tangents", METHOD_FLAGS_DEFAULT | METHOD_FLAG_EDITOR);

	ClassDB::bind_method(D_METHOD("is_mergeable_with", "other_mesh_instance", "shadows_only"), &MeshInstance::is_mergeable_with, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("merge_meshes", "mesh_instances", "use_global_space", "check_compatibility", "shadows_only"), &MeshInstance::merge_meshes, DEFVAL(false), DEFVAL(true), DEFVAL(false));
	ClassDB::set_method_flags("MeshInstance", "merge_meshes", METHOD_FLAGS_DEFAULT);
}

MeshInstance::MeshInstance() {
#ifdef MODULE_SKELETON_3D_ENABLED
	skeleton_path = NodePath("..");
	software_skinning = nullptr;
	software_skinning_flags = SoftwareSkinning::FLAG_TRANSFORM_NORMALS;
#endif
}

MeshInstance::~MeshInstance() {
#ifdef MODULE_SKELETON_3D_ENABLED
	if (software_skinning) {
		memdelete(software_skinning);
		software_skinning = nullptr;
	}
#endif
}
