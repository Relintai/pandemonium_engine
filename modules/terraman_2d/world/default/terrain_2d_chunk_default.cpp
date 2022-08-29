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

#include "terrain_2d_chunk_default.h"

#include "../../defines.h"

#include "servers/physics_server.h"
#include "servers/rendering_server.h"

#include "../../../opensimplex/open_simplex_noise.h"
#include "../../meshers/default/terrain_2d_mesher_default.h"
#include "../terrain_2d_world.h"

#include "../jobs/terrain_2d_job.h"

#include "terrain_2d_world_default.h"

#include "../jobs/terrain_2d_light_job.h"
#include "../jobs/terrain_2d_prop_job.h"
#include "../jobs/terrain_2d_terrain_job.h"
#include "scene/resources/world_3d.h"
#include "scene/resources/world_2d.h"
#include "servers/physics_2d_server.h"

const String Terrain2DChunkDefault::BINDING_STRING_BUILD_FLAGS = "Use Lighting,Use AO,Use RAO,Generate AO,Generate RAO,Bake Lights,Create Collider";

_FORCE_INLINE_ int Terrain2DChunkDefault::get_build_flags() const {
	return _build_flags;
}
_FORCE_INLINE_ void Terrain2DChunkDefault::set_build_flags(const int flags) {
	_build_flags = flags;
}

bool Terrain2DChunkDefault::get_lights_dirty() const {
	return _lights_dirty;
}
void Terrain2DChunkDefault::set_lights_dirty(const bool value) {
	_lights_dirty = value;
}

void Terrain2DChunkDefault::emit_build_finished() {
	emit_signal("mesh_generation_finished", this);

	if (_voxel_world != NULL) {
		_voxel_world->on_chunk_mesh_generation_finished(this);
	}
}

//Meshes
Dictionary Terrain2DChunkDefault::mesh_rids_get() {
	return _rids;
}
void Terrain2DChunkDefault::mesh_rids_set(const Dictionary &rids) {
	_rids = rids;
}

RID Terrain2DChunkDefault::mesh_rid_get(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return RID();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return RID();

	Variant v = m[mesh_type_index];

	if (v.get_type() != Variant::RID)
		return RID();

	return v;
}
void Terrain2DChunkDefault::mesh_rid_set(const int mesh_index, const int mesh_type_index, RID value) {
	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index)) {
		m[mesh_type_index] = value;
		_rids[mesh_index] = m;
		return;
	}

	Variant v = m[mesh_type_index];

	ERR_FAIL_COND(v.get_type() != Variant::RID);

	m[mesh_type_index] = value;
	_rids[mesh_index] = m;
}
RID Terrain2DChunkDefault::mesh_rid_get_index(const int mesh_index, const int mesh_type_index, const int index) {
	if (!_rids.has(mesh_index))
		return RID();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return RID();

	Variant v = m[mesh_type_index];

	if (v.get_type() != Variant::ARRAY)
		return RID();

	Array arr = v;

	ERR_FAIL_INDEX_V(index, arr.size(), RID());

	return arr[index];
}
void Terrain2DChunkDefault::mesh_rid_set_index(const int mesh_index, const int mesh_type_index, const int index, RID value) {
	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index)) {
		Array arr;
		arr.resize(index + 1);
		arr[index] = value;

		m[mesh_type_index] = arr;
		_rids[mesh_index] = m;
		return;
	}

	Variant v = m[mesh_type_index];

	ERR_FAIL_COND(v.get_type() != Variant::ARRAY);

	Array arr = m[mesh_type_index];

	if (arr.size() <= index)
		arr.resize(index + 1);

	arr[index] = value;

	m[mesh_type_index] = arr;
	_rids[mesh_index] = m;
}
int Terrain2DChunkDefault::mesh_rid_get_count(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return 0;

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return 0;

	Variant v = m[mesh_type_index];

	if (v.get_type() != Variant::ARRAY)
		return 0;

	Array arr = v;

	return arr.size();
}
void Terrain2DChunkDefault::mesh_rids_clear(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return;

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return;

	m.erase(mesh_type_index);
}
Array Terrain2DChunkDefault::meshes_get(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return Array();

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return Array();

	Variant v = m[mesh_type_index];

	if (v.get_type() != Variant::ARRAY)
		return Array();

	return v;
}
void Terrain2DChunkDefault::meshes_set(const int mesh_index, const int mesh_type_index, const Array &meshes) {
	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	m[mesh_type_index] = meshes;
	_rids[mesh_index] = m;
}
bool Terrain2DChunkDefault::meshes_has(const int mesh_index, const int mesh_type_index) {
	if (!_rids.has(mesh_index))
		return false;

	Dictionary m = _rids[mesh_index];

	if (!m.has(mesh_type_index))
		return false;

	return true;
}

