/*************************************************************************/
/*  terrain_world_chunk_data_manager_static_folder_resources.cpp         */
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

#include "terrain_world_chunk_data_manager_static_folder_resources.h"

#include "../world/terrain_chunk.h"

#include "core/os/dir_access.h"

String TerrainWorldChunkDataManagerStaticFolderResources::get_chunks_path() const {
	return _chunks_path;
}
void TerrainWorldChunkDataManagerStaticFolderResources::set_chunks_path(const String &p_chunks_path) {
	_chunks_path = p_chunks_path;
}

String TerrainWorldChunkDataManagerStaticFolderResources::get_chunk_file_path(const Vector2i &p_chunk_position) const {
	return _chunks_path.append_path(String("chunk_") + String::num_int64(p_chunk_position.x) + "_" + String::num_int64(p_chunk_position.y) + ".tres");
}

// Load
Ref<TerrainChunk> TerrainWorldChunkDataManagerStaticFolderResources::_load_chunk(const Vector2i &p_chunk_position) {
	String fn = get_chunk_file_path(p_chunk_position);

	if (!FileAccess::exists(fn)) {
		return Ref<TerrainChunk>();
	}

	return ResourceLoader::load(fn, "TerrainChunk");
}
Vector<Vector2i> TerrainWorldChunkDataManagerStaticFolderResources::_get_available_chunk_list() {
	Vector<Vector2i> ret;

	DirAccess *dir = DirAccess::open(_chunks_path);

	ERR_FAIL_COND_V(!dir, ret);

	dir->list_dir_begin();

	String f = dir->get_next();
	while (!f.empty()) {
		if (dir->current_is_dir()) {
			f = dir->get_next();
			continue;
		}

		Vector<String> fs = f.split("_", false);

		if (fs.size() != 3) {
			f = dir->get_next();
			continue;
		}

		if (fs[0] != "chunk") {
			f = dir->get_next();
			continue;
		}

		ret.push_back(Vector2i(fs[1].to_int(), fs[2].to_int()));

		f = dir->get_next();
	}

	dir->list_dir_end();

	memdelete(dir);

	return ret;
}

// Save
void TerrainWorldChunkDataManagerStaticFolderResources::_save_chunk(const Ref<TerrainChunk> &p_chunk) {
	String fn = get_chunk_file_path(Vector2i(p_chunk->get_position_x(), p_chunk->get_position_z()));

	ResourceSaver::save(fn, p_chunk);
}

// Delete
void TerrainWorldChunkDataManagerStaticFolderResources::_delete_chunk_data_at(const Vector2i &p_chunk_position) {
	String fn = get_chunk_file_path(p_chunk_position);

	DirAccess *da = DirAccess::create(DirAccess::ACCESS_RESOURCES);

	if (da->file_exists(fn)) {
		da->remove(fn);
	}

	memdelete(da);
}
void TerrainWorldChunkDataManagerStaticFolderResources::_delete_chunk_data(const Ref<TerrainChunk> &p_chunk) {
	String fn = get_chunk_file_path(Vector2i(p_chunk->get_position_x(), p_chunk->get_position_z()));

	DirAccess *da = DirAccess::create(DirAccess::ACCESS_RESOURCES);

	if (da->file_exists(fn)) {
		da->remove(fn);
	}

	memdelete(da);
}
void TerrainWorldChunkDataManagerStaticFolderResources::_delete_all_chunk_data() {
	Vector<Vector2i> available_chunks = get_available_chunk_list();

	for (int i = 0; i < available_chunks.size(); ++i) {
		delete_chunk_data_at(available_chunks[i]);
	}
}

// Callbacks
void TerrainWorldChunkDataManagerStaticFolderResources::_on_world_chunk_created(const Ref<TerrainChunk> &p_chunk) {
	save_chunk(p_chunk);
}
void TerrainWorldChunkDataManagerStaticFolderResources::_on_world_chunk_removed(const Ref<TerrainChunk> &p_chunk) {
}
void TerrainWorldChunkDataManagerStaticFolderResources::_on_world_chunk_added(const Ref<TerrainChunk> &p_chunk) {
}

TerrainWorldChunkDataManagerStaticFolderResources::TerrainWorldChunkDataManagerStaticFolderResources() {
}

TerrainWorldChunkDataManagerStaticFolderResources::~TerrainWorldChunkDataManagerStaticFolderResources() {
}

void TerrainWorldChunkDataManagerStaticFolderResources::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_chunks_path"), &TerrainWorldChunkDataManagerStaticFolderResources::get_chunks_path);
	ClassDB::bind_method(D_METHOD("set_chunks_path", "value"), &TerrainWorldChunkDataManagerStaticFolderResources::set_chunks_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "chunks_path"), "set_chunks_path", "get_chunks_path");
}
