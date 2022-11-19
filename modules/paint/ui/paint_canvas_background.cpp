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

#include "paint_canvas_background.h"

#include "../nodes/paint_node.h"

int PaintCanvasBackground::get_grid_size() const {
	return _grid_size;
}
void PaintCanvasBackground::set_grid_size(const int val) {
	_grid_size = val;

	update();
}

Color PaintCanvasBackground::get_grid_black() const {
	return _grid_black;
}
void PaintCanvasBackground::set_grid_black(const Color &val) {
	_grid_black = val;

	update();
}

Color PaintCanvasBackground::get_grid_white() const {
	return _grid_white;
}
void PaintCanvasBackground::set_grid_white(const Color &val) {
	_grid_white = val;

	update();
}

PaintNode *PaintCanvasBackground::get_paint_node() {
	Node *p = this;

	while (p) {
		PaintNode *pn = Object::cast_to<PaintNode>(p);

		if (pn) {
			return pn;
		}

		p = p->get_parent();
	}

	return NULL;
}

PaintCanvasBackground::PaintCanvasBackground() {
	_grid_size = 8;

	_grid_black = Color(0.6, 0.6, 0.6, 1);
	_grid_white = Color(0.79, 0.84, 0.96, 1);
}

PaintCanvasBackground::~PaintCanvasBackground() {
}

void PaintCanvasBackground::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		case PaintNode::NOTIFICATION_PARENT_PAINT_NODE_RESIZED: {
			PaintNode *pn = get_paint_node();

			if (pn) {
				set_size(pn->get_size());
			}
		} break;
		case NOTIFICATION_DRAW: {
			ERR_FAIL_COND(_grid_size <= 0);

			Size2 s = get_size();
			int size_x = s.x;
			int size_y = s.y;

			bool x_black = false;
			for (int x = 0; x < size_x; x += _grid_size) {
				int cx = size_x - x;
				if (cx > _grid_size) {
					cx = _grid_size;
				}

				bool y_black = !x_black;
				for (int y = 0; y < size_y; y += _grid_size) {
					int cy = size_y - y;
					if (cy > _grid_size) {
						cy = _grid_size;
					}

					if (y_black) {
						draw_rect(Rect2(x, y, cx, cy), _grid_black);
					} else {
						draw_rect(Rect2(x, y, cx, cy), _grid_white);
					}

					y_black = !y_black;
				}

				x_black = !x_black;
			}
		} break;
	}
}

void PaintCanvasBackground::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_grid_size"), &PaintCanvasBackground::get_grid_size);
	ClassDB::bind_method(D_METHOD("set_grid_size", "size"), &PaintCanvasBackground::set_grid_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "grid_size"), "set_grid_size", "get_grid_size");

	ClassDB::bind_method(D_METHOD("get_grid_black"), &PaintCanvasBackground::get_grid_black);
	ClassDB::bind_method(D_METHOD("set_grid_black", "color"), &PaintCanvasBackground::set_grid_black);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "grid_black"), "set_grid_black", "get_grid_black");

	ClassDB::bind_method(D_METHOD("get_grid_white"), &PaintCanvasBackground::get_grid_white);
	ClassDB::bind_method(D_METHOD("set_grid_white", "color"), &PaintCanvasBackground::set_grid_white);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "grid_white"), "set_grid_white", "get_grid_white");
}