void Terrain2DChunkDefault::rids_clear() {
	_rids.clear();
}

void Terrain2DChunkDefault::rids_free() {
	List<Variant> keys;

	_rids.get_key_list(&keys);

	for (List<Variant>::Element *E = keys.front(); E; E = E->next()) {
		Variant v = E->get();

		if (v.get_type() != Variant::INT)
			continue;

		free_index(v);
	}
}

void Terrain2DChunkDefault::meshes_create(const int mesh_index, const int mesh_count) {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(!get_library().is_valid());

	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_MESH));

	Array am;

	for (int i = 0; i < mesh_count; ++i) {
		RID mesh_rid = RS::get_singleton()->mesh_create();

		am.push_back(mesh_rid);
	}

	m[MESH_TYPE_INDEX_MESH] = am;

	_rids[mesh_index] = m;
}
void Terrain2DChunkDefault::meshes_free(const int mesh_index) {
	if (!_rids.has(mesh_index))
		return;

	Dictionary m = _rids[mesh_index];
	RID rid;

	if (m.has(MESH_TYPE_INDEX_MESH)) {
		Array a = m[MESH_TYPE_INDEX_MESH];

		for (int i = 0; i < a.size(); ++i) {
			RID r = a[i];

			if (r != rid) {
				RS::get_singleton()->free(r);
			}
		}
	}

	m.erase(MESH_TYPE_INDEX_MESH);
}

void Terrain2DChunkDefault::mesh_create(const int mesh_index) {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(!get_library().is_valid());

	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_MESH));

	RID mesh_rid = RS::get_singleton()->mesh_create();

	m[MESH_TYPE_INDEX_MESH] = mesh_rid;

	_rids[mesh_index] = m;
}
void Terrain2DChunkDefault::mesh_free(const int mesh_index) {
	if (!_rids.has(mesh_index))
		return;

	Dictionary m = _rids[mesh_index];
	RID rid;

	if (m.has(MESH_TYPE_INDEX_MESH)) {
		RID r = m[MESH_TYPE_INDEX_MESH];

		if (r != rid) {
			RS::get_singleton()->free(r);
		}
	}

	m.erase(MESH_TYPE_INDEX_MESH);
}

void Terrain2DChunkDefault::colliders_create(const int mesh_index, const int layer_mask) {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(PhysicsServer::get_singleton()->is_flushing_queries());
	//ERR_FAIL_COND(!get_voxel_world()->is_inside_tree());

	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_BODY));
	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_SHAPE));

	RID shape_rid = Physics2DServer::get_singleton()->concave_polygon_shape_create();
	RID body_rid = Physics2DServer::get_singleton()->body_create();

	Physics2DServer::get_singleton()->body_set_collision_layer(body_rid, layer_mask);
	Physics2DServer::get_singleton()->body_set_collision_mask(body_rid, layer_mask);

	Physics2DServer::get_singleton()->body_add_shape(body_rid, shape_rid);

	Physics2DServer::get_singleton()->body_set_state(body_rid, Physics2DServer::BODY_STATE_TRANSFORM, get_transform());

	if (get_voxel_world()->is_inside_tree()) {
		Ref<World3D> world = get_voxel_world()->get_world_2d();

		if (world.is_valid() && world->get_space() != RID())
			Physics2DServer::get_singleton()->body_set_space(body_rid, world->get_space());
	}

	m[MESH_TYPE_INDEX_BODY] = body_rid;
	m[MESH_TYPE_INDEX_SHAPE] = shape_rid;

	_rids[mesh_index] = m;
}
void Terrain2DChunkDefault::colliders_create_area(const int mesh_index, const int layer_mask) {
	ERR_FAIL_COND(_voxel_world == NULL);
	ERR_FAIL_COND(PhysicsServer::get_singleton()->is_flushing_queries());

	if (!_rids.has(mesh_index))
		_rids[mesh_index] = Dictionary();

	Dictionary m = _rids[mesh_index];

	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_AREA));
	ERR_FAIL_COND(m.has(MESH_TYPE_INDEX_SHAPE));

	RID shape_rid = Physics2DServer::get_singleton()->concave_polygon_shape_create();
	RID area_rid = Physics2DServer::get_singleton()->area_create();

	Physics2DServer::get_singleton()->area_attach_object_instance_id(area_rid, _voxel_world->get_instance_id());

	//Physics2DServer::get_singleton()->area_set_monitor_callback(area_rid, this, "_body_area_inout");
	//Physics2DServer::get_singleton()->area_set_area_monitor_callback(area_rid, this, "_body_area_area_inout");
	//Physics2DServer::get_singleton()->area_set_monitorable(area_rid, true);

	Physics2DServer::get_singleton()->area_set_collision_layer(area_rid, layer_mask);
	Physics2DServer::get_singleton()->area_set_collision_mask(area_rid, layer_mask);

	if (get_voxel_world()->is_inside_tree()) {
		Ref<World2D> world = get_voxel_world()->get_world_2d();

		if (world.is_valid() && world->get_space() != RID())
			Physics2DServer::get_singleton()->area_set_space(area_rid, world->get_space());
	}

	Physics2DServer::get_singleton()->area_add_shape(area_rid, shape_rid, get_transform());

	m[MESH_TYPE_INDEX_AREA] = area_rid;
	m[MESH_TYPE_INDEX_SHAPE] = shape_rid;

	_rids[mesh_index] = m;
}

