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

#include "terrain_2d_chunk.h"

#include "terrain_2d_world.h"

#include "../thirdparty/lz4/lz4.h"

#include "../defines.h"

#include "core/object/message_queue.h"
#include "jobs/terrain_2d_job.h"
#include "terrain_2d_structure.h"

#if THREAD_POOL_PRESENT
#include "../../thread_pool/thread_pool.h"
#endif

_FORCE_INLINE_ bool Terrain2DChunk::get_process() const {
	return _is_processing;
}
_FORCE_INLINE_ void Terrain2DChunk::set_process(const bool value) {
	_is_processing = value;
}

_FORCE_INLINE_ bool Terrain2DChunk::get_physics_process() const {
	return _is_phisics_processing;
}
_FORCE_INLINE_ void Terrain2DChunk::set_physics_process(const bool value) {
	_is_phisics_processing = value;
}

bool Terrain2DChunk::get_visible() const {
	return _is_visible;
}
void Terrain2DChunk::set_visible(const bool value) {
	_is_visible = value;

	visibility_changed(value);
}

_FORCE_INLINE_ bool Terrain2DChunk::get_is_generating() const {
	return _is_generating;
}
_FORCE_INLINE_ void Terrain2DChunk::set_is_generating(const bool value) {
	_is_generating = value;
}

bool Terrain2DChunk::is_build_aborted() const {
	return _abort_build;
}

bool Terrain2DChunk::is_in_tree() const {
	return _is_in_tree;
}

_FORCE_INLINE_ bool Terrain2DChunk::get_dirty() const {
	return _dirty;
}
_FORCE_INLINE_ void Terrain2DChunk::set_dirty(const bool value) {
	_dirty = value;
}

_FORCE_INLINE_ int Terrain2DChunk::get_state() const {
	return _state;
}
_FORCE_INLINE_ void Terrain2DChunk::set_state(const int value) {
	_state = value;
}

_FORCE_INLINE_ int Terrain2DChunk::get_position_x() const {
	return _position_x;
}
void Terrain2DChunk::set_position_x(const int value) {
	_position_x = value;
}
_FORCE_INLINE_ int Terrain2DChunk::get_position_y() const {
	return _position_y;
}
void Terrain2DChunk::set_position_y(const int value) {
	_position_y = value;
}

_FORCE_INLINE_ Vector2 Terrain2DChunk::get_position() const {
	return Vector2(_position_x, _position_y);
}
_FORCE_INLINE_ Vector2 Terrain2DChunk::get_world_position() const {
	return Vector2(_position_x * _size_x * _cell_size_x, _position_y * _size_y * _cell_size_y);
}

_FORCE_INLINE_ Vector2 Terrain2DChunk::get_world_size() const {
	return Vector2(_size_x * _cell_size_x, _size_y * _cell_size_y);
}

_FORCE_INLINE_ Rect2 Terrain2DChunk::get_world_rect() const {
	Vector2 v = get_world_position();

	return Rect2(Vector2(v.x, v.y), get_world_size());
}

_FORCE_INLINE_ int Terrain2DChunk::get_size_x() const {
	return _size_x;
}
_FORCE_INLINE_ int Terrain2DChunk::get_size_y() const {
	return _size_y;
}

_FORCE_INLINE_ void Terrain2DChunk::set_size_x(const int value) {
	_size_x = value;
}
_FORCE_INLINE_ void Terrain2DChunk::set_size_y(const int value) {
	_size_y = value;
}

_FORCE_INLINE_ Vector2 Terrain2DChunk::get_size() const {
	return Vector2(_size_x, _size_y);
}

_FORCE_INLINE_ int Terrain2DChunk::get_data_size_x() const {
	return _data_size_x;
}
_FORCE_INLINE_ int Terrain2DChunk::get_data_size_y() const {
	return _data_size_y;
}

_FORCE_INLINE_ void Terrain2DChunk::set_data_size_x(const int value) {
	_data_size_x = value;
}
_FORCE_INLINE_ void Terrain2DChunk::set_data_size_y(const int value) {
	_data_size_y = value;
}

void Terrain2DChunk::set_position(const int x, const int y) {
	_position_x = x;
	_position_y = y;
}

_FORCE_INLINE_ int Terrain2DChunk::get_margin_start() const {
	return _margin_start;
}
_FORCE_INLINE_ int Terrain2DChunk::get_margin_end() const {
	return _margin_end;
}

_FORCE_INLINE_ void Terrain2DChunk::set_margin_start(const int value) {
	_margin_start = value;
}
_FORCE_INLINE_ void Terrain2DChunk::set_margin_end(const int value) {
	_margin_end = value;
}

int Terrain2DChunk::material_cache_key_get() const {
	return _material_cache_key;
}
void Terrain2DChunk::material_cache_key_set(const int value) {
	_material_cache_key = value;
}

bool Terrain2DChunk::material_cache_key_has() const {
	return _material_cache_key_has;
}
void Terrain2DChunk::material_cache_key_has_set(const bool value) {
	_material_cache_key_has = value;
}

int Terrain2DChunk::liquid_material_cache_key_get() const {
	return _liquid_material_cache_key;
}
void Terrain2DChunk::liquid_material_cache_key_set(const int value) {
	_liquid_material_cache_key = value;
}

bool Terrain2DChunk::liquid_material_cache_key_has() const {
	return _liquid_material_cache_key_has;
}
void Terrain2DChunk::liquid_material_cache_key_has_set(const bool value) {
	_liquid_material_cache_key_has = value;
}

int Terrain2DChunk::prop_material_cache_key_get() const {
	return _prop_material_cache_key;
}
void Terrain2DChunk::prop_material_cache_key_set(const int value) {
	_prop_material_cache_key = value;
}

bool Terrain2DChunk::prop_material_cache_key_has() const {
	return _prop_material_cache_key_has;
}
void Terrain2DChunk::prop_material_cache_key_has_set(const bool value) {
	_prop_material_cache_key_has = value;
}

Ref<Terrain2DLibrary> Terrain2DChunk::get_library() {
	return _library;
}
void Terrain2DChunk::set_library(const Ref<Terrain2DLibrary> &value) {
	_library = value;
}

int Terrain2DChunk::get_cell_size_x() const {
	return _cell_size_x;
}
void Terrain2DChunk::set_cell_size_x(const int value) {
	_cell_size_x = value;
}

int Terrain2DChunk::get_cell_size_y() const {
	return _cell_size_y;
}
void Terrain2DChunk::set_cell_size_y(const int value) {
	_cell_size_y = value;
}

Terrain2DWorld *Terrain2DChunk::get_voxel_world() const {
	return _voxel_world;
}
void Terrain2DChunk::set_voxel_world(Terrain2DWorld *world) {
	_voxel_world = world;
}
void Terrain2DChunk::set_voxel_world_bind(Node *world) {
	set_voxel_world(Object::cast_to<Terrain2DWorld>(world));
}

Transform2D Terrain2DChunk::mesh_transform_terrain_get() {
	return _mesh_transform_terrain;
}
void Terrain2DChunk::mesh_transform_terrain_set(const Transform2D &value) {
	_mesh_transform_terrain = value;
}

Transform2D Terrain2DChunk::mesh_transform_wall_north_get() {
	return _mesh_transform_wall_north;
}
void Terrain2DChunk::mesh_transform_wall_north_set(const Transform2D &value) {
	_mesh_transform_wall_north = value;
}

