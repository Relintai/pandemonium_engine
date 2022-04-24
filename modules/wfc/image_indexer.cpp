
#include "image_indexer.h"

PoolColorArray ImageIndexer::get_colors() {
	return _colors;
}

PoolIntArray ImageIndexer::index_image(Ref<Image> image) {
	if(!image.is_valid()) {
		return  PoolIntArray();
	}

	_colors.resize(0);
	PoolIntArray color_indices;

	image->lock();

	int w = image->get_width();
	int h = image->get_height();

	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {
			Color c = image->get_pixel(x, y);

			int color_index = 0;
			if (!_col_map.lookup(c, color_index)) {
				color_index = _colors.size();
				_colors.push_back(c);
				_col_map.set(c, color_index);
			}

			color_indices.push_back(color_index);
		}
	}

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
