#ifndef TERRAIN_WORLD_CHUNK_DATA_MANAGER_STATIC_FOLDER_RESOURCES_H
#define TERRAIN_WORLD_CHUNK_DATA_MANAGER_STATIC_FOLDER_RESOURCES_H

/*************************************************************************/
/*  terrain_world_chunk_data_manager_static_folder_resources.h           */
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

class TerrainChunk;

class TerrainWorldChunkDataManagerStaticFolderResources : public TerrainWorldChunkDataManager {
	GDCLASS(TerrainWorldChunkDataManagerStaticFolderResources, TerrainWorldChunkDataManager);

public:
	String get_chunks_path() const;
	void set_chunks_path(const String &p_chunks_path);

	String get_chunk_file_path(const Vector2i &p_chunk_position) const;

	// Load
	virtual Ref<TerrainChunk> _load_chunk(const Vector2i &p_chunk_position);
	virtual Vector<Vector2i> _get_available_chunk_list();

	// Save
	virtual void _save_chunk(const Ref<TerrainChunk> &p_chunk);

	// Delete
	virtual void _delete_chunk_data_at(const Vector2i &p_chunk_position);
	virtual void _delete_chunk_data(const Ref<TerrainChunk> &p_chunk);
	virtual void _delete_all_chunk_data();

	// Callbacks
	virtual void _on_world_chunk_created(const Ref<TerrainChunk> &p_chunk);
	virtual void _on_world_chunk_removed(const Ref<TerrainChunk> &p_chunk);
	virtual void _on_world_chunk_added(const Ref<TerrainChunk> &p_chunk);

	TerrainWorldChunkDataManagerStaticFolderResources();
	~TerrainWorldChunkDataManagerStaticFolderResources();

protected:
	static void _bind_methods();

private:
	String _chunks_path;
};

#endif
