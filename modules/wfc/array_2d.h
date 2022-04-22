#ifndef FAST_WFC_UTILS_ARRAY2D_HPP_
#define FAST_WFC_UTILS_ARRAY2D_HPP_

#include "core/vector.h"

template <typename T>
class Array2D {
public:
	int height;
	int width;

	Vector<T> data;

	Array2D() {
		height = 0;
		width = 0;
	}

	Array2D(int p_height, int p_width) {
		height = p_height;
		width = p_width;
		data.resize(width * height);
	}

	Array2D(int p_height, int p_width, T p_value) {
		height = p_height;
		width = p_width;
		data.resize(width * height);
		data.fill(p_value);
	}

	void resize(int p_height, int p_width) {
		height = p_height;
		width = p_width;
		data.resize(width * height);
	}

	void resize_fill(int p_height, int p_width, T p_value) {
		height = p_height;
		width = p_width;
		data.resize(width * height);
		data.fill(p_value);
	}

	const T &get(int i, int j) const {
		CRASH_BAD_INDEX(i, height);
		CRASH_BAD_INDEX(j, width);

		return data[j + i * width];
	}

	T &get(int i, int j) {
		CRASH_BAD_INDEX(i, height);
		CRASH_BAD_INDEX(j, width);

		return data.write[j + i * width];
	}

	Array2D<T> reflected() const {
		Array2D<T> result = Array2D<T>(width, height);
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				result.get(y, x) = get(y, width - 1 - x);
			}
		}
		return result;
	}

	Array2D<T> rotated() const {
		Array2D<T> result = Array2D<T>(width, height);
		for (int y = 0; y < width; y++) {
			for (int x = 0; x < height; x++) {
				result.get(y, x) = get(x, width - 1 - y);
			}
		}
		return result;
	}

	Array2D<T> get_sub_array(int y, int x, int sub_width, int sub_height) const {
		Array2D<T> sub_array_2d = Array2D<T>(sub_width, sub_height);
		for (int ki = 0; ki < sub_height; ki++) {
			for (int kj = 0; kj < sub_width; kj++) {
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
