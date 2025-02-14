/*************************************************************************/
/*  terrain_chunk.cpp                                                    */
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

#include "terrain_chunk.h"

#include "terrain_world.h"

#include "../defines.h"

#include "core/object/message_queue.h"
#include "jobs/terrain_job.h"
#include "servers/physics_server.h"
#include "terrain_structure.h"

#include "core/os/thread_pool.h"

#include "modules/modules_enabled.gen.h"

#include "core/object/method_bind_ext.gen.inc"

#ifdef MODULE_LZ4_ENABLED
#include "modules/lz4/lz4_compressor.h"
#endif

_FORCE_INLINE_ bool TerrainChunk::get_process() const {
	return _is_processing;
}
_FORCE_INLINE_ void TerrainChunk::set_process(const bool value) {
	_is_processing = value;
}

_FORCE_INLINE_ bool TerrainChunk::get_physics_process() const {
	return _is_phisics_processing;
}
_FORCE_INLINE_ void TerrainChunk::set_physics_process(const bool value) {
	_is_phisics_processing = value;
}

bool TerrainChunk::get_visible() const {
	return _is_visible;
}
void TerrainChunk::set_visible(const bool value) {
	_is_visible = value;

	visibility_changed(value);
}

bool TerrainChunk::get_is_generating() const {
	return _is_generating;
}
void TerrainChunk::set_is_generating(const bool value) {
	_is_generating = value;
}

bool TerrainChunk::get_is_immediate_build() const {
	return _is_immediate_build;
}
void TerrainChunk::set_is_immediate_build(const bool value) {
	_is_immediate_build = value;
}

bool TerrainChunk::get_is_terrain_generated() const {
	return _is_terrain_generated;
}
void TerrainChunk::set_is_terrain_generated(const bool value) {
	_is_terrain_generated = value;
}

bool TerrainChunk::is_build_aborted() const {
	return _abort_build;
}

bool TerrainChunk::is_in_tree() const {
	return _is_in_tree;
}

_FORCE_INLINE_ bool TerrainChunk::get_dirty() const {
	return _dirty;
}
_FORCE_INLINE_ void TerrainChunk::set_dirty(const bool value) {
	_dirty = value;
}

_FORCE_INLINE_ int TerrainChunk::get_state() const {
	return _state;
}
_FORCE_INLINE_ void TerrainChunk::set_state(const int value) {
	_state = value;
}

_FORCE_INLINE_ int TerrainChunk::get_position_x() const {
	return _position_x;
}
void TerrainChunk::set_position_x(const int value) {
	_position_x = value;
}
_FORCE_INLINE_ int TerrainChunk::get_position_z() const {
	return _position_z;
}
void TerrainChunk::set_position_z(const int value) {
	_position_z = value;
}

_FORCE_INLINE_ Vector2 TerrainChunk::get_position() const {
	return Vector2(_position_x, _position_z);
}
_FORCE_INLINE_ Vector2 TerrainChunk::get_world_position() const {
	return Vector2(_position_x * _size_x * _voxel_scale, _position_z * _size_z * _voxel_scale);
}

_FORCE_INLINE_ Vector3 TerrainChunk::get_world_size() const {
	return Vector3(_size_x * _voxel_scale, _world_height * _voxel_scale, _size_z * _voxel_scale);
}

_FORCE_INLINE_ AABB TerrainChunk::get_world_aabb() const {
	Vector2 v = get_world_position();

	return AABB(Vector3(v.x, 0, v.y), get_world_size());
}

_FORCE_INLINE_ int TerrainChunk::get_size_x() const {
	return _size_x;
}
_FORCE_INLINE_ int TerrainChunk::get_size_z() const {
	return _size_z;
}

_FORCE_INLINE_ void TerrainChunk::set_size_x(const int value) {
	_size_x = value;
}
_FORCE_INLINE_ void TerrainChunk::set_size_z(const int value) {
	_size_z = value;
}

_FORCE_INLINE_ Vector3 TerrainChunk::get_size() const {
	return Vector3(_size_x, _world_height, _size_z);
}

_FORCE_INLINE_ int TerrainChunk::get_data_size_x() const {
	return _data_size_x;
}
_FORCE_INLINE_ int TerrainChunk::get_data_size_z() const {
	return _data_size_z;
}

_FORCE_INLINE_ void TerrainChunk::set_data_size_x(const int value) {
	_data_size_x = value;
}
_FORCE_INLINE_ void TerrainChunk::set_data_size_z(const int value) {
	_data_size_z = value;
}

_FORCE_INLINE_ float TerrainChunk::get_world_height() const {
	return _world_height;
}
void TerrainChunk::set_world_height(const float value) {
	_world_height = value;
}

void TerrainChunk::set_position(const int x, const int z) {
	_position_x = x;
	_position_z = z;
}

_FORCE_INLINE_ int TerrainChunk::get_margin_start() const {
	return _margin_start;
}
_FORCE_INLINE_ int TerrainChunk::get_margin_end() const {
	return _margin_end;
}

_FORCE_INLINE_ void TerrainChunk::set_margin_start(const int value) {
	_margin_start = value;
}
_FORCE_INLINE_ void TerrainChunk::set_margin_end(const int value) {
	_margin_end = value;
}

int TerrainChunk::material_cache_key_get() const {
	return _material_cache_key;
}
void TerrainChunk::material_cache_key_set(const int value) {
	_material_cache_key = value;

	_material_cache_key_invalid = false;
}

bool TerrainChunk::material_cache_key_has() const {
	return _material_cache_key_has;
}
void TerrainChunk::material_cache_key_has_set(const bool value) {
	_material_cache_key_has = value;
}

bool TerrainChunk::material_cache_key_invalid_get() const {
	return _material_cache_key_invalid;
}
void TerrainChunk::material_cache_key_invalid_set(const bool value) {
	_material_cache_key_invalid = value;
}

int TerrainChunk::liquid_material_cache_key_get() const {
	return _liquid_material_cache_key;
}
void TerrainChunk::liquid_material_cache_key_set(const int value) {
	_liquid_material_cache_key = value;

	_liquid_material_cache_key_invalid = false;
}

bool TerrainChunk::liquid_material_cache_key_has() const {
	return _liquid_material_cache_key_has;
}
void TerrainChunk::liquid_material_cache_key_has_set(const bool value) {
	_liquid_material_cache_key_has = value;
}

bool TerrainChunk::liquid_material_cache_key_invalid_get() const {
	return _liquid_material_cache_key_invalid;
}
void TerrainChunk::liquid_material_cache_key_invalid_set(const bool value) {
	_liquid_material_cache_key_invalid = value;
}

int TerrainChunk::prop_material_cache_key_get() const {
	return _prop_material_cache_key;
}
void TerrainChunk::prop_material_cache_key_set(const int value) {
	_prop_material_cache_key = value;

	_prop_material_cache_key_invalid = false;
}

bool TerrainChunk::prop_material_cache_key_has() const {
	return _prop_material_cache_key_has;
}
void TerrainChunk::prop_material_cache_key_has_set(const bool value) {
	_prop_material_cache_key_has = value;
}

bool TerrainChunk::prop_material_cache_key_invalid_get() const {
	return _prop_material_cache_key_invalid;
}
void TerrainChunk::prop_material_cache_key_invalid_set(const bool value) {
	_prop_material_cache_key_invalid = value;
}

Ref<TerrainLibrary> TerrainChunk::get_library() {
	return _library;
}
void TerrainChunk::set_library(const Ref<TerrainLibrary> &value) {
	_library = value;
}

float TerrainChunk::get_voxel_scale() const {
	return _voxel_scale;
}
void TerrainChunk::set_voxel_scale(const float value) {
	_voxel_scale = value;
}

TerrainWorld *TerrainChunk::get_voxel_world() const {
	return _voxel_world;
}
void TerrainChunk::set_voxel_world(TerrainWorld *world) {
	_voxel_world = world;
}
void TerrainChunk::set_voxel_world_bind(Node *world) {
	if (world == NULL) {
		_voxel_world = NULL;
		return;
	}

	_voxel_world = Object::cast_to<TerrainWorld>(world);
}

Ref<TerrainJob> TerrainChunk::job_get(int index) const {
	ERR_FAIL_INDEX_V(index, _jobs.size(), Ref<TerrainJob>());

	return _jobs.get(index);
}
void TerrainChunk::job_set(int index, const Ref<TerrainJob> &job) {
	ERR_FAIL_INDEX(index, _jobs.size());

	_jobs.set(index, job);
}
void TerrainChunk::job_remove(const int index) {
	ERR_FAIL_INDEX(index, _jobs.size());

	_jobs.remove(index);
}
void TerrainChunk::job_add(const Ref<TerrainJob> &job) {
	_jobs.push_back(job);
}
int TerrainChunk::job_get_count() const {
	return _jobs.size();
}

