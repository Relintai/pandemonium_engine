#ifndef TERRAIN_2D_TOOLS_H
#define TERRAIN_2D_TOOLS_H

/*************************************************************************/
/*  terrain_2d_mesher.h                                                  */
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

#include "core/containers/vector.h"
#include "core/math/color.h"
#include "core/object/reference.h"

#include "../defines.h"

#include "core/containers/pool_vector.h"
#include "scene/3d/mesh_instance.h"

#include "core/math/rect2.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "scene/main/node.h"
#include "scene/resources/material/material.h"
#include "scene/resources/mesh/mesh.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../../mesh_data_resource/mesh_data_resource.h"
#endif

#include "scene/resources/shapes_2d/shape_2d.h"

#include "../library/terrain_2d_library.h"

class Terrain2DLibrary;
class Terrain2DChunk;

class Terrain2DMesher : public Reference {
	GDCLASS(Terrain2DMesher, Reference);

public:
	const double PI_2 = 3.141592653589793238463 / 2;
	const double PI = 3.141592653589793238463;

	struct Vertex {
		Vector2 vertex;
		Color color;
		Vector2 uv;
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

	int get_format() const;
	void set_format(const int value);

	int get_texture_scale() const;
	void set_texture_scale(const int value);

	Transform2D get_mesh_transform();
	void set_mesh_transform(const Transform2D &value);

	Ref<Terrain2DLibrary> get_library();
	void set_library(const Ref<Terrain2DLibrary> &library);

	Ref<Material> get_material();
	void set_material(const Ref<Material> &material);

	float get_ao_strength() const;
	void set_ao_strength(const float value);

	float get_base_light_value() const;
	void set_base_light_value(const float value);

	int get_cell_size_x() const;
	void set_cell_size_x(const int val);

	int get_cell_size_y() const;
	void set_cell_size_y(const int val);

	Rect2 get_uv_margin() const;
	void set_uv_margin(const Rect2 margin);

	virtual void reset();

	void add_chunk(Ref<Terrain2DChunk> chunk);

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	void add_mesh_data_resource(Ref<MeshDataResource> mesh, const Vector3 position = Vector3(0, 0, 0), const Vector3 rotation = Vector3(0, 0, 0), const Vector3 scale = Vector3(1.0, 1.0, 1.0), const Rect2 uv_rect = Rect2(0, 0, 1, 1));
	void add_mesh_data_resource_transform(Ref<MeshDataResource> mesh, const Transform2D transform, const Rect2 uv_rect = Rect2(0, 0, 1, 1));
	void add_mesh_data_resource_transform_colored(Ref<MeshDataResource> mesh, const Transform2D transform, const PoolColorArray &colors, const Rect2 uv_rect = Rect2(0, 0, 1, 1));
#endif

	void add_mesher(const Ref<Terrain2DMesher> &mesher);
	void _add_mesher(const Ref<Terrain2DMesher> &mesher);

	void bake_colors(Ref<Terrain2DChunk> chunk);
	void bake_liquid_colors(Ref<Terrain2DChunk> chunk);

	PoolVector<Vector2> build_collider() const;
	virtual void create_tile_colliders(Ref<Terrain2DChunk> chunk);

	void bake_lights(MeshInstance *node, Vector<Ref<Terrain2DLight>> &lights);

	Array build_mesh();
	void build_mesh_into(RID mesh);

	void remove_doubles();
	void remove_doubles_hashed();

	void store_mesh();
	int get_stored_mesh_count() const;
	Array build_stored_mesh(const int index);
	Transform2D get_stored_mesh_transform(const int index);
	void build_stored_mesh_into(const int index, RID mesh);
	AABB calculate_stored_mesh_aabb(const int index);

	virtual Ref<Shape2D> create_terrain_tile_collider_shape(Ref<Terrain2DChunk> chunk);

	PoolVector<Vector2> get_vertices() const;
	void set_vertices(const PoolVector<Vector2> &values);
	int get_vertex_count() const;
	Vector2 get_vertex(const int idx) const;
	void remove_vertex(const int idx);
	void add_vertex(const Vector2 &vertex);

	PoolVector<Color> get_colors() const;
	void set_colors(const PoolVector<Color> &values);
	Color get_color(const int idx) const;
	void add_color(const Color &color);

	PoolVector<Vector2> get_uvs() const;
	void set_uvs(const PoolVector<Vector2> &values);
	Vector2 get_uv(const int idx) const;
	void add_uv(const Vector2 &vector);

	PoolVector<int> get_indices() const;
	void set_indices(const PoolVector<int> &values);
	int get_indices_count() const;
	int get_index(const int idx) const;
	void remove_index(const int idx);
	void add_indices(const int index);

	Terrain2DMesher(const Ref<Terrain2DLibrary> &library);
	Terrain2DMesher();
	~Terrain2DMesher();

protected:
	static void _bind_methods();

	int _channel_index_type;
	int _format;

	int _texture_scale;

	Transform2D _mesh_transform;

	PoolVector<Vertex> _vertices;
	PoolVector<int> _indices;

	Color _last_color;
	Vector2 _last_uv;
	Vector<int> _last_bones;
	Vector<float> _last_weights;

	Ref<Terrain2DLibrary> _library;
	Ref<Material> _material;

	int _cell_size_x;
	int _cell_size_y;

	float _ao_strength;
	float _base_light_value;
	Rect2 _uv_margin;

	struct Terrain2DMesherStoredMesh {
		PoolVector<Vertex> vertices;
		PoolVector<int> indices;
		Transform2D transform;
	};

	Vector<Terrain2DMesherStoredMesh> _stored_meshes;
};

#endif
