#ifndef FAST_WFC_WAVE_HPP_
#define FAST_WFC_WAVE_HPP_

#include "array_2d.h"
#include <random>
#include "core/vector.h"

// Struct containing the values needed to compute the entropy of all the cells.
// This struct is updated every time the wave is changed.
// p'(pattern) is equal to patterns_frequencies[pattern] if wave.get(cell,
// pattern) is set to true, otherwise 0.
struct EntropyMemoisation {
	Vector<double> plogp_sum; // The sum of p'(pattern)// log(p'(pattern)).
	Vector<double> sum; // The sum of p'(pattern).
	Vector<double> log_sum; // The log of sum.
	Vector<uint32_t> nb_patterns; // The number of patterns present
	Vector<double> entropy; // The entropy of the cell.
};

// Contains the pattern possibilities in every cell.
// Also contains information about cell entropy.
class Wave {
private:
	// The patterns frequencies p given to wfc.
	const Vector<double> patterns_frequencies;

	// The precomputation of p * log(p).
	const Vector<double> plogp_patterns_frequencies;

	// The precomputation of min (p * log(p)) / 2.
	// This is used to define the maximum value of the noise.
	const double min_abs_half_plogp;

	EntropyMemoisation memoisation;

	// This value is set to true if there is a contradiction in the wave (all elements set to false in a cell).
	bool is_impossible;

	// The number of distinct patterns.
	const size_t nb_patterns;

	// The actual wave. data.get(index, pattern) is equal to 0 if the pattern can
	// be placed in the cell index.
	Array2D<uint8_t> data;

public:
	// The size of the wave.
	const uint32_t width;
	const uint32_t height;
	const uint32_t size;

	// Initialize the wave with every cell being able to have every pattern.
	Wave(uint32_t height, uint32_t width, const Vector<double> &patterns_frequencies);

	// Return true if pattern can be placed in cell index.
	bool get(uint32_t index, uint32_t pattern) const {
		return data.get(index, pattern);
	}

	// Return true if pattern can be placed in cell (i,j)
	bool get(uint32_t i, uint32_t j, uint32_t pattern) const {
		return get(i * width + j, pattern);
	}

	// Set the value of pattern in cell index.
	void set(uint32_t index, uint32_t pattern, bool value);

	// Set the value of pattern in cell (i,j).
	void set(uint32_t i, uint32_t j, uint32_t pattern, bool value) {
		set(i * width + j, pattern, value);
	}

	// Return the index of the cell with lowest entropy different of 0.
	// If there is a contradiction in the wave, return -2.
	// If every cell is decided, return -1.
	int get_min_entropy(std::minstd_rand &gen) const;
};

#endif // FAST_WFC_WAVE_HPP_