Transform2D Terrain2DChunk::mesh_transform_wall_south_get() {
	return _mesh_transform_wall_south;
}
void Terrain2DChunk::mesh_transform_wall_south_set(const Transform2D &value) {
	_mesh_transform_wall_south = value;
}

Transform2D Terrain2DChunk::mesh_transform_wall_east_get() {
	return _mesh_transform_wall_east;
}
void Terrain2DChunk::mesh_transform_wall_east_set(const Transform2D &value) {
	_mesh_transform_wall_east = value;
}

Transform2D Terrain2DChunk::mesh_transform_wall_west_get() {
	return _mesh_transform_wall_west;
}
void Terrain2DChunk::mesh_transform_wall_west_set(const Transform2D &value) {
	_mesh_transform_wall_west = value;
}

Ref<Shape2D> Terrain2DChunk::get_default_tile_shape() {
	return _default_tile_shape;
}
void Terrain2DChunk::set_default_tile_shape(const Ref<Shape2D> &shape) {
	_default_tile_shape = shape;
}

Ref<Terrain2DJob> Terrain2DChunk::job_get(int index) const {
	ERR_FAIL_INDEX_V(index, _jobs.size(), Ref<Terrain2DJob>());

	return _jobs.get(index);
}
void Terrain2DChunk::job_set(int index, const Ref<Terrain2DJob> &job) {
	ERR_FAIL_INDEX(index, _jobs.size());

	_jobs.set(index, job);
}
void Terrain2DChunk::job_remove(const int index) {
	ERR_FAIL_INDEX(index, _jobs.size());

	_jobs.remove(index);
}
void Terrain2DChunk::job_add(const Ref<Terrain2DJob> &job) {
	_jobs.push_back(job);
}
int Terrain2DChunk::job_get_count() const {
	return _jobs.size();
}

int Terrain2DChunk::job_get_current_index() {
	return _current_job;
}
void Terrain2DChunk::job_next() {
	if (_abort_build) {
		_is_generating = false;
		_current_job = -1;
		return;
	}

	_THREAD_SAFE_METHOD_

	++_current_job;

	if (_current_job >= _jobs.size()) {
		_current_job = -1;
		set_is_generating(false);
		finalize_build();
		return;
	}

	Ref<Terrain2DJob> j = _jobs[_current_job];

	if (!j.is_valid()) {
		//skip if invalid
		job_next();
	}

	j->reset();
	j->set_complete(false);

	if (j->get_build_phase_type() == Terrain2DJob::BUILD_PHASE_TYPE_NORMAL) {
#if THREAD_POOL_PRESENT
		ThreadPool::get_singleton()->add_job(j);
#else
		j->execute();
#endif
	}
}
Ref<Terrain2DJob> Terrain2DChunk::job_get_current() {
	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size()) {
		return Ref<Terrain2DJob>();
	}

	return _jobs[_current_job];
}

//Terra Data
void Terrain2DChunk::channel_setup() {
	ERR_FAIL_COND_MSG(!has_method("_channel_setup"), "Terrain2DChunk: _setup_channels() is missing! Please implement it!");

	call("_channel_setup");
}

void Terrain2DChunk::set_size(const int size_x, const int size_y, const int margin_start, const int margin_end) {
	if (_size_x == size_x && _size_y == size_y && _margin_start == margin_start && _margin_end == margin_end) {
		return;
	}

	_size_x = size_x;
	_size_y = size_y;

	_data_size_x = size_x + margin_start + margin_end;
	_data_size_y = size_y + margin_start + margin_end;

	_margin_start = margin_start;
	_margin_end = margin_end;

	for (int i = 0; i < _channels.size(); ++i) {
		uint8_t *ch = _channels[i];

		if (ch != NULL) {
			memdelete_arr(ch);
		}
	}

	_channels.clear();

	channel_setup();
}

bool Terrain2DChunk::validate_data_position(const int x, const int y) const {
	return x < _data_size_x && y < _data_size_y;
}

uint8_t Terrain2DChunk::get_voxel(const int p_x, const int p_y, const int p_channel_index) const {
	int x = p_x + _margin_start;
	int y = p_y + _margin_start;

	ERR_FAIL_INDEX_V(p_channel_index, _channels.size(), 0);
	ERR_FAIL_COND_V_MSG(!validate_data_position(x, y), 0, "Error, index out of range! " + String::num(x) + " " + String::num(y));

	uint8_t *ch = _channels.get(p_channel_index);

	if (!ch)
		return 0;

	return ch[get_data_index(x, y)];
}
void Terrain2DChunk::set_voxel(const uint8_t p_value, const int p_x, const int p_y, const int p_channel_index) {
	int x = p_x + _margin_start;
	int y = p_y + _margin_start;

	ERR_FAIL_INDEX(p_channel_index, _channels.size());
	ERR_FAIL_COND_MSG(!validate_data_position(x, y), "Error, index out of range! " + String::num(x) + " " + String::num(y));

	uint8_t *ch = channel_get_valid(p_channel_index);

	ch[get_data_index(x, y)] = p_value;
}

int Terrain2DChunk::channel_get_count() const {
	return _channels.size();
}

void Terrain2DChunk::channel_set_count(const int count) {
	if (count == _channels.size())
		return;

	if (_channels.size() >= count) {
		for (int i = count; i < _channels.size(); ++i) {
			uint8_t *ch = _channels[i];

			if (ch != NULL) {
				memdelete_arr(ch);
			}

			_channels.set(i, NULL);
		}

		_channels.resize(count);
		return;
	}

	int s = _channels.size();
	_channels.resize(count);

	for (int i = s; i < count; ++i) {
		_channels.set(i, NULL);
	}
}
bool Terrain2DChunk::channel_is_allocated(const int channel_index) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), false);

	return _channels[channel_index] != NULL;
}
void Terrain2DChunk::channel_ensure_allocated(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	if (_channels[channel_index] == NULL)
		channel_allocate(channel_index, default_value);
}
void Terrain2DChunk::channel_allocate(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	if (_channels[channel_index] != NULL)
		return;

	uint32_t size = _data_size_x * _data_size_y;

	uint8_t *ch = memnew_arr(uint8_t, size);
	memset(ch, default_value, size);

	_channels.set(channel_index, ch);
}
void Terrain2DChunk::channel_fill(const uint8_t value, const int channel_index) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		channel_allocate(channel_index, value);
		return;
	}

	uint32_t size = get_data_size();

	for (uint32_t i = 0; i < size; ++i) {
		ch[i] = value;
	}
}
void Terrain2DChunk::channel_dealloc(const int channel_index) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	uint8_t *ch = _channels.get(channel_index);

	if (ch != NULL) {
		memdelete_arr(ch);

		_channels.set(channel_index, NULL);
	}
}

uint8_t *Terrain2DChunk::channel_get(const int channel_index) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), NULL);

	return _channels.get(channel_index);
}
uint8_t *Terrain2DChunk::channel_get_valid(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), 0);

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		channel_allocate(channel_index, default_value);

		return _channels.get(channel_index);
	}

	return ch;
}

PoolByteArray Terrain2DChunk::channel_get_array(const int channel_index) const {
	PoolByteArray arr;

	uint32_t size = _data_size_x * _data_size_y;

	if (channel_index >= _channels.size())
		return arr;

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL)
		return arr;

	arr.resize(size);

	for (uint32_t i = 0; i < size; ++i) {
		arr.set(i, ch[i]);
	}

	return arr;
}
void Terrain2DChunk::channel_set_array(const int channel_index, const PoolByteArray &array) {
	if (array.size() == 0)
		return;

	if (_channels.size() <= channel_index)
		channel_set_count(channel_index + 1);

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		if (_channels[channel_index] != NULL)
			return;

		ch = memnew_arr(uint8_t, array.size());
		_channels.set(channel_index, ch);
	}

	for (int i = 0; i < array.size(); ++i) {
		ch[i] = array[i];
	}
}

