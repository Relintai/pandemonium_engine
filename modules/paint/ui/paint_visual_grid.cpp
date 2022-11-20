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

int PaintVisualGrid::get_grid_size() {
	return _grid_size;
}
void PaintVisualGrid::set_grid_size(const int gs) {
	_grid_size = gs;
	update();
}

Color PaintVisualGrid::get_grid_color() {
	return _grid_color;
}
void PaintVisualGrid::set_grid_color(const Color &val) {
	_grid_color = val;
	update();
}

void PaintVisualGrid::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		case PaintNode::NOTIFICATION_PARENT_PAINT_NODE_RESIZED: {
			PaintNode *pn = get_parent_paint_node();

			if (pn) {
				set_size(pn->get_size());
			}
		} break;
		case NOTIFICATION_DRAW: {
			ERR_FAIL_COND(_grid_size <= 0);

			Size2 rect_size = get_size();
			int sx = rect_size.x;
			int sy = rect_size.y;

			for (int x = 0; x < sx; x += _grid_size) {
				draw_line(Vector2(x, 0), Vector2(x, sy), _grid_color, 1);
			}

			for (int y = 0; y < sy; y += _grid_size) {
				draw_line(Vector2(0, y), Vector2(sx, y), _grid_color, 1);
			}
		} break;
	}
}

PaintVisualGrid::PaintVisualGrid() {
	_grid_color = Color(1, 1, 1, 0.42);
	_grid_size = 2;
}

PaintVisualGrid::~PaintVisualGrid() {
}

void PaintVisualGrid::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_grid_size"), &PaintVisualGrid::get_grid_size);
	ClassDB::bind_method(D_METHOD("set_grid_size", "size"), &PaintVisualGrid::set_grid_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "grid_size"), "set_grid_size", "get_grid_size");

	ClassDB::bind_method(D_METHOD("get_grid_color"), &PaintVisualGrid::get_grid_color);
	ClassDB::bind_method(D_METHOD("set_grid_color", "color"), &PaintVisualGrid::set_grid_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "grid_color"), "set_grid_color", "get_grid_color");
}
