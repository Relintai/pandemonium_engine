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

#include "paint_visual_grid.h"

void PaintVisualGrid::_draw() {
	/*
	if size == 0:
		size = 1

	var temp_size = size + zoom

	var wrap_offset = Vector2(wrapf(offset.x, 0, temp_size), wrapf(offset.y, 0, temp_size))

	var ceil_x = ceil(rect_size.x / temp_size)
	var ceil_y = ceil(rect_size.y / temp_size)

	for i in ceil_y:
		var start_x = Vector2(0, (i * temp_size) + wrap_offset.y)
		var end_x = Vector2(rect_size.x, (i * temp_size) + wrap_offset.y)
#		var end_x = Vector2(int(rect_size.x) + size - int(rect_size.x) % size, (i * temp_size) + wrap_offset.y)
		draw_line(start_x, end_x, color, 1)

	for i in ceil_x:
		var start_y = Vector2((i * temp_size) + wrap_offset.x, 0)
		var end_y = Vector2((i * temp_size) + (wrap_offset.x), rect_size.y)
#		var end_y = Vector2((i * temp_size) + (wrap_offset.x), int(rect_size.y) + size - int(rect_size.y) % size)
		draw_line(start_y, end_y, color, 1)


	*/
}

void PaintVisualGrid::_notification(int p_what) {
	/*
	func _enter_tree():
	set_process(true)

	func _process(delta):
	if not is_visible_in_tree():
		return
	update()
	*/
}

PaintVisualGrid::PaintVisualGrid() {
	/*
	export var color = Color()
	export var size:int = 16
	export var zoom = 0
	export var offset = Vector2(0, 0)
	*/
}

PaintVisualGrid::~PaintVisualGrid() {
}

void PaintVisualGrid::_bind_methods() {
}
