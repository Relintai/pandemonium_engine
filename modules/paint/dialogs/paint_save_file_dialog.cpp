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

#include "../paint_canvas.h"
#include "../paint_canvas_layer.h"

#include "core/io/image.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"

void PaintSaveFileDialog::save_file() {
	Ref<Image> image;
	image.instance();

	image->create(canvas->get_canvas_width(), canvas->get_canvas_height(), true, Image::FORMAT_RGBA8);
	image->lock();

	for (int i = 0; i < canvas->layers.size(); ++i) {
		Ref<PaintCanvasLayer> layer = canvas->layers[i];

		if (!layer->get_visible()) {
			continue;
		}

		for (int x = 0; x < layer->layer_width; ++x) {
			for (int y = 0; y < layer->layer_height; ++y) {
				Color color = layer->get_pixel(x, y);
				Color image_color = image->get_pixel(x, y);

				if (color.a < 0.999998) {
					image->set_pixel(x, y, image_color.blend(color));
				} else {
					image->set_pixel(x, y, color);
				}
			}
		}
	}

	image->unlock();

	if (FileAccess::exists(file_path)) {
		DirAccess::remove_file_or_error(file_path);
	}

	image->save_png(file_path);
}

void PaintSaveFileDialog::_on_SaveFileDialog_file_selected(const String &path) {
	file_path = path;
	save_file();
}
void PaintSaveFileDialog::_on_SaveFileDialog_about_to_show() {
	invalidate();
}
void PaintSaveFileDialog::_on_SaveFileDialog_visibility_changed() {
	invalidate();
}

void PaintSaveFileDialog::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			connect("file_selected", this, "_on_SaveFileDialog_file_selected");
			connect("about_to_show", this, "_on_SaveFileDialog_about_to_show");
			connect("visibility_changed", this, "_on_SaveFileDialog_visibility_changed");
		} break;
	}
}

PaintSaveFileDialog::PaintSaveFileDialog() {
	add_filter("*.png ; PNG Images");
	set_show_hidden_files(true);
	set_resizable(true);
	set_size(Size2(600, 400));
}

PaintSaveFileDialog::~PaintSaveFileDialog() {
}

void PaintSaveFileDialog::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_SaveFileDialog_file_selected"), &PaintSaveFileDialog::_on_SaveFileDialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_SaveFileDialog_about_to_show"), &PaintSaveFileDialog::_on_SaveFileDialog_about_to_show);
	ClassDB::bind_method(D_METHOD("_on_SaveFileDialog_visibility_changed"), &PaintSaveFileDialog::_on_SaveFileDialog_visibility_changed);
}
