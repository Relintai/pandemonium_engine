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

#include "../paint_canvas.h"
#include "../paint_canvas_layer.h"
#include "../paint_window.h"
#include "../paint_utilities.h"

void PaintLoadFileDialog::load_img() {
	Ref<Image> image;
	image.instance();

	if (image->load(file_path) != OK) {
		ERR_FAIL_MSG("couldn't load image!");
	}

	PoolByteArray image_data = image->get_data();
	Ref<PaintCanvasLayer> layer = window->add_new_layer();

	int width = image->get_width();
	int height = image->get_height();

	if (canvas->get_canvas_width() < width) {
		canvas->resize(width, canvas->get_canvas_height());
	}

	if (canvas->get_canvas_height() < height) {
		canvas->resize(canvas->get_canvas_width(), height);
	}

	int iss = image_data.size() / 4;
	for (int i = 0; i < iss; ++i) {
		Color color = Color(image_data[i*4] / 255.0, image_data[i*4+1] / 255.0, image_data[i*4+2] / 255.0, image_data[i*4+3] / 255.0);
		Vector2i pos = PaintUtilities::to_2D(i, image->get_width());
		if (pos.x > layer->layer_width) {
			continue;
		}

		layer->set_pixel(pos.x, pos.y, color);
	}

	layer->update_texture();
}

void PaintLoadFileDialog::_on_LoadFileDialog_file_selected(const String &path) {
	file_path = path;
	load_img();
}
void PaintLoadFileDialog::_on_LoadFileDialog_confirmed() {
	/*
	return
	#print("ere")
	#load_img()
	*/
}
void PaintLoadFileDialog::_on_LoadFileDialog_about_to_show() {
	invalidate();
}
void PaintLoadFileDialog::_on_LoadFileDialog_visibility_changed() {
	invalidate();
}

void PaintLoadFileDialog::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			connect("file_selected", this, "_on_LoadFileDialog_file_selected");
			connect("about_to_show", this, "_on_LoadFileDialog_about_to_show");
			connect("confirmed", this, "_on_LoadFileDialog_confirmed");
			connect("visibility_changed", this, "_on_LoadFileDialog_visibility_changed");
		} break;
	}
}

PaintLoadFileDialog::PaintLoadFileDialog() {
	add_filter("*.png ; PNG Images");
	set_mode(FileDialog::MODE_OPEN_FILE);
	set_show_hidden_files(true);
	set_resizable(true);
	set_size(Size2(600, 400));
	set_title("Open a File");
}

PaintLoadFileDialog::~PaintLoadFileDialog() {
}

void PaintLoadFileDialog::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_LoadFileDialog_file_selected", "path"), &PaintLoadFileDialog::_on_LoadFileDialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_LoadFileDialog_confirmed"), &PaintLoadFileDialog::_on_LoadFileDialog_confirmed);
	ClassDB::bind_method(D_METHOD("_on_LoadFileDialog_about_to_show"), &PaintLoadFileDialog::_on_LoadFileDialog_about_to_show);
	ClassDB::bind_method(D_METHOD("_on_LoadFileDialog_visibility_changed"), &PaintLoadFileDialog::_on_LoadFileDialog_visibility_changed);
}
