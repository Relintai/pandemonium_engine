#ifndef FAST_WFC_DIRECTION_HPP_
#define FAST_WFC_DIRECTION_HPP_

constexpr int directions_x[4] = { 0, -1, 1, 0 };
constexpr int directions_y[4] = { -1, 0, 0, 1 };

constexpr uint32_t get_opposite_direction(uint32_t direction) {
	return 3 - direction;
}

#endif
