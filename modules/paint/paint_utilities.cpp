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

#include "paint_utilities.h"

Array PaintUtilities::PaintUtilities::get_pixels_in_line(Vector2 from, Vector2 to) {
	/*
	var dx = to[0] - from[0]
	var dy = to[1] - from[1]
	var nx = abs(dx)
	var ny = abs(dy)
	var signX = sign(dx)
	var signY = sign(dy)
	var p = from
	var points : Array = [p]

	var ix = 0
	var iy = 0

	while ix < nx || iy < ny:
		if (1 + (ix << 1)) * ny < (1 + (iy << 1)) * nx:
			p[0] += signX
			ix +=1
		else:
			p[1] += signY
			iy += 1
		points.append(p)
	return points
	*/
}

int to_1D_v(Vector2 p, float w) {
	/*
	return p.x + p.y * w
	*/
}
int PaintUtilities::to_1D(float x, float y, float w) {
	/*
	return x + y * w
	*/
}
Vector2 PaintUtilities::to_2D(int idx, float w) {
	/*
	var p = Vector2()
	p.x = int(idx) % int(w)
	p.y = int(idx / w)
	return p
	*/
}

Color PaintUtilities::color_from_array(PoolRealArray color_array) {
	/*
	var r = color_array[0]
	var g = color_array[1]
	var b = color_array[2]
	var a = color_array[3]
	return Color(r, g, b, a)
	*/
}
Color PaintUtilities::random_color() {
	/*
	return Color(randf(), randf(), randf())
	*/
}
Color PaintUtilities::random_color_alt() {
	/*
	var rand = randi() % 6

	match rand:
		#red
		0:
			return Color.red
		#blue
		1:
			return Color.blue
		#green
		2:
			return Color.green
		#orange
		3:
			return Color.orange
		#yellow
		4:
			return Color.yellow
		#purple
		5:
			return Color.purple
	*/
}

String PaintUtilities::get_line_string(String file, int number) {
	/*
	return file.get_as_text().split("\n")[number - 1].strip_edges()
	*/
}

void PaintUtilities::printv(Variant variable) {
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
}

PaintUtilities::PaintUtilities() {
}

PaintUtilities::~PaintUtilities() {
}

void PaintUtilities::_bind_methods() {
}
