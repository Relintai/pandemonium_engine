/*************************************************************************/
/*  terrain_world_chunk_data_manager.cpp                                 */
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

#include "terrain_world_chunk_data_manager.h"

#include "../world/terrain_chunk.h"
#include "../world/terrain_world.h"

Ref<TerrainChunk> TerrainWorldChunkDataManager::load_chunk(const Vector2i &p_chunk_position) {
	return call("_load_chunk", p_chunk_position);
}
Ref<TerrainChunk> TerrainWorldChunkDataManager::_load_chunk(const Vector2i &p_chunk_position) {
	return Ref<TerrainChunk>();
}

void TerrainWorldChunkDataManager::load_all_chunks(TerrainWorld *p_world) {
	ERR_FAIL_COND(!p_world || !ObjectDB::instance_validate(p_world));

	Vector<Vector2i> chunks = get_available_chunk_list();

	for (int i = 0; i < chunks.size(); ++i) {
		Vector2i chunk_position = chunks[i];

		if (!p_world->chunk_has(chunk_position.x, chunk_position.y)) {
			Ref<TerrainChunk> chunk = load_chunk(chunk_position);

			ERR_CONTINUE(!chunk.is_valid());

			p_world->chunk_add(chunk, chunk_position.x, chunk_position.y);
		}
	}
}

void TerrainWorldChunkDataManager::load_chunks(TerrainWorld *p_world, const Vector<Vector2i> &p_chunks) {
	ERR_FAIL_COND(!p_world || !ObjectDB::instance_validate(p_world));

	for (int i = 0; i < p_chunks.size(); ++i) {
		Vector2i chunk_position = p_chunks[i];

		if (!p_world->chunk_has(chunk_position.x, chunk_position.y)) {
			Ref<TerrainChunk> chunk = load_chunk(chunk_position);

			// Invalid value got passed in, just skip so scripts don't have to worry about checking validity one by one
			// (like a spawn range script)
			if (!chunk.is_valid()) {
				continue;
			}

			p_world->chunk_add(chunk, chunk_position.x, chunk_position.y);
		}
	}
}

void TerrainWorldChunkDataManager::load_all_chunks_bind(Node *p_world) {
	load_all_chunks(Object::cast_to<TerrainWorld>(p_world));
}
void TerrainWorldChunkDataManager::load_chunks_bind(Node *p_world, const Vector<Vector2i> &p_chunks) {
	load_chunks(Object::cast_to<TerrainWorld>(p_world), p_chunks);
}

Vector<Vector2i> TerrainWorldChunkDataManager::get_available_chunk_list() {
	return call("_get_available_chunk_list");
}
Vector<Vector2i> TerrainWorldChunkDataManager::_get_available_chunk_list() {
	return Vector<Vector2i>();
}

// Save
void TerrainWorldChunkDataManager::save_chunk(const Ref<TerrainChunk> &p_chunk) {
	call("_save_chunk", p_chunk);
}
void TerrainWorldChunkDataManager::_save_chunk(const Ref<TerrainChunk> &p_chunk) {
}

void TerrainWorldChunkDataManager::save_all_chunks(TerrainWorld *p_world) {
	ERR_FAIL_COND(!p_world || !ObjectDB::instance_validate(p_world));

	for (int i = 0; i < p_world->chunk_get_count(); ++i) {
		save_chunk(p_world->chunk_get_index(i));
	}
}
void TerrainWorldChunkDataManager::save_all_chunks_bind(Node *p_world) {
	save_all_chunks(Object::cast_to<TerrainWorld>(p_world));
}

// Delete
void TerrainWorldChunkDataManager::delete_chunk_data_at(const Vector2i &p_chunk_position) {
	call("_delete_chunk_data_at", p_chunk_position);
}
void TerrainWorldChunkDataManager::_delete_chunk_data_at(const Vector2i &p_chunk_position) {
}

void TerrainWorldChunkDataManager::delete_chunk_data(const Ref<TerrainChunk> &p_chunk) {
	call("_delete_chunk_data", p_chunk);
}
void TerrainWorldChunkDataManager::_delete_chunk_data(const Ref<TerrainChunk> &p_chunk) {
}

void TerrainWorldChunkDataManager::delete_all_chunk_data() {
	call("_delete_all_chunk_data");
}
void TerrainWorldChunkDataManager::_delete_all_chunk_data() {
}

// Callbacks
void TerrainWorldChunkDataManager::on_world_chunk_created(const Ref<TerrainChunk> &p_chunk) {
	call("_on_world_chunk_created", p_chunk);
}
void TerrainWorldChunkDataManager::_on_world_chunk_created(const Ref<TerrainChunk> &p_chunk) {
}