PoolByteArray Terrain2DChunk::channel_get_compressed(const int channel_index) const {
	PoolByteArray arr;

	int size = _data_size_x * _data_size_y;

	if (channel_index >= _channels.size())
		return arr;

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL)
		return arr;

	int bound = LZ4_compressBound(size);
	arr.resize(bound);

	PoolByteArray::Write w = arr.write();

	int ns = LZ4_compress_default(reinterpret_cast<char *>(ch), reinterpret_cast<char *>(w.ptr()), size, bound);

	w.release();

	arr.resize(ns);

	return arr;
}
void Terrain2DChunk::channel_set_compressed(const int channel_index, const PoolByteArray &data) {
	if (data.size() == 0)
		return;

	int size = _data_size_x * _data_size_y;

	if (_channels.size() <= channel_index)
		channel_set_count(channel_index + 1);

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		if (_channels[channel_index] != NULL)
			return;

		ch = memnew_arr(uint8_t, size);
		_channels.set(channel_index, ch);
	}

	int ds = data.size();

	PoolByteArray::Read r = data.read();

	//We are not going to write to it
	uint8_t *data_arr = const_cast<uint8_t *>(r.ptr());

	LZ4_decompress_safe(reinterpret_cast<char *>(data_arr), reinterpret_cast<char *>(ch), ds, size);
}

_FORCE_INLINE_ int Terrain2DChunk::get_index(const int x, const int y) const {
	return ((x + _margin_start) + _data_size_x * (y + _margin_start));
}

_FORCE_INLINE_ int Terrain2DChunk::get_data_index(const int x, const int y) const {
	return (x + _data_size_x * y);
}

_FORCE_INLINE_ int Terrain2DChunk::get_data_size() const {
	return _data_size_x * _data_size_y;
}

//Terra Structures

Ref<Terrain2DStructure> Terrain2DChunk::voxel_structure_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_structures.size(), Ref<Terrain2DStructure>());

	return _voxel_structures.get(index);
}
void Terrain2DChunk::voxel_structure_add(const Ref<Terrain2DStructure> &structure) {
	_voxel_structures.push_back(structure);
}
void Terrain2DChunk::voxel_structure_remove(const Ref<Terrain2DStructure> &structure) {
	if (!structure.is_valid())
		return;

	int index = _voxel_structures.find(structure);

	if (index != -1)
		_voxel_structures.remove(index);
}
void Terrain2DChunk::voxel_structure_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _voxel_structures.size());

	_voxel_structures.remove(index);
}
void Terrain2DChunk::voxel_structure_clear() {
	_voxel_structures.clear();
}
int Terrain2DChunk::voxel_structure_get_count() const {
	return _voxel_structures.size();
}
void Terrain2DChunk::voxel_structure_add_at_position(Ref<Terrain2DStructure> structure, const Vector2 &world_position) {
	ERR_FAIL_COND(!structure.is_valid());

	structure->set_position_x(static_cast<int>(world_position.x / _cell_size_x));
	structure->set_position_y(static_cast<int>(world_position.y / _cell_size_y));

	voxel_structure_add(structure);
}

Vector<Variant> Terrain2DChunk::voxel_structures_get() {
	VARIANT_ARRAY_GET(_voxel_structures);
}
void Terrain2DChunk::voxel_structures_set(const Vector<Variant> &structures) {
	voxel_structure_clear();

	for (int i = 0; i < structures.size(); ++i) {
		Ref<Terrain2DLight> structure = Ref<Terrain2DLight>(structures[i]);

		voxel_structure_add(structure);
	}
}

void Terrain2DChunk::build() {
	ERR_FAIL_COND(!ObjectDB::instance_validate(get_voxel_world()));
	ERR_FAIL_COND(!get_voxel_world()->is_inside_tree());
	ERR_FAIL_COND(!is_in_tree());

	call("_build");
}

void Terrain2DChunk::_build() {
	if (get_is_generating()) {
		_queued_generation = true;
		return;
	}

	_is_generating = true;

	job_next();
}

void Terrain2DChunk::clear() {
	ERR_FAIL_COND_MSG(!has_method("_clear"), "Terrain2DChunk: _clear() is missing! Please implement it!");

	call("_clear");
}

void Terrain2DChunk::finalize_build() {
	if (has_method("_finalize_build")) {
		call("_finalize_build");
	}
}

void Terrain2DChunk::cancel_build() {
	_queued_generation = false;

	_abort_build = true;

#if THREAD_POOL_PRESENT
	if (_is_generating) {
		Ref<Terrain2DJob> job = job_get_current();

		if (job.is_valid()) {
			ThreadPool::get_singleton()->cancel_job(job);
		}
	}
#endif
}

void Terrain2DChunk::bake_lights() {
	if (has_method("_bake_lights"))
		call("_bake_lights");
}
void Terrain2DChunk::bake_light(Ref<Terrain2DLight> light) {
	if (!light.is_valid())
		return;

	if (has_method("_bake_lights"))
		call("_bake_light", light);
}
void Terrain2DChunk::clear_baked_lights() {
	if (has_method("_clear_baked_lights"))
		call("_clear_baked_lights");
}

#if PROPS_2D_PRESENT
void Terrain2DChunk::prop_add(const Transform2D &tarnsform, const Ref<Prop2DData> &prop) {
	ERR_FAIL_COND(!prop.is_valid());

	Prop2DDataStore s;
	s.transform = tarnsform;
	s.prop = prop;

	_props.push_back(s);
}
Ref<Prop2DData> Terrain2DChunk::prop_get(int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<Prop2DData>());

	return _props.get(index).prop;
}
Transform2D Terrain2DChunk::prop_get_tarnsform(const int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), Transform2D());

	return _props.get(index).transform;
}
int Terrain2DChunk::prop_get_count() const {
	return _props.size();
}
void Terrain2DChunk::prop_remove(const int index) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.remove(index);
}
void Terrain2DChunk::props_clear() {
	_props.clear();
}
#endif

#if MESH_DATA_RESOURCE_PRESENT
int Terrain2DChunk::mesh_data_resource_addv(const Vector2 &local_data_pos, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture, const Color &color, const bool apply_voxel_scale) {
	//TODO
	/*

		ERR_FAIL_COND_V(!mesh.is_valid(), 0);

		int index = _mesh_data_resources.size();

		MeshDataResourceEntry e;

		if (apply_voxel_scale) {
			e.transform = Transform(Basis().scaled(Vector3(_voxel_scale, _voxel_scale, _voxel_scale)));
			e.transform.origin = local_data_pos * _voxel_scale;
		} else {
			e.transform.origin = local_data_pos;
		}

		e.mesh = mesh;
		e.texture = texture;
		e.color = color;

		AABB aabb = AABB(Vector3(), get_world_size());
		AABB mesh_aabb = e.transform.xform(mesh->get_aabb());
		e.is_inside = aabb.encloses(mesh_aabb);

	#if PROPS_2D_PRESENT
		if (get_library().is_valid() && texture.is_valid()) {
			e.uv_rect = get_library()->get_prop_uv_rect(texture);
		} else {
			e.uv_rect = Rect2(0, 0, 1, 1);
		}
	#else
		e.uv_rect = Rect2(0, 0, 1, 1);
	#endif

		_mesh_data_resources.push_back(e);

		if (has_method("_mesh_data_resource_added"))
			call("_mesh_data_resource_added", index);

		return index;

		*/

	return 0;
}

