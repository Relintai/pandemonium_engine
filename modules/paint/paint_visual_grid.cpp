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

void PaintVisualGrid::set_grid_size(const int gs) {
	size = gs;
	update();
}

void PaintVisualGrid::_draw() {
	if (size == 0) {
		size = 1;
	}

	Size2 rect_size = get_size();
	int sx = rect_size.x;
	int sy = rect_size.y;

	for (int x = 0; x < sx; x += size) {
		draw_line(Vector2(x, 0), Vector2(x, sy), color, 1);
	}

	for (int y = 0; y < sy; y += size) {
		draw_line(Vector2(0, y), Vector2(sx, y), color, 1);
	}

	/*
	  float temp_size = size + zoom;

	  Vector2 wrap_offset = Vector2(Math::wrapf(offset.x, 0, temp_size), Math::wrapf(offset.y, 0, temp_size));

	  Size2 rect_size = get_size();

	  float ceil_x = Math::ceil(rect_size.x / temp_size);
	  float ceil_y = Math::ceil(rect_size.y / temp_size);

	  for (int i = 0; i < ceil_y; ++i) {
		  Point2 start_x = Vector2(0, (i * temp_size) + wrap_offset.y);
		  Point2 end_x = Vector2(rect_size.x, (i * temp_size) + wrap_offset.y);
		  //var end_x = Vector2(int(rect_size.x) + size - int(rect_size.x) % size, (i * temp_size) + wrap_offset.y);
		  draw_line(start_x, end_x, color, 1);
	  }

	  for (int i = 0; i < ceil_x; ++i) {
		  Point2 start_y = Vector2((i * temp_size) + wrap_offset.x, 0);
		  Point2 end_y = Vector2((i * temp_size) + (wrap_offset.x), rect_size.y);
		  //var end_y = Vector2((i * temp_size) + (wrap_offset.x), int(rect_size.y) + size - int(rect_size.y) % size);
		  draw_line(start_y, end_y, color, 1);
	  }
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

	switch (p_what) {
		/*
		case NOTIFICATION_PROCESS: {
			if (!is_visible_in_tree()) {
				return;
			}

			update();
		} break;
		*/
		case NOTIFICATION_DRAW: {
			_draw();
		} break;
	}
}

PaintVisualGrid::PaintVisualGrid() {
	color = Color(1, 1, 1, 0.42);
	size = 16;
	//zoom = 0;
}

PaintVisualGrid::~PaintVisualGrid() {
}

void PaintVisualGrid::_bind_methods() {
}
