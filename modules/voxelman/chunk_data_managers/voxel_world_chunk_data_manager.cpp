/*************************************************************************/
/*  voxel_world_chunk_data_manager.cpp                                   */
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

#include "voxel_world_chunk_data_manager.h"

#include "../world/voxel_chunk.h"
#include "../world/voxel_world.h"

Ref<VoxelChunk> VoxelWorldChunkDataManager::load_chunk(const Vector3i &p_chunk_position) {
	return call("_load_chunk", p_chunk_position);
}
Ref<VoxelChunk> VoxelWorldChunkDataManager::_load_chunk(const Vector3i &p_chunk_position) {
	return Ref<VoxelChunk>();
}

void VoxelWorldChunkDataManager::load_all_chunks(VoxelWorld *p_world) {
	ERR_FAIL_COND(!p_world || !ObjectDB::instance_validate(p_world));

	Vector<Vector3i> chunks = get_available_chunk_list();

	for (int i = 0; i < chunks.size(); ++i) {
		Vector3i chunk_position = chunks[i];

		if (!p_world->chunk_has(chunk_position.x, chunk_position.y, chunk_position.z)) {
			Ref<VoxelChunk> chunk = load_chunk(chunk_position);

			ERR_CONTINUE(!chunk.is_valid());

			p_world->chunk_add(chunk, chunk_position.x, chunk_position.y, chunk_position.z);
		}
	}
}

void VoxelWorldChunkDataManager::load_chunks(VoxelWorld *p_world, const Vector<Vector3i> &p_chunks) {
	ERR_FAIL_COND(!p_world || !ObjectDB::instance_validate(p_world));

	for (int i = 0; i < p_chunks.size(); ++i) {
		Vector3i chunk_position = p_chunks[i];

		if (!p_world->chunk_has(chunk_position.x, chunk_position.y, chunk_position.z)) {
			Ref<VoxelChunk> chunk = load_chunk(chunk_position);

			// Invalid value got passed in, just skip so scripts don't have to worry about checking validity one by one
			// (like a spawn range script)
			if (!chunk.is_valid()) {
				continue;
			}

			p_world->chunk_add(chunk, chunk_position.x, chunk_position.y, chunk_position.z);
		}
	}
}

void VoxelWorldChunkDataManager::load_all_chunks_bind(Node *p_world) {
	load_all_chunks(Object::cast_to<VoxelWorld>(p_world));
}
void VoxelWorldChunkDataManager::load_chunks_bind(Node *p_world, const Vector<Vector3i> &p_chunks) {
	load_chunks(Object::cast_to<VoxelWorld>(p_world), p_chunks);
}

Vector<Vector3i> VoxelWorldChunkDataManager::get_available_chunk_list() {
	return call("_get_available_chunk_list");
}
Vector<Vector3i> VoxelWorldChunkDataManager::_get_available_chunk_list() {
	return Vector<Vector3i>();
}

// Save
void VoxelWorldChunkDataManager::save_chunk(const Ref<VoxelChunk> &p_chunk) {
	call("_save_chunk", p_chunk);
}
void VoxelWorldChunkDataManager::_save_chunk(const Ref<VoxelChunk> &p_chunk) {
}

void VoxelWorldChunkDataManager::save_all_chunks(VoxelWorld *p_world) {
	ERR_FAIL_COND(!p_world || !ObjectDB::instance_validate(p_world));

	for (int i = 0; i < p_world->chunk_get_count(); ++i) {
		save_chunk(p_world->chunk_get_index(i));
	}
}
void VoxelWorldChunkDataManager::save_all_chunks_bind(Node *p_world) {
	save_all_chunks(Object::cast_to<VoxelWorld>(p_world));
}

// Delete
void VoxelWorldChunkDataManager::delete_chunk_data_at(const Vector3i &p_chunk_position) {
	call("_delete_chunk_data_at", p_chunk_position);
}
void VoxelWorldChunkDataManager::_delete_chunk_data_at(const Vector3i &p_chunk_position) {
}

void VoxelWorldChunkDataManager::delete_chunk_data(const Ref<VoxelChunk> &p_chunk) {
	call("_delete_chunk_data", p_chunk);
}
void VoxelWorldChunkDataManager::_delete_chunk_data(const Ref<VoxelChunk> &p_chunk) {
}

void VoxelWorldChunkDataManager::delete_all_chunk_data() {
	call("_delete_all_chunk_data");
}
void VoxelWorldChunkDataManager::_delete_all_chunk_data() {
}

// Callbacks
void VoxelWorldChunkDataManager::on_world_chunk_created(const Ref<VoxelChunk> &p_chunk) {
	call("_on_world_chunk_created", p_chunk);
}
void VoxelWorldChunkDataManager::_on_world_chunk_created(const Ref<VoxelChunk> &p_chunk) {
}

