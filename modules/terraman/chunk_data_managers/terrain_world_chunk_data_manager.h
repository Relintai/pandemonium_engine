#ifndef TERRAIN_WORLD_CHUNK_DATA_MANAGER_H
#define TERRAIN_WORLD_CHUNK_DATA_MANAGER_H

/*************************************************************************/
/*  terrain_world_chunk_data_manager.h                                   */
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

#include "core/object/resource.h"

class TerrainChunk;
class TerrainWorld;

class TerrainWorldChunkDataManager : public Resource {
	GDCLASS(TerrainWorldChunkDataManager, Resource);

public:
	// Load
	Ref<TerrainChunk> load_chunk(const Vector2i &p_chunk_position);
	virtual Ref<TerrainChunk> _load_chunk(const Vector2i &p_chunk_position);

	void load_all_chunks(TerrainWorld *p_world);
	void load_chunks(TerrainWorld *p_world, const Vector<Vector2i> &p_chunks);
	void load_all_chunks_bind(Node *p_world);
	void load_chunks_bind(Node *p_world, const Vector<Vector2i> &p_chunks);

	Vector<Vector2i> get_available_chunk_list();
	virtual Vector<Vector2i> _get_available_chunk_list();

	// Save
	void save_chunk(const Ref<TerrainChunk> &p_chunk);
	virtual void _save_chunk(const Ref<TerrainChunk> &p_chunk);

	void save_all_chunks(TerrainWorld *p_world);
	void save_all_chunks_bind(Node *p_world);

	// Delete
	void delete_chunk_data_at(const Vector2i &p_chunk_position);
	virtual void _delete_chunk_data_at(const Vector2i &p_chunk_position);

	void delete_chunk_data(const Ref<TerrainChunk> &p_chunk);
	virtual void _delete_chunk_data(const Ref<TerrainChunk> &p_chunk);

	void delete_all_chunk_data();
	virtual void _delete_all_chunk_data();

	// Callbacks
	void on_world_chunk_created(const Ref<TerrainChunk> &p_chunk);
	virtual void _on_world_chunk_created(const Ref<TerrainChunk> &p_chunk);

	void on_world_chunk_removed(const Ref<TerrainChunk> &p_chunk);
	virtual void _on_world_chunk_removed(const Ref<TerrainChunk> &p_chunk);

	void on_world_chunk_added(const Ref<TerrainChunk> &p_chunk);
	virtual void _on_world_chunk_added(const Ref<TerrainChunk> &p_chunk);

	TerrainWorldChunkDataManager();
	~TerrainWorldChunkDataManager();

protected:
	static void _bind_methods();
};

#endif
