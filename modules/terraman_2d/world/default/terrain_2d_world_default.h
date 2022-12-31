#ifndef TERRAIN_2D_WORLD_DEFAULT_H
#define TERRAIN_2D_WORLD_DEFAULT_H
/*
Copyright (c) 2019-2023 Péter Magyar

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

#include "../terrain_2d_world.h"

class Terrain2DWorldDefault : public Terrain2DWorld {
	GDCLASS(Terrain2DWorldDefault, Terrain2DWorld);

public:
	int get_build_flags() const;
	void set_build_flags(const int flags);

	PoolColorArray get_vertex_colors(const Transform2D &transform, const PoolVector2Array &vertices, const float base_light_value = 0.45, const float ao_strength = 0.2);

	Terrain2DWorldDefault();
	~Terrain2DWorldDefault();

protected:
	void _notification(int p_what);
	Ref<Terrain2DChunk> _create_chunk(int x, int y, Ref<Terrain2DChunk> p_chunk);
	virtual void _chunk_added(Ref<Terrain2DChunk> chunk);
	int _get_channel_index_info(const ChannelTypeInfo channel_type);

	//virtual void _notification(int p_what);

	static void _bind_methods();

private:
	int _build_flags;
};

#endif