int TerrainChunk::job_get_current_index() {
	return _current_job;
}
void TerrainChunk::job_next() {
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

	Ref<TerrainJob> j = _jobs[_current_job];

	if (!j.is_valid()) {
		//skip if invalid
		job_next();
	}

	j->reset();
	j->set_complete(false);

	if (j->get_build_phase_type() == TerrainJob::BUILD_PHASE_TYPE_NORMAL) {
		if (!_is_immediate_build) {
			ThreadPool::get_singleton()->add_job(j);
		} else {
			j->execute();
		}
	}
}
Ref<TerrainJob> TerrainChunk::job_get_current() {
	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size()) {
		return Ref<TerrainJob>();
	}

	return _jobs[_current_job];
}

//Terra Data
void TerrainChunk::channel_setup() {
	ERR_FAIL_COND_MSG(!has_method("_channel_setup"), "TerrainChunk: _setup_channels() is missing! Please implement it!");

	call("_channel_setup");
}

void TerrainChunk::set_size(const int size_x, const int size_z, const int margin_start, const int margin_end) {
	if (_size_x == size_x && _size_z == size_z && _margin_start == margin_start && _margin_end == margin_end) {
		return;
	}

	_size_x = size_x;
	_size_z = size_z;

	_data_size_x = size_x + margin_start + margin_end;
	_data_size_z = size_z + margin_start + margin_end;

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

bool TerrainChunk::validate_data_position(const int x, const int z) const {
	return x < _data_size_x && z < _data_size_z;
}

uint8_t TerrainChunk::get_voxel(const int p_x, const int p_z, const int p_channel_index) const {
	int x = p_x + _margin_start;
	int z = p_z + _margin_start;

	ERR_FAIL_INDEX_V(p_channel_index, _channels.size(), 0);
	ERR_FAIL_COND_V_MSG(!validate_data_position(x, z), 0, "Error, index out of range! " + String::num(x) + " " + String::num(z));

	uint8_t *ch = _channels.get(p_channel_index);

	if (!ch) {
		return 0;
	}

	return ch[get_data_index(x, z)];
}
void TerrainChunk::set_voxel(const uint8_t p_value, const int p_x, const int p_z, const int p_channel_index) {
	int x = p_x + _margin_start;
	int z = p_z + _margin_start;

	ERR_FAIL_INDEX(p_channel_index, _channels.size());
	ERR_FAIL_COND_MSG(!validate_data_position(x, z), "Error, index out of range! " + String::num(x) + " " + String::num(z));

	uint8_t *ch = channel_get_valid(p_channel_index);

	ch[get_data_index(x, z)] = p_value;
}

int TerrainChunk::channel_get_count() const {
	return _channels.size();
}

void TerrainChunk::channel_set_count(const int count) {
	if (count == _channels.size()) {
		return;
	}

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
bool TerrainChunk::channel_is_allocated(const int channel_index) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), false);

	return _channels[channel_index] != NULL;
}
void TerrainChunk::channel_ensure_allocated(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	if (_channels[channel_index] == NULL) {
		channel_allocate(channel_index, default_value);
	}
}
void TerrainChunk::channel_allocate(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	if (_channels[channel_index] != NULL) {
		return;
	}

	uint32_t size = _data_size_x * _data_size_z;

	uint8_t *ch = memnew_arr(uint8_t, size);
	memset(ch, default_value, size);

	_channels.set(channel_index, ch);
}
void TerrainChunk::channel_fill(const uint8_t value, const int channel_index) {
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
void TerrainChunk::channel_dealloc(const int channel_index) {
	ERR_FAIL_INDEX(channel_index, _channels.size());

	uint8_t *ch = _channels.get(channel_index);

	if (ch != NULL) {
		memdelete_arr(ch);

		_channels.set(channel_index, NULL);
	}
}

uint8_t *TerrainChunk::channel_get(const int channel_index) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), NULL);

	return _channels.get(channel_index);
}
uint8_t *TerrainChunk::channel_get_valid(const int channel_index, const uint8_t default_value) {
	ERR_FAIL_INDEX_V(channel_index, _channels.size(), 0);

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		channel_allocate(channel_index, default_value);

		return _channels.get(channel_index);
	}

	return ch;
}

PoolByteArray TerrainChunk::channel_get_array(const int channel_index) const {
	PoolByteArray arr;

	uint32_t size = _data_size_x * _data_size_z;

	if (channel_index >= _channels.size()) {
		return arr;
	}

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		return arr;
	}

	arr.resize(size);

	for (uint32_t i = 0; i < size; ++i) {
		arr.set(i, ch[i]);
	}

	return arr;
}
void TerrainChunk::channel_set_array(const int channel_index, const PoolByteArray &array) {
	if (array.size() == 0) {
		return;
	}

	if (_channels.size() <= channel_index) {
		channel_set_count(channel_index + 1);
	}

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		if (_channels[channel_index] != NULL) {
			return;
		}

		ch = memnew_arr(uint8_t, array.size());
		_channels.set(channel_index, ch);
	}

	for (int i = 0; i < array.size(); ++i) {
		ch[i] = array[i];
	}
}

PoolByteArray TerrainChunk::channel_get_compressed(const int channel_index) const {
	PoolByteArray arr;
#ifdef MODULE_LZ4_ENABLED
	int size = _data_size_x * _data_size_z;

	if (channel_index >= _channels.size()) {
		return arr;
	}

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		return arr;
	}

	int bound = LZ4Compressor::LZ4_compressBound(size);
	arr.resize(bound);

	PoolByteArray::Write w = arr.write();

	int ns = LZ4Compressor::LZ4_compress_default(reinterpret_cast<char *>(ch), reinterpret_cast<char *>(w.ptr()), size, bound);

	w.release();
	arr.resize(ns);
#endif
	return arr;
}
void TerrainChunk::channel_set_compressed(const int channel_index, const PoolByteArray &data) {
#ifdef MODULE_LZ4_ENABLED
	if (data.size() == 0) {
		return;
	}

	int size = _data_size_x * _data_size_z;

	if (_channels.size() <= channel_index) {
		channel_set_count(channel_index + 1);
	}

	uint8_t *ch = _channels.get(channel_index);

	if (ch == NULL) {
		if (_channels[channel_index] != NULL) {
			return;
		}

		ch = memnew_arr(uint8_t, size);
		_channels.set(channel_index, ch);
	}

	int ds = data.size();

	PoolByteArray::Read r = data.read();

	//We are not going to write to it
	uint8_t *data_arr = const_cast<uint8_t *>(r.ptr());

	LZ4Compressor::LZ4_decompress_safe(reinterpret_cast<char *>(data_arr), reinterpret_cast<char *>(ch), ds, size);
#endif
}

_FORCE_INLINE_ int TerrainChunk::get_index(const int x, const int z) const {
	return ((x + _margin_start) + _data_size_x * (z + _margin_start));
}

_FORCE_INLINE_ int TerrainChunk::get_data_index(const int x, const int z) const {
	return (x + _data_size_x * z);
}

_FORCE_INLINE_ int TerrainChunk::get_data_size() const {
	return _data_size_x * _data_size_z;
}

//Lights
void TerrainChunk::light_add(Ref<TerrainLight> p_light) {
	if (!p_light.is_valid()) {
		return;
	}

	p_light->set_has_owner_chunk(true);
	p_light->set_owner_chunk_position(Vector2i(_position_x, _position_z));
	p_light->connect("light_moved", this, "_on_light_moved");

	_lights.push_back(p_light);

	TerrainWorld *world = get_voxel_world();

	if (ObjectDB::instance_validate(world)) {
		world->world_light_added(p_light);
	}
}
bool TerrainChunk::light_remove(Ref<TerrainLight> p_light) {
	if (!p_light.is_valid()) {
		return false;
	}

	if (_lights.erase(p_light)) {
		p_light->set_has_owner_chunk(false);
		p_light->disconnect("light_moved", this, "_on_light_moved");

		TerrainWorld *world = get_voxel_world();

		if (ObjectDB::instance_validate(world)) {
			world->world_light_removed(p_light);
		}

		return true;
	}

	return false;
}

bool TerrainChunk::light_has(const Ref<TerrainLight> &p_light) {
	return _lights.find(p_light) != -1;
}

Ref<TerrainLight> TerrainChunk::light_get_index(const int index) {
	ERR_FAIL_INDEX_V(index, _lights.size(), Ref<TerrainLight>());

	return _lights.get(index);
}
void TerrainChunk::light_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _lights.size());

	Ref<TerrainLight> light = _lights[index];

	TerrainWorld *world = get_voxel_world();

	if (ObjectDB::instance_validate(world)) {
		world->world_light_removed(light);
	}
}
int TerrainChunk::light_get_count() const {
	return _lights.size();
}
void TerrainChunk::lights_clear() {
	TerrainWorld *world = get_voxel_world();

	if (!ObjectDB::instance_validate(world)) {
		world = NULL;
	}

	for (int i = 0; i < _lights.size(); ++i) {
		Ref<TerrainLight> light = _lights[i];

		if (!light.is_valid()) {
			continue;
		}

		light->set_has_owner_chunk(false);

		if (world) {
			world->world_light_removed(light);
		}
	}

	_lights.clear();
}

