#ifndef TERRAIN_MESHER_BLOCKY_H
#define TERRAIN_MESHER_BLOCKY_H
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

#include "core/math/color.h"

#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "../../world/default/terrain_chunk_default.h"
#include "../default/terrain_mesher_default.h"

class TerrainMesherBlocky : public TerrainMesherDefault {
	GDCLASS(TerrainMesherBlocky, TerrainMesherDefault);

public:
	bool get_always_add_colors() const;
	void set_always_add_colors(const bool value);

	void _add_chunk(Ref<TerrainChunk> p_chunk);

	void add_chunk_normal(Ref<TerrainChunkDefault> chunk);

	void add_chunk_lod(Ref<TerrainChunkDefault> chunk);
	void create_margin_zmin(Ref<TerrainChunkDefault> chunk);
	void create_margin_zmax(Ref<TerrainChunkDefault> chunk);
	void create_margin_xmin(Ref<TerrainChunkDefault> chunk);
	void create_margin_xmax(Ref<TerrainChunkDefault> chunk);
	void create_margin_corners(Ref<TerrainChunkDefault> chunk);
	void create_face(Ref<TerrainChunkDefault> chunk, int dataxmin, int dataxmax, int datazmin, int datazmax);

	TerrainMesherBlocky();
	~TerrainMesherBlocky();

protected:
	static void _bind_methods();

private:
	bool _always_add_colors;
};

#endif
