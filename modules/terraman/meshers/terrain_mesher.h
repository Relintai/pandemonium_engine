#ifndef TERRAIN_TOOLS_H
#define TERRAIN_TOOLS_H
/*
Copyright (c) 2019-2022 Péter Magyar

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

#include "core/color.h"
#include "core/reference.h"
#include "core/vector.h"

#include "../defines.h"

#include "core/pool_vector.h"

#include "scene/3d/mesh_instance.h"

#include "core/math/rect2.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "scene/main/node.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh.h"

#ifdef MESH_DATA_RESOURCE_PRESENT
#include "../../mesh_data_resource/mesh_data_resource.h"
#endif

#include "../library/terrain_library.h"

class TerrainLibrary;
class TerrainChunk;

class TerrainMesher : public Reference {
	GDCLASS(TerrainMesher, Reference);

public:
	const double PI_2 = 3.141592653589793238463 / 2;
	const double PI = 3.141592653589793238463;

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
	int get_channel_index_type() const;
	void set_channel_index_type(const int value);

	int get_channel_index_isolevel() const;
	void set_channel_index_isolevel(const int value);

	int get_mesher_index() const;
	void set_mesher_index(const int value);

	int get_format() const;
	void set_format(const int value);

	int get_texture_scale() const;
	void set_texture_scale(const int value);

	int get_lod_index() const;
	void set_lod_index(const int value);

	Ref<TerrainLibrary> get_library();
	void set_library(const Ref<TerrainLibrary> &library);

	Ref<Material> get_material();
	void set_material(const Ref<Material> &material);

	float get_ao_strength() const;
	void set_ao_strength(const float value);

	float get_base_light_value() const;
	void set_base_light_value(const float value);

	float get_voxel_scale() const;
	void set_voxel_scale(const float voxel_scale);

	Rect2 get_uv_margin() const;
	void set_uv_margin(const Rect2 margin);

	bool get_is_liquid_mesher() const;
	void set_is_liquid_mesher(const bool value);

	void reset();

	void add_chunk(Ref<TerrainChunk> chunk);

#ifdef MESH_DATA_RESOURCE_PRESENT
	void add_mesh_data_resource(Ref<MeshDataResource> mesh, const Vector3 position = Vector3(0, 0, 0), const Vector3 rotation = Vector3(0, 0, 0), const Vector3 scale = Vector3(1.0, 1.0, 1.0), const Rect2 uv_rect = Rect2(0, 0, 1, 1));
	void add_mesh_data_resource_transform(Ref<MeshDataResource> mesh, const Transform transform, const Rect2 uv_rect = Rect2(0, 0, 1, 1));
	void add_mesh_data_resource_transform_colored(Ref<MeshDataResource> mesh, const Transform transform, const PoolColorArray &colors, const Rect2 uv_rect = Rect2(0, 0, 1, 1));
#endif

	void add_mesher(const Ref<TerrainMesher> &mesher);
	void _add_mesher(const Ref<TerrainMesher> &mesher);

	void bake_colors(Ref<TerrainChunk> chunk);
	void bake_liquid_colors(Ref<TerrainChunk> chunk);

	PoolVector<Vector3> build_collider() const;

	void bake_lights(MeshInstance *node, Vector<Ref<TerrainLight>> &lights);

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

	PoolVector<int> get_indices() const;
	void set_indices(const PoolVector<int> &values);
	int get_indices_count() const;
	int get_index(const int idx) const;
	void remove_index(const int idx);
	void add_indices(const int index);

	TerrainMesher(const Ref<TerrainLibrary> &library);
	TerrainMesher();
	~TerrainMesher();

protected:
	static void _bind_methods();

	int _channel_index_type;
	int _channel_index_isolevel;

	int _mesher_index;

	int _format;

	int _texture_scale;

	int _lod_index;

	bool _is_liquid_mesher;

	PoolVector<Vertex> _vertices;
	PoolVector<int> _indices;

	Color _last_color;
	Vector3 _last_normal;
	Vector2 _last_uv;
	Vector2 _last_uv2;
	Vector<int> _last_bones;
	Vector<float> _last_weights;
	Plane _last_tangent;

	Ref<TerrainLibrary> _library;
	Ref<Material> _material;

	float _voxel_scale;

	float _ao_strength;
	float _base_light_value;
	Rect2 _uv_margin;
};

#endif
