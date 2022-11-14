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

#include "paint_canvas_outline.h"

void PaintCanvasOutline::draw_outline_box(const Vector2 &pos, const Vector2 &size, const Color &color, const int width) {
	//Top line
	draw_line(pos, pos + Vector2(size.x, 0), color, width);
	//Left line
	draw_line(pos, pos + Vector2(0, size.y), color, width);
	//Bottom line
	draw_line(pos + Vector2(0, size.y), pos + Vector2(size.x, size.y), color, width);
	//Right line
	draw_line(pos + Vector2(size.x, 0), pos + Vector2(size.x, size.y), color, width);
}

void PaintCanvasOutline::_notification(int p_what) {
	switch(p_what) {
		/*
		case NOTIFICATION_PROCESS: {
			if (!is_visible_in_tree()) {
				return;
			}

			update();
		} break;
		*/
		case NOTIFICATION_DRAW: {
			//Control *pc = get_parent_control();

			//if (!pc) {
			//	return;
			//}

			//Vector2 size = pc.rect_size;

			//var pos = Vector2(); //get_parent().rect_global_position
			draw_outline_box(Vector2(), get_size(), color, 1);
		} break;
	}
}

PaintCanvasOutline::PaintCanvasOutline() {
	color = Color(0, 1, 0, 1);
}

PaintCanvasOutline::~PaintCanvasOutline() {
}

void PaintCanvasOutline::_bind_methods() {
	ClassDB::bind_method(D_METHOD("draw_outline_box", "pos", "size", "color", "width"), &PaintCanvasOutline::draw_outline_box);
}
