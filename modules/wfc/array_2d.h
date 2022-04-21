#ifndef FAST_WFC_UTILS_ARRAY2D_HPP_
#define FAST_WFC_UTILS_ARRAY2D_HPP_

#include "assert.h"
#include <vector>

template <typename T>
class Array2D {
public:
	std::size_t height;
	std::size_t width;

	std::vector<T> data;

	Array2D(std::size_t p_height, std::size_t p_width) {
		height = p_height;
		width = p_width;
		data.resize(width * height);
	}

	Array2D(std::size_t p_height, std::size_t p_width, T p_value) {
		height = p_height;
		width = p_width;
		data.resize(width * height, p_value);
	}

	const T &get(std::size_t i, std::size_t j) const {
		assert(i < height && j < width);
		return data[j + i * width];
	}

	T &get(std::size_t i, std::size_t j) {
		assert(i < height && j < width);
		return data[j + i * width];
	}

	Array2D<T> reflected() const {
		Array2D<T> result = Array2D<T>(width, height);
		for (std::size_t y = 0; y < height; y++) {
			for (std::size_t x = 0; x < width; x++) {
				result.get(y, x) = get(y, width - 1 - x);
			}
		}
		return result;
	}

	Array2D<T> rotated() const {
		Array2D<T> result = Array2D<T>(width, height);
		for (std::size_t y = 0; y < width; y++) {
			for (std::size_t x = 0; x < height; x++) {
				result.get(y, x) = get(x, width - 1 - y);
			}
		}
		return result;
	}

	Array2D<T> get_sub_array(std::size_t y, std::size_t x, std::size_t sub_width, std::size_t sub_height) const {
		Array2D<T> sub_array_2d = Array2D<T>(sub_width, sub_height);
		for (std::size_t ki = 0; ki < sub_height; ki++) {
			for (std::size_t kj = 0; kj < sub_width; kj++) {
				sub_array_2d.get(ki, kj) = get((y + ki) % height, (x + kj) % width);
			}
		}
		return sub_array_2d;
	}

	bool operator==(const Array2D<T> &a) const {
		if (height != a.height || width != a.width) {
			return false;
		}

		for (std::size_t i = 0; i < data.size(); i++) {
			if (a.data[i] != data[i]) {
				return false;
			}
		}
		return true;
	}
};

namespace std {
template <typename T>
class hash<Array2D<T>> {
public:
	std::size_t operator()(const Array2D<T> &a) const {
		std::size_t seed = a.data.size();
		for (const T &i : a.data) {
			seed ^= hash<T>()(i) + (std::size_t)0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};
} //namespace std

#endif
