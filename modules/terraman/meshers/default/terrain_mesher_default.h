#ifndef TERRAIN_MESHER_DEFAULT_H
#define TERRAIN_MESHER_DEFAULT_H
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

#include "core/color.h"

#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "../terrain_mesher.h"

class TerrainMesherDefault : public TerrainMesher {
	GDCLASS(TerrainMesherDefault, TerrainMesher);

public:
	int get_build_flags() const;
	void set_build_flags(const int flags);

	TerrainMesherDefault();
	~TerrainMesherDefault();

protected:
	virtual void _bake_colors(Ref<TerrainChunk> p_chunk);
	virtual void _bake_liquid_colors(Ref<TerrainChunk> p_chunk);

	static void _bind_methods();

private:
	int _build_flags;
};

#endif