Vector<Variant> TerrainChunk::lights_get() {
	VARIANT_ARRAY_GET(_lights);
}
void TerrainChunk::lights_set(const Vector<Variant> &p_lights) {
	lights_clear();

	for (int i = 0; i < p_lights.size(); ++i) {
		Ref<TerrainLight> light = Ref<TerrainLight>(p_lights[i]);

		light_add(light);
	}
}

Vector<Variant> TerrainChunk::owned_lights_get() {
	Vector<Variant> r;
	for (int i = 0; i < _lights.size(); i++) {
		Ref<TerrainLight> l = _lights[i];

		if (!l.is_valid()) {
			continue;
		}

		if (l->get_owner_type() != TerrainLight::OWNER_TYPE_NONE) {
			continue;
		}

		r.push_back(_lights[i].get_ref_ptr());
	}
	return r;
}
void TerrainChunk::owned_lights_set(const Vector<Variant> &p_lights) {
	for (int i = 0; i < _lights.size(); ++i) {
		Ref<TerrainLight> l = _lights[i];

		if (!l.is_valid()) {
			continue;
		}

		if (l->get_owner_type() != TerrainLight::OWNER_TYPE_NONE) {
			continue;
		}

		light_remove_index(i);
		--i;
	}

	for (int i = 0; i < p_lights.size(); ++i) {
		Ref<TerrainLight> light = Ref<TerrainLight>(p_lights[i]);

		if (!light.is_valid()) {
			continue;
		}

		light_add(light);
	}
}

void TerrainChunk::_on_light_moved(const Ref<TerrainLight> &p_light) {
	TerrainWorld *world = get_voxel_world();

	if (ObjectDB::instance_validate(world)) {
		world->world_light_moved(p_light);
	}
}

//Terra Structures

Ref<TerrainStructure> TerrainChunk::voxel_structure_get(const int index) const {
	ERR_FAIL_INDEX_V(index, _voxel_structures.size(), Ref<TerrainStructure>());

	return _voxel_structures.get(index);
}
void TerrainChunk::voxel_structure_add(const Ref<TerrainStructure> &structure) {
	_voxel_structures.push_back(structure);
}
void TerrainChunk::voxel_structure_remove(const Ref<TerrainStructure> &structure) {
	if (!structure.is_valid()) {
		return;
	}

	int index = _voxel_structures.find(structure);

	if (index != -1) {
		_voxel_structures.remove(index);
	}
}
void TerrainChunk::voxel_structure_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _voxel_structures.size());

	_voxel_structures.remove(index);
}
void TerrainChunk::voxel_structure_clear() {
	_voxel_structures.clear();
}
int TerrainChunk::voxel_structure_get_count() const {
	return _voxel_structures.size();
}
void TerrainChunk::voxel_structure_add_at_position(Ref<TerrainStructure> structure, const Vector3 &world_position) {
	ERR_FAIL_COND(!structure.is_valid());

	structure->set_position_x(static_cast<int>(world_position.x / _voxel_scale));
	structure->set_position_y(static_cast<int>(world_position.y / _voxel_scale));
	structure->set_position_z(static_cast<int>(world_position.z / _voxel_scale));

	voxel_structure_add(structure);
}

Vector<Variant> TerrainChunk::voxel_structures_get() {
	VARIANT_ARRAY_GET(_voxel_structures);
}
void TerrainChunk::voxel_structures_set(const Vector<Variant> &structures) {
	voxel_structure_clear();

	for (int i = 0; i < structures.size(); ++i) {
		Ref<TerrainLight> structure = Ref<TerrainLight>(structures[i]);

		voxel_structure_add(structure);
	}
}

//Scenes

void TerrainChunk::scene_add(const Ref<PackedScene> &p_scene, const Transform &p_transform, const Node *p_node, const bool p_original) {
	ERR_FAIL_COND(!p_scene.is_valid());

	SceneDataStore s;
	s.original = p_original;
	s.transform = p_transform;
	s.scene = p_scene;
	s.node = 0;

	if (p_node) {
		s.node = p_node->get_instance_id();
	}

	_scenes.push_back(s);

	if (_is_in_tree && !p_node) {
		scene_instance(_scenes.size() - 1);
	}
}

Ref<PackedScene> TerrainChunk::scene_get(int index) {
	ERR_FAIL_INDEX_V(index, _scenes.size(), Ref<PackedScene>());

	return _scenes.get(index).scene;
}
void TerrainChunk::scene_set(const int index, const Ref<PackedScene> &p_scene) {
	ERR_FAIL_INDEX(index, _scenes.size());

	_scenes.write[index].scene = p_scene;
}

Transform TerrainChunk::scene_get_transform(const int index) {
	ERR_FAIL_INDEX_V(index, _scenes.size(), Transform());

	return _scenes.get(index).transform;
}
void TerrainChunk::scene_set_transform(const int index, const Transform &p_transform) {
	ERR_FAIL_INDEX(index, _scenes.size());

	_scenes.write[index].transform = p_transform;
}

bool TerrainChunk::scene_get_is_original(const int index) {
	ERR_FAIL_INDEX_V(index, _scenes.size(), false);

	return _scenes.get(index).original;
}
void TerrainChunk::scene_set_is_original(const int index, const bool p_original) {
	ERR_FAIL_INDEX(index, _scenes.size());

	_scenes.write[index].original = p_original;
}

Node *TerrainChunk::scene_get_node(const int index) {
	ERR_FAIL_INDEX_V(index, _scenes.size(), 0);

	return Object::cast_to<Node>(ObjectDB::get_instance(_scenes.get(index).node));
}
void TerrainChunk::scene_set_node(const int index, const Node *p_node) {
	ERR_FAIL_INDEX(index, _scenes.size());

	ObjectID id = 0;

	if (p_node) {
		id = p_node->get_instance_id();
	}

	_scenes.write[index].node = id;
}

int TerrainChunk::scene_get_count() const {
	return _scenes.size();
}
void TerrainChunk::scene_remove(const int index) {
	ERR_FAIL_INDEX(index, _scenes.size());

	_scenes.remove(index);
}
void TerrainChunk::scenes_clear() {
	_scenes.clear();
}

void TerrainChunk::scene_instance(const int index) {
	ERR_FAIL_INDEX(index, _scenes.size());

	if (!_is_in_tree) {
		return;
	}

	TerrainWorld *world = get_voxel_world();

	ERR_FAIL_COND(!world);

	SceneDataStore &e = _scenes.write[index];

	if (e.node != 0) {
		// Ignore if already instanced. It was added manually.
		return;
	}

	if (!e.scene.is_valid()) {
		return;
	}

	Node *n = e.scene->instance();
	world->add_child(n);
	//n->set_owner(world);

	Spatial *sp = Object::cast_to<Spatial>(n);

	if (sp) {
		sp->set_transform(e.transform);
	}

	e.node = n->get_instance_id();
}

void TerrainChunk::scene_queue_free(const int index) {
	ERR_FAIL_INDEX(index, _scenes.size());

	SceneDataStore &e = _scenes.write[index];

	if (e.node != 0) {
		Node *n = Object::cast_to<Node>(ObjectDB::get_instance(e.node));

		if (n) {
			n->queue_delete();
		}

		e.node = 0;
	}
}

Array TerrainChunk::scenes_get() {
	Array ret;

	for (int i = 0; i < _scenes.size(); i++) {
		Array prop_data;

		prop_data.push_back(_scenes[i].original);
		prop_data.push_back(_scenes[i].transform);
		prop_data.push_back(_scenes[i].scene.get_ref_ptr());

		ret.push_back(prop_data);
	}

	return ret;
}
void TerrainChunk::scenes_set(const Array &p_scenes) {
	scenes_clear();

	for (int i = 0; i < p_scenes.size(); ++i) {
		Array scene_data = p_scenes[i];

		ERR_CONTINUE(scene_data.size() != 3);

		bool original = scene_data[0];
		Transform transform = scene_data[1];
		Ref<PackedScene> scene = Ref<PackedScene>(scene_data[2]);

		scene_add(scene, transform, NULL, original);
	}
}

//Meshing

void TerrainChunk::build() {
	ERR_FAIL_COND(!ObjectDB::instance_validate(get_voxel_world()));
	ERR_FAIL_COND(!get_voxel_world()->is_inside_tree());
	ERR_FAIL_COND(!is_in_tree());

	call("_build");
}

void TerrainChunk::build_immediate() {
	ERR_FAIL_COND(!ObjectDB::instance_validate(get_voxel_world()));
	ERR_FAIL_COND(!get_voxel_world()->is_inside_tree());
	ERR_FAIL_COND(!is_in_tree());

	call("_build_immediate");
}

void TerrainChunk::_build() {
	if (get_is_generating()) {
		_queued_generation = true;
		return;
	}

	_is_generating = true;
	_is_immediate_build = false;

	job_next();
}

void TerrainChunk::_build_immediate() {
	if (get_is_generating()) {
		_queued_generation = true;
		return;
	}

	_is_generating = true;
	_is_immediate_build = true;

	job_next();
}