int Terrain2DChunk::mesh_data_resource_add(const Transform2D &local_transform, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture, const Color &color, const bool apply_voxel_scale) {
	//TODO

	/*
	ERR_FAIL_COND_V(!mesh.is_valid(), 0);

	int index = _mesh_data_resources.size();

	MeshDataResourceEntry e;

	e.transform = local_transform;

	if (apply_voxel_scale) {
		e.transform.basis = e.transform.basis.scaled(Vector3(_voxel_scale, _voxel_scale, _voxel_scale));
		e.transform.origin = e.transform.origin * _voxel_scale;
	}

	e.mesh = mesh;
	e.texture = texture;
	e.color = color;

	AABB aabb = AABB(Vector3(), get_world_size());
	AABB mesh_aabb = e.transform.xform(mesh->get_aabb());
	e.is_inside = aabb.encloses(mesh_aabb);

#if PROPS_2D_PRESENT
	if (get_library().is_valid() && texture.is_valid())
		e.uv_rect = get_library()->get_prop_uv_rect(texture);
	else
		e.uv_rect = Rect2(0, 0, 1, 1);
#else
	e.uv_rect = Rect2(0, 0, 1, 1);
#endif

	_mesh_data_resources.push_back(e);

	if (has_method("_mesh_data_resource_added"))
		call("_mesh_data_resource_added", index);

	return index;
	*/

	return 0;
}

Ref<MeshDataResource> Terrain2DChunk::mesh_data_resource_get(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Ref<MeshDataResource>());

	return _mesh_data_resources[index].mesh;
}

void Terrain2DChunk::mesh_data_resource_set(const int index, const Ref<MeshDataResource> &mesh) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());
}

Ref<Texture> Terrain2DChunk::mesh_data_resource_get_texture(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Ref<Texture>());

	return _mesh_data_resources[index].texture;
}
void Terrain2DChunk::mesh_data_resource_set_texture(const int index, const Ref<Texture> &texture) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].texture = texture;
}

Color Terrain2DChunk::mesh_data_resource_get_color(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Color());

	return _mesh_data_resources[index].color;
}
void Terrain2DChunk::mesh_data_resource_set_color(const int index, const Color &color) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].color = color;
}

Rect2 Terrain2DChunk::mesh_data_resource_get_uv_rect(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Rect2());

	return _mesh_data_resources[index].uv_rect;
}
void Terrain2DChunk::mesh_data_resource_set_uv_rect(const int index, const Rect2 &uv_rect) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].uv_rect = uv_rect;
}

Transform2D Terrain2DChunk::mesh_data_resource_get_transform(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Transform2D());

	return _mesh_data_resources.write[index].transform;
}
void Terrain2DChunk::mesh_data_resource_set_transform(const int index, const Transform2D &transform) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].transform = transform;
}

bool Terrain2DChunk::mesh_data_resource_get_is_inside(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), true);

	return _mesh_data_resources[index].is_inside;
}
void Terrain2DChunk::mesh_data_resource_set_is_inside(const int index, const bool inside) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].is_inside = inside;
}

int Terrain2DChunk::mesh_data_resource_get_count() const {
	return _mesh_data_resources.size();
}
void Terrain2DChunk::mesh_data_resource_remove(const int index) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.remove(index);
}
void Terrain2DChunk::mesh_data_resource_clear() {
	_mesh_data_resources.clear();
}

#endif

int Terrain2DChunk::collider_add(const Transform2D &local_transform, const Ref<Shape2D> &shape, const RID &shape_rid, const RID &body) {
	ERR_FAIL_COND_V(!shape.is_valid() && shape_rid == RID(), 0);

	int index = _colliders.size();

	ColliderBody e;
	e.transform = local_transform;
	e.body = body;
	e.shape = shape;
	e.shape_rid = shape_rid;

	_colliders.push_back(e);

	return index;
}

Transform2D Terrain2DChunk::collider_get_transform(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Transform2D());

	return _colliders[index].transform;
}
void Terrain2DChunk::collider_set_transform(const int index, const Transform2D &transform) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].transform = transform;
}

Ref<Shape2D> Terrain2DChunk::collider_get_shape(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Ref<Shape2D>());

	return _colliders[index].shape;
}

void Terrain2DChunk::collider_set_shape(const int index, const Ref<Shape2D> &shape) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].shape = shape;
}

RID Terrain2DChunk::collider_get_shape_rid(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].shape_rid;
}
void Terrain2DChunk::collider_set_shape_rid(const int index, const RID &rid) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].shape_rid = rid;
}

RID Terrain2DChunk::collider_get_body(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].body;
}
void Terrain2DChunk::collider_set_body(const int index, const RID &rid) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].body = rid;
}

int Terrain2DChunk::collider_get_count() const {
	return _colliders.size();
}
void Terrain2DChunk::collider_remove(const int index) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.remove(index);
}
void Terrain2DChunk::colliders_clear() {
	_colliders.clear();
}

void Terrain2DChunk::enter_tree() {
	_is_in_tree = true;

	if (has_method("_enter_tree"))
		call("_enter_tree");
}
void Terrain2DChunk::exit_tree() {
	_is_in_tree = false;

	if (has_method("_exit_tree"))
		call("_exit_tree");
}
void Terrain2DChunk::process(const float delta) {
	if (has_method("_process"))
		call("_process", delta);
}
void Terrain2DChunk::physics_process(const float delta) {
	if (has_method("_physics_process"))
		call("_physics_process", delta);
}
void Terrain2DChunk::world_transform_changed() {
	call("_world_transform_changed");
}
void Terrain2DChunk::visibility_changed(const bool visible) {
	if (has_method("_visibility_changed"))
		call("_visibility_changed", _is_visible);
}
void Terrain2DChunk::world_light_added(const Ref<Terrain2DLight> &light) {
	if (has_method("_world_light_added"))
		call("_world_light_added", light);
}
void Terrain2DChunk::world_light_removed(const Ref<Terrain2DLight> &light) {
	if (has_method("_world_light_removed"))
		call("_world_light_removed", light);
}
void Terrain2DChunk::generation_process(const float delta) {
	call("_generation_process", delta);
}
void Terrain2DChunk::generation_physics_process(const float delta) {
	call("_generation_physics_process", delta);
}

void Terrain2DChunk::draw() {
	call("_draw");
}

Transform2D Terrain2DChunk::get_transform() const {
	return _transform;
}
void Terrain2DChunk::set_transform(const Transform2D &transform) {
	_transform = transform;
}

Transform2D Terrain2DChunk::get_global_transform() const {
	ERR_FAIL_COND_V(!get_voxel_world(), Transform2D());

	return get_voxel_world()->get_global_transform() * _transform;
}

Vector2 Terrain2DChunk::to_local(Vector2 p_global) const {
	return get_global_transform().affine_inverse().xform(p_global);
}

Vector2 Terrain2DChunk::to_global(Vector2 p_local) const {
	return get_global_transform().xform(p_local);
}

bool Terrain2DChunk::is_safe_to_delete() {
#if THREAD_POOL_PRESENT
	if (!_is_generating) {
		return true;
	}

	Ref<Terrain2DJob> job = job_get_current();

	if (!job.is_valid()) {
		return true;
	}

	return !ThreadPool::get_singleton()->has_job(job);
#else
	return true;
#endif
}