void Terrain2DChunkDefault::colliders_free(const int mesh_index) {
	if (!_rids.has(mesh_index))
		return;

	Dictionary m = _rids[mesh_index];
	RID rid;

	if (m.has(MESH_TYPE_INDEX_SHAPE)) {
		RID r = m[MESH_TYPE_INDEX_SHAPE];

		Physics2DServer::get_singleton()->free(r);
	}

	if (m.has(MESH_TYPE_INDEX_BODY)) {
		RID r = m[MESH_TYPE_INDEX_BODY];

		Physics2DServer::get_singleton()->free(r);
	}

	m.erase(MESH_TYPE_INDEX_SHAPE);
	m.erase(MESH_TYPE_INDEX_BODY);

	_rids[mesh_index] = m;
}

void Terrain2DChunkDefault::free_index(const int mesh_index) {
	meshes_free(mesh_index);
	mesh_free(mesh_index);
	colliders_free(mesh_index);
}

void Terrain2DChunkDefault::update_transforms() {
	RID empty_rid;
	Transform2D t = get_transform();

	List<Variant> keys;

	_rids.get_key_list(&keys);

	for (List<Variant>::Element *E = keys.front(); E; E = E->next()) {
		Variant v = E->get();

		if (v.get_type() != Variant::INT)
			continue;

		Dictionary d = _rids[v];

		if (d.has(MESH_TYPE_INDEX_BODY)) {
			RID rid = d[MESH_TYPE_INDEX_BODY];

			if (rid != empty_rid) {
				Physics2DServer::get_singleton()->body_set_state(rid, Physics2DServer::BODY_STATE_TRANSFORM, t);
			}
		}

		if (d.has(MESH_TYPE_INDEX_AREA)) {
			RID rid = d[MESH_TYPE_INDEX_AREA];

			if (rid != empty_rid) {
				Physics2DServer::get_singleton()->area_set_shape_transform(rid, 0, t);
			}
		}
	}

	for (int i = 0; i < collider_get_count(); ++i) {
		Physics2DServer::get_singleton()->body_set_state(collider_get_body(i), Physics2DServer::BODY_STATE_TRANSFORM, get_transform() * collider_get_transform(i));
	}

	if (_2d_body_rid != RID()) {
		Physics2DServer::get_singleton()->body_set_state(_2d_body_rid, Physics2DServer::BODY_STATE_TRANSFORM, get_transform());
	}

	//if (_debug_mesh_instance != RID()) {
	//	RS::get_singleton()->instance_set_transform(_debug_mesh_instance, get_transform());
	//}
}

//Lights
Ref<Terrain2DLight> Terrain2DChunkDefault::get_light(const int index) {
	ERR_FAIL_INDEX_V(index, _lights.size(), Ref<Terrain2DLight>());

	return _lights.get(index);
}
int Terrain2DChunkDefault::get_light_count() const {
	return _lights.size();
}