void VoxelWorldChunkDataManager::on_world_chunk_removed(const Ref<VoxelChunk> &p_chunk) {
	call("_on_world_chunk_removed", p_chunk);
}
void VoxelWorldChunkDataManager::_on_world_chunk_removed(const Ref<VoxelChunk> &p_chunk) {
}

void VoxelWorldChunkDataManager::on_world_chunk_added(const Ref<VoxelChunk> &p_chunk) {
	call("_on_world_chunk_added", p_chunk);
}
void VoxelWorldChunkDataManager::_on_world_chunk_added(const Ref<VoxelChunk> &p_chunk) {
}

VoxelWorldChunkDataManager::VoxelWorldChunkDataManager() {
}

VoxelWorldChunkDataManager::~VoxelWorldChunkDataManager() {
}

void VoxelWorldChunkDataManager::_bind_methods() {
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk"),
			"_load_chunk",
			PropertyInfo(Variant::VECTOR3I, "chunk_position")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::POOL_VECTOR3I_ARRAY, "chunks"), "_get_available_chunk_list"));

	BIND_VMETHOD(MethodInfo("_save_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	BIND_VMETHOD(MethodInfo("_delete_chunk_data_at", PropertyInfo(Variant::VECTOR3I, "chunk_position")));
	BIND_VMETHOD(MethodInfo("_delete_chunk_data", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_delete_all_chunk_data"));

	BIND_VMETHOD(MethodInfo("_on_world_chunk_created", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_on_world_chunk_removed", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));
	BIND_VMETHOD(MethodInfo("_on_world_chunk_added", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk")));

	ClassDB::bind_method(D_METHOD("load_chunk", "chunk_position"), &VoxelWorldChunkDataManager::load_chunk);
	ClassDB::bind_method(D_METHOD("_load_chunk", "chunk_position"), &VoxelWorldChunkDataManager::_load_chunk);

	ClassDB::bind_method(D_METHOD("load_all_chunks", "world"), &VoxelWorldChunkDataManager::load_all_chunks_bind);
	ClassDB::bind_method(D_METHOD("load_chunks", "world", "chunks"), &VoxelWorldChunkDataManager::load_chunks_bind);

	ClassDB::bind_method(D_METHOD("get_available_chunk_list"), &VoxelWorldChunkDataManager::get_available_chunk_list);
	ClassDB::bind_method(D_METHOD("_get_available_chunk_list"), &VoxelWorldChunkDataManager::_get_available_chunk_list);

	// Save
	ClassDB::bind_method(D_METHOD("save_chunk", "chunk"), &VoxelWorldChunkDataManager::save_chunk);
	ClassDB::bind_method(D_METHOD("_save_chunk", "chunk"), &VoxelWorldChunkDataManager::_save_chunk);

	ClassDB::bind_method(D_METHOD("save_all_chunks", "world"), &VoxelWorldChunkDataManager::save_all_chunks_bind);

	// Delete
	ClassDB::bind_method(D_METHOD("delete_chunk_data_at", "chunk_position"), &VoxelWorldChunkDataManager::delete_chunk_data_at);
	ClassDB::bind_method(D_METHOD("_delete_chunk_data_at", "chunk_position"), &VoxelWorldChunkDataManager::_delete_chunk_data_at);

	ClassDB::bind_method(D_METHOD("delete_chunk_data", "chunk"), &VoxelWorldChunkDataManager::delete_chunk_data);
	ClassDB::bind_method(D_METHOD("_delete_chunk_data", "chunk"), &VoxelWorldChunkDataManager::_delete_chunk_data);

	ClassDB::bind_method(D_METHOD("delete_all_chunk_data"), &VoxelWorldChunkDataManager::delete_all_chunk_data);
	ClassDB::bind_method(D_METHOD("_delete_all_chunk_data"), &VoxelWorldChunkDataManager::_delete_all_chunk_data);

	// Callbacks
	ClassDB::bind_method(D_METHOD("on_world_chunk_created", "chunk"), &VoxelWorldChunkDataManager::on_world_chunk_created);
	ClassDB::bind_method(D_METHOD("_on_world_chunk_created", "chunk"), &VoxelWorldChunkDataManager::_on_world_chunk_created);

	ClassDB::bind_method(D_METHOD("on_world_chunk_removed", "chunk"), &VoxelWorldChunkDataManager::on_world_chunk_removed);
	ClassDB::bind_method(D_METHOD("_on_world_chunk_removed", "chunk"), &VoxelWorldChunkDataManager::_on_world_chunk_removed);

	ClassDB::bind_method(D_METHOD("on_world_chunk_added", "chunk"), &VoxelWorldChunkDataManager::on_world_chunk_added);
	ClassDB::bind_method(D_METHOD("_on_world_chunk_added", "chunk"), &VoxelWorldChunkDataManager::_on_world_chunk_added);
}
