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

#include "scene/resources/material.h"
#include "scene/resources/shader.h"
#include "core/io/image.h"
#include "scene/resources/texture.h"

#include "../shaders/shaders.h"

#include "../paint_icons/paint_icons.h"

static float scale = 1;

template <class T>
static Ref<Texture> make_icon(T p_src) {
	Ref<ImageTexture> texture(memnew(ImageTexture));
	Ref<Image> img = memnew(Image(p_src));
	if (scale > 1) {
		Size2 orig_size = Size2(img->get_width(), img->get_height());

		img->convert(Image::FORMAT_RGBA8);
		img->expand_x2_hq2x();
		if (scale != 2.0) {
			img->resize(orig_size.x * scale, orig_size.y * scale);
		}
	} else if (scale < 1) {
		Size2 orig_size = Size2(img->get_width(), img->get_height());
		img->convert(Image::FORMAT_RGBA8);
		img->resize(orig_size.x * scale, orig_size.y * scale);
	}
	texture->create_from_image(img, ImageTexture::FLAG_FILTER);

	return texture;
}

float PaintCanvasBackground::get_pixel_size() const {
	return _pixel_size;
}
void PaintCanvasBackground::set_pixel_size(const float val) {
	_pixel_size = val;

	_material->set_shader_param("pixel_size", _pixel_size);
}

PaintCanvasBackground::PaintCanvasBackground() {
	_pixel_size = 1;

	set_expand(true);
	set_stretch_mode(TextureRect::STRETCH_TILE);

	set_texture(make_icon(grid_png));

	_shader.instance();
	_shader->set_code(background_shader_shader_code);

	_material.instance();
	_material->set_shader(_shader);
	_material->set_shader_param("pixel_size", _pixel_size);

	set_material(_material);
}

PaintCanvasBackground::~PaintCanvasBackground() {
	_material.unref();
	_shader.unref();
}

void PaintCanvasBackground::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_pixel_size"), &PaintCanvasBackground::get_pixel_size);
	ClassDB::bind_method(D_METHOD("set_pixel_size"), &PaintCanvasBackground::set_pixel_size);
}