void TerrainChunk::clear() {
	ERR_FAIL_COND_MSG(!has_method("_clear"), "TerrainChunk: _clear() is missing! Please implement it!");

	call("_clear");
}

void TerrainChunk::finalize_build() {
	if (has_method("_finalize_build")) {
		call("_finalize_build");
	}
}

void TerrainChunk::cancel_build() {
	_queued_generation = false;

	_abort_build = true;

	if (_is_generating) {
		Ref<TerrainJob> job = job_get_current();

		if (job.is_valid()) {
			ThreadPool::get_singleton()->cancel_job(job);
		}
	}
}

void TerrainChunk::bake_lights() {
	if (has_method("_bake_lights")) {
		call("_bake_lights");
	}
}
void TerrainChunk::bake_light(Ref<TerrainLight> light) {
	if (!light.is_valid()) {
		return;
	}

	if (has_method("_bake_lights")) {
		call("_bake_light", light);
	}
}
void TerrainChunk::clear_baked_lights() {
	if (has_method("_clear_baked_lights")) {
		call("_clear_baked_lights");
	}
}

#ifdef MODULE_PROPS_ENABLED
void TerrainChunk::prop_add(const Transform &transform, const Ref<PropData> &prop, const bool p_original) {
	ERR_FAIL_COND(!prop.is_valid());

	PropDataStore s;
	s.original = p_original;
	s.transform = transform;
	s.prop = prop;

	_props.push_back(s);
}

Ref<PropData> TerrainChunk::prop_get(int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<PropData>());

	return _props.get(index).prop;
}
void TerrainChunk::prop_set(const int index, const Ref<PropData> &p_prop) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.write[index].prop = p_prop;
}

Transform TerrainChunk::prop_get_transform(const int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), Transform());

	return _props.get(index).transform;
}
void TerrainChunk::prop_set_transform(const int index, const Transform &p_transform) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.write[index].transform = p_transform;
}

bool TerrainChunk::prop_get_is_original(const int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), false);

	return _props.get(index).original;
}
void TerrainChunk::prop_set_is_original(const int index, const bool p_original) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.write[index].original = p_original;
}

int TerrainChunk::prop_get_count() const {
	return _props.size();
}
void TerrainChunk::prop_remove(const int index) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.remove(index);
}
void TerrainChunk::props_clear() {
	_props.clear();
}

Array TerrainChunk::props_get() {
	Array ret;

	for (int i = 0; i < _props.size(); i++) {
		Array prop_data;

		prop_data.push_back(_props[i].original);
		prop_data.push_back(_props[i].transform);
		prop_data.push_back(_props[i].prop.get_ref_ptr());

		ret.push_back(prop_data);
	}

	return ret;
}
void TerrainChunk::props_set(const Array &p_props) {
	props_clear();

	for (int i = 0; i < p_props.size(); ++i) {
		Array prop_data = p_props[i];

		ERR_CONTINUE(prop_data.size() != 3);

		bool original = prop_data[0];
		Transform transform = prop_data[1];
		Ref<PropData> prop = Ref<PropData>(prop_data[2]);

		prop_add(transform, prop, original);
	}
}
#endif

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
int TerrainChunk::mesh_data_resource_addv(const Vector3 &local_data_pos, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture, const Color &color, const bool apply_voxel_scale, const bool p_original) {
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
	e.is_original = p_original;

	AABB aabb = AABB(Vector3(), get_world_size());
	AABB mesh_aabb = e.transform.xform(mesh->get_aabb());
	e.is_inside = aabb.encloses(mesh_aabb);

#ifdef MODULE_PROPS_ENABLED
	if (get_library().is_valid() && texture.is_valid()) {
		e.uv_rect = get_library()->get_prop_uv_rect(texture);
	} else {
		e.uv_rect = Rect2(0, 0, 1, 1);
	}
#else
	e.uv_rect = Rect2(0, 0, 1, 1);
#endif

	_mesh_data_resources.push_back(e);

	if (has_method("_mesh_data_resource_added")) {
		call("_mesh_data_resource_added", index);
	}

	return index;
}

int TerrainChunk::mesh_data_resource_add(const Transform &local_transform, const Ref<MeshDataResource> &mesh, const Ref<Texture> &texture, const Color &color, const bool apply_voxel_scale, const bool p_original) {
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
	e.is_original = p_original;

	AABB aabb = AABB(Vector3(), get_world_size());
	AABB mesh_aabb = e.transform.xform(mesh->get_aabb());
	e.is_inside = aabb.encloses(mesh_aabb);

#ifdef MODULE_PROPS_ENABLED
	if (get_library().is_valid() && texture.is_valid()) {
		e.uv_rect = get_library()->get_prop_uv_rect(texture);
	} else {
		e.uv_rect = Rect2(0, 0, 1, 1);
	}
#else
	e.uv_rect = Rect2(0, 0, 1, 1);
#endif

	_mesh_data_resources.push_back(e);

	if (has_method("_mesh_data_resource_added")) {
		call("_mesh_data_resource_added", index);
	}

	return index;
}

Ref<MeshDataResource> TerrainChunk::mesh_data_resource_get(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Ref<MeshDataResource>());

	return _mesh_data_resources[index].mesh;
}

void TerrainChunk::mesh_data_resource_set(const int index, const Ref<MeshDataResource> &mesh) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());
}

Ref<Texture> TerrainChunk::mesh_data_resource_get_texture(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Ref<Texture>());

	return _mesh_data_resources[index].texture;
}
void TerrainChunk::mesh_data_resource_set_texture(const int index, const Ref<Texture> &texture) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].texture = texture;
}

Color TerrainChunk::mesh_data_resource_get_color(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Color());

	return _mesh_data_resources[index].color;
}
void TerrainChunk::mesh_data_resource_set_color(const int index, const Color &color) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].color = color;
}

Rect2 TerrainChunk::mesh_data_resource_get_uv_rect(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Rect2());

	return _mesh_data_resources[index].uv_rect;
}
void TerrainChunk::mesh_data_resource_set_uv_rect(const int index, const Rect2 &uv_rect) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].uv_rect = uv_rect;
}

Transform TerrainChunk::mesh_data_resource_get_transform(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), Transform());

	return _mesh_data_resources.write[index].transform;
}
void TerrainChunk::mesh_data_resource_set_transform(const int index, const Transform &transform) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].transform = transform;
}

bool TerrainChunk::mesh_data_resource_get_is_inside(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), true);

	return _mesh_data_resources[index].is_inside;
}
void TerrainChunk::mesh_data_resource_set_is_inside(const int index, const bool inside) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].is_inside = inside;
}

bool TerrainChunk::mesh_data_resource_get_is_original(const int index) {
	ERR_FAIL_INDEX_V(index, _mesh_data_resources.size(), false);

	return _mesh_data_resources[index].is_original;
}
void TerrainChunk::mesh_data_resource_set_is_original(const int index, const bool p_original) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.write[index].is_original = p_original;
}

int TerrainChunk::mesh_data_resource_get_count() const {
	return _mesh_data_resources.size();
}
void TerrainChunk::mesh_data_resource_remove(const int index) {
	ERR_FAIL_INDEX(index, _mesh_data_resources.size());

	_mesh_data_resources.remove(index);
}
void TerrainChunk::mesh_data_resource_clear() {
	_mesh_data_resources.clear();
}

Array TerrainChunk::mesh_data_resources_get() {
	Array ret;

	for (int i = 0; i < _mesh_data_resources.size(); i++) {
		MeshDataResourceEntry e = _mesh_data_resources[i];

		Array mdr_data;

		mdr_data.push_back(e.mesh.get_ref_ptr());
		mdr_data.push_back(e.texture);
		mdr_data.push_back(e.color);
		mdr_data.push_back(e.transform);
		mdr_data.push_back(e.is_original);

		ret.push_back(mdr_data);
	}

	return ret;
}

void TerrainChunk::mesh_data_resources_set(const Array &p_mesh_data_resources) {
	mesh_data_resource_clear();

	for (int i = 0; i < p_mesh_data_resources.size(); ++i) {
		Array mdr_data = p_mesh_data_resources[i];

		ERR_CONTINUE(mdr_data.size() != 5);

		Ref<MeshDataResource> mesh = Ref<MeshDataResource>(mdr_data[0]);
		Ref<Texture> texture = Ref<Texture>(mdr_data[1]);
		Color color = mdr_data[2];
		Transform transform = mdr_data[3];
		bool is_original = mdr_data[4];

		mesh_data_resource_add(transform, mesh, texture, color, false, is_original);
	}
}

#endif

int TerrainChunk::collider_add(const Transform &local_transform, const Ref<Shape> &shape, const RID &shape_rid, const RID &body) {
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

Transform TerrainChunk::collider_get_transform(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Transform());

	return _colliders[index].transform;
}
void TerrainChunk::collider_set_transform(const int index, const Transform &transform) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].transform = transform;
}

