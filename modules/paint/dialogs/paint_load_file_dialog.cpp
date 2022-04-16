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

#include "paint_load_file_dialog.h"

void PaintLoadFileDialog::_on_LoadFileDialog_file_selected(const String &path) {
	/*
	file_path = path
	#print("1ere")
	load_img()
	*/
}
void PaintLoadFileDialog::_on_LoadFileDialog_confirmed() {
	/*
	return
	#print("ere")
	#load_img()
	*/
}
void PaintLoadFileDialog::load_img() {
	/*
	var image = Image.new()
	if image.load(file_path) != OK:
		print("couldn't load image!")
		return

	var image_data = image.get_data()
	var layer: GELayer = owner.add_new_layer()

	var width = image.get_width()
	var height = image.get_height()

	if owner.paint_canvas.canvas_width < width:
		owner.paint_canvas.resize(width, owner.paint_canvas.canvas_height)

	if owner.paint_canvas.canvas_height < height:
		owner.paint_canvas.resize(owner.paint_canvas.canvas_width, height)

	for i in range(image_data.size() / 4):
		var color = Color(image_data[i*4] / 255.0, image_data[i*4+1] / 255.0, image_data[i*4+2] / 255.0, image_data[i*4+3] / 255.0)
		var pos = GEUtils.to_2D(i, image.get_width())
		if pos.x > layer.layer_width:
			continue

		layer.set_pixel(pos.x, pos.y, color)
	layer.update_texture()
	*/
}
void PaintLoadFileDialog::_on_LoadFileDialog_about_to_show() {
	/*
	invalidate()
	*/
}
void PaintLoadFileDialog::_on_LoadFileDialog_visibility_changed() {
	/*
	invalidate()
	*/
}

PaintLoadFileDialog::PaintLoadFileDialog() {
	//var file_path = ""

/*
	get_line_edit().connect("text_entered", self, "_on_LineEdit_text_entered")
	invalidate()
	clear_filters()
	add_filter("*.png ; PNG Images")
*/

	/*

[gd_scene load_steps=2 format=2]

[sub_resource type="GDScript" id=1]
script/source = "extends ConfirmationDialog

func _ready():
	get_ok().connect(\"pressed\", self, \"hide\")
	get_cancel().connect(\"pressed\", self, \"hide\")



"

[node name="LoadFileDialog" type="FileDialog"]
margin_right = 604.0
margin_bottom = 367.0
window_title = "Open a File"
mode = 0
access = 2
current_dir = "/Projects/BitBucket/GraphicsEditor"
current_path = "/Projects/BitBucket/GraphicsEditor/"
script = SubResource( 1 )


	*/
}

PaintLoadFileDialog::~PaintLoadFileDialog() {
}

void PaintLoadFileDialog::_bind_methods() {
}