void Terrain2DChunk::setup_canvas_items_size(const int amount) {
	if (amount > _canvas_items.size()) {
		while (amount > _canvas_items.size()) {
			RID ci = RenderingServer::get_singleton()->canvas_item_create();

			if (_voxel_world) {
				RenderingServer::get_singleton()->canvas_item_set_parent(ci, get_voxel_world()->get_canvas_item());
			}

			_canvas_items.push_back(ci);
		}

		return;
	} else {
		while (amount != _canvas_items.size()) {
			RID ci = _canvas_items[_canvas_items.size() - 1];

			RenderingServer::get_singleton()->free(ci);

			_canvas_items.resize(_canvas_items.size() - 1);
		}
	}
}

Terrain2DChunk::Terrain2DChunk() {
	_is_processing = false;
	_is_phisics_processing = false;
	_is_in_tree = false;

	_is_visible = true;

	_is_generating = false;
	_dirty = false;
	_state = TERRAIN_2D_CHUNK_STATE_OK;

	_cell_size_x = 32;
	_cell_size_y = 32;

	_voxel_world = NULL;

	_position_x = 0;
	_position_y = 0;

	_size_x = 0;
	_size_y = 0;

	_data_size_x = 0;
	_data_size_y = 0;

	_margin_start = 0;
	_margin_end = 0;

	_material_cache_key = 0;
	_material_cache_key_has = false;

	_liquid_material_cache_key = 0;
	_liquid_material_cache_key_has = false;

	_prop_material_cache_key = 0;
	_prop_material_cache_key_has = false;

	_current_job = -1;

	_queued_generation = false;
}

Terrain2DChunk::~Terrain2DChunk() {
	setup_canvas_items_size(0);

	if (_library.is_valid()) {
		_library.unref();
	}

#if PROPS_2D_PRESENT
	props_clear();
#endif

#if MESH_DATA_RESOURCE_PRESENT
	mesh_data_resource_clear();
#endif

	for (int i = 0; i < _channels.size(); ++i) {
		uint8_t *ch = _channels[i];

		if (ch != NULL) {
			memdelete_arr(ch);
		}
	}

	for (int i = 0; i < _colliders.size(); ++i) {
		//PhysicsServer::get_singleton()->free(_colliders[i].body);
	}

	_colliders.clear();

	_jobs.clear();
}

void Terrain2DChunk::_enter_tree() {
	if (_voxel_world) {
		for (int i = 0; i < _canvas_items.size(); ++i) {
			RenderingServer::get_singleton()->canvas_item_set_parent(_canvas_items[i], get_voxel_world()->get_canvas_item());
		}
	}

	for (int i = 0; i < _jobs.size(); ++i) {
		Ref<Terrain2DJob> j = _jobs[i];

		if (j.is_valid()) {
			j->set_chunk(Ref<Terrain2DChunk>(this));
		}
	}
}

void Terrain2DChunk::_exit_tree() {
	if (_is_generating) {
		cancel_build();
	}

	for (int i = 0; i < _jobs.size(); ++i) {
		Ref<Terrain2DJob> j = _jobs[i];

		if (j.is_valid()) {
			j->chunk_exit_tree();
		}
	}

	if (_library.is_valid() && _library->supports_caching()) {
		if (material_cache_key_has()) {
			_library->material_cache_unref(material_cache_key_get());
		}
	}
}

void Terrain2DChunk::_generation_process(const float delta) {
	if (_abort_build) {
		return;
	}

	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size())
		return;

	Ref<Terrain2DJob> job = _jobs[_current_job];

	ERR_FAIL_COND(!job.is_valid());

	if (job->get_build_phase_type() == Terrain2DJob::BUILD_PHASE_TYPE_PROCESS) {
		if (!_voxel_world->can_chunk_do_build_step())
			return;

		job->process(delta);

		if (job->get_build_phase_type() == Terrain2DJob::BUILD_PHASE_TYPE_NORMAL) {
#if THREAD_POOL_PRESENT
			ThreadPool::get_singleton()->add_job(job);
#else
			job->execute();
#endif
		}
	}
}
void Terrain2DChunk::_generation_physics_process(const float delta) {
	if (_abort_build) {
		return;
	}

	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size())
		return;

	Ref<Terrain2DJob> job = _jobs[_current_job];

	ERR_FAIL_COND(!job.is_valid());

	if (job->get_build_phase_type() == Terrain2DJob::BUILD_PHASE_TYPE_PHYSICS_PROCESS) {
		if (!_voxel_world->can_chunk_do_build_step())
			return;

		job->physics_process(delta);

		if (job->get_build_phase_type() == Terrain2DJob::BUILD_PHASE_TYPE_NORMAL) {
#if THREAD_POOL_PRESENT
			ThreadPool::get_singleton()->add_job(job);
#else
			job->execute();
#endif
		}
	}
}

void Terrain2DChunk::_world_transform_changed() {
	Transform2D t;
	Vector2 pos = Vector2(_position_x * static_cast<int>(_size_x) * _cell_size_x, _position_y * static_cast<int>(_size_y) * _cell_size_y);

	pos = _mesh_transform_terrain.xform(pos);

	//t *= _custom_transform;
	t.set_origin(pos);

	set_transform(t);

	if (get_canvas_item_count() > 0) {
		RenderingServer::get_singleton()->canvas_item_set_transform(get_canvas_item(0), t);
	}

#if TOOLS_ENABLED
	if (_debug_canvas_item.is_valid()) {
		RenderingServer::get_singleton()->canvas_item_set_transform(_debug_canvas_item, t);
	}
#endif
}

/*
bool Terrain2DChunk::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name.begins_with("channels/")) {

		int index = name.get_slicec('/', 1).to_int();

		if (_channels.size() <= index) {
			set_channel_count(index);
		}

		PoolByteArray arr = p_value;

		if (arr.size() == 0)
			return true;

		set_channel_array(index, arr);
	}

	return true;
}

bool Terrain2DChunk::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name.begins_with("channels/")) {

		int index = name.get_slicec('/', 1).to_int();

		r_ret = get_channel_array(index);

		return true;
	}

	return false;
}

void Terrain2DChunk::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < _channels.size(); ++i) {
		p_list->push_back(PropertyInfo(Variant::POOL_BYTE_ARRAY, "channels/" + String::num(i), PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL));
	}
}
*/