Ref<Shape> TerrainChunk::collider_get_shape(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), Ref<Shape>());

	return _colliders[index].shape;
}

void TerrainChunk::collider_set_shape(const int index, const Ref<Shape> &shape) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].shape = shape;
}

RID TerrainChunk::collider_get_shape_rid(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].shape_rid;
}
void TerrainChunk::collider_set_shape_rid(const int index, const RID &rid) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].shape_rid = rid;
}

RID TerrainChunk::collider_get_body(const int index) {
	ERR_FAIL_INDEX_V(index, _colliders.size(), RID());

	return _colliders[index].body;
}
void TerrainChunk::collider_set_body(const int index, const RID &rid) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.write[index].body = rid;
}

int TerrainChunk::collider_get_count() const {
	return _colliders.size();
}
void TerrainChunk::collider_remove(const int index) {
	ERR_FAIL_INDEX(index, _colliders.size());

	_colliders.remove(index);
}
void TerrainChunk::colliders_clear() {
	_colliders.clear();
}

void TerrainChunk::enter_tree() {
	_is_in_tree = true;

	if (has_method("_enter_tree")) {
		call("_enter_tree");
	}
}
void TerrainChunk::exit_tree() {
	_is_in_tree = false;

	if (has_method("_exit_tree")) {
		call("_exit_tree");
	}
}
void TerrainChunk::process(const float delta) {
	if (has_method("_process")) {
		call("_process", delta);
	}
}
void TerrainChunk::physics_process(const float delta) {
	if (has_method("_physics_process")) {
		call("_physics_process", delta);
	}
}
void TerrainChunk::world_transform_changed() {
	call("_world_transform_changed");
}
void TerrainChunk::visibility_changed(const bool visible) {
	if (has_method("_visibility_changed")) {
		call("_visibility_changed", _is_visible);
	}
}
void TerrainChunk::world_light_added(const Ref<TerrainLight> &light) {
	if (has_method("_world_light_added")) {
		call("_world_light_added", light);
	}
}
void TerrainChunk::world_light_removed(const Ref<TerrainLight> &light) {
	if (has_method("_world_light_removed")) {
		call("_world_light_removed", light);
	}
}
void TerrainChunk::generation_process(const float delta) {
	call("_generation_process", delta);
}
void TerrainChunk::generation_physics_process(const float delta) {
	call("_generation_physics_process", delta);
}

Transform TerrainChunk::get_transform() const {
	return _transform;
}
void TerrainChunk::set_transform(const Transform &transform) {
	_transform = transform;
}

Transform TerrainChunk::get_global_transform() const {
	ERR_FAIL_COND_V(!get_voxel_world(), Transform());

	return get_voxel_world()->get_global_transform() * _transform;
}

Vector3 TerrainChunk::to_local(Vector3 p_global) const {
	return get_global_transform().affine_inverse().xform(p_global);
}

Vector3 TerrainChunk::to_global(Vector3 p_local) const {
	return get_global_transform().xform(p_local);
}

bool TerrainChunk::is_safe_to_delete() {
	if (!_is_generating) {
		return true;
	}

	Ref<TerrainJob> job = job_get_current();

	if (!job.is_valid()) {
		return true;
	}

	return !ThreadPool::get_singleton()->has_job(job);
}

TerrainChunk::TerrainChunk() {
	_is_processing = false;
	_is_phisics_processing = false;
	_is_in_tree = false;

	_is_visible = true;

	_is_generating = false;
	_is_immediate_build = false;
	_is_terrain_generated = false;
	_dirty = false;
	_state = TERRAIN_CHUNK_STATE_OK;

	_voxel_scale = 1;

	_voxel_world = NULL;

	_position_x = 0;
	_position_z = 0;

	_size_x = 0;
	_size_z = 0;

	_data_size_x = 0;
	_data_size_z = 0;

	_margin_start = 0;
	_margin_end = 0;

	_material_cache_key = 0;
	_material_cache_key_has = false;
	_material_cache_key_invalid = false;

	_liquid_material_cache_key = 0;
	_liquid_material_cache_key_has = false;
	_liquid_material_cache_key_invalid = false;

	_prop_material_cache_key = 0;
	_prop_material_cache_key_has = false;
	_prop_material_cache_key_invalid = false;

	_current_job = -1;

	_world_height = 256;

	_queued_generation = false;
}

TerrainChunk::~TerrainChunk() {
	if (_library.is_valid()) {
		_library.unref();
	}

#ifdef MODULE_PROPS_ENABLED
	props_clear();
#endif

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	mesh_data_resource_clear();
#endif

	for (int i = 0; i < _channels.size(); ++i) {
		uint8_t *ch = _channels[i];

		if (ch != NULL) {
			memdelete_arr(ch);
		}
	}

	for (int i = 0; i < _colliders.size(); ++i) {
		PhysicsServer::get_singleton()->free(_colliders[i].body);
	}

	_colliders.clear();

	_jobs.clear();

	_lights.clear();
}

void TerrainChunk::_enter_tree() {
	for (int i = 0; i < _jobs.size(); ++i) {
		Ref<TerrainJob> j = _jobs[i];

		if (j.is_valid()) {
			j->set_chunk(Ref<TerrainChunk>(this));
		}
	}

	for (int i = 0; i < _scenes.size(); ++i) {
		scene_instance(i);
	}
}

void TerrainChunk::_exit_tree() {
	for (int i = 0; i < _scenes.size(); ++i) {
		scene_queue_free(i);
	}

	if (_is_generating) {
		cancel_build();
	}

	for (int i = 0; i < _jobs.size(); ++i) {
		Ref<TerrainJob> j = _jobs[i];

		if (j.is_valid()) {
			j->chunk_exit_tree();
		}
	}

	if (_library.is_valid() && _library->supports_caching()) {
		if (material_cache_key_has()) {
			_library->material_cache_unref(material_cache_key_get());

			material_cache_key_set(0);
			material_cache_key_has_set(false);
		}
	}
}

void TerrainChunk::_generation_process(const float delta) {
	if (_abort_build) {
		return;
	}

	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size()) {
		return;
	}

	Ref<TerrainJob> job = _jobs[_current_job];

	ERR_FAIL_COND(!job.is_valid());

	if (job->get_build_phase_type() == TerrainJob::BUILD_PHASE_TYPE_PROCESS) {
		if (!_voxel_world->can_chunk_do_build_step()) {
			return;
		}

		job->process(delta);

		if (job->get_build_phase_type() == TerrainJob::BUILD_PHASE_TYPE_NORMAL) {
			ThreadPool::get_singleton()->add_job(job);
		}
	}
}
void TerrainChunk::_generation_physics_process(const float delta) {
	if (_abort_build) {
		return;
	}

	_THREAD_SAFE_METHOD_

	if (_current_job < 0 || _current_job >= _jobs.size()) {
		return;
	}

	Ref<TerrainJob> job = _jobs[_current_job];

	ERR_FAIL_COND(!job.is_valid());

	if (job->get_build_phase_type() == TerrainJob::BUILD_PHASE_TYPE_PHYSICS_PROCESS) {
		if (!_voxel_world->can_chunk_do_build_step()) {
			return;
		}

		job->physics_process(delta);

		if (job->get_build_phase_type() == TerrainJob::BUILD_PHASE_TYPE_NORMAL) {
			ThreadPool::get_singleton()->add_job(job);
		}
	}
}

void TerrainChunk::_world_transform_changed() {
	Transform wt;

	if (_voxel_world != NULL) {
		wt = _voxel_world->get_transform();
	}

	set_transform(wt * Transform(Basis(), Vector3(_position_x * static_cast<int>(_size_x) * _voxel_scale, 0, _position_z * static_cast<int>(_size_z) * _voxel_scale)));
}

/*
bool TerrainChunk::_set(const StringName &p_name, const Variant &p_value) {
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

bool TerrainChunk::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name.begins_with("channels/")) {

		int index = name.get_slicec('/', 1).to_int();

		r_ret = get_channel_array(index);

		return true;
	}

	return false;
}

void TerrainChunk::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < _channels.size(); ++i) {
		p_list->push_back(PropertyInfo(Variant::POOL_BYTE_ARRAY, "channels/" + String::num(i), PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL));
	}
}
*/

