#ifndef FAST_WFC_UTILS_ARRAY3D_HPP_
#define FAST_WFC_UTILS_ARRAY3D_HPP_

#include "core/containers/vector.h"

template <typename T>
class Array3D {
public:
	int height;
	int width;
	int depth;

	Vector<T> data;

	Array3D() {
		height = 0;
		width = 0;
		depth = 0;
	}

	Array3D(int p_height, int p_width, int p_depth) {
		height = p_height;
		width = p_width;
		depth = p_depth;
		data.resize(width * height * depth);
	}

	Array3D(int p_height, int p_width, int p_depth, T value) {
		height = p_height;
		width = p_width;
		depth = p_depth;
		data.resize(width * height * depth);
		data.fill(value);
	}

	void resize(int p_height, int p_width, int p_depth) {
		height = p_height;
		width = p_width;
		depth = p_depth;
		data.resize(width * height * depth);
	}

	void resize_fill(int p_height, int p_width, int p_depth, T value) {
		height = p_height;
		width = p_width;
		depth = p_depth;
		data.resize(width * height * depth);
		data.fill(value);
	}

	const T &get(int i, int j, int k) const {
		CRASH_BAD_INDEX(i, height);
		CRASH_BAD_INDEX(j, width);
		CRASH_BAD_INDEX(k, depth);

		return data[i * width * depth + j * depth + k];
	}

	T &get(int i, int j, int k) {
		CRASH_BAD_INDEX(i, height);
		CRASH_BAD_INDEX(j, width);
		CRASH_BAD_INDEX(k, depth);

		return data.write[i * width * depth + j * depth + k];
	}

	bool operator==(const Array3D &a) const {
		if (height != a.height || width != a.width || depth != a.depth) {
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

#endif // FAST_WFC_UTILS_ARRAY3D_HPP_
