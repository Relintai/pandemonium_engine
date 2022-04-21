#include "wave.h"

#include <limits>

namespace {

// Return distribution * log(distribution).
Vector<double> get_plogp(const Vector<double> &distribution) {
	Vector<double> plogp;

	for (int i = 0; i < distribution.size(); i++) {
		plogp.push_back(distribution[i] * log(distribution[i]));
	}

	return plogp;
}

// Return min(v) / 2.
double get_min_abs_half(const Vector<double> &v) {
	double min_abs_half = std::numeric_limits<double>::infinity();

	for (int i = 0; i < v.size(); i++) {
		min_abs_half = std::min(min_abs_half, std::abs(v[i] / 2.0));
	}

	return min_abs_half;
}

} // namespace

Wave::Wave(uint32_t height, uint32_t width,
		const Vector<double> &patterns_frequencies) :
		patterns_frequencies(patterns_frequencies),
		plogp_patterns_frequencies(get_plogp(patterns_frequencies)),
		min_abs_half_plogp(get_min_abs_half(plogp_patterns_frequencies)),
		is_impossible(false),
		nb_patterns(patterns_frequencies.size()),
		data(width * height, nb_patterns, 1),
		width(width),
		height(height),
		size(height * width) {

	// Initialize the memoisation of entropy.
	double base_entropy = 0;
	double base_s = 0;

	for (uint32_t i = 0; i < nb_patterns; i++) {
		base_entropy += plogp_patterns_frequencies[i];
		base_s += patterns_frequencies[i];
	}

	double log_base_s = log(base_s);
	double entropy_base = log_base_s - base_entropy / base_s;

	memoisation_plogp_sum.resize(width * height);
	memoisation_plogp_sum.fill(base_entropy);

	memoisation_sum.resize(width * height);
	memoisation_sum.fill(base_s);

	memoisation_log_sum.resize(width * height);
	memoisation_log_sum.fill(log_base_s);

	memoisation_nb_patterns.resize(width * height);
	memoisation_nb_patterns.fill(static_cast<uint32_t>(nb_patterns));

	memoisation_entropy.resize(width * height);
	memoisation_entropy.fill(entropy_base);
}

void Wave::set(uint32_t index, uint32_t pattern, bool value) {
	bool old_value = data.get(index, pattern);

	// If the value isn't changed, nothing needs to be done.
	if (old_value == value) {
		return;
	}

	// Otherwise, the memoisation should be updated.
	data.get(index, pattern) = value;
	memoisation_plogp_sum.write[index] -= plogp_patterns_frequencies[pattern];
	memoisation_sum.write[index] -= patterns_frequencies[pattern];
	memoisation_log_sum.write[index] = log(memoisation_sum[index]);
	memoisation_nb_patterns.write[index]--;
	memoisation_entropy.write[index] = memoisation_log_sum[index] - memoisation_plogp_sum[index] / memoisation_sum[index];

	// If there is no patterns possible in the cell, then there is a contradiction.
	if (memoisation_nb_patterns[index] == 0) {
		is_impossible = true;
	}
}

int Wave::get_min_entropy(std::minstd_rand &gen) const {
	if (is_impossible) {
		return -2;
	}

	std::uniform_real_distribution<> dis(0, min_abs_half_plogp);

	// The minimum entropy (plus a small noise)
	double min = std::numeric_limits<double>::infinity();
	int argmin = -1;

	for (uint32_t i = 0; i < size; i++) {
		// If the cell is decided, we do not compute the entropy (which is equal
		// to 0).
		double nb_patterns_local = memoisation_nb_patterns[i];
		
		if (nb_patterns_local == 1) {
			continue;
		}

		// Otherwise, we take the memoised entropy.
		double entropy = memoisation_entropy[i];

		// We first check if the entropy is less than the minimum.
		// This is important to reduce noise computation (which is not
		// negligible).
		if (entropy <= min) {
			// Then, we add noise to decide randomly which will be chosen.
			// noise is smaller than the smallest p * log(p), so the minimum entropy
			// will always be chosen.
			double noise = dis(gen);
			if (entropy + noise < min) {
				min = entropy + noise;
				argmin = i;
			}
		}
	}

	return argmin;
}
