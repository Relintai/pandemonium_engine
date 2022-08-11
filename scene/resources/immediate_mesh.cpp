/*************************************************************************/
/*  immediate_mesh.cpp                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "immediate_mesh.h"

void ImmediateMesh::surface_begin(PrimitiveType p_primitive, const Ref<Material> &p_material) {
	ERR_FAIL_COND_MSG(surface_active, "Already creating a new surface.");
	active_surface_data.primitive = p_primitive;
	active_surface_data.material = p_material;
	surface_active = true;
}
void ImmediateMesh::surface_set_color(const Color &p_color) {
	ERR_FAIL_COND_MSG(!surface_active, "Not creating any surface. Use surface_begin() to do it.");

	if (!uses_colors) {
		colors.resize(vertices.size());
		for (int i = 0; i < colors.size(); i++) {
			colors.write[i] = p_color;
		}
		uses_colors = true;
	}

	current_color = p_color;
}
void ImmediateMesh::surface_set_normal(const Vector3 &p_normal) {
	ERR_FAIL_COND_MSG(!surface_active, "Not creating any surface. Use surface_begin() to do it.");

	if (!uses_normals) {
		normals.resize(vertices.size());
		for (int i = 0; i < normals.size(); i++) {
			normals.write[i] = p_normal;
		}
		uses_normals = true;
	}

	current_normal = p_normal;
}
void ImmediateMesh::surface_set_tangent(const Plane &p_tangent) {
	ERR_FAIL_COND_MSG(!surface_active, "Not creating any surface. Use surface_begin() to do it.");
	if (!uses_tangents) {
		tangents.resize(vertices.size());
		for (int i = 0; i < tangents.size(); i++) {
			tangents.write[i] = p_tangent;
		}
		uses_tangents = true;
	}

	current_tangent = p_tangent;
}
void ImmediateMesh::surface_set_uv(const Vector2 &p_uv) {
	ERR_FAIL_COND_MSG(!surface_active, "Not creating any surface. Use surface_begin() to do it.");
	if (!uses_uvs) {
		uvs.resize(vertices.size());
		for (int i = 0; i < uvs.size(); i++) {
			uvs.write[i] = p_uv;
		}
		uses_uvs = true;
	}

	current_uv = p_uv;
}
void ImmediateMesh::surface_set_uv2(const Vector2 &p_uv2) {
	ERR_FAIL_COND_MSG(!surface_active, "Not creating any surface. Use surface_begin() to do it.");
	if (!uses_uv2s) {
		uv2s.resize(vertices.size());
		for (int i = 0; i < uv2s.size(); i++) {
			uv2s.write[i] = p_uv2;
		}
		uses_uv2s = true;
	}

	current_uv2 = p_uv2;
}
void ImmediateMesh::surface_add_vertex(const Vector3 &p_vertex) {
	ERR_FAIL_COND_MSG(!surface_active, "Not creating any surface. Use surface_begin() to do it.");
	ERR_FAIL_COND_MSG(vertices.size() && active_surface_data.vertex_2d, "Can't mix 2D and 3D vertices in a surface.");

	if (uses_colors) {
		colors.push_back(current_color);
	}
	if (uses_normals) {
		normals.push_back(current_normal);
	}
	if (uses_tangents) {
		tangents.push_back(current_tangent);
	}
	if (uses_uvs) {
		uvs.push_back(current_uv);
	}
	if (uses_uv2s) {
		uv2s.push_back(current_uv2);
	}
	vertices.push_back(p_vertex);
}

void ImmediateMesh::surface_add_vertex_2d(const Vector2 &p_vertex) {
	ERR_FAIL_COND_MSG(!surface_active, "Not creating any surface. Use surface_begin() to do it.");
	ERR_FAIL_COND_MSG(vertices.size() && !active_surface_data.vertex_2d, "Can't mix 2D and 3D vertices in a surface.");

	if (uses_colors) {
		colors.push_back(current_color);
	}
	if (uses_normals) {
		normals.push_back(current_normal);
	}
	if (uses_tangents) {
		tangents.push_back(current_tangent);
	}
	if (uses_uvs) {
		uvs.push_back(current_uv);
	}
	if (uses_uv2s) {
		uv2s.push_back(current_uv2);
	}
	Vector3 v(p_vertex.x, p_vertex.y, 0);
	vertices.push_back(v);

	active_surface_data.vertex_2d = true;
}

void ImmediateMesh::surface_end() {
	ERR_FAIL_COND_MSG(!surface_active, "Not creating any surface. Use surface_begin() to do it.");
	ERR_FAIL_COND_MSG(!vertices.size(), "No vertices were added, surface can't be created.");

	uint32_t format = ARRAY_FORMAT_VERTEX;

	uint32_t vertex_stride = 0;
	if (active_surface_data.vertex_2d) {
		format |= ARRAY_FLAG_USE_2D_VERTICES;
		vertex_stride = sizeof(float) * 2;
	} else {
		vertex_stride = sizeof(float) * 3;
	}

	uint32_t normal_offset = 0;
	if (uses_normals) {
		format |= ARRAY_FORMAT_NORMAL;
		normal_offset = vertex_stride;
		vertex_stride += sizeof(float) * 3;
	}
	uint32_t tangent_offset = 0;
	if (uses_tangents) {
		format |= ARRAY_FORMAT_TANGENT;
		tangent_offset = vertex_stride;
		vertex_stride += sizeof(float) * 4;
	}

	uint32_t color_offset = 0;
	if (uses_colors) {
		format |= ARRAY_FORMAT_COLOR;
		color_offset = vertex_stride;
		vertex_stride += sizeof(float) * 4;
	}
	uint32_t uv_offset = 0;
	if (uses_uvs) {
		format |= ARRAY_FORMAT_TEX_UV;
		uv_offset = vertex_stride;
		vertex_stride += sizeof(float) * 2;
	}
	uint32_t uv2_offset = 0;
	if (uses_uv2s) {
		format |= ARRAY_FORMAT_TEX_UV2;
		uv2_offset = vertex_stride;
		vertex_stride += sizeof(float) * 2;
	}

	AABB aabb;

	{
		surface_array_create_cache.resize(vertex_stride * vertices.size());

		PoolVector<uint8_t>::Write surface_array_write = surface_array_create_cache.write();
		uint8_t *surface_vertex_ptr = surface_array_write.ptr();

		for (int i = 0; i < vertices.size(); i++) {
			{
				float *vtx = (float *)&surface_vertex_ptr[i * vertex_stride];
				vtx[0] = vertices[i].x;
				vtx[1] = vertices[i].y;
				if (!active_surface_data.vertex_2d) {
					vtx[2] = vertices[i].z;
				}
				if (i == 0) {
					aabb = AABB(vertices[i], SMALL_VEC3); // Must have a bit of size.
				} else {
					aabb.expand_to(vertices[i]);
				}
			}
			if (uses_normals) {
				Vector3 *normal = (Vector3 *)&surface_vertex_ptr[i * vertex_stride + normal_offset];
				Vector3 n = normals[i];
				*normal = n;
			}
			if (uses_tangents) {
				Plane *tangent = (Plane *)&surface_vertex_ptr[i * vertex_stride + tangent_offset];
				Plane t = tangents[i];
				*tangent = t;
			}
		}
	}

	if (uses_colors || uses_uvs || uses_uv2s) {
		PoolVector<uint8_t>::Write surface_array_write = surface_array_create_cache.write();
		uint8_t *surface_attribute_ptr = surface_array_write.ptr();

		for (int i = 0; i < vertices.size(); i++) {
			if (uses_colors) {
				float *color = (float *)&surface_attribute_ptr[i * vertex_stride + color_offset];
				Color c = colors[i];

				color[0] = c.r;
				color[1] = c.g;
				color[2] = c.b;
				color[3] = c.a;
			}
			if (uses_uvs) {
				float *uv = (float *)&surface_attribute_ptr[i * vertex_stride + uv_offset];

				uv[0] = uvs[i].x;
				uv[1] = uvs[i].y;
			}

			if (uses_uv2s) {
				float *uv2 = (float *)&surface_attribute_ptr[i * vertex_stride + uv2_offset];

				uv2[0] = uv2s[i].x;
				uv2[1] = uv2s[i].y;
			}
		}
	}

	int sc = VisualServer::get_singleton()->mesh_get_surface_count(mesh);

	for (int i = 0; i < sc; ++i) {
		VisualServer::get_singleton()->mesh_remove_surface(mesh, 0);
	}

	VisualServer::get_singleton()->mesh_add_surface(mesh, format, static_cast<VS::PrimitiveType>(active_surface_data.primitive), surface_array_create_cache, vertices.size(), PoolByteArray(), 0, aabb);

	if (active_surface_data.material.is_valid()) {
		VisualServer::get_singleton()->mesh_surface_set_material(mesh, surfaces.size(), active_surface_data.material->get_rid());
	}

	active_surface_data.aabb = aabb;

	active_surface_data.format = format;
	active_surface_data.array_len = vertices.size();

	surfaces.push_back(active_surface_data);

	colors.clear();
	normals.clear();
	tangents.clear();
	uvs.clear();
	uv2s.clear();
	vertices.clear();

	uses_colors = false;
	uses_normals = false;
	uses_tangents = false;
	uses_uvs = false;
	uses_uv2s = false;

	surface_active = false;
}

void ImmediateMesh::clear_surfaces() {
	VS::get_singleton()->mesh_clear(mesh);
	surfaces.clear();
	surface_active = false;

	colors.clear();
	normals.clear();
	tangents.clear();
	uvs.clear();
	uv2s.clear();
	vertices.clear();

	uses_colors = false;
	uses_normals = false;
	uses_tangents = false;
	uses_uvs = false;
	uses_uv2s = false;
}

int ImmediateMesh::get_surface_count() const {
	return surfaces.size();
}
int ImmediateMesh::surface_get_array_len(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, int(surfaces.size()), -1);
	return surfaces[p_idx].array_len;
}
int ImmediateMesh::surface_get_array_index_len(int p_idx) const {
	return 0;
}
Array ImmediateMesh::surface_get_arrays(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, int(surfaces.size()), Array());
	return VS::get_singleton()->mesh_surface_get_arrays(mesh, p_surface);
}
Array ImmediateMesh::surface_get_blend_shape_arrays(int p_surface) const {
	return Array();
}
Dictionary ImmediateMesh::surface_get_lods(int p_surface) const {
	return Dictionary();
}
uint32_t ImmediateMesh::surface_get_format(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, int(surfaces.size()), 0);
	return surfaces[p_idx].format;
}
Mesh::PrimitiveType ImmediateMesh::surface_get_primitive_type(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, surfaces.size(), PRIMITIVE_TRIANGLES);
	return surfaces[p_idx].primitive;
}
void ImmediateMesh::surface_set_material(int p_idx, const Ref<Material> &p_material) {
	ERR_FAIL_INDEX(p_idx, int(surfaces.size()));
	surfaces.write[p_idx].material = p_material;
	RID mat;
	if (p_material.is_valid()) {
		mat = p_material->get_rid();
	}
	VS::get_singleton()->mesh_surface_set_material(mesh, p_idx, mat);
}
Ref<Material> ImmediateMesh::surface_get_material(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, int(surfaces.size()), Ref<Material>());
	return surfaces[p_idx].material;
}
int ImmediateMesh::get_blend_shape_count() const {
	return 0;
}
StringName ImmediateMesh::get_blend_shape_name(int p_index) const {
	return StringName();
}
void ImmediateMesh::set_blend_shape_name(int p_index, const StringName &p_name) {
}

AABB ImmediateMesh::get_aabb() const {
	AABB aabb;
	for (int i = 0; i < surfaces.size(); i++) {
		if (i == 0) {
			aabb = surfaces[i].aabb;
		} else {
			aabb = aabb.merge(surfaces[i].aabb);
		}
	}
	return aabb;
}

void ImmediateMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("surface_begin", "primitive", "material"), &ImmediateMesh::surface_begin, DEFVAL(Ref<Material>()));
	ClassDB::bind_method(D_METHOD("surface_set_color", "color"), &ImmediateMesh::surface_set_color);
	ClassDB::bind_method(D_METHOD("surface_set_normal", "normal"), &ImmediateMesh::surface_set_normal);
	ClassDB::bind_method(D_METHOD("surface_set_tangent", "tangent"), &ImmediateMesh::surface_set_tangent);
	ClassDB::bind_method(D_METHOD("surface_set_uv", "uv"), &ImmediateMesh::surface_set_uv);
	ClassDB::bind_method(D_METHOD("surface_set_uv2", "uv2"), &ImmediateMesh::surface_set_uv2);
	ClassDB::bind_method(D_METHOD("surface_add_vertex", "vertex"), &ImmediateMesh::surface_add_vertex);
	ClassDB::bind_method(D_METHOD("surface_add_vertex_2d", "vertex"), &ImmediateMesh::surface_add_vertex_2d);
	ClassDB::bind_method(D_METHOD("surface_end"), &ImmediateMesh::surface_end);

	ClassDB::bind_method(D_METHOD("clear_surfaces"), &ImmediateMesh::clear_surfaces);
}

RID ImmediateMesh::get_rid() const {
	return mesh;
}

ImmediateMesh::ImmediateMesh() {
	mesh = VS::get_singleton()->mesh_create();
}
ImmediateMesh::~ImmediateMesh() {
	VS::get_singleton()->free(mesh);
}