void Terrain2DChunkDefault::debug_mesh_allocate() {
	if (_debug_mesh_rid == RID()) {
		_debug_mesh_rid = RenderingServer::get_singleton()->mesh_create();
	}

	if (_debug_mesh_instance == RID()) {
		_debug_mesh_instance = RenderingServer::get_singleton()->instance_create();

		//if (get_voxel_world()->get_world().is_valid())
		//	RS::get_singleton()->instance_set_scenario(_debug_mesh_instance, get_voxel_world()->get_world()->get_scenario());

		RS::get_singleton()->instance_set_base(_debug_mesh_instance, _debug_mesh_rid);
		//RS::get_singleton()->instance_set_transform(_debug_mesh_instance, get_transform());
		RS::get_singleton()->instance_set_visible(_debug_mesh_instance, true);
	}
}
void Terrain2DChunkDefault::debug_mesh_free() {
	if (_debug_mesh_instance != RID()) {
		RenderingServer::get_singleton()->free(_debug_mesh_instance);
	}

	if (_debug_mesh_rid != RID()) {
		RenderingServer::get_singleton()->free(_debug_mesh_rid);
	}
}
bool Terrain2DChunkDefault::debug_mesh_has() {
	return _debug_mesh_rid != RID();
}
void Terrain2DChunkDefault::debug_mesh_clear() {
	if (_debug_mesh_rid != RID()) {
		RenderingServer::get_singleton()->mesh_clear(_debug_mesh_rid);
	}
}
void Terrain2DChunkDefault::debug_mesh_array_clear() {
	_debug_mesh_array.resize(0);
}
void Terrain2DChunkDefault::debug_mesh_add_vertices_to(const PoolVector2Array &arr) {
	_debug_mesh_array.append_array(arr);

	if (_debug_mesh_array.size() % 2 == 1) {
		_debug_mesh_array.append(_debug_mesh_array[_debug_mesh_array.size() - 1]);
	}
}
void Terrain2DChunkDefault::debug_mesh_send() {
	debug_mesh_allocate();
	debug_mesh_clear();

	if (_debug_mesh_array.size() == 0)
		return;

	SceneTree *st = SceneTree::get_singleton();

	Array arr;
	arr.resize(RenderingServer::ARRAY_MAX);
	arr[RenderingServer::ARRAY_VERTEX] = _debug_mesh_array;

	RenderingServer::get_singleton()->mesh_add_surface_from_arrays(_debug_mesh_rid, RenderingServer::PRIMITIVE_LINES, arr);

	if (st) {
		RenderingServer::get_singleton()->mesh_surface_set_material(_debug_mesh_rid, 0, SceneTree::get_singleton()->get_debug_collision_material()->get_rid());
	}

	debug_mesh_array_clear();
}

void Terrain2DChunkDefault::draw_cross_voxels(Vector2 pos) {
	pos.x *= get_cell_size_x();
	pos.y *= get_cell_size_x();

	int size = _debug_mesh_array.size();
	_debug_mesh_array.resize(_debug_mesh_array.size() + 6);

	_debug_mesh_array.set(size, pos + Vector2(0, -0.2 * _cell_size_y));
	_debug_mesh_array.set(size + 1, pos + Vector2(0, 0.2 * _cell_size_y));

	_debug_mesh_array.set(size + 4, pos + Vector2(-0.2 * _cell_size_x, 0));
	_debug_mesh_array.set(size + 5, pos + Vector2(0.2 * _cell_size_x, 0));
}

void Terrain2DChunkDefault::draw_cross_voxels_fill(Vector2 pos, float fill) {
	pos.x *= get_cell_size_x();
	pos.y *= get_cell_size_x();

	int size = _debug_mesh_array.size();
	_debug_mesh_array.resize(_debug_mesh_array.size() + 6);

	_debug_mesh_array.set(size, pos + Vector2(0, -0.2 * fill * _cell_size_y));
	_debug_mesh_array.set(size + 1, pos + Vector2(0, 0.2 * fill * _cell_size_y));

	_debug_mesh_array.set(size + 4, pos + Vector2(-0.2 * fill * _cell_size_x, 0));
	_debug_mesh_array.set(size + 5, pos + Vector2(0.2 * fill * _cell_size_x, 0));
}

void Terrain2DChunkDefault::draw_debug_voxels(int max, Color color) {
	if (!debug_mesh_has()) {
		debug_mesh_allocate();
	}

	//debug_mesh_array_clear();

	//_debug_drawer->begin(Mesh::PRIMITIVE_LINES);

	int a = 0;

	int64_t sx = static_cast<int64_t>(_size_x);
	int64_t sy = static_cast<int64_t>(_size_y);

	for (int y = 0; y < sy; ++y) {
		for (int x = 0; x < sx; ++x) {
			int type = get_voxel(x, y, Terrain2DChunkDefault::DEFAULT_CHANNEL_TYPE);

			if (type == 0) {
				continue;
			}

			draw_cross_voxels_fill(Vector2(x, y), 1);

			++a;

			if (a > max) {
				break;
			}
		}
	}

	debug_mesh_send();
}

void Terrain2DChunkDefault::draw_debug_voxel_lights() {
	if (!debug_mesh_has()) {
		debug_mesh_allocate();
	}

	//debug_mesh_array_clear();

	//_debug_drawer->begin(Mesh::PrimitiveType::PRIMITIVE_LINES);

	for (int i = 0; i < _lights.size(); ++i) {
		Ref<Terrain2DLight> v = _lights[i];

		int pos_x = v->get_world_position_x() - (_size_x * _position_x);
		int pos_y = v->get_world_position_y() - (_size_y * _position_y);

		draw_cross_voxels_fill(Vector2(pos_x, pos_y), 1.0);
	}

	debug_mesh_send();
}

