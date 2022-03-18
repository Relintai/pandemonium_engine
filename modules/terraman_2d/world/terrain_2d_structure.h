#ifndef TERRAIN_2D_STRUCTURE_H
#define TERRAIN_2D_STRUCTURE_H
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

#include "core/hash_map.h"
#include "core/resource.h"

#include "../defines.h"

#include "core/math/rect2.h"
#include "core/pool_vector.h"
#include "terrain_2d_chunk.h"

class Terrain2DStructure : public Resource {
	GDCLASS(Terrain2DStructure, Resource);

public:
	bool get_use_rect() const;
	void set_use_rect(const bool value);

	Rect2 get_chunk_rect() const;
	void set_chunk_rect(const Rect2 &value);

	int get_position_x() const;
	void set_position_x(const int value);

	int get_position_y() const;
	void set_position_y(const int value);

	void set_position(const int x, const int y);

	void write_to_chunk(Ref<Terrain2DChunk> chunk);

	Terrain2DStructure();
	~Terrain2DStructure();

protected:
	static void _bind_methods();

private:
	bool _use_rect;
	Rect2 _chunk_rect;

	int _position_x;
	int _position_y;
};

#endif
