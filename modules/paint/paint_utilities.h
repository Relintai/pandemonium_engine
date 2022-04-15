#ifndef PAINT_UTILITIES_H
#define PAINT_UTILITIES_H

/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
Copyright (c) 2022 Péter Magyar

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

#include "core/object.h"

#include "core/pool_vector.h"
#include "core/variant.h"

//class_name GEUtils
class PaintUtilities : public Object {
	GDCLASS(PaintUtilities, Object);

public:
	static Array get_pixels_in_line(Vector2 from, Vector2 to);

	static int to_1D_v(Vector2 p, float w);
	static int to_1D(float x, float y, float w);
	static Vector2 to_2D(int idx, float w);

	static Color color_from_array(PoolRealArray color_array);
	static Color random_color();
	static Color random_color_alt();

	static String get_line_string(String file, int number);

	static void printv(Variant variable);

	PaintUtilities();
	~PaintUtilities();

protected:
	static void _bind_methods();
};

#endif
