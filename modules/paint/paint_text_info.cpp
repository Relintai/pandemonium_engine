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

#include "paint_text_info.h"

//TODO: To make reading the text easier, the text info with the longest text should have it's length applied to all the
//the other text infos

void PaintTextInfo::add_text_info(String text_name, Node *custom_node) {
	/*
	var last_text_info_child = null
	var child_count = get_child_count()
	if not child_count <= 0:
		last_text_info_child = get_children()[get_children().size() - 1]
	var label = Label.new()
	label.name = text_name
	label.rect_size = Vector2(size, 14)
	if not last_text_info_child == null:
		var x = last_text_info_child.rect_position.x
		var y = last_text_info_child.rect_position.y
		var temp_size = size
		if child_count == 4:
			x = 0
			y = 20
			temp_size = 0
		label.rect_position = Vector2(x + temp_size, y)
	if not custom_node == null:
		label.add_child(custom_node)
	add_child(label)
	*/
}
void PaintTextInfo::update_text_info(String text_name, Node *text_value, Node *node, Node *node_target_value, Node *node_value) {
	/*
	var text_label = self.get_node(text_name)
	if text_label == null:
		return
	if not node == null:
		get_node(text_name).get_node(node).set(node_target_value, node_value)
	if text_value == null:
		text_label.text = "%s: %s" % [text_name, null]
	else:
		text_label.text = "%s: %s" % [text_name, String(text_value)]
	*/
}

PaintTextInfo::PaintTextInfo() {
	//var size = 240
}

PaintTextInfo::~PaintTextInfo() {
}

void PaintTextInfo::_bind_methods() {
}