void TerrainChunk::_bind_methods() {
	ADD_SIGNAL(MethodInfo("mesh_generation_finished", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));

	BIND_VMETHOD(MethodInfo("_mesh_data_resource_added", PropertyInfo(Variant::INT, "index")));

	BIND_VMETHOD(MethodInfo("_channel_setup"));

	BIND_VMETHOD(MethodInfo("_bake_lights"));
	BIND_VMETHOD(MethodInfo("_bake_light", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "TerrainLight")));
	BIND_VMETHOD(MethodInfo("_clear_baked_lights"));

	ClassDB::bind_method(D_METHOD("bake_lights"), &TerrainChunk::bake_lights);
	ClassDB::bind_method(D_METHOD("bake_light", "light"), &TerrainChunk::bake_light);
	ClassDB::bind_method(D_METHOD("clear_baked_lights"), &TerrainChunk::clear_baked_lights);

	BIND_VMETHOD(MethodInfo("_enter_tree"));
	BIND_VMETHOD(MethodInfo("_exit_tree"));
	BIND_VMETHOD(MethodInfo("_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_physics_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_world_transform_changed"));
	BIND_VMETHOD(MethodInfo("_visibility_changed", PropertyInfo(Variant::BOOL, "visible")));
	BIND_VMETHOD(MethodInfo("_world_light_added", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "TerrainLight")));
	BIND_VMETHOD(MethodInfo("_world_light_removed", PropertyInfo(Variant::OBJECT, "light", PROPERTY_HINT_RESOURCE_TYPE, "TerrainLight")));

	BIND_VMETHOD(MethodInfo("_generation_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_generation_physics_process", PropertyInfo(Variant::REAL, "delta")));

	BIND_VMETHOD(MethodInfo("_finalize_build"));

	ClassDB::bind_method(D_METHOD("enter_tree"), &TerrainChunk::enter_tree);
	ClassDB::bind_method(D_METHOD("exit_tree"), &TerrainChunk::exit_tree);
	ClassDB::bind_method(D_METHOD("process", "delta"), &TerrainChunk::process);
	ClassDB::bind_method(D_METHOD("physics_process", "delta"), &TerrainChunk::physics_process);
	ClassDB::bind_method(D_METHOD("world_transform_changed"), &TerrainChunk::world_transform_changed);
	ClassDB::bind_method(D_METHOD("visibility_changed", "visible"), &TerrainChunk::visibility_changed);
	ClassDB::bind_method(D_METHOD("world_light_added", "light"), &TerrainChunk::world_light_added);
	ClassDB::bind_method(D_METHOD("world_light_removed", "light"), &TerrainChunk::world_light_removed);

	ClassDB::bind_method(D_METHOD("generation_process", "delta"), &TerrainChunk::generation_process);
	ClassDB::bind_method(D_METHOD("generation_physics_process", "delta"), &TerrainChunk::generation_physics_process);

	ClassDB::bind_method(D_METHOD("finalize_build"), &TerrainChunk::finalize_build);

	ClassDB::bind_method(D_METHOD("cancel_build"), &TerrainChunk::cancel_build);

	ClassDB::bind_method(D_METHOD("get_process"), &TerrainChunk::get_process);
	ClassDB::bind_method(D_METHOD("set_process", "value"), &TerrainChunk::set_process);

	ClassDB::bind_method(D_METHOD("get_physics_process"), &TerrainChunk::get_physics_process);
	ClassDB::bind_method(D_METHOD("set_physics_process", "value"), &TerrainChunk::set_physics_process);

	ClassDB::bind_method(D_METHOD("is_in_tree"), &TerrainChunk::is_in_tree);

	ClassDB::bind_method(D_METHOD("get_transform"), &TerrainChunk::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &TerrainChunk::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "transform"), "set_transform", "get_transform");

	ClassDB::bind_method(D_METHOD("get_visible"), &TerrainChunk::get_visible);
	ClassDB::bind_method(D_METHOD("set_visible", "value"), &TerrainChunk::set_visible);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "visible"), "set_visible", "get_visible");

	ClassDB::bind_method(D_METHOD("get_is_generating"), &TerrainChunk::get_is_generating);
	ClassDB::bind_method(D_METHOD("set_is_generating", "value"), &TerrainChunk::set_is_generating);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_generating", PROPERTY_HINT_NONE, "", 0), "set_is_generating", "get_is_generating");

	ClassDB::bind_method(D_METHOD("get_is_immediate_build"), &TerrainChunk::get_is_immediate_build);
	ClassDB::bind_method(D_METHOD("set_is_immediate_build", "value"), &TerrainChunk::set_is_immediate_build);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_immediate_build", PROPERTY_HINT_NONE, "", 0), "set_is_immediate_build", "get_is_immediate_build");

	ClassDB::bind_method(D_METHOD("get_is_terrain_generated"), &TerrainChunk::get_is_terrain_generated);
	ClassDB::bind_method(D_METHOD("set_is_terrain_generated", "value"), &TerrainChunk::set_is_terrain_generated);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_terrain_generated"), "set_is_terrain_generated", "get_is_terrain_generated");

	ClassDB::bind_method(D_METHOD("is_build_aborted"), &TerrainChunk::is_build_aborted);

	ClassDB::bind_method(D_METHOD("get_dirty"), &TerrainChunk::get_dirty);
	ClassDB::bind_method(D_METHOD("set_dirty", "value"), &TerrainChunk::set_dirty);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "dirty", PROPERTY_HINT_NONE, "", 0), "set_dirty", "get_dirty");

	ClassDB::bind_method(D_METHOD("get_state"), &TerrainChunk::get_state);
	ClassDB::bind_method(D_METHOD("set_state", "value"), &TerrainChunk::set_state);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "state", PROPERTY_HINT_NONE, "", 0), "set_state", "get_state");

	ClassDB::bind_method(D_METHOD("get_position_x"), &TerrainChunk::get_position_x);
	ClassDB::bind_method(D_METHOD("set_position_x", "value"), &TerrainChunk::set_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_x"), "set_position_x", "get_position_x");

	ClassDB::bind_method(D_METHOD("get_position_z"), &TerrainChunk::get_position_z);
	ClassDB::bind_method(D_METHOD("set_position_z", "value"), &TerrainChunk::set_position_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "position_z"), "set_position_z", "get_position_z");

	ClassDB::bind_method(D_METHOD("get_size_x"), &TerrainChunk::get_size_x);
	ClassDB::bind_method(D_METHOD("set_size_x"), &TerrainChunk::set_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size_x"), "set_size_x", "get_size_x");

	ClassDB::bind_method(D_METHOD("get_size_z"), &TerrainChunk::get_size_z);
	ClassDB::bind_method(D_METHOD("set_size_z"), &TerrainChunk::set_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size_z"), "set_size_z", "get_size_z");

	ClassDB::bind_method(D_METHOD("get_data_size_x"), &TerrainChunk::get_data_size_x);
	ClassDB::bind_method(D_METHOD("set_data_size_x"), &TerrainChunk::set_data_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_size_x"), "set_data_size_x", "get_data_size_x");

	ClassDB::bind_method(D_METHOD("get_data_size_z"), &TerrainChunk::get_data_size_z);
	ClassDB::bind_method(D_METHOD("set_data_size_z"), &TerrainChunk::set_data_size_z);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "data_size_z"), "set_data_size_z", "get_data_size_z");

	ClassDB::bind_method(D_METHOD("get_world_height"), &TerrainChunk::get_world_height);
	ClassDB::bind_method(D_METHOD("set_world_height", "height"), &TerrainChunk::set_world_height);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "world_height"), "set_world_height", "get_world_height");

	ClassDB::bind_method(D_METHOD("get_position"), &TerrainChunk::get_position);
	ClassDB::bind_method(D_METHOD("set_position", "x", "z"), &TerrainChunk::set_position);

	ClassDB::bind_method(D_METHOD("get_world_position"), &TerrainChunk::get_world_position);
	ClassDB::bind_method(D_METHOD("get_world_size"), &TerrainChunk::get_world_size);
	ClassDB::bind_method(D_METHOD("get_world_aabb"), &TerrainChunk::get_world_aabb);

	ClassDB::bind_method(D_METHOD("get_margin_start"), &TerrainChunk::get_margin_start);
	ClassDB::bind_method(D_METHOD("set_margin_start"), &TerrainChunk::set_margin_start);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin_start"), "set_margin_start", "get_margin_start");

	ClassDB::bind_method(D_METHOD("get_margin_end"), &TerrainChunk::get_margin_end);
	ClassDB::bind_method(D_METHOD("set_margin_end"), &TerrainChunk::set_margin_end);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin_end"), "set_margin_end", "get_margin_end");

	ClassDB::bind_method(D_METHOD("material_cache_key_get"), &TerrainChunk::material_cache_key_get);
	ClassDB::bind_method(D_METHOD("material_cache_key_set"), &TerrainChunk::material_cache_key_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "material_cache_key", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "material_cache_key_set", "material_cache_key_get");

	ClassDB::bind_method(D_METHOD("material_cache_key_has_get"), &TerrainChunk::material_cache_key_has);
	ClassDB::bind_method(D_METHOD("material_cache_key_has_set"), &TerrainChunk::material_cache_key_has_set);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "material_cache_key_has", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "material_cache_key_has_set", "material_cache_key_has_get");

	ClassDB::bind_method(D_METHOD("material_cache_key_invalid_get"), &TerrainChunk::material_cache_key_invalid_get);
	ClassDB::bind_method(D_METHOD("material_cache_key_invalid_set"), &TerrainChunk::material_cache_key_invalid_set);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "material_cache_key_invalid", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "material_cache_key_invalid_set", "material_cache_key_invalid_get");

	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_get"), &TerrainChunk::liquid_material_cache_key_get);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_set"), &TerrainChunk::liquid_material_cache_key_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "liquid_material_cache_key", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "liquid_material_cache_key_set", "liquid_material_cache_key_get");

	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_has_get"), &TerrainChunk::liquid_material_cache_key_has);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_has_set"), &TerrainChunk::liquid_material_cache_key_has_set);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "liquid_material_cache_key_has", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "liquid_material_cache_key_has_set", "liquid_material_cache_key_has_get");

	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_invalid_get"), &TerrainChunk::liquid_material_cache_key_invalid_get);
	ClassDB::bind_method(D_METHOD("liquid_material_cache_key_invalid_set"), &TerrainChunk::liquid_material_cache_key_invalid_set);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "liquid_material_cache_key_invalid", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "liquid_material_cache_key_invalid_set", "liquid_material_cache_key_invalid_get");

	ClassDB::bind_method(D_METHOD("prop_material_cache_key_get"), &TerrainChunk::prop_material_cache_key_get);
	ClassDB::bind_method(D_METHOD("prop_material_cache_key_set"), &TerrainChunk::prop_material_cache_key_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "prop_material_cache_key", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "prop_material_cache_key_set", "prop_material_cache_key_get");

	ClassDB::bind_method(D_METHOD("prop_material_cache_key_has_get"), &TerrainChunk::prop_material_cache_key_has);
	ClassDB::bind_method(D_METHOD("prop_material_cache_key_has_set"), &TerrainChunk::prop_material_cache_key_has_set);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "prop_material_cache_key_has", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "prop_material_cache_key_has_set", "prop_material_cache_key_has_get");

	ClassDB::bind_method(D_METHOD("prop_material_cache_key_invalid_get"), &TerrainChunk::prop_material_cache_key_invalid_get);
	ClassDB::bind_method(D_METHOD("prop_material_cache_key_invalid_set"), &TerrainChunk::prop_material_cache_key_invalid_set);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "prop_material_cache_key_invalid", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "prop_material_cache_key_invalid_set", "prop_material_cache_key_invalid_get");

	ClassDB::bind_method(D_METHOD("get_library"), &TerrainChunk::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "value"), &TerrainChunk::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "TerrainLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &TerrainChunk::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &TerrainChunk::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("job_get", "index"), &TerrainChunk::job_get);
	ClassDB::bind_method(D_METHOD("job_set", "index", "job"), &TerrainChunk::job_set);
	ClassDB::bind_method(D_METHOD("job_remove", "index"), &TerrainChunk::job_remove);
	ClassDB::bind_method(D_METHOD("job_add", "job"), &TerrainChunk::job_add);
	ClassDB::bind_method(D_METHOD("job_get_count"), &TerrainChunk::job_get_count);

	ClassDB::bind_method(D_METHOD("job_get_current_index"), &TerrainChunk::job_get_current_index);
	ClassDB::bind_method(D_METHOD("job_next"), &TerrainChunk::job_next);
	ClassDB::bind_method(D_METHOD("job_get_current"), &TerrainChunk::job_get_current);

	ClassDB::bind_method(D_METHOD("get_voxel_world"), &TerrainChunk::get_voxel_world);
	ClassDB::bind_method(D_METHOD("set_voxel_world", "world"), &TerrainChunk::set_voxel_world_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "voxel_world", PROPERTY_HINT_RESOURCE_TYPE, "TerrainWorld", 0), "set_voxel_world", "get_voxel_world");

	//Terra Data
	ClassDB::bind_method(D_METHOD("channel_setup"), &TerrainChunk::channel_setup);

	ClassDB::bind_method(D_METHOD("set_size", "size_x", "size_z", "margin_start", "margin_end"), &TerrainChunk::set_size, DEFVAL(0), DEFVAL(0));

	ClassDB::bind_method(D_METHOD("validate_data_position", "x", "z"), &TerrainChunk::validate_data_position);

	ClassDB::bind_method(D_METHOD("get_voxel", "x", "z", "index"), &TerrainChunk::get_voxel);
	ClassDB::bind_method(D_METHOD("set_voxel", "value", "x", "z", "index"), &TerrainChunk::set_voxel);

	ClassDB::bind_method(D_METHOD("channel_get_count"), &TerrainChunk::channel_get_count);
	ClassDB::bind_method(D_METHOD("channel_set_count", "count"), &TerrainChunk::channel_set_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_count"), "channel_set_count", "channel_get_count");

	ClassDB::bind_method(D_METHOD("channel_is_allocated", "index"), &TerrainChunk::channel_is_allocated);
	ClassDB::bind_method(D_METHOD("channel_ensure_allocated", "index", "default_value"), &TerrainChunk::channel_ensure_allocated);
	ClassDB::bind_method(D_METHOD("channel_allocate", "index", "default_value"), &TerrainChunk::channel_allocate);
	ClassDB::bind_method(D_METHOD("channel_fill", "value", "index"), &TerrainChunk::channel_fill);
	ClassDB::bind_method(D_METHOD("channel_dealloc", "index"), &TerrainChunk::channel_dealloc);

	ClassDB::bind_method(D_METHOD("channel_get_array", "index"), &TerrainChunk::channel_get_array);
	ClassDB::bind_method(D_METHOD("channel_set_array", "index", "array"), &TerrainChunk::channel_set_array);

	ClassDB::bind_method(D_METHOD("channel_get_compressed", "index"), &TerrainChunk::channel_get_compressed);
	ClassDB::bind_method(D_METHOD("channel_set_compressed", "index", "array"), &TerrainChunk::channel_set_compressed);

	ClassDB::bind_method(D_METHOD("get_index", "x", "z"), &TerrainChunk::get_index);
	ClassDB::bind_method(D_METHOD("get_data_index", "x", "z"), &TerrainChunk::get_data_index);
	ClassDB::bind_method(D_METHOD("get_data_size"), &TerrainChunk::get_data_size);

	// Lights
	ClassDB::bind_method(D_METHOD("light_add", "light"), &TerrainChunk::light_add);
	ClassDB::bind_method(D_METHOD("light_remove", "light"), &TerrainChunk::light_remove);
	ClassDB::bind_method(D_METHOD("light_has", "light"), &TerrainChunk::light_has);

	ClassDB::bind_method(D_METHOD("light_get_index", "index"), &TerrainChunk::light_get_index);
	ClassDB::bind_method(D_METHOD("light_remove_index", "index"), &TerrainChunk::light_remove_index);
	ClassDB::bind_method(D_METHOD("light_get_count"), &TerrainChunk::light_get_count);
	ClassDB::bind_method(D_METHOD("lights_clear"), &TerrainChunk::lights_clear);

	ClassDB::bind_method(D_METHOD("lights_get"), &TerrainChunk::lights_get);
	ClassDB::bind_method(D_METHOD("lights_set", "chunks"), &TerrainChunk::lights_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "lights", PROPERTY_HINT_NONE, "23/20:TerrainLight", 0, "TerrainLight"), "lights_set", "lights_get");

	ClassDB::bind_method(D_METHOD("owned_lights_get"), &TerrainChunk::owned_lights_get);
	ClassDB::bind_method(D_METHOD("owned_lights_set", "chunks"), &TerrainChunk::owned_lights_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "owned_lights", PROPERTY_HINT_NONE, "23/20:TerrainLight", PROPERTY_USAGE_DEFAULT, "TerrainLight"), "owned_lights_set", "owned_lights_get");

	ClassDB::bind_method(D_METHOD("_on_light_moved"), &TerrainChunk::_on_light_moved);

	// Structures
	ClassDB::bind_method(D_METHOD("voxel_structure_get", "index"), &TerrainChunk::voxel_structure_get);
	ClassDB::bind_method(D_METHOD("voxel_structure_add", "structure"), &TerrainChunk::voxel_structure_add);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove", "structure"), &TerrainChunk::voxel_structure_remove);
	ClassDB::bind_method(D_METHOD("voxel_structure_remove_index", "index"), &TerrainChunk::voxel_structure_remove_index);
	ClassDB::bind_method(D_METHOD("voxel_structure_clear"), &TerrainChunk::voxel_structure_clear);
	ClassDB::bind_method(D_METHOD("voxel_structure_get_count"), &TerrainChunk::voxel_structure_get_count);
	ClassDB::bind_method(D_METHOD("voxel_structure_add_at_position", "structure", "world_position"), &TerrainChunk::voxel_structure_add_at_position);

	ClassDB::bind_method(D_METHOD("voxel_structures_get"), &TerrainChunk::voxel_structures_get);
	ClassDB::bind_method(D_METHOD("voxel_structures_set"), &TerrainChunk::voxel_structures_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "voxel_structures", PROPERTY_HINT_NONE, "23/20:TerrainStructure", PROPERTY_USAGE_DEFAULT, "TerrainStructure"), "voxel_structures_set", "voxel_structures_get");

	//Scenes

	ClassDB::bind_method(D_METHOD("scene_add", "scene", "transform", "node", "original"), &TerrainChunk::scene_add, DEFVAL(Transform()), DEFVAL(0), DEFVAL(true));

	ClassDB::bind_method(D_METHOD("scene_get", "index"), &TerrainChunk::scene_get);
	ClassDB::bind_method(D_METHOD("scene_set", "index", "scene"), &TerrainChunk::scene_set);

	ClassDB::bind_method(D_METHOD("scene_get_transform", "index"), &TerrainChunk::scene_get_transform);
	ClassDB::bind_method(D_METHOD("scene_set_transform", "index", "transform"), &TerrainChunk::scene_set_transform);

	ClassDB::bind_method(D_METHOD("scene_get_is_original", "index"), &TerrainChunk::scene_get_is_original);
	ClassDB::bind_method(D_METHOD("scene_set_is_original", "index", "original"), &TerrainChunk::scene_set_is_original);

	ClassDB::bind_method(D_METHOD("scene_get_node", "index"), &TerrainChunk::scene_get_node);
	ClassDB::bind_method(D_METHOD("scene_set_node", "index", "node"), &TerrainChunk::scene_set_node);

	ClassDB::bind_method(D_METHOD("scene_get_count"), &TerrainChunk::scene_get_count);
	ClassDB::bind_method(D_METHOD("scene_remove", "index"), &TerrainChunk::scene_remove);
	ClassDB::bind_method(D_METHOD("scenes_clear"), &TerrainChunk::scenes_clear);

	ClassDB::bind_method(D_METHOD("scene_instance", "index"), &TerrainChunk::scene_instance);
	ClassDB::bind_method(D_METHOD("scene_queue_free", "index"), &TerrainChunk::scene_queue_free);

	ClassDB::bind_method(D_METHOD("scenes_get"), &TerrainChunk::scenes_get);
	ClassDB::bind_method(D_METHOD("scenes_set"), &TerrainChunk::scenes_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "scenes"), "scenes_set", "scenes_get");

	//Props

