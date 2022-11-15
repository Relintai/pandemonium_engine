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

#include "core/io/image.h"
#include "scene/resources/material.h"
#include "scene/resources/shader.h"
#include "scene/resources/texture.h"

#include "../shaders/shaders.h"

#include "../paint_icons/icons.h"

float PaintCanvasBackground::get_pixel_size() const {
	return _pixel_size;
}
void PaintCanvasBackground::set_pixel_size(const float val) {
	_pixel_size = val;

	if (_material.is_valid()) {
		_material->set_shader_param("pixel_size", _pixel_size);
	}
}

PaintCanvasBackground::PaintCanvasBackground() {
	_pixel_size = 1;

	set_expand(true);
	set_stretch_mode(TextureRect::STRETCH_TILE);
}

PaintCanvasBackground::~PaintCanvasBackground() {
}

void PaintCanvasBackground::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
			_shader.instance();
			_shader->set_code(background_shader_shader_code);

			_material.instance();
			_material->set_shader(_shader);
			_material->set_shader_param("pixel_size", _pixel_size);

			set_material(_material);

			set_texture(PaintIcons::make_icon_grid_png());
			break;
		case NOTIFICATION_EXIT_TREE:
			set_material(Ref<Material>());
			_shader.unref();
			_material.unref();
			break;
	}
}

void PaintCanvasBackground::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_pixel_size"), &PaintCanvasBackground::get_pixel_size);
	ClassDB::bind_method(D_METHOD("set_pixel_size"), &PaintCanvasBackground::set_pixel_size);
}