void Terrain2DChunkDefault::draw_debug_mdr_colliders() {
	if (!debug_mesh_has()) {
		debug_mesh_allocate();
	}

	for (int i = 0; i < collider_get_count(); ++i) {
		Ref<Shape2D> shape = collider_get_shape(i);

		if (!shape.is_valid())
			continue;

		//Transform2D t = collider_get_transform(i);

		//shape->add_vertices_to_array(_debug_mesh_array, t);
		//draw
	}
}

void Terrain2DChunkDefault::_exit_tree() {
	Terrain2DChunk::_exit_tree();

	if (!_is_generating) {
		rids_free();
		rids_clear();
	}
}

void Terrain2DChunkDefault::_world_transform_changed() {
	Terrain2DChunk::_world_transform_changed();

	update_transforms();
}

void Terrain2DChunkDefault::_draw() {
	if (_is_generating) {
		return;
	}

	setup_canvas_items_size(mesh_rid_get_count(MESH_INDEX_TERRAIN, MESH_TYPE_INDEX_MESH));

	for (int i = 0; i < get_canvas_item_count(); ++i) {
		RenderingServer::get_singleton()->canvas_item_clear(get_canvas_item(i));
	}

	Terrain2DWorld *world = get_voxel_world();

	ERR_FAIL_COND(!world);

	for (int i = 0; i < mesh_rid_get_count(MESH_INDEX_TERRAIN, MESH_TYPE_INDEX_MESH); ++i) {
		RID terrain_mesh_rid = mesh_rid_get_index(MESH_INDEX_TERRAIN, MESH_TYPE_INDEX_MESH, i);

		if (terrain_mesh_rid != RID()) {
			RID terrain_texture_rid = mesh_rid_get(MESH_INDEX_TERRAIN, MESH_TYPE_INDEX_TEXTURE_RID);

			RenderingServer::get_singleton()->canvas_item_clear(get_canvas_item(i));

			Transform2D t = get_transform() * _mesh_transforms[i];

			RenderingServer::get_singleton()->canvas_item_set_transform(get_canvas_item(i), t);

			//if (i != 0) {
			//	RenderingServer::get_singleton()->canvas_item_set_custom_rect(get_canvas_item(i), true, Rect2(t.xform(Vector2()), Vector2(128, 128)));
			//}

			//Note: the transform parameter is not implemented in gles2
			RenderingServer::get_singleton()->canvas_item_add_mesh(get_canvas_item(i), terrain_mesh_rid, Transform2D(), Color(1, 1, 1, 1), terrain_texture_rid, RID());
		}
	}

	RID liquid_mesh_rid = mesh_rid_get(MESH_INDEX_LIQUID, MESH_TYPE_INDEX_MESH);

	if (liquid_mesh_rid != RID()) {
		RID liquid_texture_rid = mesh_rid_get(MESH_INDEX_LIQUID, MESH_TYPE_INDEX_TEXTURE_RID);

		//Note: the transform parameter is not implemented in gles2
		RenderingServer::get_singleton()->canvas_item_add_mesh(get_canvas_item(0), liquid_mesh_rid, Transform2D(), Color(1, 1, 1, 1), liquid_texture_rid, RID());
	}

	RID prop_mesh_rid = mesh_rid_get(MESH_INDEX_PROP, MESH_TYPE_INDEX_MESH);

	if (prop_mesh_rid != RID()) {
		RID prop_texture_rid = mesh_rid_get(MESH_INDEX_PROP, MESH_TYPE_INDEX_TEXTURE_RID);

		//Note: the transform parameter is not implemented in gles2
		RenderingServer::get_singleton()->canvas_item_add_mesh(get_canvas_item(0), prop_mesh_rid, Transform2D(), Color(1, 1, 1, 1), prop_texture_rid, RID());
	}

#if TOOLS_ENABLED
	SceneTree *st = SceneTree::get_singleton();

	bool debug_shapes = false;
	if (st) {
		if (Engine::get_singleton()->is_editor_hint()) {
			//debug_shapes = show_collision;
		} else {
			debug_shapes = st->is_debugging_collisions_hint();
		}
	}

	if (debug_shapes) {
		if (_debug_canvas_item == RID()) {
			_debug_canvas_item = RenderingServer::get_singleton()->canvas_item_create();

			if (_voxel_world) {
				RenderingServer::get_singleton()->canvas_item_set_parent(_debug_canvas_item, get_voxel_world()->get_canvas_item());
			}

			RenderingServer::get_singleton()->canvas_item_set_transform(_debug_canvas_item, get_transform());
			RenderingServer::get_singleton()->canvas_item_set_z_index(_debug_canvas_item, 1);
		}

		RenderingServer::get_singleton()->canvas_item_clear(_debug_canvas_item);

		Color debug_collision_color = st->get_debug_collisions_color();

		Ref<Shape2D> shape = get_default_tile_shape();

		for (int i = 0; i < _debug_terrain_collider_transforms.size(); ++i) {
			RenderingServer::get_singleton()->canvas_item_add_set_transform(_debug_canvas_item, _debug_terrain_collider_transforms[i]);
			shape->draw(_debug_canvas_item, debug_collision_color);
		}
	}
#endif
}

