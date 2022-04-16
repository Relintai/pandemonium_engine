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

#include "paint_save_file_dialog.h"

void PaintSaveFileDialog::_ready() {
	/*
	# warning-ignore:return_value_discarded
	get_line_edit().connect("text_entered", self, "_on_LineEdit_text_entered")
	invalidate()
	clear_filters()
	add_filter("*.png ; PNG Images")
	*/
}
void PaintSaveFileDialog::_on_SaveFileDialog_file_selected(const String &path) {
	/*
	#print("selected file: ", path)
	file_path = path
	save_file()
	*/
}
void PaintSaveFileDialog::save_file() {
	/*
	var image = Image.new()
	var canvas = get_parent().paint_canvas
	image.create(canvas.canvas_width, canvas.canvas_height, true, Image.FORMAT_RGBA8)
	image.lock()

	for layer in canvas.layers:
		var idx = 0
		if not layer.visible:
			continue
		for x in range(layer.layer_width):
			for y in range(layer.layer_height):
				var color = layer.get_pixel(x, y)
				var image_color = image.get_pixel(x, y)

				if color.a != 0:
					image.set_pixel(x, y, image_color.blend(color))
				else:
					image.set_pixel(x, y, color)
	image.unlock()

	var dir = Directory.new()
	if dir.file_exists(file_path):
		dir.remove(file_path)

	image.save_png(file_path)
	*/
}
void PaintSaveFileDialog::_on_SaveFileDialog_about_to_show() {
	/*
	invalidate()
	*/
}
void PaintSaveFileDialog::_on_SaveFileDialog_visibility_changed() {
	/*
	invalidate()
	*/
}

PaintSaveFileDialog::PaintSaveFileDialog() {
	//var file_path = ""
}

PaintSaveFileDialog::~PaintSaveFileDialog() {
}

void PaintSaveFileDialog::_bind_methods() {
}
