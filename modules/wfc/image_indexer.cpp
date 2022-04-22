
#include "image_indexer.h"

#include "core/oa_hash_map.h"

PoolColorArray ImageIndexer::get_colors() {
	return _colors;
}
PoolIntArray ImageIndexer::get_color_indices() {
	return _color_indices;
}

void ImageIndexer::index_image(Ref<Image> image) {
	ERR_FAIL_COND(!image.is_valid());

	_colors.resize(0);
	_color_indices.resize(0);

	OAHashMap<Color, int> col_map;

	image->lock();

	int w = image->get_width();
	int h = image->get_height();

	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {
			Color c = image->get_pixel(x, y);

			int color_index = 0;
			if (!col_map.lookup(c, color_index)) {
				color_index = _colors.size();
				_colors.push_back(c);
				col_map.set(c, color_index);
			}

			_color_indices.push_back(color_index);
		}
	}

	image->unlock();
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
	ClassDB::bind_method(D_METHOD("get_color_indices"), &ImageIndexer::get_color_indices);
	ClassDB::bind_method(D_METHOD("index_image"), &ImageIndexer::index_image);
	ClassDB::bind_method(D_METHOD("indices_to_argb8_data"), &ImageIndexer::indices_to_argb8_data);
}
