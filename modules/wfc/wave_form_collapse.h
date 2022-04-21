#ifndef WAVE_FORM_COLLAPSE_H
#define WAVE_FORM_COLLAPSE_H

#include "core/reference.h"

#include <optional>
#include <random>

#include "array_2d.h"
#include "propagator.h"
#include "wave.h"

class WaveFormCollapse : public Reference {
	GDCLASS(WaveFormCollapse, Reference);

public:
	enum ObserveStatus {
		OBSERVE_STATUS_SUCCESS,
		OBSERVE_STATUS_FAILURE,
		OBSERVE_STATUS_TO_CONTINUE
	};

	Array2D<uint32_t> run();

	ObserveStatus observe();

	void propagate() { propagator.propagate(wave); }

	void remove_wave_pattern(uint32_t i, uint32_t j, uint32_t pattern) {
		if (wave.get(i, j, pattern)) {
			wave.set(i, j, pattern, false);
			propagator.add_to_propagator(i, j, pattern);
		}
	}

	WaveFormCollapse();
	WaveFormCollapse(bool periodic_output, int seed, Vector<double> patterns_frequencies,
			Vector<Propagator::PropagatorEntry> propagator, uint32_t wave_height,
			uint32_t wave_width);

protected:
	static void bind_methods();

private:
	std::minstd_rand gen;

	const Vector<double> patterns_frequencies;

	Wave wave;

	// The number of distinct patterns.
	const size_t nb_patterns;

	Propagator propagator;

	// Transform the wave to a valid output (a 2d array of patterns that aren't in
	// contradiction). This function should be used only when all cell of the wave
	// are defined.
	Array2D<uint32_t> wave_to_output() const;
};

#endif
