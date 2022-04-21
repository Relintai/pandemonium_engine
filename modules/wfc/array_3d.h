#ifndef FAST_WFC_UTILS_ARRAY3D_HPP_
#define FAST_WFC_UTILS_ARRAY3D_HPP_

#include "assert.h"
#include "core/vector.h"

template <typename T>
class Array3D {
public:
	std::size_t height;
	std::size_t width;
	std::size_t depth;

	Vector<T> data;

	Array3D(std::size_t p_height, std::size_t p_width, std::size_t p_depth) {
		height = p_height;
		width = p_width;
		depth = p_depth;
		data.resize(width * height * depth);
	}

	Array3D(std::size_t p_height, std::size_t p_width, std::size_t p_depth, T value) {
		height = p_height;
		width = p_width;
		depth = p_depth;
		data.resize(width * height * depth);
		data.fill(value);
	}

	const T &get(std::size_t i, std::size_t j, std::size_t k) const {
		assert(i < height && j < width && k < depth);
		return data[i * width * depth + j * depth + k];
	}

	T &get(std::size_t i, std::size_t j, std::size_t k) {
		return data.write[i * width * depth + j * depth + k];
	}

	bool operator==(const Array3D &a) const {
		if (height != a.height || width != a.width || depth != a.depth) {
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

#endif // FAST_WFC_UTILS_ARRAY3D_HPP_
