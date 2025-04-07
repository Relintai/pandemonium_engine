#ifndef TERRAIN_WORLD_DEFAULT_H
#define TERRAIN_WORLD_DEFAULT_H

/*************************************************************************/
/*  terrain_world_default.h                                              */
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

#include "../terrain_world.h"

class TerrainWorldDefault : public TerrainWorld {
	GDCLASS(TerrainWorldDefault, TerrainWorld);

public:
	int get_build_flags() const;
	void set_build_flags(const int flags);

	float get_lod_update_interval() const;
	void set_lod_update_interval(const float value);

	int get_chunk_lod_first_falloff() const;
	void set_chunk_lod_first_falloff(const int value);

	int get_chunk_lod_falloff() const;
	void set_chunk_lod_falloff(const int value);

	int get_num_lods() const;
	void set_num_lods(const int value);

	void update_lods();

	PoolColorArray get_vertex_colors(const Transform &transform, const PoolVector3Array &vertices, const float base_light_value = 0.45, const float ao_strength = 0.2);

	TerrainWorldDefault();
	~TerrainWorldDefault();

protected:
	void _notification(int p_what);
	void _update_lods();
	Ref<TerrainChunk> _create_chunk(int x, int z, Ref<TerrainChunk> p_chunk);
	virtual void _setup_chunk(Ref<TerrainChunk> p_chunk);
	virtual void _chunk_added(Ref<TerrainChunk> chunk);
	int _get_channel_index_info(const ChannelTypeInfo channel_type);

	//virtual void _notification(int p_what);

	static void _bind_methods();

private:
	int _build_flags;
	float _lod_update_timer;
	float _lod_update_interval;
	int _chunk_lod_first_falloff;
	int _chunk_lod_falloff;
	int _num_lods;
};

#endif