void Terrain2DChunk::_bind_methods() {
	ADD_SIGNAL(MethodInfo("mesh_generation_finished", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));

	BIND_VMETHOD(MethodInfo("_mesh_data_resource_added", PropertyInfo(Variant::INT, "index")));

	BIND_VMETHOD(MethodInfo("_channel_setup"));

	BIND_VMETHOD(MethodInfo("_bake_lights"));
	BIND_VMETHOD(MethodInfo("_bake_light", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DLight")));
	BIND_VMETHOD(MethodInfo("_clear_baked_lights"));

	ClassDB::bind_method(D_METHOD("bake_lights"), &Terrain2DChunk::bake_lights);
	ClassDB::bind_method(D_METHOD("bake_light", "light"), &Terrain2DChunk::bake_light);
	ClassDB::bind_method(D_METHOD("clear_baked_lights"), &Terrain2DChunk::clear_baked_lights);

	BIND_VMETHOD(MethodInfo("_enter_tree"));
	BIND_VMETHOD(MethodInfo("_exit_tree"));
	BIND_VMETHOD(MethodInfo("_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_physics_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_world_transform_changed"));
	BIND_VMETHOD(MethodInfo("_visibility_changed", PropertyInfo(Variant::BOOL, "visible")));
	BIND_VMETHOD(MethodInfo("_world_light_added", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DLight")));
	BIND_VMETHOD(MethodInfo("_world_light_removed", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DLight")));
	BIND_VMETHOD(MethodInfo("_draw"));

	BIND_VMETHOD(MethodInfo("_generation_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_generation_physics_process", PropertyInfo(Variant::REAL, "delta")));

	BIND_VMETHOD(MethodInfo("_finalize_build"));

	ClassDB::bind_method(D_METHOD("enter_tree"), &Terrain2DChunk::enter_tree);
	ClassDB::bind_method(D_METHOD("exit_tree"), &Terrain2DChunk::exit_tree);
	ClassDB::bind_method(D_METHOD("process", "delta"), &Terrain2DChunk::process);
	ClassDB::bind_method(D_METHOD("physics_process", "delta"), &Terrain2DChunk::physics_process);
	ClassDB::bind_method(D_METHOD("world_transform_changed"), &Terrain2DChunk::world_transform_changed);
	ClassDB::bind_method(D_METHOD("visibility_changed", "visible"), &Terrain2DChunk::visibility_changed);
	ClassDB::bind_method(D_METHOD("world_light_added", "light"), &Terrain2DChunk::world_light_added);
	ClassDB::bind_method(D_METHOD("world_light_removed", "light"), &Terrain2DChunk::world_light_removed);

	ClassDB::bind_method(D_METHOD("generation_process", "delta"), &Terrain2DChunk::generation_process);
	ClassDB::bind_method(D_METHOD("generation_physics_process", "delta"), &Terrain2DChunk::generation_physics_process);

	ClassDB::bind_method(D_METHOD("finalize_build"), &Terrain2DChunk::finalize_build);

	ClassDB::bind_method(D_METHOD("cancel_build"), &Terrain2DChunk::cancel_build);

	ClassDB::bind_method(D_METHOD("get_process"), &Terrain2DChunk::get_process);
	ClassDB::bind_method(D_METHOD("set_process", "value"), &Terrain2DChunk::set_process);

	ClassDB::bind_method(D_METHOD("get_physics_process"), &Terrain2DChunk::get_physics_process);
	ClassDB::bind_method(D_METHOD("set_physics_process", "value"), &Terrain2DChunk::set_physics_process);

	ClassDB::bind_method(D_METHOD("is_in_tree"), &Terrain2DChunk::is_in_tree);

	ClassDB::bind_method(D_METHOD("get_transform"), &Terrain2DChunk::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &Terrain2DChunk::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "transform"), "set_transform", "get_transform");

	ClassDB::bind_method(D_METHOD("get_visible"), &Terrain2DChunk::get_visible);
	ClassDB::bind_method(D_METHOD("set_visible", "value"), &Terrain2DChunk::set_visible);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "visible"), "set_visible", "get_visible");

	ClassDB::bind_method(D_METHOD("get_is_generating"), &Terrain2DChunk::get_is_generating);
	ClassDB::bind_method(D_METHOD("set_is_generating", "value"), &Terrain2DChunk::set_is_generating);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_generating", PROPERTY_HINT_NONE, "", 0), "set_is_generating", "get_is_generating");

	ClassDB::bind_method(D_METHOD("is_build_aborted"), &Terrain2DChunk::is_build_aborted);

	ClassDB::bind_method(D_METHOD("get_dirty"), &Terrain2DChunk::get_dirty);
	ClassDB::bind_method(D_METHOD("set_dirty", "value"), &Terrain2DChunk::set_dirty);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "dirty", PROPERTY_HINT_NONE, "", 0), "set_dirty", "get_dirty");

	ClassDB::bind_method(D_METHOD("get_state"), &Terrain2DChunk::get_state);
	ClassDB::bind_method(D_METHOD("set_state", "value"), &Terrain2DChunk::set_state);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "state", PROPERTY_HINT_NONE, "", 0), "set_state", "get_state");

	ClassDB::bind_method(D_METHOD("get_position_x"), &Terrain2DChunk::get_position_x);
	ClassDB::bind_method(D_METHOD("set_position_x", "value"), &Terrain2DChunk::set_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_x"), "set_position_x", "get_position_x");

	ClassDB::bind_method(D_METHOD("get_position_y"), &Terrain2DChunk::get_position_y);
	ClassDB::bind_method(D_METHOD("set_position_y", "value"), &Terrain2DChunk::set_position_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_y"), "set_position_y", "get_position_y");

	ClassDB::bind_method(D_METHOD("get_size_x"), &Terrain2DChunk::get_size_x);
	ClassDB::bind_method(D_METHOD("set_size_x"), &Terrain2DChunk::set_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size_x"), "set_size_x", "get_size_x");

	ClassDB::bind_method(D_METHOD("get_size_y"), &Terrain2DChunk::get_size_y);
	ClassDB::bind_method(D_METHOD("set_size_y"), &Terrain2DChunk::set_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size_y"), "set_size_y", "get_size_y");

	ClassDB::bind_method(D_METHOD("get_data_size_x"), &Terrain2DChunk::get_data_size_x);
	ClassDB::bind_method(D_METHOD("set_data_size_x"), &Terrain2DChunk::set_data_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_size_x"), "set_data_size_x", "get_data_size_x");

	ClassDB::bind_method(D_METHOD("get_data_size_y"), &Terrain2DChunk::get_data_size_y);
	ClassDB::bind_method(D_METHOD("set_data_size_y"), &Terrain2DChunk::set_data_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_size_y"), "set_data_size_y", "get_data_size_y");

	ClassDB::bind_method(D_METHOD("get_position"), &Terrain2DChunk::get_position);
	ClassDB::bind_method(D_METHOD("set_position", "x", "z"), &Terrain2DChunk::set_position);

	ClassDB::bind_method(D_METHOD("get_world_position"), &Terrain2DChunk::get_world_position);
	ClassDB::bind_method(D_METHOD("get_world_size"), &Terrain2DChunk::get_world_size);
	ClassDB::bind_method(D_METHOD("get_world_rect"), &Terrain2DChunk::get_world_rect);

	ClassDB::bind_method(D_METHOD("get_margin_start"), &Terrain2DChunk::get_margin_start);
	ClassDB::bind_method(D_METHOD("set_margin_start"), &Terrain2DChunk::set_margin_start);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin_start"), "set_margin_start", "get_margin_start");

	ClassDB::bind_method(D_METHOD("get_margin_end"), &Terrain2DChunk::get_margin_end);
	ClassDB::bind_method(D_METHOD("set_margin_end"), &Terrain2DChunk::set_margin_end);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin_end"), "set_margin_end", "get_margin_end");

	ClassDB::bind_method(D_METHOD("material_cache_key_get"), &Terrain2DChunk::material_cache_key_get);
	ClassDB::bind_method(D_METHOD("material_cache_key_set"), &Terrain2DChunk::material_cache_key_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "material_cache_key", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "material_cache_key_set", "material_cache_key_get");

	ClassDB::bind_method(D_METHOD("material_cache_key_has"), &Terrain2DChunk::material_cache_key_has);
	ClassDB::bind_method(D_METHOD("material_cache_key_has_set"), &Terrain2DChunk::material_cache_key_has_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "material_cache_key_has", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "material_cache_key_has_set", "material_cache_key_has");

	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_get"), &Terrain2DChunk::liquid_material_cache_key_get);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_set"), &Terrain2DChunk::liquid_material_cache_key_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "liquid_material_cache_key", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "liquid_material_cache_key_set", "liquid_material_cache_key_get");

	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_has"), &Terrain2DChunk::liquid_material_cache_key_has);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_has_set"), &Terrain2DChunk::liquid_material_cache_key_has_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "liquid_material_cache_key_has", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "liquid_material_cache_key_has_set", "liquid_material_cache_key_has");

	ClassDB::bind_method(D_METHOD("prop_material_cache_key_get"), &Terrain2DChunk::prop_material_cache_key_get);
	ClassDB::bind_method(D_METHOD("prop_material_cache_key_set"), &Terrain2DChunk::prop_material_cache_key_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "prop_material_cache_key", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "prop_material_cache_key_set", "prop_material_cache_key_get");

	ClassDB::bind_method(D_METHOD("prop_material_cache_key_has"), &Terrain2DChunk::prop_material_cache_key_has);
	ClassDB::bind_method(D_METHOD("prop_material_cache_key_has_set"), &Terrain2DChunk::prop_material_cache_key_has_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "prop_material_cache_key_has", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "prop_material_cache_key_has_set", "prop_material_cache_key_has");

	ClassDB::bind_method(D_METHOD("get_library"), &Terrain2DChunk::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "value"), &Terrain2DChunk::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_cell_size_x"), &Terrain2DChunk::get_cell_size_x);
	ClassDB::bind_method(D_METHOD("set_cell_size_x", "value"), &Terrain2DChunk::set_cell_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cell_size_x"), "set_cell_size_x", "get_cell_size_x");

	ClassDB::bind_method(D_METHOD("get_cell_size_y"), &Terrain2DChunk::get_cell_size_y);
	ClassDB::bind_method(D_METHOD("set_cell_size_y", "value"), &Terrain2DChunk::set_cell_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cell_size_y"), "set_cell_size_y", "get_cell_size_y");

	ClassDB::bind_method(D_METHOD("job_get", "index"), &Terrain2DChunk::job_get);
	ClassDB::bind_method(D_METHOD("job_set", "index", "job"), &Terrain2DChunk::job_set);
	ClassDB::bind_method(D_METHOD("job_remove", "index"), &Terrain2DChunk::job_remove);
	ClassDB::bind_method(D_METHOD("job_add", "job"), &Terrain2DChunk::job_add);
	ClassDB::bind_method(D_METHOD("job_get_count"), &Terrain2DChunk::job_get_count);

	ClassDB::bind_method(D_METHOD("job_get_current_index"), &Terrain2DChunk::job_get_current_index);
	ClassDB::bind_method(D_METHOD("job_next"), &Terrain2DChunk::job_next);
	ClassDB::bind_method(D_METHOD("job_get_current"), &Terrain2DChunk::job_get_current);

	ClassDB::bind_method(D_METHOD("get_voxel_world"), &Terrain2DChunk::get_voxel_world);
	ClassDB::bind_method(D_METHOD("set_voxel_world", "world"), &Terrain2DChunk::set_voxel_world_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "voxel_world", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DWorld", 0), "set_voxel_world", "get_voxel_world");

	ADD_GROUP("Mesh Transforms", "mesh_transform");
	ClassDB::bind_method(D_METHOD("mesh_transform_terrain_get"), &Terrain2DChunk::mesh_transform_terrain_get);
	ClassDB::bind_method(D_METHOD("mesh_transform_terrain_set", "player"), &Terrain2DChunk::mesh_transform_terrain_set);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform_terrain"), "mesh_transform_terrain_set", "mesh_transform_terrain_get");

	ClassDB::bind_method(D_METHOD("mesh_transform_wall_north_get"), &Terrain2DChunk::mesh_transform_wall_north_get);
	ClassDB::bind_method(D_METHOD("mesh_transform_wall_north_set", "player"), &Terrain2DChunk::mesh_transform_wall_north_set);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform_wall_north"), "mesh_transform_wall_north_set", "mesh_transform_wall_north_get");

	ClassDB::bind_method(D_METHOD("mesh_transform_wall_south_get"), &Terrain2DChunk::mesh_transform_wall_south_get);
	ClassDB::bind_method(D_METHOD("mesh_transform_wall_south_set", "player"), &Terrain2DChunk::mesh_transform_wall_south_set);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform_wall_south"), "mesh_transform_wall_south_set", "mesh_transform_wall_south_get");

	ClassDB::bind_method(D_METHOD("mesh_transform_wall_east_get"), &Terrain2DChunk::mesh_transform_wall_east_get);
	ClassDB::bind_method(D_METHOD("mesh_transform_wall_east_set", "player"), &Terrain2DChunk::mesh_transform_wall_east_set);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform_wall_east"), "mesh_transform_wall_east_set", "mesh_transform_wall_east_get");

	ClassDB::bind_method(D_METHOD("mesh_transform_wall_west_get"), &Terrain2DChunk::mesh_transform_wall_west_get);
	ClassDB::bind_method(D_METHOD("mesh_transform_wall_west_set", "player"), &Terrain2DChunk::mesh_transform_wall_west_set);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform_wall_west"), "mesh_transform_wall_west_set", "mesh_transform_wall_west_get");

	ClassDB::bind_method(D_METHOD("get_default_tile_shape"), &Terrain2DChunk::get_default_tile_shape);
	ClassDB::bind_method(D_METHOD("set_default_tile_shape", "shape"), &Terrain2DChunk::set_default_tile_shape);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "default_tile_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape2D"), "set_default_tile_shape", "get_default_tile_shape");

	//Terra Data
	ClassDB::bind_method(D_METHOD("channel_setup"), &Terrain2DChunk::channel_setup);

	ClassDB::bind_method(D_METHOD("set_size", "size_x", "size_y", "margin_start", "margin_end"), &Terrain2DChunk::set_size, DEFVAL(0), DEFVAL(0));

	ClassDB::bind_method(D_METHOD("validate_data_position", "x", "y"), &Terrain2DChunk::validate_data_position);

	ClassDB::bind_method(D_METHOD("get_voxel", "x", "y", "index"), &Terrain2DChunk::get_voxel);
	ClassDB::bind_method(D_METHOD("set_voxel", "value", "x", "y", "index"), &Terrain2DChunk::set_voxel);

	ClassDB::bind_method(D_METHOD("channel_get_count"), &Terrain2DChunk::channel_get_count);
	ClassDB::bind_method(D_METHOD("channel_set_count", "count"), &Terrain2DChunk::channel_set_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_count"), "channel_set_count", "channel_get_count");

	ClassDB::bind_method(D_METHOD("channel_is_allocated", "index"), &Terrain2DChunk::channel_is_allocated);
	ClassDB::bind_method(D_METHOD("channel_ensure_allocated", "index", "default_value"), &Terrain2DChunk::channel_ensure_allocated);
	ClassDB::bind_method(D_METHOD("channel_allocate", "index", "default_value"), &Terrain2DChunk::channel_allocate);
	ClassDB::bind_method(D_METHOD("channel_fill", "value", "index"), &Terrain2DChunk::channel_fill);
	ClassDB::bind_method(D_METHOD("channel_dealloc", "index"), &Terrain2DChunk::channel_dealloc);

	ClassDB::bind_method(D_METHOD("channel_get_array", "index"), &Terrain2DChunk::channel_get_array);
	ClassDB::bind_method(D_METHOD("channel_set_array", "index", "array"), &Terrain2DChunk::channel_set_array);

	ClassDB::bind_method(D_METHOD("channel_get_compressed", "index"), &Terrain2DChunk::channel_get_compressed);
	ClassDB::bind_method(D_METHOD("channel_set_compressed", "index", "array"), &Terrain2DChunk::channel_set_compressed);

	ClassDB::bind_method(D_METHOD("get_index", "x", "y"), &Terrain2DChunk::get_index);
	ClassDB::bind_method(D_METHOD("get_data_index", "x", "y"), &Terrain2DChunk::get_data_index);
	ClassDB::bind_method(D_METHOD("get_data_size"), &Terrain2DChunk::get_data_size);

	ClassDB::bind_method(D_METHOD("voxel_structure_get", "index"), &Terrain2DChunk::voxel_structure_get);
	ClassDB::bind_method(D_METHOD("voxel_structure_add", "structure"), &Terrain2DChunk::voxel_structure_add);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove", "structure"), &Terrain2DChunk::voxel_structure_remove);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove_index", "index"), &Terrain2DChunk::voxel_structure_remove_index);
	ClassDB::bind_method(D_METHOD("voxel_structure_clear"), &Terrain2DChunk::voxel_structure_clear);
	ClassDB::bind_method(D_METHOD("voxel_structure_get_count"), &Terrain2DChunk::voxel_structure_get_count);
	ClassDB::bind_method(D_METHOD("voxel_structure_add_at_position", "structure", "world_position"), &Terrain2DChunk::voxel_structure_add_at_position);

	ClassDB::bind_method(D_METHOD("voxel_structures_get"), &Terrain2DChunk::voxel_structures_get);
	ClassDB::bind_method(D_METHOD("voxel_structures_set"), &Terrain2DChunk::voxel_structures_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_structures", PROPERTY_HINT_NONE, "23/19:Terrain2DStructure", PROPERTY_USAGE_DEFAULT, "Terrain2DStructure"), "voxel_structures_set", "voxel_structures_get");

	//Meshes

#if PROPS_2D_PRESENT
	ClassDB::bind_method(D_METHOD("prop_add", "prop"), &Terrain2DChunk::prop_add);
	ClassDB::bind_method(D_METHOD("prop_get", "index"), &Terrain2DChunk::prop_get);
	ClassDB::bind_method(D_METHOD("prop_get_count"), &Terrain2DChunk::prop_get_count);
	ClassDB::bind_method(D_METHOD("prop_remove", "index"), &Terrain2DChunk::prop_remove);
	ClassDB::bind_method(D_METHOD("props_clear"), &Terrain2DChunk::props_clear);
#endif

#if MESH_DATA_RESOURCE_PRESENT
	ClassDB::bind_method(D_METHOD("mesh_data_resource_addv", "local_data_pos", "mesh", "texture", "color", "apply_voxel_scale"), &Terrain2DChunk::mesh_data_resource_addv, DEFVAL(Ref<Texture>()), DEFVAL(Color(1, 1, 1, 1)), DEFVAL(true));
	ClassDB::bind_method(D_METHOD("mesh_data_resource_add", "local_transform", "mesh", "texture", "color", "apply_voxel_scale"), &Terrain2DChunk::mesh_data_resource_add, DEFVAL(Ref<Texture>()), DEFVAL(Color(1, 1, 1, 1)), DEFVAL(true));

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get", "index"), &Terrain2DChunk::mesh_data_resource_get);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set", "index", "mesh"), &Terrain2DChunk::mesh_data_resource_set);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_texture", "index"), &Terrain2DChunk::mesh_data_resource_get_texture);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_texture", "index", "texture"), &Terrain2DChunk::mesh_data_resource_set_texture);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_color", "index"), &Terrain2DChunk::mesh_data_resource_get_color);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_color", "index", "color"), &Terrain2DChunk::mesh_data_resource_set_color);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_uv_rect", "index"), &Terrain2DChunk::mesh_data_resource_get_uv_rect);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_uv_rect", "index", "uv_rect"), &Terrain2DChunk::mesh_data_resource_set_uv_rect);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_transform", "index"), &Terrain2DChunk::mesh_data_resource_get_transform);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_transform", "index", "transform"), &Terrain2DChunk::mesh_data_resource_set_transform);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_is_inside", "index"), &Terrain2DChunk::mesh_data_resource_get_is_inside);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_is_inside", "index", "inside"), &Terrain2DChunk::mesh_data_resource_set_is_inside);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_count"), &Terrain2DChunk::mesh_data_resource_get_count);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_remove", "index"), &Terrain2DChunk::mesh_data_resource_remove);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_clear"), &Terrain2DChunk::mesh_data_resource_clear);
#endif

	ClassDB::bind_method(D_METHOD("collider_add", "local_transform", "shape", "shape_rid", "body"), &Terrain2DChunk::collider_add, DEFVAL(RID()), DEFVAL(RID()));

	ClassDB::bind_method(D_METHOD("collider_get_transform", "index"), &Terrain2DChunk::collider_get_transform);
	ClassDB::bind_method(D_METHOD("collider_set_transform", "index", "transform"), &Terrain2DChunk::collider_set_transform);

	ClassDB::bind_method(D_METHOD("collider_get_shape", "index"), &Terrain2DChunk::collider_get_shape);
	ClassDB::bind_method(D_METHOD("collider_set_shape", "index", "shape"), &Terrain2DChunk::collider_set_shape);

	ClassDB::bind_method(D_METHOD("collider_get_shape_rid", "index"), &Terrain2DChunk::collider_get_shape_rid);
	ClassDB::bind_method(D_METHOD("collider_set_shape_rid", "index", "rid"), &Terrain2DChunk::collider_set_shape_rid);

	ClassDB::bind_method(D_METHOD("collider_get_body", "index"), &Terrain2DChunk::collider_get_body);
	ClassDB::bind_method(D_METHOD("collider_set_body", "index", "rid"), &Terrain2DChunk::collider_set_body);

	ClassDB::bind_method(D_METHOD("collider_get_count"), &Terrain2DChunk::collider_get_count);
	ClassDB::bind_method(D_METHOD("collider_remove", "index"), &Terrain2DChunk::collider_remove);
	ClassDB::bind_method(D_METHOD("colliders_clear"), &Terrain2DChunk::colliders_clear);

	BIND_VMETHOD(MethodInfo("_build"));
	ClassDB::bind_method(D_METHOD("build"), &Terrain2DChunk::build);
	ClassDB::bind_method(D_METHOD("_build"), &Terrain2DChunk::_build);

	ClassDB::bind_method(D_METHOD("get_global_transform"), &Terrain2DChunk::get_global_transform);
	ClassDB::bind_method(D_METHOD("to_local", "global"), &Terrain2DChunk::to_local);
	ClassDB::bind_method(D_METHOD("to_global", "local"), &Terrain2DChunk::to_global);

	ClassDB::bind_method(D_METHOD("_world_transform_changed"), &Terrain2DChunk::_world_transform_changed);
	ClassDB::bind_method(D_METHOD("_enter_tree"), &Terrain2DChunk::_enter_tree);
	ClassDB::bind_method(D_METHOD("_exit_tree"), &Terrain2DChunk::_exit_tree);

	ClassDB::bind_method(D_METHOD("_generation_process"), &Terrain2DChunk::_generation_process);
	ClassDB::bind_method(D_METHOD("_generation_physics_process"), &Terrain2DChunk::_generation_physics_process);

	ClassDB::bind_method(D_METHOD("is_safe_to_delete"), &Terrain2DChunk::is_safe_to_delete);
}
