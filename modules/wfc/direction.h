#ifndef FAST_WFC_DIRECTION_HPP_
#define FAST_WFC_DIRECTION_HPP_

constexpr int directions_x[4] = { 0, -1, 1, 0 };
constexpr int directions_y[4] = { -1, 0, 0, 1 };

constexpr unsigned get_opposite_direction(unsigned direction) {
	return 3 - direction;
}

#endif
