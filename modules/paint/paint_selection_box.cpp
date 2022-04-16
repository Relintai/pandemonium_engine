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

#include "paint_selection_box.h"

void PaintSelectionBox::_process(float delta) {
	/*
	update()
	*/
}
void PaintSelectionBox::_draw() {
	/*
if not rect_size == Vector2():
		draw_outline_box(rect_size, Color.gray, outline_size)
	*/
}
void PaintSelectionBox::draw_outline_box(const Vector2 &size, const Color &color, const int width) {
	/*
		#Top line
		draw_line(Vector2(0 + 1, 0), Vector2(size.x, 0), color, width)
		#Left line
		draw_line(Vector2(0 + 1, 0), Vector2(0, size.y), color, width)
		#Bottom line
		draw_line(Vector2(0 + 1, size.y), Vector2(size.x, size.y), color, width)
		#Right line
		draw_line(Vector2(size.x, 0), Vector2(size.x, size.y), color, width)
	*/
}

PaintSelectionBox::PaintSelectionBox() {
	////export var outline_size = 3
}

PaintSelectionBox::~PaintSelectionBox() {
}

void PaintSelectionBox::_bind_methods() {
}
