/*

Copyright (c) 2020-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef MESH_MERGER_H
#define MESH_MERGER_H

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/object/reference.h"
#include "core/templates/vector.h"
#include "core/math/color.h"
#else
#include "core/reference.h"
#include "core/vector.h"
#include "core/color.h"
#endif

#include "defines.h"

#include pool_vector_h
include_pool_vector

#include mesh_instance_h

#include "core/math/rect2.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "scene/main/node.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh.h"

#ifdef MESH_DATA_RESOURCE_PRESENT
#include "../mesh_data_resource/mesh_data_resource.h"
#endif

		class MeshMerger : public Reference {
	GDCLASS(MeshMerger, Reference);

public:
	struct Vertex {

		Vector3 vertex;
		Color color;
		Vector3 normal; // normal, binormal, tangent
		Vector3 binormal;
		Vector3 tangent;
		Vector2 uv;
		Vector2 uv2;
		Vector<int> bones;
		Vector<float> weights;

		bool operator==(const Vertex &p_vertex) const;

		Vertex() {}
	};

	struct VertexHasher {
		static _FORCE_INLINE_ uint32_t hash(const Vertex &p_vtx);
	};

	struct WeightSort {
		int index;
		float weight;
		bool operator<(const WeightSort &p_right) const {
			return weight < p_right.weight;
		}
	};

public:
	int get_mesher_index() const;
	void set_mesher_index(const int value);

	int get_format() const;
	void set_format(const int value);

	Ref<Material> get_material();
	void set_material(const Ref<Material> &material);

	float get_ao_strength() const;
	void set_ao_strength(const float value);

	float get_base_light_value() const;
	void set_base_light_value(const float value);

	float get_voxel_scale() const;
	void set_voxel_scale(const float voxel_scale);

	int get_lod_size() const;
	void set_lod_size(const int lod_size);

	Rect2 get_uv_margin() const;
	void set_uv_margin(const Rect2 margin);

	void reset();

#ifdef MESH_DATA_RESOURCE_PRESENT
	void add_mesh_data_resource(Ref<MeshDataResource> mesh, const Transform transform = Transform(), const Rect2 uv_rect = Rect2(0, 0, 1, 1));
	void add_mesh_data_resource_bone(Ref<MeshDataResource> mesh, const Vector<int> &bones, const Vector<float> &weights, const Transform transform = Transform(), const Rect2 uv_rect = Rect2(0, 0, 1, 1));
#endif

	void add_mesher(const Ref<MeshMerger> &mesher);

	PoolVector<Vector3> build_collider() const;

	Array build_mesh();
	void build_mesh_into(RID mesh);

	void generate_normals(bool p_flip = false);
	void remove_doubles();
	void remove_doubles_hashed();

	PoolVector<Vector3> get_vertices() const;
	void set_vertices(const PoolVector<Vector3> &values);
	int get_vertex_count() const;
	Vector3 get_vertex(const int idx) const;
	void remove_vertex(const int idx);
	void add_vertex(const Vector3 &vertex);

	PoolVector<Vector3> get_normals() const;
	void set_normals(const PoolVector<Vector3> &values);
	Vector3 get_normal(const int idx) const;
	void add_normal(const Vector3 &normal);

	PoolVector<Color> get_colors() const;
	void set_colors(const PoolVector<Color> &values);
	Color get_color(const int idx) const;
	void add_color(const Color &color);

	PoolVector<Vector2> get_uvs() const;
	void set_uvs(const PoolVector<Vector2> &values);
	Vector2 get_uv(const int idx) const;
	void add_uv(const Vector2 &vector);

	PoolVector<Vector2> get_uv2s() const;
	void set_uv2s(const PoolVector<Vector2> &values);
	Vector2 get_uv2(const int idx) const;
	void add_uv2(const Vector2 &vector);

	Vector<int> get_bones(const int idx) const;
	void add_bones(const Vector<int> &arr);

	Vector<float> get_bone_weights(const int idx) const;
	void add_bone_weights(const Vector<float> &arr);

	PoolVector<int> get_indices() const;
	void set_indices(const PoolVector<int> &values);
	int get_indices_count() const;
	int get_index(const int idx) const;
	void remove_index(const int idx);
	void add_indices(const int index);

	MeshMerger();
	~MeshMerger();

protected:
	static void _bind_methods();

	int _mesher_index;

	int _format;

	PoolVector<Vertex> _vertices;
	PoolVector<int> _indices;

	Color _last_color;
	Vector3 _last_normal;
	Vector2 _last_uv;
	Vector2 _last_uv2;
	Vector<int> _last_bones;
	Vector<float> _last_weights;
	Plane _last_tangent;

	Ref<Material> _material;

	float _voxel_scale;
	int _lod_size;

	float _ao_strength;
	float _base_light_value;
	Rect2 _uv_margin;
};

#endif
