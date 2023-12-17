/*************************************************************************/
/*  paint_utilities.cpp                                                  */
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

/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
Copyright (c) 2022-2023 Péter Magyar

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

#include "paint_utilities.h"

PoolVector2iArray PaintUtilities::PaintUtilities::get_pixels_in_line(const Vector2i &from, const Vector2i &to) {
	int dx = to[0] - from[0];
	int dy = to[1] - from[1];
	int nx = abs(dx);
	int ny = abs(dy);
	int signX = SGN(dx);
	int signY = SGN(dy);

	Vector2i p = from;

	PoolVector2iArray points;
	points.append(p);

	int ix = 0;
	int iy = 0;

	while (ix < nx || iy < ny) {
		if ((1 + (ix << 1)) * ny < (1 + (iy << 1)) * nx) {
			p[0] += signX;
			ix += 1;
		} else {
			p[1] += signY;
			iy += 1;
		}

		points.append(p);
	}

	return points;
}

int PaintUtilities::to_1D_v(const Vector2i &p, int w) {
	return p.x + p.y * w;
}

int PaintUtilities::to_1D(int x, int y, int w) {
	return x + y * w;
}

Vector2i PaintUtilities::to_2D(int idx, int w) {
	Vector2i p;

	p.x = idx % w;
	p.y = idx / w;

	return p;
}

Color PaintUtilities::color_from_array(const PoolRealArray &color_array) {
	float r = color_array[0];
	float g = color_array[1];
	float b = color_array[2];
	float a = color_array[3];

	return Color(r, g, b, a);
}

Color PaintUtilities::random_color() {
	return Color(Math::randf(), Math::randf(), Math::randf());
}

Color PaintUtilities::random_color_alt() {
	int rand = Math::rand() % 6;

	switch (rand) {
		//red
		case 0:
			return Color(1.00, 0.00, 0.00);
		//blue
		case 1:
			return Color(0.00, 0.00, 1.00);
		//green
		case 2:
			return Color(0.00, 1.00, 0.00);
		//orange
		case 3:
			return Color(1.00, 0.65, 0.00);
		//yellow
		case 4:
			return Color(1.00, 1.00, 0.00);
		//purple
		case 5:
			return Color(0.63, 0.13, 0.94);
	}

	return Color();
}

String PaintUtilities::get_line_string(const String &file, const int number) {
	/*
	return file.get_as_text().split("\n")[number - 1].strip_edges()
	*/

	return "";
}

void PaintUtilities::printv(const Variant &variable) {
	/*
	var stack = get_stack()[get_stack().size() - 1]
	var line = stack.line
	var source = stack.source
	var file = File.new()
	file.open(source, File.READ)
	var line_string = get_line_string(file, line)
	file.close()
	var left_p = line_string.find("(")
	var left_p_string = line_string.right(left_p + 1)
	var right_p = left_p_string.find(")")
	var variable_name = left_p_string.left(right_p)
	print("%s: %s" % [variable_name, variable])
	*/

	ERR_PRINT(variable);
}

PaintUtilities::PaintUtilities() {
}

PaintUtilities::~PaintUtilities() {
}

void PaintUtilities::_bind_methods() {
}
