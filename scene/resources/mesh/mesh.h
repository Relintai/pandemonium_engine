#ifndef MESH_H
#define MESH_H

/*************************************************************************/
/*  mesh.h                                                               */
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

#include "core/containers/local_vector.h"
#include "core/math/face3.h"
#include "core/math/triangle_mesh.h"
#include "core/object/resource.h"

#include "scene/resources/material/material.h"
#include "scene/resources/shapes/shape.h"
#include "servers/rendering_server.h"

class Mesh : public Resource {
	GDCLASS(Mesh, Resource);

	mutable Ref<TriangleMesh> triangle_mesh; //cached
	mutable Vector<Vector3> debug_lines;
	Size2i lightmap_size_hint;

protected:
	static void _bind_methods();

public:
	enum {

		NO_INDEX_ARRAY = RenderingServer::NO_INDEX_ARRAY,
		ARRAY_WEIGHTS_SIZE = RenderingServer::ARRAY_WEIGHTS_SIZE
	};

	enum ArrayType {

		ARRAY_VERTEX = RenderingServer::ARRAY_VERTEX,
		ARRAY_NORMAL = RenderingServer::ARRAY_NORMAL,
		ARRAY_TANGENT = RenderingServer::ARRAY_TANGENT,
		ARRAY_COLOR = RenderingServer::ARRAY_COLOR,
		ARRAY_TEX_UV = RenderingServer::ARRAY_TEX_UV,
		ARRAY_TEX_UV2 = RenderingServer::ARRAY_TEX_UV2,
		ARRAY_BONES = RenderingServer::ARRAY_BONES,
		ARRAY_WEIGHTS = RenderingServer::ARRAY_WEIGHTS,
		ARRAY_INDEX = RenderingServer::ARRAY_INDEX,
		ARRAY_MAX = RenderingServer::ARRAY_MAX

	};

	enum ArrayFormat {
		/* ARRAY FORMAT FLAGS */
		ARRAY_FORMAT_VERTEX = 1 << ARRAY_VERTEX, // mandatory
		ARRAY_FORMAT_NORMAL = 1 << ARRAY_NORMAL,
		ARRAY_FORMAT_TANGENT = 1 << ARRAY_TANGENT,
		ARRAY_FORMAT_COLOR = 1 << ARRAY_COLOR,
		ARRAY_FORMAT_TEX_UV = 1 << ARRAY_TEX_UV,
		ARRAY_FORMAT_TEX_UV2 = 1 << ARRAY_TEX_UV2,
		ARRAY_FORMAT_BONES = 1 << ARRAY_BONES,
		ARRAY_FORMAT_WEIGHTS = 1 << ARRAY_WEIGHTS,
		ARRAY_FORMAT_INDEX = 1 << ARRAY_INDEX,

