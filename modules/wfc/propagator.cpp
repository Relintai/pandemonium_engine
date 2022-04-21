#include "propagator.h"
#include "wave.h"

void Propagator::init_compatible() {
	CompatibilityEntry value;
	// We compute the number of pattern compatible in all directions.
	for (uint32_t y = 0; y < wave_height; y++) {
		for (uint32_t x = 0; x < wave_width; x++) {
			for (uint32_t pattern = 0; pattern < patterns_size; pattern++) {
				
				for (int direction = 0; direction < 4; direction++) {
					value.direction[direction] = static_cast<uint32_t>(propagator_state[pattern].directions[get_opposite_direction(direction)].size());
				}

				compatible.get(y, x, pattern) = value;
			}
		}
	}
}

void Propagator::propagate(Wave &wave) {
	// We propagate every element while there is element to propagate.
	while (propagating.size() != 0) {
		// The cell and pattern that has been set to false.
		uint32_t y1, x1, pattern;
		std::tie(y1, x1, pattern) = propagating[propagating.size() - 1];
		propagating.resize(propagating.size() - 1);

		// We propagate the information in all 4 directions.
		for (uint32_t direction = 0; direction < 4; direction++) {
			// We get the next cell in the direction direction.
			int dx = directions_x[direction];
			int dy = directions_y[direction];
			int x2, y2;
			if (periodic_output) {
				x2 = ((int)x1 + dx + (int)wave.width) % wave.width;
				y2 = ((int)y1 + dy + (int)wave.height) % wave.height;
			} else {
				x2 = x1 + dx;
				y2 = y1 + dy;
				if (x2 < 0 || x2 >= (int)wave.width) {
					continue;
				}
				if (y2 < 0 || y2 >= (int)wave.height) {
					continue;
				}
			}

			// The index of the second cell, and the patterns compatible
			uint32_t i2 = x2 + y2 * wave.width;
			const Vector<uint32_t> &patterns = propagator_state[pattern].directions[direction];

			// For every pattern that could be placed in that cell without being in
			// contradiction with pattern1
			int size = patterns.size();
			for (int i = 0; i < size; ++i) {
				uint32_t pattern = patterns[i];
				
				// We decrease the number of compatible patterns in the opposite
				// direction If the pattern was discarded from the wave, the element
				// is still negative, which is not a problem
				CompatibilityEntry &value = compatible.get(y2, x2, pattern);
				value.direction[direction]--;

				// If the element was set to 0 with this operation, we need to remove
				// the pattern from the wave, and propagate the information
				if (value.direction[direction] == 0) {
					add_to_propagator(y2, x2, pattern);
					wave.set(i2, pattern, false);
				}
			}
		}
	}
}