//Lights
void Terrain2DChunkDefault::_bake_lights() {
	clear_baked_lights();

	for (int i = 0; i < _lights.size(); ++i) {
		bake_light(_lights.get(i));
	}
}
void Terrain2DChunkDefault::_bake_light(Ref<Terrain2DLight> light) {
	ERR_FAIL_COND(!light.is_valid());

	Color color = light->get_color();
	int size = light->get_size();

	int local_x = light->get_world_position_x() - (_position_x * _size_x);
	int local_y = light->get_world_position_y() - (_position_y * _size_y);

	ERR_FAIL_COND(size < 0);

	int64_t dsx = static_cast<int64_t>(_data_size_x);
	int64_t dsy = static_cast<int64_t>(_data_size_y);

	uint8_t *channel_color_r = channel_get(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
	uint8_t *channel_color_g = channel_get(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
	uint8_t *channel_color_b = channel_get(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

	ERR_FAIL_COND(channel_color_r == NULL || channel_color_g == NULL || channel_color_b == NULL);

	for (int y = local_y - size; y <= local_y + size; ++y) {
		if (y < 0 || y >= dsy)
			continue;

		for (int x = local_x - size; x <= local_x + size; ++x) {
			if (x < 0 || x >= dsx)
				continue;

			int lx = x - local_x;
			int ly = y - local_y;

			float str = size - (((float)lx * lx + ly * ly));
			str /= size;

			if (str < 0)
				continue;

			int index = get_data_index(x, y);

			int r = color.r * str * 255.0;
			int g = color.g * str * 255.0;
			int b = color.b * str * 255.0;

			r += channel_color_r[index];
			g += channel_color_g[index];
			b += channel_color_b[index];

			if (r > 255)
				r = 255;

			if (g > 255)
				g = 255;

			if (b > 255)
				b = 255;

			channel_color_r[index] = r;
			channel_color_g[index] = g;
			channel_color_b[index] = b;
		}
	}
}
void Terrain2DChunkDefault::_clear_baked_lights() {
	channel_fill(0, DEFAULT_CHANNEL_LIGHT_COLOR_R);
	channel_fill(0, DEFAULT_CHANNEL_LIGHT_COLOR_G);
	channel_fill(0, DEFAULT_CHANNEL_LIGHT_COLOR_B);
}
void Terrain2DChunkDefault::_world_light_added(const Ref<Terrain2DLight> &light) {
	_lights.push_back(light);

	set_lights_dirty(true);
}
void Terrain2DChunkDefault::_world_light_removed(const Ref<Terrain2DLight> &light) {
	int index = _lights.find(light);

	if (index != -1) {
		_lights.remove(index);

		set_lights_dirty(true);
	}
}

void Terrain2DChunkDefault::free_chunk() {
	rids_free();
}

void Terrain2DChunkDefault::_finalize_build() {
	ERR_FAIL_COND(!_library.is_valid());

#if TOOLS_ENABLED
	if (_debug_mesh_array.size() > 0) {
		debug_mesh_send();
	}
#endif

	call_deferred("update_transforms");
	//call_deferred("update");
}

Terrain2DChunkDefault::Terrain2DChunkDefault() {
	_abort_build = false;

	_enabled = true;

	_build_flags = BUILD_FLAG_CREATE_COLLIDER;
}

Terrain2DChunkDefault::~Terrain2DChunkDefault() {
	_abort_build = true;

	_lights.clear();

	debug_mesh_free();

	rids_free();
	rids_clear();
}

void Terrain2DChunkDefault::_channel_setup() {
	channel_set_count(MAX_DEFAULT_CHANNELS);
}

void Terrain2DChunkDefault::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_build_flags"), &Terrain2DChunkDefault::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &Terrain2DChunkDefault::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, BINDING_STRING_BUILD_FLAGS, 0), "set_build_flags", "get_build_flags");

	ClassDB::bind_method(D_METHOD("get_lights_dirty"), &Terrain2DChunkDefault::get_lights_dirty);
	ClassDB::bind_method(D_METHOD("set_lights_dirty", "value"), &Terrain2DChunkDefault::set_lights_dirty);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "lights_dirty", PROPERTY_HINT_NONE, "", 0), "set_lights_dirty", "get_lights_dirty");

	//Meshes
	ClassDB::bind_method(D_METHOD("get_mesh_rids"), &Terrain2DChunkDefault::mesh_rids_get);
	ClassDB::bind_method(D_METHOD("set_mesh_rids", "rids"), &Terrain2DChunkDefault::mesh_rids_set);
	ClassDB::bind_method(D_METHOD("clear_rids"), &Terrain2DChunkDefault::rids_clear);

	ClassDB::bind_method(D_METHOD("mesh_rid_get", "mesh_index", "mesh_type_index"), &Terrain2DChunkDefault::mesh_rid_get);
	ClassDB::bind_method(D_METHOD("mesh_rid_set", "mesh_index", "mesh_type_index", "value"), &Terrain2DChunkDefault::mesh_rid_set);
	ClassDB::bind_method(D_METHOD("mesh_rid_get_index", "mesh_index", "mesh_type_index", "index"), &Terrain2DChunkDefault::mesh_rid_get_index);
	ClassDB::bind_method(D_METHOD("mesh_rid_set_index", "mesh_index", "mesh_type_index", "index", "value"), &Terrain2DChunkDefault::mesh_rid_set_index);
	ClassDB::bind_method(D_METHOD("mesh_rid_get_count", "mesh_index", "mesh_type_index"), &Terrain2DChunkDefault::mesh_rid_get_count);
	ClassDB::bind_method(D_METHOD("mesh_rids_clear", "mesh_index", "mesh_type_index"), &Terrain2DChunkDefault::mesh_rids_clear);
	ClassDB::bind_method(D_METHOD("meshes_get", "mesh_index", "mesh_type_index"), &Terrain2DChunkDefault::meshes_get);
	ClassDB::bind_method(D_METHOD("meshes_set", "mesh_index", "mesh_type_index", "meshes"), &Terrain2DChunkDefault::meshes_set);
	ClassDB::bind_method(D_METHOD("meshes_has", "mesh_index", "mesh_type_index"), &Terrain2DChunkDefault::meshes_has);

	ClassDB::bind_method(D_METHOD("rids_free"), &Terrain2DChunkDefault::rids_free);
	ClassDB::bind_method(D_METHOD("free_index", "mesh_index"), &Terrain2DChunkDefault::free_index);

	ClassDB::bind_method(D_METHOD("meshes_create", "mesh_index", "mesh_count"), &Terrain2DChunkDefault::meshes_create);
	ClassDB::bind_method(D_METHOD("meshes_free", "mesh_index"), &Terrain2DChunkDefault::meshes_free);
	ClassDB::bind_method(D_METHOD("mesh_create", "mesh_index"), &Terrain2DChunkDefault::mesh_create);
	ClassDB::bind_method(D_METHOD("mesh_free", "mesh_index"), &Terrain2DChunkDefault::mesh_free);

	ClassDB::bind_method(D_METHOD("create_colliders", "mesh_index", "layer_mask"), &Terrain2DChunkDefault::colliders_create, DEFVAL(1));
	ClassDB::bind_method(D_METHOD("free_colliders", "mesh_index"), &Terrain2DChunkDefault::colliders_free);

	//Lights
	ClassDB::bind_method(D_METHOD("get_light", "index"), &Terrain2DChunkDefault::get_light);
	ClassDB::bind_method(D_METHOD("get_light_count"), &Terrain2DChunkDefault::get_light_count);

	//Debug
	ClassDB::bind_method(D_METHOD("debug_mesh_allocate"), &Terrain2DChunkDefault::debug_mesh_allocate);
	ClassDB::bind_method(D_METHOD("debug_mesh_free"), &Terrain2DChunkDefault::debug_mesh_free);

	ClassDB::bind_method(D_METHOD("debug_mesh_has"), &Terrain2DChunkDefault::debug_mesh_has);
	ClassDB::bind_method(D_METHOD("debug_mesh_clear"), &Terrain2DChunkDefault::debug_mesh_clear);
	ClassDB::bind_method(D_METHOD("debug_mesh_array_clear"), &Terrain2DChunkDefault::debug_mesh_array_clear);
	ClassDB::bind_method(D_METHOD("debug_mesh_add_vertices_to", "arr"), &Terrain2DChunkDefault::debug_mesh_add_vertices_to);
	ClassDB::bind_method(D_METHOD("debug_mesh_send"), &Terrain2DChunkDefault::debug_mesh_send);

	ClassDB::bind_method(D_METHOD("draw_cross_voxels", "max"), &Terrain2DChunkDefault::draw_cross_voxels);
	ClassDB::bind_method(D_METHOD("draw_cross_voxels_fill", "max", "fill"), &Terrain2DChunkDefault::draw_cross_voxels_fill);
	ClassDB::bind_method(D_METHOD("draw_debug_voxels", "max", "color"), &Terrain2DChunkDefault::draw_debug_voxels, DEFVAL(Color(1, 1, 1)));

	ClassDB::bind_method(D_METHOD("draw_debug_voxel_lights"), &Terrain2DChunkDefault::draw_debug_voxel_lights);
	ClassDB::bind_method(D_METHOD("draw_debug_mdr_colliders"), &Terrain2DChunkDefault::draw_debug_mdr_colliders);

	//Free
	ClassDB::bind_method(D_METHOD("free_chunk"), &Terrain2DChunkDefault::free_chunk);

	//etc
	ClassDB::bind_method(D_METHOD("emit_build_finished"), &Terrain2DChunkDefault::emit_build_finished);

	//virtuals
	ClassDB::bind_method(D_METHOD("_channel_setup"), &Terrain2DChunkDefault::_channel_setup);
	ClassDB::bind_method(D_METHOD("_draw"), &Terrain2DChunkDefault::_draw);

	//lights
	ClassDB::bind_method(D_METHOD("_bake_lights"), &Terrain2DChunkDefault::_bake_lights);
	ClassDB::bind_method(D_METHOD("_bake_light", "light"), &Terrain2DChunkDefault::_bake_light);
	ClassDB::bind_method(D_METHOD("_clear_baked_lights"), &Terrain2DChunkDefault::_clear_baked_lights);

	ClassDB::bind_method(D_METHOD("_world_light_added", "light"), &Terrain2DChunkDefault::_world_light_added);
	ClassDB::bind_method(D_METHOD("_world_light_removed", "light"), &Terrain2DChunkDefault::_world_light_removed);

	ClassDB::bind_method(D_METHOD("_finalize_build"), &Terrain2DChunkDefault::_finalize_build);

	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_TYPE);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_FLAGS);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIQUID_TYPE);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIGHT_COLOR_R);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIGHT_COLOR_G);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_LIGHT_COLOR_B);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_AO);
	BIND_ENUM_CONSTANT(DEFAULT_CHANNEL_RANDOM_AO);
	BIND_ENUM_CONSTANT(MAX_DEFAULT_CHANNELS);

	BIND_ENUM_CONSTANT(FLAG_CHANNEL_WALL_NORTH);
	BIND_ENUM_CONSTANT(FLAG_CHANNEL_WALL_PY);
	BIND_ENUM_CONSTANT(FLAG_CHANNEL_WALL_SOUTH);
	BIND_ENUM_CONSTANT(FLAG_CHANNEL_WALL_NY);
	BIND_ENUM_CONSTANT(FLAG_CHANNEL_WALL_EAST);
	BIND_ENUM_CONSTANT(FLAG_CHANNEL_WALL_PX);
	BIND_ENUM_CONSTANT(FLAG_CHANNEL_WALL_WEST);
	BIND_ENUM_CONSTANT(FLAG_CHANNEL_WALL_NX);
	BIND_ENUM_CONSTANT(FLAG_CHANNEL_COLLIDER);
	BIND_ENUM_CONSTANT(FLAG_CHANNEL_FLIP_H);
	BIND_ENUM_CONSTANT(FLAG_CHANNEL_FLIP_V);
	BIND_ENUM_CONSTANT(FLAG_CHANNEL_WALL_HOLE);

	BIND_CONSTANT(MESH_INDEX_TERRAIN);
	BIND_CONSTANT(MESH_INDEX_PROP);
	BIND_CONSTANT(MESH_INDEX_LIQUID);
	BIND_CONSTANT(MESH_INDEX_CLUTTER);

	BIND_CONSTANT(MESH_TYPE_INDEX_MESH);
	BIND_CONSTANT(MESH_TYPE_INDEX_SHAPE);
	BIND_CONSTANT(MESH_TYPE_INDEX_BODY);
	BIND_CONSTANT(MESH_TYPE_INDEX_AREA);
	BIND_CONSTANT(MESH_TYPE_INDEX_TEXTURE_RID);

	BIND_ENUM_CONSTANT(BUILD_FLAG_USE_LIGHTING);
	BIND_ENUM_CONSTANT(BUILD_FLAG_USE_AO);
	BIND_ENUM_CONSTANT(BUILD_FLAG_USE_RAO);
	BIND_ENUM_CONSTANT(BUILD_FLAG_GENERATE_AO);
	BIND_ENUM_CONSTANT(BUILD_FLAG_AUTO_GENERATE_RAO);
	BIND_ENUM_CONSTANT(BUILD_FLAG_BAKE_LIGHTS);
	BIND_ENUM_CONSTANT(BUILD_FLAG_CREATE_COLLIDER);
}
