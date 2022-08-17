#ifndef TERRAIN_2D_CHUNK_H
#define TERRAIN_2D_CHUNK_H
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

#include "core/variant/array.h"
#include "core/engine.h"
#include "core/object/resource.h"
#include "core/ustring.h"

#include "../defines.h"

#include "core/containers/pool_vector.h"

#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "core/os/thread_safe.h"

#include "scene/resources/packed_scene.h"

#include "terrain_2d_world.h"

#include "../data/terrain_2d_light.h"

#include "../meshers/terrain_2d_mesher.h"

#if PROPS_2D_PRESENT
#include "../../props_2d/props/prop_2d_data.h"
#endif

#include "scene/resources/shape_2d.h"

#include "../library/terrain_2d_library.h"
#include "../library/terrain_2d_surface.h"

//debug meshes update
//light ppu -> max(cell_size.x, cell_zise.y)
//Props->ppu -> cell size
//mesh gen-> dont scale images, put exact sized meshes, but on the grid Also pivots (just a vector2 offset should work)
//Add custom mesh transform
//create lods and use isolevel build flag remove

//chunk marbgin dfefaul = 1 ok
//reimplement colliders
// add colliders where tile is missing, but has neighbours
//Add pivot support to surfaces
//Remove faces from the surfaces, shoul donly have one.
//Scale RAO to the cell size

//Reimplement props

//Add wall layer (normal types, but gets a different mesh) ? -> TiledWalls are probably better, and they do this if props are fixed
//Also if they are not merged they can be hidden one by one (even automatically)

//mdi2d
//sprite -> convert to mdi2d
//meshinstance2d->mdi2d

class Terrain2DJob;
class Terrain2DWorld;
class Terrain2DStructure;

class Terrain2DChunk : public Resource {
	GDCLASS(Terrain2DChunk, Resource);

	_THREAD_SAFE_CLASS_

public:
	enum {
		TERRAIN_2D_CHUNK_STATE_OK = 0,
	};

public:
	bool get_process() const;
	void set_process(const bool value);

	bool get_physics_process() const;
	void set_physics_process(const bool value);

	bool get_visible() const;
	void set_visible(const bool value);

	bool get_is_generating() const;
	void set_is_generating(const bool value);

	bool is_build_aborted() const;

	bool is_in_tree() const;

	bool get_dirty() const;
	void set_dirty(const bool value);

	int get_state() const;
	void set_state(int value);

	int get_position_x() const;
	void set_position_x(const int value);
	int get_position_y() const;
	void set_position_y(const int value);

	int get_size_x() const;
	int get_size_y() const;
	void set_size_x(const int value);
	void set_size_y(const int value);

	int get_data_size_x() const;
	int get_data_size_y() const;
	void set_data_size_x(const int value);
	void set_data_size_y(const int value);

	Vector2 get_position() const;
	Vector2 get_size() const;

	Vector2 get_world_position() const;
	Vector2 get_world_size() const;
	Rect2 get_world_rect() const;

	void set_position(const int x, const int y);

	int get_margin_start() const;
	int get_margin_end() const;
	void set_margin_start(const int value);
	void set_margin_end(const int value);

	int material_cache_key_get() const;
	void material_cache_key_set(const int value);

	bool material_cache_key_has() const;
	void material_cache_key_has_set(const bool value);

	int liquid_material_cache_key_get() const;
	void liquid_material_cache_key_set(const int value);

	bool liquid_material_cache_key_has() const;
	void liquid_material_cache_key_has_set(const bool value);

	int prop_material_cache_key_get() const;
	void prop_material_cache_key_set(const int value);

	bool prop_material_cache_key_has() const;
	void prop_material_cache_key_has_set(const bool value);

	Ref<Terrain2DLibrary> get_library();
	void set_library(const Ref<Terrain2DLibrary> &value);

	int get_cell_size_x() const;
	void set_cell_size_x(const int value);

	int get_cell_size_y() const;
	void set_cell_size_y(const int value);

	Terrain2DWorld *get_voxel_world() const;
	void set_voxel_world(Terrain2DWorld *world);
	void set_voxel_world_bind(Node *world);

	Transform2D mesh_transform_terrain_get();
	void mesh_transform_terrain_set(const Transform2D &value);

	Transform2D mesh_transform_wall_north_get();
	void mesh_transform_wall_north_set(const Transform2D &value);

	Transform2D mesh_transform_wall_south_get();
	void mesh_transform_wall_south_set(const Transform2D &value);