#ifdef MODULE_PROPS_ENABLED
	ClassDB::bind_method(D_METHOD("prop_add", "transform", "prop", "original"), &TerrainChunk::prop_add, DEFVAL(true));

	ClassDB::bind_method(D_METHOD("prop_get", "index"), &TerrainChunk::prop_get);
	ClassDB::bind_method(D_METHOD("prop_set", "index", "prop"), &TerrainChunk::prop_set);

	ClassDB::bind_method(D_METHOD("prop_get_transform", "index"), &TerrainChunk::prop_get_transform);
	ClassDB::bind_method(D_METHOD("prop_set_transform", "index", "transform"), &TerrainChunk::prop_set_transform);

	ClassDB::bind_method(D_METHOD("prop_get_is_original", "index"), &TerrainChunk::prop_get_is_original);
	ClassDB::bind_method(D_METHOD("prop_set_is_original", "index", "original"), &TerrainChunk::prop_set_is_original);

	ClassDB::bind_method(D_METHOD("prop_get_count"), &TerrainChunk::prop_get_count);
	ClassDB::bind_method(D_METHOD("prop_remove", "index"), &TerrainChunk::prop_remove);
	ClassDB::bind_method(D_METHOD("props_clear"), &TerrainChunk::props_clear);

	ClassDB::bind_method(D_METHOD("props_get"), &TerrainChunk::props_get);
	ClassDB::bind_method(D_METHOD("props_set"), &TerrainChunk::props_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "props"), "props_set", "props_get");
