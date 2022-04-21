#ifndef FAST_WFC_PROPAGATOR_HPP_
#define FAST_WFC_PROPAGATOR_HPP_

#include "array_3d.h"
#include "direction.h"
#include <array>
#include <tuple>
#include "core/vector.h"

class Wave;

class Propagator {
public:
	using PropagatorState = Vector<std::array<Vector<unsigned>, 4>>;

private:
	const std::size_t patterns_size;

	PropagatorState propagator_state;

	const unsigned wave_width;
	const unsigned wave_height;

	const bool periodic_output;

	// All the tuples (y, x, pattern) that should be propagated.
	// The tuple should be propagated when wave.get(y, x, pattern) is set to
	// false.
	Vector<std::tuple<unsigned, unsigned, unsigned>> propagating;

	// compatible.get(y, x, pattern)[direction] contains the number of patterns
	// present in the wave that can be placed in the cell next to (y,x) in the
	// opposite direction of direction without being in contradiction with pattern
	// placed in (y,x). If wave.get(y, x, pattern) is set to false, then
	// compatible.get(y, x, pattern) has every element negative or null
	Array3D<std::array<int, 4>> compatible;

	void init_compatible();

public:
	Propagator(unsigned wave_height, unsigned wave_width, bool periodic_output,
			PropagatorState propagator_state) :
			patterns_size(propagator_state.size()),
			propagator_state(propagator_state),
			wave_width(wave_width),
			wave_height(wave_height),
			periodic_output(periodic_output),
			compatible(wave_height, wave_width, patterns_size) {
		init_compatible();
	}

	void add_to_propagator(unsigned y, unsigned x, unsigned pattern) {
		// All the direction are set to 0, since the pattern cannot be set in (y,x).
		std::array<int, 4> temp = {};
		compatible.get(y, x, pattern) = temp;
		
		propagating.push_back(std::tuple<unsigned, unsigned, unsigned>(y, x, pattern));
	}

	void propagate(Wave &wave);
};

#endif // FAST_WFC_PROPAGATOR_HPP_