	Transform2D mesh_transform_wall_east_get();
	void mesh_transform_wall_east_set(const Transform2D &value);

	Transform2D mesh_transform_wall_west_get();
	void mesh_transform_wall_west_set(const Transform2D &value);

	Ref<Shape2D> get_default_tile_shape();
	void set_default_tile_shape(const Ref<Shape2D> &shape);

	//Jobs
	Ref<Terrain2DJob> job_get(const int index) const;
	void job_set(const int index, const Ref<Terrain2DJob> &job);
	void job_remove(const int index);
	void job_add(const Ref<Terrain2DJob> &job);
	int job_get_count() const;

	int job_get_current_index();
	void job_next();
	Ref<Terrain2DJob> job_get_current();

	//Channels
	void channel_setup();

	void set_size(const int size_x, const int size_y, const int margin_start = 0, const int margin_end = 0);

	bool validate_data_position(const int x, const int y) const;

	uint8_t get_voxel(const int p_x, const int p_y, const int p_index) const;
	void set_voxel(const uint8_t p_value, const int p_x, const int p_y, const int p_index);

	int channel_get_count() const;
	void channel_set_count(const int count);

	bool channel_is_allocated(const int channel_index);
	void channel_ensure_allocated(const int channel_index, const uint8_t default_value = 0);
	void channel_allocate(const int channel_index, const uint8_t default_value = 0);
	void channel_fill(const uint8_t value, const int channel_index);
	void channel_dealloc(const int channel_index);

	uint8_t *channel_get(const int channel_index);
	uint8_t *channel_get_valid(const int channel_index, const uint8_t default_value = 0);

	PoolByteArray channel_get_array(const int channel_index) const;
	void channel_set_array(const int channel_index, const PoolByteArray &array);

	PoolByteArray channel_get_compressed(const int channel_index) const;
	void channel_set_compressed(const int channel_index, const PoolByteArray &data);

	int get_index(const int x, const int y) const;
	int get_data_index(const int x, const int y) const;
	int get_data_size() const;

	//Terra Structures
	Ref<Terrain2DStructure> voxel_structure_get(const int index) const;
	void voxel_structure_add(const Ref<Terrain2DStructure> &structure);
	void voxel_structure_remove(const Ref<Terrain2DStructure> &structure);
	void voxel_structure_remove_index(const int index);
	void voxel_structure_clear();
	int voxel_structure_get_count() const;
	void voxel_structure_add_at_position(Ref<Terrain2DStructure> structure, const Vector2 &world_position);

	Vector<Variant> voxel_structures_get();
	void voxel_structures_set(const Vector<Variant> &structures);

	//Meshing
	void build();
	void clear();
	void finalize_build();
	void cancel_build();

	void _build();

	//light Baking
	void bake_lights();
	void bake_light(Ref<Terrain2DLight> light);
	void clear_baked_lights();

#if PROPS_2D_PRESENT
	void prop_add(const Transform2D &tarnsform, const Ref<Prop2DData> &prop);
	Ref<Prop2DData> prop_get(const int index);
	Transform2D prop_get_tarnsform(const int index);
	int prop_get_count() const;
	void prop_remove(const int index);
	void props_clear();
#endif

#if MESH_DATA_RESOURCE_PRESENT
	int mesh_data_resource_addv(const Vector2 &local_data_pos, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture = Ref<Texture>(), const Color &color = Color(1, 1, 1, 1), const bool apply_scale = true);
	int mesh_data_resource_add(const Transform2D &local_transform, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture = Ref<Texture>(), const Color &color = Color(1, 1, 1, 1), const bool apply_scale = true);

	Ref<MeshDataResource> mesh_data_resource_get(const int index);
	void mesh_data_resource_set(const int index, const Ref<MeshDataResource> &mesh);

	Ref<Texture> mesh_data_resource_get_texture(const int index);
	void mesh_data_resource_set_texture(const int index, const Ref<Texture> &texture);

	Color mesh_data_resource_get_color(const int index);
	void mesh_data_resource_set_color(const int index, const Color &color);

	Rect2 mesh_data_resource_get_uv_rect(const int index);
	void mesh_data_resource_set_uv_rect(const int index, const Rect2 &uv_rect);

	Transform2D mesh_data_resource_get_transform(const int index);
	void mesh_data_resource_set_transform(const int index, const Transform2D &transform);

	bool mesh_data_resource_get_is_inside(const int index);
	void mesh_data_resource_set_is_inside(const int index, const bool inside);