void TerrainWorldChunkDataManager::on_world_chunk_removed(const Ref<TerrainChunk> &p_chunk) {
	call("_on_world_chunk_removed", p_chunk);
}
void TerrainWorldChunkDataManager::_on_world_chunk_removed(const Ref<TerrainChunk> &p_chunk) {
}

void TerrainWorldChunkDataManager::on_world_chunk_added(const Ref<TerrainChunk> &p_chunk) {
	call("_on_world_chunk_added", p_chunk);
}
void TerrainWorldChunkDataManager::_on_world_chunk_added(const Ref<TerrainChunk> &p_chunk) {
}

TerrainWorldChunkDataManager::TerrainWorldChunkDataManager() {
}

TerrainWorldChunkDataManager::~TerrainWorldChunkDataManager() {
}

void TerrainWorldChunkDataManager::_bind_methods() {
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk"),
			"_load_chunk",
			PropertyInfo(Variant::VECTOR2I, "chunk_position")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::POOL_VECTOR2I_ARRAY, "chunks"), "_get_available_chunk_list"));

	BIND_VMETHOD(MethodInfo("_save_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));

	BIND_VMETHOD(MethodInfo("_delete_chunk_data_at", PropertyInfo(Variant::VECTOR2I, "chunk_position")));
	BIND_VMETHOD(MethodInfo("_delete_chunk_data", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));
	BIND_VMETHOD(MethodInfo("_delete_all_chunk_data"));

	BIND_VMETHOD(MethodInfo("_on_world_chunk_created", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));
	BIND_VMETHOD(MethodInfo("_on_world_chunk_removed", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));
	BIND_VMETHOD(MethodInfo("_on_world_chunk_added", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));

	ClassDB::bind_method(D_METHOD("load_chunk", "chunk_position"), &TerrainWorldChunkDataManager::load_chunk);
	ClassDB::bind_method(D_METHOD("_load_chunk", "chunk_position"), &TerrainWorldChunkDataManager::_load_chunk);

	ClassDB::bind_method(D_METHOD("load_all_chunks", "world"), &TerrainWorldChunkDataManager::load_all_chunks_bind);
	ClassDB::bind_method(D_METHOD("load_chunks", "world", "chunks"), &TerrainWorldChunkDataManager::load_chunks_bind);

	ClassDB::bind_method(D_METHOD("get_available_chunk_list"), &TerrainWorldChunkDataManager::get_available_chunk_list);
	ClassDB::bind_method(D_METHOD("_get_available_chunk_list"), &TerrainWorldChunkDataManager::_get_available_chunk_list);

	// Save
	ClassDB::bind_method(D_METHOD("save_chunk", "chunk"), &TerrainWorldChunkDataManager::save_chunk);
	ClassDB::bind_method(D_METHOD("_save_chunk", "chunk"), &TerrainWorldChunkDataManager::_save_chunk);

	ClassDB::bind_method(D_METHOD("save_all_chunks", "world"), &TerrainWorldChunkDataManager::save_all_chunks_bind);

	// Delete
	ClassDB::bind_method(D_METHOD("delete_chunk_data_at", "chunk_position"), &TerrainWorldChunkDataManager::delete_chunk_data_at);
	ClassDB::bind_method(D_METHOD("_delete_chunk_data_at", "chunk_position"), &TerrainWorldChunkDataManager::_delete_chunk_data_at);

	ClassDB::bind_method(D_METHOD("delete_chunk_data", "chunk"), &TerrainWorldChunkDataManager::delete_chunk_data);
	ClassDB::bind_method(D_METHOD("_delete_chunk_data", "chunk"), &TerrainWorldChunkDataManager::_delete_chunk_data);

	ClassDB::bind_method(D_METHOD("delete_all_chunk_data"), &TerrainWorldChunkDataManager::delete_all_chunk_data);
	ClassDB::bind_method(D_METHOD("_delete_all_chunk_data"), &TerrainWorldChunkDataManager::_delete_all_chunk_data);

	// Callbacks
	ClassDB::bind_method(D_METHOD("on_world_chunk_created", "chunk"), &TerrainWorldChunkDataManager::on_world_chunk_created);
	ClassDB::bind_method(D_METHOD("_on_world_chunk_created", "chunk"), &TerrainWorldChunkDataManager::_on_world_chunk_created);

	ClassDB::bind_method(D_METHOD("on_world_chunk_removed", "chunk"), &TerrainWorldChunkDataManager::on_world_chunk_removed);
	ClassDB::bind_method(D_METHOD("_on_world_chunk_removed", "chunk"), &TerrainWorldChunkDataManager::_on_world_chunk_removed);

	ClassDB::bind_method(D_METHOD("on_world_chunk_added", "chunk"), &TerrainWorldChunkDataManager::on_world_chunk_added);
	ClassDB::bind_method(D_METHOD("_on_world_chunk_added", "chunk"), &TerrainWorldChunkDataManager::_on_world_chunk_added);
}
