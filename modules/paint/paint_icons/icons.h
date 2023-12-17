#ifndef PAINT_ICONS_H
#define PAINT_ICONS_H

/*************************************************************************/
/*  icons.h                                                              */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "paint_icons.h"

#include "core/io/image.h"
#include "scene/resources/texture.h"

class PaintIcons {
public:
	template <class T>
	static Ref<Texture> make_icon(T p_src) {
		float scale = 1;

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

	static Ref<Texture> make_icon_brush_circle_png() {
		return make_icon(brush_circle_png);
	}

	static Ref<Texture> make_icon_brush_circle_hovered_png() {
		return make_icon(brush_circle_hovered_png);
	}

	static Ref<Texture> make_icon_brush_h_line_png() {
		return make_icon(brush_h_line_png);
	}

	static Ref<Texture> make_icon_brush_h_line_hovered_png() {
		return make_icon(brush_h_line_hovered_png);
	}

	static Ref<Texture> make_icon_brush_rect_png() {
		return make_icon(brush_rect_png);
	}

	static Ref<Texture> make_icon_brush_rect_hovered_png() {
		return make_icon(brush_rect_hovered_png);
	}

	static Ref<Texture> make_icon_brush_v_line_png() {
		return make_icon(brush_v_line_png);
	}

	static Ref<Texture> make_icon_brush_v_line_hovered_png() {
		return make_icon(brush_v_line_hovered_png);
	}
};

#endif
