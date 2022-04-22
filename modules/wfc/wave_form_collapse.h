#ifndef WAVE_FORM_COLLAPSE_H
#define WAVE_FORM_COLLAPSE_H

#include "core/int_types.h"
#include "core/math/random_pcg.h"

#include "array_2d.h"
#include "array_3d.h"
#include "core/vector.h"

#include "core/reference.h"

class WaveFormCollapse : public Reference {
	GDCLASS(WaveFormCollapse, Reference);

public:
	enum ObserveStatus {
		OBSERVE_STATUS_SUCCESS,
		OBSERVE_STATUS_FAILURE,
		OBSERVE_STATUS_TO_CONTINUE
	};

	struct PropagatorStateEntry {
		Vector<uint32_t> directions[4];
	};

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

	struct CompatibilityEntry {
		int direction[4];

		CompatibilityEntry() {
			for (int i = 0; i < 4; ++i) {
				direction[i] = 0;
			}
		}
	};

	static constexpr int directions_x[4] = { 0, -1, 1, 0 };
	static constexpr int directions_y[4] = { -1, 0, 0, 1 };

public:
	uint32_t get_width() const;
	uint32_t get_height() const;

	bool get_periodic_output() const;
	void set_periodic_output(const bool val);

	void set_seed(const int seed);

	void set_size(uint32_t p_width, uint32_t p_height);

	void set_propagator_state(const Vector<PropagatorStateEntry> &p_propagator_state);
	void set_pattern_frequencies(const Vector<double> &p_patterns_frequencies, const bool p_normalize = true);

	Array2D<uint32_t> run();

	ObserveStatus observe();

	//dvoid propagate() { propagator.propagate(wave); }

	void remove_wave_pattern(uint32_t i, uint32_t j, uint32_t pattern) {
		if (wave_get(i, j, pattern)) {
			wave_set(i, j, pattern, false);
			add_to_propagator(i, j, pattern);
		}
	}

	// Return true if pattern can be placed in cell index.
	bool wave_get(uint32_t index, uint32_t pattern) const {
		return data.get(index, pattern);
	}

	// Return true if pattern can be placed in cell (i,j)
	bool wave_get(uint32_t i, uint32_t j, uint32_t pattern) const {
		return wave_get(i * wave_width + j, pattern);
	}

	// Set the value of pattern in cell index.
	void wave_set(uint32_t index, uint32_t pattern, bool value);

	// Set the value of pattern in cell (i,j).
	void wave_set(uint32_t i, uint32_t j, uint32_t pattern, bool value) {
		wave_set(i * wave_width + j, pattern, value);
	}

	// Return the index of the cell with lowest entropy different of 0.
	// If there is a contradiction in the wave, return -2.
	// If every cell is decided, return -1.
	int wave_get_min_entropy() const;

	void add_to_propagator(uint32_t y, uint32_t x, uint32_t pattern) {
		// All the direction are set to 0, since the pattern cannot be set in (y,x).
		CompatibilityEntry temp;
		compatible.get(y, x, pattern) = temp;

		propagating.push_back(PropagatingEntry(y, x, pattern));
	}

	constexpr uint32_t get_opposite_direction(uint32_t direction) {
		return 3 - direction;
	}

	void normalize(Vector<double> &v);
	Vector<double> get_plogp(const Vector<double> &distribution);
	double get_min_abs_half(const Vector<double> &v);

	void propagate();

	virtual void initialize();

	WaveFormCollapse();
	~WaveFormCollapse();

protected:
	static void bind_methods();

private:
	RandomPCG gen;

	// The number of distinct patterns.
	size_t nb_patterns;

	// Transform the wave to a valid output (a 2d array of patterns that aren't in
	// contradiction). This function should be used only when all cell of the wave
	// are defined.
	Array2D<uint32_t> wave_to_output() const;

	//Wave
	uint32_t wave_width;
	uint32_t wave_height;
	uint32_t wave_size;

	// The patterns frequencies p given to wfc.
	Vector<double> patterns_frequencies;

	// The precomputation of p * log(p).
	Vector<double> plogp_patterns_frequencies;

	// The precomputation of min (p * log(p)) / 2.
	// This is used to define the maximum value of the noise.
	double min_abs_half_plogp;

	Vector<double> memoisation_plogp_sum; // The sum of p'(pattern)// log(p'(pattern)).
	Vector<double> memoisation_sum; // The sum of p'(pattern).
	Vector<double> memoisation_log_sum; // The log of sum.
	Vector<uint32_t> memoisation_nb_patterns; // The number of patterns present
	Vector<double> memoisation_entropy; // The entropy of the cell.

	// This value is set to true if there is a contradiction in the wave (all elements set to false in a cell).
	bool is_impossible;

	// The actual wave. data.get(index, pattern) is equal to 0 if the pattern can
	// be placed in the cell index.
	Array2D<uint8_t> data;

	//Propagator
	Vector<PropagatorStateEntry> propagator_state;

	bool periodic_output;

	// All the tuples (y, x, pattern) that should be propagated.
	// The tuple should be propagated when wave.get(y, x, pattern) is set to false.
	Vector<PropagatingEntry> propagating;

	// compatible.get(y, x, pattern)[direction] contains the number of patterns
	// present in the wave that can be placed in the cell next to (y,x) in the
	// opposite direction of direction without being in contradiction with pattern
	// placed in (y,x). If wave.get(y, x, pattern) is set to false, then
	// compatible.get(y, x, pattern) has every element negative or null
	Array3D<CompatibilityEntry> compatible;

	void init_compatible();
};

#endif
