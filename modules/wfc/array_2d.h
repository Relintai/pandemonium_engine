#ifndef FAST_WFC_UTILS_ARRAY2D_HPP_
#define FAST_WFC_UTILS_ARRAY2D_HPP_

#include "core/vector.h"

template <typename T>
class Array2D {
public:
	uint32_t height;
	uint32_t width;

	Vector<T> data;

	Array2D(uint32_t p_height, uint32_t p_width) {
		height = p_height;
		width = p_width;
		data.resize(width * height);
	}

	Array2D(uint32_t p_height, uint32_t p_width, T p_value) {
		height = p_height;
		width = p_width;
		data.resize(width * height);
		data.fill(p_value);
	}

	const T &get(uint32_t i, uint32_t j) const {
		CRASH_BAD_INDEX(i, height);
		CRASH_BAD_INDEX(j, width);

		return data[j + i * width];
	}

	T &get(uint32_t i, uint32_t j) {
		CRASH_BAD_INDEX(i, height);
		CRASH_BAD_INDEX(j, width);
		
		return data.write[j + i * width];
	}

	Array2D<T> reflected() const {
		Array2D<T> result = Array2D<T>(width, height);
		for (uint32_t y = 0; y < height; y++) {
			for (uint32_t x = 0; x < width; x++) {
				result.get(y, x) = get(y, width - 1 - x);
			}
		}
		return result;
	}

	Array2D<T> rotated() const {
		Array2D<T> result = Array2D<T>(width, height);
		for (uint32_t y = 0; y < width; y++) {
			for (uint32_t x = 0; x < height; x++) {
				result.get(y, x) = get(x, width - 1 - y);
			}
		}
		return result;
	}

	Array2D<T> get_sub_array(uint32_t y, uint32_t x, uint32_t sub_width, uint32_t sub_height) const {
		Array2D<T> sub_array_2d = Array2D<T>(sub_width, sub_height);
		for (uint32_t ki = 0; ki < sub_height; ki++) {
			for (uint32_t kj = 0; kj < sub_width; kj++) {
				sub_array_2d.get(ki, kj) = get((y + ki) % height, (x + kj) % width);
			}
		}
		return sub_array_2d;
	}

	bool operator==(const Array2D<T> &a) const {
		if (height != a.height || width != a.width) {
			return false;
		}

		for (int i = 0; i < data.size(); i++) {
			if (a.data[i] != data[i]) {
				return false;
			}
		}
		return true;
	}
};

#endif