	int mesh_data_resource_get_count() const;
	void mesh_data_resource_remove(const int index);
	void mesh_data_resource_clear();
#endif

	//Colliders
	int collider_add(const Transform2D &local_transform, const Ref<Shape2D> &shape, const RID &shape_rid = RID(), const RID &body = RID());

	Transform2D collider_get_transform(const int index);
	void collider_set_transform(const int index, const Transform2D &transform);

	Ref<Shape2D> collider_get_shape(const int index);
	void collider_set_shape(const int index, const Ref<Shape2D> &shape);

	RID collider_get_shape_rid(const int index);
	void collider_set_shape_rid(const int index, const RID &rid);

	RID collider_get_body(const int index);
	void collider_set_body(const int index, const RID &rid);

	int collider_get_count() const;
	void collider_remove(const int index);
	void colliders_clear();

	//handlers
	void enter_tree();
	void exit_tree();
	void process(const float delta);
	void physics_process(const float delta);
	void world_transform_changed();
	void visibility_changed(const bool visible);
	void world_light_added(const Ref<Terrain2DLight> &light);
	void world_light_removed(const Ref<Terrain2DLight> &light);
	void generation_process(const float delta);
	void generation_physics_process(const float delta);
	void draw();

	Transform2D get_transform() const;
	void set_transform(const Transform2D &transform);

	Transform2D get_global_transform() const;
	Vector2 to_local(Vector2 p_global) const;
	Vector2 to_global(Vector2 p_local) const;

	bool is_safe_to_delete();

	_FORCE_INLINE_ RID get_canvas_item(const int index) const {
		return _canvas_items[index];
	}
	_FORCE_INLINE_ int get_canvas_item_count() {
		return _canvas_items.size();
	}
	void setup_canvas_items_size(const int amount);

	Terrain2DChunk();
	~Terrain2DChunk();

#if TOOLS_ENABLED
	RID _debug_canvas_item;
#endif

protected:
	virtual void _enter_tree();
	virtual void _exit_tree();
	virtual void _generation_process(const float delta);
	virtual void _generation_physics_process(const float delta);

protected:
#if PROPS_2D_PRESENT
	struct Prop2DDataStore {
		Transform2D transform;
		Ref<Prop2DData> prop;
	};
#endif

#if MESH_DATA_RESOURCE_PRESENT
	struct MeshDataResourceEntry {
		Ref<MeshDataResource> mesh;
		Ref<Texture> texture;
		Color color;
		Rect2 uv_rect;
		Transform2D transform;
		bool is_inside;
	};
#endif

	struct ColliderBody {
		Transform2D transform;
		RID body;
		Ref<Shape2D> shape;
		RID shape_rid;
	};

protected:
	virtual void _world_transform_changed();

	/*
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	*/
	static void _bind_methods();

	bool _is_processing;
	bool _is_phisics_processing;

	bool _is_visible;

	bool _is_generating;
	bool _dirty;
	int _state;

	bool _is_in_tree;

	Terrain2DWorld *_voxel_world;

	int _position_x;
	int _position_y;

	int _size_x;
	int _size_y;

	int _data_size_x;
	int _data_size_y;

	int _margin_start;
	int _margin_end;

	int _material_cache_key;
	bool _material_cache_key_has;

	int _liquid_material_cache_key;
	bool _liquid_material_cache_key_has;

	int _prop_material_cache_key;
	bool _prop_material_cache_key_has;

	Vector<uint8_t *> _channels;

	int _cell_size_x;
	int _cell_size_y;

	int _current_job;
	Vector<Ref<Terrain2DJob>> _jobs;

	Ref<Terrain2DLibrary> _library;

	Vector<Ref<Terrain2DStructure>> _voxel_structures;

#if PROPS_2D_PRESENT
	Vector<Prop2DDataStore> _props;
#endif

#if MESH_DATA_RESOURCE_PRESENT
	Vector<MeshDataResourceEntry> _mesh_data_resources;
#endif

	Vector<ColliderBody> _colliders;

	Transform2D _transform;

	bool _abort_build;
	bool _queued_generation;

	Ref<Shape2D> _default_tile_shape;

	Vector<RID> _canvas_items;

	Transform2D _mesh_transform_terrain;
	Transform2D _mesh_transform_wall_north;
	Transform2D _mesh_transform_wall_south;
	Transform2D _mesh_transform_wall_east;
	Transform2D _mesh_transform_wall_west;
};

#endif
