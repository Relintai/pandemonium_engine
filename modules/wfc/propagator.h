#ifndef FAST_WFC_PROPAGATOR_HPP_
#define FAST_WFC_PROPAGATOR_HPP_

#include "array_3d.h"
#include "core/vector.h"
#include "direction.h"

class Wave;

class Propagator {
public:
	struct PropagatorEntry {
		Vector<uint32_t> directions[4];
	};

private:
	const uint32_t patterns_size;

	Vector<PropagatorEntry> propagator_state;

	const uint32_t wave_width;
	const uint32_t wave_height;

	const bool periodic_output;

	struct PropagatingEntry {
		uint32_t data[3];

		PropagatingEntry() {
			for (int i = 0; i < 3; ++i) {
				data[i] = 0;
			}
		}

		PropagatingEntry(uint32_t x, uint32_t y, uint32_t z) {
			data[0] = x;
			data[1] = y;
			data[2] = z;
		}
	};

	// All the tuples (y, x, pattern) that should be propagated.
	// The tuple should be propagated when wave.get(y, x, pattern) is set to
	// false.
	Vector<PropagatingEntry> propagating;

	struct CompatibilityEntry {
		int direction[4];

		CompatibilityEntry() {
			for (int i = 0; i < 4; ++i) {
				direction[i] = 0;
			}
		}
	};

	// compatible.get(y, x, pattern)[direction] contains the number of patterns
	// present in the wave that can be placed in the cell next to (y,x) in the
	// opposite direction of direction without being in contradiction with pattern
	// placed in (y,x). If wave.get(y, x, pattern) is set to false, then
	// compatible.get(y, x, pattern) has every element negative or null
	Array3D<CompatibilityEntry> compatible;

	void init_compatible();

public:
	Propagator(uint32_t wave_height, uint32_t wave_width, bool periodic_output, Vector<PropagatorEntry> propagator_state) :
			patterns_size(propagator_state.size()),
			propagator_state(propagator_state),
			wave_width(wave_width),
			wave_height(wave_height),
			periodic_output(periodic_output),
			compatible(wave_height, wave_width, patterns_size) {
		init_compatible();
	}

	void add_to_propagator(uint32_t y, uint32_t x, uint32_t pattern) {
		// All the direction are set to 0, since the pattern cannot be set in (y,x).
		CompatibilityEntry temp;
		compatible.get(y, x, pattern) = temp;

		propagating.push_back(PropagatingEntry(y, x, pattern));
	}

	void propagate(Wave &wave);
};

#endif // FAST_WFC_PROPAGATOR_HPP_
