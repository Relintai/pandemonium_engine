#ifndef FAST_WFC_UTILS_ARRAY3D_HPP_
#define FAST_WFC_UTILS_ARRAY3D_HPP_

#include "core/vector.h"

template <typename T>
class Array3D {
public:
	uint32_t height;
	uint32_t width;
	uint32_t depth;

	Vector<T> data;

	Array3D(uint32_t p_height, uint32_t p_width, uint32_t p_depth) {
		height = p_height;
		width = p_width;
		depth = p_depth;
		data.resize(width * height * depth);
	}

	Array3D(uint32_t p_height, uint32_t p_width, uint32_t p_depth, T value) {
		height = p_height;
		width = p_width;
		depth = p_depth;
		data.resize(width * height * depth);
		data.fill(value);
	}

	const T &get(uint32_t i, uint32_t j, uint32_t k) const {
		CRASH_BAD_INDEX(i, height);
		CRASH_BAD_INDEX(j, width);
		CRASH_BAD_INDEX(k, depth);

		return data[i * width * depth + j * depth + k];
	}

	T &get(uint32_t i, uint32_t j, uint32_t k) {
		CRASH_BAD_INDEX(i, height);
		CRASH_BAD_INDEX(j, width);
		CRASH_BAD_INDEX(k, depth);

		return data.write[i * width * depth + j * depth + k];
	}

	bool operator==(const Array3D &a) const {
		if (height != a.height || width != a.width || depth != a.depth) {
			return false;
		}

		for (uint32_t i = 0; i < data.size(); i++) {
			if (a.data[i] != data[i]) {
				return false;
			}
		}
		return true;
	}
};

#endif // FAST_WFC_UTILS_ARRAY3D_HPP_
