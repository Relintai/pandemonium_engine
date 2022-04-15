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

#include "paint_color_grid.h"

void PaintColorGrid::_enter_tree() {
	/*
	for child in get_children():
		child.set("custom_styles/normal", StyleBoxFlat.new())
		child.get("custom_styles/normal").set("bg_color", Color(randf(), randf(), randf()))
	for child in get_children():
		if child.is_connected("pressed", self, "change_color_to"):
			return
		child.connect("pressed", self, "change_color_to", [child.get("custom_styles/normal").bg_color])
	*/
}
void PaintColorGrid::change_color_to(Color color) {
	/*
	emit_signal("color_change_request", color)
	*/
}
void PaintColorGrid::add_color_prefab(Color color) {
	/*
	var dup = get_child(0).duplicate()
	add_child(dup)
	move_child(dup, 0)
	dup.set("custom_styles/normal", StyleBoxFlat.new())
	dup.get("custom_styles/normal").set("bg_color", color)
	for child in get_children():
		if child.is_connected("pressed", self, "change_color_to"):
			return
		child.connect("pressed", self, "change_color_to", [child.get("custom_styles/normal").bg_color])
	*/
}

PaintColorGrid::PaintColorGrid() {
}

PaintColorGrid::~PaintColorGrid() {
}

void PaintColorGrid::_bind_methods() {
	//signal color_change_request
}