		ARRAY_COMPRESS_BASE = (ARRAY_INDEX + 1),
		ARRAY_COMPRESS_VERTEX = 1 << (ARRAY_VERTEX + ARRAY_COMPRESS_BASE), // mandatory
		ARRAY_COMPRESS_NORMAL = 1 << (ARRAY_NORMAL + ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_TANGENT = 1 << (ARRAY_TANGENT + ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_COLOR = 1 << (ARRAY_COLOR + ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_TEX_UV = 1 << (ARRAY_TEX_UV + ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_TEX_UV2 = 1 << (ARRAY_TEX_UV2 + ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_BONES = 1 << (ARRAY_BONES + ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_WEIGHTS = 1 << (ARRAY_WEIGHTS + ARRAY_COMPRESS_BASE),
		ARRAY_COMPRESS_INDEX = 1 << (ARRAY_INDEX + ARRAY_COMPRESS_BASE),

		ARRAY_FLAG_USE_2D_VERTICES = ARRAY_COMPRESS_INDEX << 1,
		ARRAY_FLAG_USE_16_BIT_BONES = ARRAY_COMPRESS_INDEX << 2,
		ARRAY_FLAG_USE_DYNAMIC_UPDATE = ARRAY_COMPRESS_INDEX << 3,
		ARRAY_FLAG_USE_OCTAHEDRAL_COMPRESSION = ARRAY_COMPRESS_INDEX << 4,
		ARRAY_FLAG_USE_VERTEX_CACHE_OPTIMIZATION = ARRAY_COMPRESS_INDEX << 5,

		ARRAY_COMPRESS_DEFAULT = ARRAY_COMPRESS_NORMAL | ARRAY_COMPRESS_TANGENT | ARRAY_COMPRESS_COLOR | ARRAY_COMPRESS_TEX_UV | ARRAY_COMPRESS_TEX_UV2 | ARRAY_COMPRESS_WEIGHTS | ARRAY_FLAG_USE_OCTAHEDRAL_COMPRESSION

	};

	enum PrimitiveType {
		PRIMITIVE_POINTS = RenderingServer::PRIMITIVE_POINTS,
		PRIMITIVE_LINES = RenderingServer::PRIMITIVE_LINES,
		PRIMITIVE_LINE_STRIP = RenderingServer::PRIMITIVE_LINE_STRIP,
		PRIMITIVE_LINE_LOOP = RenderingServer::PRIMITIVE_LINE_LOOP,
		PRIMITIVE_TRIANGLES = RenderingServer::PRIMITIVE_TRIANGLES,
		PRIMITIVE_TRIANGLE_STRIP = RenderingServer::PRIMITIVE_TRIANGLE_STRIP,
		PRIMITIVE_TRIANGLE_FAN = RenderingServer::PRIMITIVE_TRIANGLE_FAN,
	};

	enum BlendShapeMode {

		BLEND_SHAPE_MODE_NORMALIZED = RS::BLEND_SHAPE_MODE_NORMALIZED,
		BLEND_SHAPE_MODE_RELATIVE = RS::BLEND_SHAPE_MODE_RELATIVE,
	};

	enum StorageMode {
		STORAGE_MODE_GPU,
		STORAGE_MODE_CPU,
		STORAGE_MODE_CPU_AND_GPU,
	};

	struct CachedStats {
		bool dirty = true;
		uint32_t triangle_count = 0;
		uint32_t vertex_count = 0;
		uint32_t index_count = 0;
		uint32_t array_format = 0;
	};

	virtual int get_surface_count() const = 0;
	virtual int surface_get_array_len(int p_idx) const = 0;
	virtual int surface_get_array_index_len(int p_idx) const = 0;
	virtual bool surface_is_softbody_friendly(int p_idx) const;
	virtual Array surface_get_arrays(int p_surface) const = 0;
	virtual Array surface_get_blend_shape_arrays(int p_surface) const = 0;
	virtual uint32_t surface_get_format(int p_idx) const = 0;
	virtual PrimitiveType surface_get_primitive_type(int p_idx) const = 0;
	virtual void surface_set_material(int p_idx, const Ref<Material> &p_material) = 0;
	virtual Ref<Material> surface_get_material(int p_idx) const = 0;
	virtual int get_blend_shape_count() const = 0;
	int surface_get_triangle_count(int p_idx) const;
	int surface_get_index_count(int p_idx) const;
	virtual StringName get_blend_shape_name(int p_index) const = 0;
	virtual void set_blend_shape_name(int p_index, const StringName &p_name) = 0;

	int get_triangle_count() const;
	PoolVector<Face3> get_faces() const;
#ifdef TOOLS_ENABLED
	const CachedStats &get_cached_stats() const;
#endif
	Ref<TriangleMesh> generate_triangle_mesh() const;
	Ref<TriangleMesh> generate_triangle_mesh_from_aabb() const;
	void generate_debug_mesh_lines(Vector<Vector3> &r_lines);
	void generate_debug_mesh_indices(Vector<Vector3> &r_points);

	Ref<Shape> create_trimesh_shape() const;
	Ref<Shape> create_convex_shape(bool p_clean = true, bool p_simplify = false) const;

	Ref<Mesh> create_outline(float p_margin) const;

	virtual AABB get_aabb() const = 0;
	virtual void set_storage_mode(StorageMode p_storage_mode);

	void set_lightmap_size_hint(const Vector2i &p_size);
	Size2i get_lightmap_size_hint() const;

	void clear_cache() const;

	typedef Vector<PoolVector<Vector3>> (*ConvexDecompositionFunc)(const real_t *p_vertices, int p_vertex_count, const uint32_t *p_triangles, int p_triangle_count, int p_max_convex_hulls, Vector<PoolVector<uint32_t>> *r_convex_indices);

	static ConvexDecompositionFunc convex_decomposition_function;

	Vector<Ref<Shape>> convex_decompose(int p_max_convex_hulls = -1) const;

	Mesh();

private:
#ifdef TOOLS_ENABLED
	// Only for use in the editor.
	// No need to bloat exports.
	mutable CachedStats _cached_stats;
#endif
};

class ArrayMesh : public Mesh {
	GDCLASS(ArrayMesh, Mesh);
	RES_BASE_EXTENSION("mesh");

private:
	// Storing the mesh data on CPU
	struct CPUSurface {
		Array arrays;
		Array blend_shapes;
		PrimitiveType primitive_type;
		int num_verts = 0;
		int num_inds = 0;
	};

	struct Surface {
		String name;
		AABB aabb;
		Ref<Material> material;
		bool is_2d;

		// Watch for bugs here.
		// When calling add_surface() rather than add_surface_from_arrays(),
		// the creation flags will be unset, and left at default.
		// Conversion from CPU to GPU memory assumes that creation_flags are
		// correct, so is only TRULY safe when used with add_surface_from_arrays().
		uint32_t creation_flags = ARRAY_COMPRESS_DEFAULT;
		uint32_t creation_format = 0;
	};

	Vector<Surface> surfaces;
	LocalVector<CPUSurface *> _cpu_surfaces;
	RID mesh;
	AABB aabb;
	BlendShapeMode blend_shape_mode;
	Vector<StringName> blend_shapes;
	AABB custom_aabb;

	void _recompute_aabb();

	// Data can be held on GPU, CPU or both.
	// CPU is quicker for modifications, but can't be
	// used for rendering.
	bool _on_cpu = false;
	bool _on_gpu = true;
	StorageMode _storage_mode = STORAGE_MODE_GPU;

	void add_surface_from_arrays_cpu(PrimitiveType p_primitive, const Array &p_arrays, const Array &p_blend_shapes);
	void add_surface_from_arrays_cpu_with_probe(PrimitiveType p_primitive, const Array &p_arrays, const Array &p_blend_shapes, uint32_t p_flags, int p_surface_id);
	void clear_cpu_surfaces();
	bool on_cpu() const { return _on_cpu && ((int)_cpu_surfaces.size() == surfaces.size()); }

protected:
	virtual bool _is_generated() const { return false; }

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	static void _bind_methods();

public:
	void add_surface_from_arrays(PrimitiveType p_primitive, const Array &p_arrays, const Array &p_blend_shapes = Array(), uint32_t p_flags = ARRAY_COMPRESS_DEFAULT);
	void add_surface(uint32_t p_format, PrimitiveType p_primitive, const PoolVector<uint8_t> &p_array, int p_vertex_count, const PoolVector<uint8_t> &p_index_array, int p_index_count, const AABB &p_aabb, const Vector<PoolVector<uint8_t>> &p_blend_shapes = Vector<PoolVector<uint8_t>>(), const Vector<AABB> &p_bone_aabbs = Vector<AABB>());

	Array surface_get_arrays(int p_surface) const;
	Array surface_get_blend_shape_arrays(int p_surface) const;

	void add_blend_shape(const StringName &p_name);
	int get_blend_shape_count() const;
	StringName get_blend_shape_name(int p_index) const;
	void set_blend_shape_name(int p_index, const StringName &p_name);
	void clear_blend_shapes();

	void set_blend_shape_mode(BlendShapeMode p_mode);
	BlendShapeMode get_blend_shape_mode() const;

	void surface_update_region(int p_surface, int p_offset, const PoolVector<uint8_t> &p_data);

	int get_surface_count() const;
	void surface_remove(int p_idx);
	void clear_surfaces();

	void surface_set_custom_aabb(int p_idx, const AABB &p_aabb); //only recognized by driver

	int surface_get_array_len(int p_idx) const;
	int surface_get_array_index_len(int p_idx) const;
	uint32_t surface_get_format(int p_idx) const;
	PrimitiveType surface_get_primitive_type(int p_idx) const;

	virtual void surface_set_material(int p_idx, const Ref<Material> &p_material);
	virtual Ref<Material> surface_get_material(int p_idx) const;

	int surface_find_by_name(const String &p_name) const;
	void surface_set_name(int p_idx, const String &p_name);
	String surface_get_name(int p_idx) const;

	void add_surface_from_mesh_data(const Geometry::MeshData &p_mesh_data);

	void set_custom_aabb(const AABB &p_custom);
	AABB get_custom_aabb() const;

	AABB get_aabb() const;
	virtual RID get_rid() const;

	void regen_normalmaps();

	virtual void reload_from_file();

	virtual void set_storage_mode(StorageMode p_storage_mode);

	ArrayMesh();

	~ArrayMesh();
};

VARIANT_ENUM_CAST(Mesh::ArrayType);
VARIANT_ENUM_CAST(Mesh::ArrayFormat);
VARIANT_ENUM_CAST(Mesh::PrimitiveType);
VARIANT_ENUM_CAST(Mesh::BlendShapeMode);

#endif
