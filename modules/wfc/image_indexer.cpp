/*************************************************************************/
/*  image_indexer.cpp                                                    */
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

#include "image_indexer.h"

PoolColorArray ImageIndexer::get_colors() {
	return _colors;
}

PoolIntArray ImageIndexer::index_image(Ref<Image> image) {
	ERR_FAIL_COND_V(!image.is_valid(), PoolIntArray());

	image->lock();

	int w = image->get_width();
	int h = image->get_height();

	PoolIntArray color_indices;
	color_indices.resize(w * h);
	PoolIntArray::Write wci = color_indices.write();
	int *wi = wci.ptr();

	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {
			Color c = image->get_pixel(x, y);

			int color_index = 0;
			if (!_col_map.lookup(c, color_index)) {
				color_index = _colors.size();
				_colors.push_back(c);
				_col_map.set(c, color_index);
			}

			wi[y * w + x] = color_index;
		}
	}

	wci.release();
	image->unlock();

	return color_indices;
}

void ImageIndexer::reset() {
	_colors.resize(0);
	_col_map.clear();
}

PoolByteArray ImageIndexer::indices_to_argb8_data(const PoolIntArray &indices) {
	PoolByteArray arr;
	arr.resize(indices.size() * 4);

	PoolByteArray::Write arrw = arr.write();
	PoolIntArray::Read indr = indices.read();
	int s = indices.size();

	for (int i = 0; i < s; ++i) {
		int arrind = i * 4;

		Color c = _colors[indr[i]];

		arrw[arrind] = static_cast<uint8_t>(c.r * 255);
		arrw[arrind + 1] = static_cast<uint8_t>(c.g * 255);
		arrw[arrind + 2] = static_cast<uint8_t>(c.b * 255);
		arrw[arrind + 3] = static_cast<uint8_t>(c.a * 255);
	}

	return arr;
}

ImageIndexer::ImageIndexer() {
}
ImageIndexer::~ImageIndexer() {
}

void ImageIndexer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_colors"), &ImageIndexer::get_colors);
	ClassDB::bind_method(D_METHOD("index_image", "image"), &ImageIndexer::index_image);
	ClassDB::bind_method(D_METHOD("reset"), &ImageIndexer::reset);
	ClassDB::bind_method(D_METHOD("indices_to_argb8_data", "indices"), &ImageIndexer::indices_to_argb8_data);
}
