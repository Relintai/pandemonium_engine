#include "wave_form_collapse.h"

#include <limits>

namespace {
// Normalize a vector so the sum of its elements is equal to 1.0f
Vector<double> &normalize(Vector<double> &v) {
	double sum_weights = 0.0;
	int size = v.size();
	const double *vpr = v.ptr();
	for (int i = 0; i < size; ++i) {
		sum_weights += vpr[i];
	}

	double *vpw = v.ptrw();
	double inv_sum_weights = 1.0 / sum_weights;
	for (int i = 0; i < size; ++i) {
		vpw[i] *= inv_sum_weights;
	}

	return v;
}
} //namespace

Array2D<uint32_t> WaveFormCollapse::wave_to_output() const {
	Array2D<uint32_t> output_patterns(wave.height, wave.width);

	for (uint32_t i = 0; i < wave.size; i++) {
		for (uint32_t k = 0; k < nb_patterns; k++) {
			if (wave.get(i, k)) {
				output_patterns.data.write[i] = k;
			}
		}
	}
	return output_patterns;
}

//WaveFormCollapse::WaveFormCollapse() {
//}

WaveFormCollapse::WaveFormCollapse(bool periodic_output, int seed,
		Vector<double> patterns_frequencies,
		Vector<Propagator::PropagatorEntry> propagator, uint32_t wave_height, uint32_t wave_width) :
		gen(seed), patterns_frequencies(normalize(patterns_frequencies)), wave(wave_height, wave_width, patterns_frequencies), nb_patterns(propagator.size()), propagator(wave.height, wave.width, periodic_output, propagator) {
}

Array2D<uint32_t> WaveFormCollapse::run() {
	while (true) {
		// Define the value of an undefined cell.
		ObserveStatus result = observe();

		// Check if the algorithm has terminated.
		if (result == OBSERVE_STATUS_FAILURE) {
			return Array2D<uint32_t>(0, 0);
		} else if (result == OBSERVE_STATUS_FAILURE) {
			return wave_to_output();
		}

		// Propagate the information.
		propagator.propagate(wave);
	}
}

WaveFormCollapse::ObserveStatus WaveFormCollapse::observe() {
	// Get the cell with lowest entropy.
	int argmin = wave.get_min_entropy(gen);

	// If there is a contradiction, the algorithm has failed.
	if (argmin == -2) {
		return OBSERVE_STATUS_FAILURE;
	}

	// If the lowest entropy is 0, then the algorithm has succeeded and
	// finished.
	if (argmin == -1) {
		wave_to_output();
		return OBSERVE_STATUS_SUCCESS;
	}

	// Choose an element according to the pattern distribution
	double s = 0;
	for (uint32_t k = 0; k < nb_patterns; k++) {
		s += wave.get(argmin, k) ? patterns_frequencies[k] : 0;
	}

	std::uniform_real_distribution<> dis(0, s);
	double random_value = dis(gen);
	size_t chosen_value = nb_patterns - 1;

	for (uint32_t k = 0; k < nb_patterns; k++) {
		random_value -= wave.get(argmin, k) ? patterns_frequencies[k] : 0;
		if (random_value <= 0) {
			chosen_value = k;
			break;
		}
	}

	// And define the cell with the pattern.
	for (uint32_t k = 0; k < nb_patterns; k++) {
		if (wave.get(argmin, k) != (k == chosen_value)) {
			propagator.add_to_propagator(argmin / wave.width, argmin % wave.width, k);
			wave.set(argmin, k, false);
		}
	}

	return OBSERVE_STATUS_TO_CONTINUE;
}

void WaveFormCollapse::bind_methods() {
}