#endif

	//Meshes

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	ClassDB::bind_method(D_METHOD("mesh_data_resource_addv", "local_data_pos", "mesh", "texture", "color", "apply_voxel_scale"), &TerrainChunk::mesh_data_resource_addv, DEFVAL(Ref<Texture>()), DEFVAL(Color(1, 1, 1, 1)), DEFVAL(true));
	ClassDB::bind_method(D_METHOD("mesh_data_resource_add", "local_transform", "mesh", "texture", "color", "apply_voxel_scale"), &TerrainChunk::mesh_data_resource_add, DEFVAL(Ref<Texture>()), DEFVAL(Color(1, 1, 1, 1)), DEFVAL(true));

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get", "index"), &TerrainChunk::mesh_data_resource_get);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set", "index", "mesh"), &TerrainChunk::mesh_data_resource_set);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_texture", "index"), &TerrainChunk::mesh_data_resource_get_texture);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_texture", "index", "texture"), &TerrainChunk::mesh_data_resource_set_texture);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_color", "index"), &TerrainChunk::mesh_data_resource_get_color);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_color", "index", "color"), &TerrainChunk::mesh_data_resource_set_color);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_uv_rect", "index"), &TerrainChunk::mesh_data_resource_get_uv_rect);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_uv_rect", "index", "uv_rect"), &TerrainChunk::mesh_data_resource_set_uv_rect);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_transform", "index"), &TerrainChunk::mesh_data_resource_get_transform);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_transform", "index", "transform"), &TerrainChunk::mesh_data_resource_set_transform);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_is_inside", "index"), &TerrainChunk::mesh_data_resource_get_is_inside);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_is_inside", "index", "inside"), &TerrainChunk::mesh_data_resource_set_is_inside);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_is_original", "index"), &TerrainChunk::mesh_data_resource_get_is_original);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_set_is_original", "index", "original"), &TerrainChunk::mesh_data_resource_set_is_original);

	ClassDB::bind_method(D_METHOD("mesh_data_resource_get_count"), &TerrainChunk::mesh_data_resource_get_count);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_remove", "index"), &TerrainChunk::mesh_data_resource_remove);
	ClassDB::bind_method(D_METHOD("mesh_data_resource_clear"), &TerrainChunk::mesh_data_resource_clear);

	ClassDB::bind_method(D_METHOD("mesh_data_resources_get"), &TerrainChunk::mesh_data_resources_get);
	ClassDB::bind_method(D_METHOD("mesh_data_resources_set"), &TerrainChunk::mesh_data_resources_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "mesh_data_resources"), "mesh_data_resources_set", "mesh_data_resources_get");
#endif

	ClassDB::bind_method(D_METHOD("collider_add", "local_transform", "shape", "shape_rid", "body"), &TerrainChunk::collider_add, DEFVAL(RID()), DEFVAL(RID()));

	ClassDB::bind_method(D_METHOD("collider_get_transform", "index"), &TerrainChunk::collider_get_transform);
	ClassDB::bind_method(D_METHOD("collider_set_transform", "index", "transform"), &TerrainChunk::collider_set_transform);

	ClassDB::bind_method(D_METHOD("collider_get_shape", "index"), &TerrainChunk::collider_get_shape);
	ClassDB::bind_method(D_METHOD("collider_set_shape", "index", "shape"), &TerrainChunk::collider_set_shape);

	ClassDB::bind_method(D_METHOD("collider_get_shape_rid", "index"), &TerrainChunk::collider_get_shape_rid);
	ClassDB::bind_method(D_METHOD("collider_set_shape_rid", "index", "rid"), &TerrainChunk::collider_set_shape_rid);

	ClassDB::bind_method(D_METHOD("collider_get_body", "index"), &TerrainChunk::collider_get_body);
	ClassDB::bind_method(D_METHOD("collider_set_body", "index", "rid"), &TerrainChunk::collider_set_body);

	ClassDB::bind_method(D_METHOD("collider_get_count"), &TerrainChunk::collider_get_count);
	ClassDB::bind_method(D_METHOD("collider_remove", "index"), &TerrainChunk::collider_remove);
	ClassDB::bind_method(D_METHOD("colliders_clear"), &TerrainChunk::colliders_clear);

	BIND_VMETHOD(MethodInfo("_build"));
	ClassDB::bind_method(D_METHOD("build"), &TerrainChunk::build);
	ClassDB::bind_method(D_METHOD("_build"), &TerrainChunk::_build);

	BIND_VMETHOD(MethodInfo("_build_immediate"));
	ClassDB::bind_method(D_METHOD("build_immediate"), &TerrainChunk::build_immediate);
	ClassDB::bind_method(D_METHOD("_build_immediate"), &TerrainChunk::_build_immediate);

	ClassDB::bind_method(D_METHOD("get_global_transform"), &TerrainChunk::get_global_transform);
	ClassDB::bind_method(D_METHOD("to_local", "global"), &TerrainChunk::to_local);
	ClassDB::bind_method(D_METHOD("to_global", "local"), &TerrainChunk::to_global);

	ClassDB::bind_method(D_METHOD("_world_transform_changed"), &TerrainChunk::_world_transform_changed);
	ClassDB::bind_method(D_METHOD("_enter_tree"), &TerrainChunk::_enter_tree);
	ClassDB::bind_method(D_METHOD("_exit_tree"), &TerrainChunk::_exit_tree);

	ClassDB::bind_method(D_METHOD("_generation_process"), &TerrainChunk::_generation_process);
	ClassDB::bind_method(D_METHOD("_generation_physics_process"), &TerrainChunk::_generation_physics_process);

	ClassDB::bind_method(D_METHOD("is_safe_to_delete"), &TerrainChunk::is_safe_to_delete);
}
