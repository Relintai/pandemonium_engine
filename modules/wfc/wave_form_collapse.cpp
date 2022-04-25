#include "wave_form_collapse.h"

const int WaveFormCollapse::DIRECTIONS_X[4] = { 0, -1, 1, 0 };
const int WaveFormCollapse::DIRECTIONS_Y[4] = { -1, 0, 0, 1 };

// Normalize a vector so the sum of its elements is equal to 1.0f
void WaveFormCollapse::normalize(Vector<double> &v) {
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
}

// Return distribution * log(distribution).
Vector<double> WaveFormCollapse::get_plogp(const Vector<double> &distribution) {
	Vector<double> plogp;

	for (int i = 0; i < distribution.size(); i++) {
		plogp.push_back(distribution[i] * log(distribution[i]));
	}

	return plogp;
}

// Return min(v) / 2.
double WaveFormCollapse::get_min_abs_half(const Vector<double> &v) {
	double min_abs_half = Math_INF;

	for (int i = 0; i < v.size(); i++) {
		min_abs_half = MIN(min_abs_half, ABS(v[i] / 2.0));
	}

	return min_abs_half;
}

int WaveFormCollapse::get_wave_width() const {
	return _wave_width;
}
void WaveFormCollapse::set_wave_width(const int val) {
	_wave_width = val;
}

int WaveFormCollapse::get_wave_height() const {
	return _wave_height;
}
void WaveFormCollapse::set_wave_height(const int val) {
	_wave_height = val;
}

bool WaveFormCollapse::get_periodic_output() const {
	return _periodic_output;
}
void WaveFormCollapse::set_periodic_output(const bool val) {
	_periodic_output = val;
}

void WaveFormCollapse::set_seed(const int seed) {
	_gen.seed(seed);
}

void WaveFormCollapse::set_wave_size(int p_width, int p_height) {
	_wave_width = p_width;
	_wave_height = p_height;
	_wave_size = p_height * p_width;
}

void WaveFormCollapse::init_wave() {
	_wave_size = _wave_height * _wave_width;
}

void WaveFormCollapse::set_propagator_state(const Vector<PropagatorStateEntry> &p_propagator_state) {
	_propagator_state = p_propagator_state;
}

void WaveFormCollapse::set_pattern_frequencies(const Vector<double> &p_patterns_frequencies, const bool p_normalize) {
	_patterns_frequencies = p_patterns_frequencies;

	if (p_normalize) {
		normalize(_patterns_frequencies);
	}
}

void WaveFormCollapse::set_input(const PoolIntArray &p_data, int p_width, int p_height) {
	_input.resize(p_height, p_width);

	ERR_FAIL_COND(_input.data.size() != p_data.size());

	int *w = _input.data.ptrw();
	int s = _input.data.size();

	PoolIntArray::Read r = p_data.read();

	for (int i = 0; i < s; ++i) {
		w[i] = r[i];
	}
}

Array2D<int> WaveFormCollapse::run() {
	while (true) {
		// Define the value of an undefined cell.
		ObserveStatus result = observe();

		// Check if the algorithm has terminated.
		if (result == OBSERVE_STATUS_FAILURE) {
			return Array2D<int>(0, 0);
		} else if (result == OBSERVE_STATUS_SUCCESS) {
			return wave_to_output();
		}

		propagate();
	}
}

PoolIntArray WaveFormCollapse::generate_image_index_data() {
	PoolIntArray arr;

	Array2D<int> a = run();

	if (a.width == 0 && a.height == 0) {
		return arr;
	}

	const int *r = a.data.ptr();
	int s = a.data.size();

	arr.resize(s);
	PoolIntArray::Write w = arr.write();

	for (int i = 0; i < s; ++i) {
		w[i] = r[i];
	}

	w.release();

	return arr;
}

WaveFormCollapse::ObserveStatus WaveFormCollapse::observe() {
	// Get the cell with lowest entropy.
	int argmin = wave_get_min_entropy();

	// If there is a contradiction, the algorithm has failed.
	if (argmin == -2) {
		return OBSERVE_STATUS_FAILURE;
	}

	// If the lowest entropy is 0, then the algorithm has succeeded and finished.
	if (argmin == -1) {
		wave_to_output();
		return OBSERVE_STATUS_SUCCESS;
	}

	// Choose an element according to the pattern distribution
	double s = 0;
	for (int k = 0; k < _patterns_frequencies.size(); k++) {
		s += wave_get(argmin, k) ? _patterns_frequencies[k] : 0;
	}

	double random_value = _gen.random(0.0, s);

	int chosen_value = _patterns_frequencies.size() - 1;

	for (int k = 0; k < _patterns_frequencies.size(); k++) {
		random_value -= wave_get(argmin, k) ? _patterns_frequencies[k] : 0;
		if (random_value <= 0) {
			chosen_value = k;
			break;
		}
	}

	// And define the cell with the pattern.
	for (int k = 0; k < _patterns_frequencies.size(); k++) {
		if (wave_get(argmin, k) != (k == chosen_value)) {
			add_to_propagator(argmin / _wave_width, argmin % _wave_width, k);
			wave_set(argmin, k, false);
		}
	}

	return OBSERVE_STATUS_TO_CONTINUE;
}

Array2D<int> WaveFormCollapse::wave_to_output() const {
	Array2D<int> output_patterns(_wave_height, _wave_width);

	for (int i = 0; i < _wave_size; i++) {
		for (int k = 0; k < _patterns_frequencies.size(); k++) {
			if (wave_get(i, k)) {
				output_patterns.data.write[i] = k;
			}
		}
	}

	return output_patterns;
}

void WaveFormCollapse::wave_set(int index, int pattern, bool value) {
	bool old_value = _wave_data.get(index, pattern);

	// If the value isn't changed, nothing needs to be done.
	if (old_value == value) {
		return;
	}

	// Otherwise, the memoisation should be updated.
	_wave_data.get(index, pattern) = value;

	_memoisation_plogp_sum.write[index] -= _plogp_patterns_frequencies[pattern];
	_memoisation_sum.write[index] -= _patterns_frequencies[pattern];
	_memoisation_log_sum.write[index] = Math::log(_memoisation_sum[index]);
	_memoisation_nb_patterns.write[index]--;
	_memoisation_entropy.write[index] = _memoisation_log_sum[index] - _memoisation_plogp_sum[index] / _memoisation_sum[index];

	// If there is no patterns possible in the cell, then there is a contradiction.
	if (_memoisation_nb_patterns[index] == 0) {
		_is_impossible = true;
	}
}

int WaveFormCollapse::wave_get_min_entropy() const {
	if (_is_impossible) {
		return -2;
	}

	RandomPCG pcg;

	// The minimum entropy (plus a small noise)
	double min = Math_INF;

	int argmin = -1;

	for (int i = 0; i < _wave_size; i++) {
		// If the cell is decided, we do not compute the entropy (which is equal to 0).
		int nb_patterns_local = _memoisation_nb_patterns[i];

		if (nb_patterns_local == 1) {
			continue;
		}

		// Otherwise, we take the memoised entropy.
		double entropy = _memoisation_entropy[i];

		// We first check if the entropy is less than the minimum.
		// This is important to reduce noise computation (which is not
		// negligible).
		if (entropy <= min) {
			// Then, we add noise to decide randomly which will be chosen.
			// noise is smaller than the smallest p * log(p), so the minimum entropy
			// will always be chosen.
			double noise = pcg.random(0.0, _min_abs_half_plogp);
			if (entropy + noise < min) {
				min = entropy + noise;
				argmin = i;
			}
		}
	}

	return argmin;
}

void WaveFormCollapse::init_compatible() {
	// We compute the number of pattern compatible in all directions.
	for (int y = 0; y < _wave_height; y++) {
		for (int x = 0; x < _wave_width; x++) {
			for (int pattern = 0; pattern < _propagator_state.size(); pattern++) {
				CompatibilityEntry &value = _compatible.get(y, x, pattern);

				for (int direction = 0; direction < 4; direction++) {
					value.direction[direction] = _propagator_state[pattern].directions[get_opposite_direction(direction)].size();
				}
			}
		}
	}
}

void WaveFormCollapse::propagate() {
	// We propagate every element while there is element to propagate.
	while (_propagating.size() != 0) {
		// The cell and pattern that has been set to false.

		const PropagatingEntry &e = _propagating[_propagating.size() - 1];

		int y1 = e.data[0];
		int x1 = e.data[1];
		int pattern = e.data[2];

		_propagating.resize(_propagating.size() - 1);

		// We propagate the information in all 4 directions.
		for (int direction = 0; direction < 4; direction++) {
			// We get the next cell in the direction direction.
			int dx = DIRECTIONS_X[direction];
			int dy = DIRECTIONS_Y[direction];
			int x2, y2;

			if (_periodic_output) {
				x2 = ((int)x1 + dx + _wave_width) % _wave_width;
				y2 = ((int)y1 + dy + _wave_height) % _wave_height;
			} else {
				x2 = x1 + dx;
				y2 = y1 + dy;

				if (x2 < 0 || x2 >= _wave_width) {
					continue;
				}

				if (y2 < 0 || y2 >= _wave_height) {
					continue;
				}
			}

			// The index of the second cell, and the patterns compatible
			int i2 = x2 + y2 * _wave_width;
			const Vector<int> &patterns = _propagator_state[pattern].directions[direction];

			// For every pattern that could be placed in that cell without being in
			// contradiction with pattern1
			int size = patterns.size();
			for (int i = 0; i < size; ++i) {
				int pattern_entry = patterns[i];

				// We decrease the number of compatible patterns in the opposite
				// direction If the pattern was discarded from the wave, the element
				// is still negative, which is not a problem
				CompatibilityEntry &value = _compatible.get(y2, x2, pattern_entry);
				value.direction[direction]--;

				// If the element was set to 0 with this operation, we need to remove
				// the pattern from the wave, and propagate the information
				if (value.direction[direction] == 0) {
					add_to_propagator(y2, x2, pattern_entry);
					wave_set(i2, pattern_entry, false);
				}
			}
		}
	}
}

void WaveFormCollapse::initialize() {
	//wave
	init_wave();

	_plogp_patterns_frequencies = get_plogp(_patterns_frequencies);
	_min_abs_half_plogp = get_min_abs_half(_plogp_patterns_frequencies);

	_is_impossible = false;
	_nb_patterns = _patterns_frequencies.size();

	_wave_data.resize_fill(_wave_size, _nb_patterns, true);

	// Initialize the memoisation of entropy.
	double base_entropy = 0;
	double base_s = 0;

	for (int i = 0; i < _patterns_frequencies.size(); i++) {
		base_entropy += _plogp_patterns_frequencies[i];
		base_s += _patterns_frequencies[i];
	}

	double log_base_s = Math::log(base_s);
	double entropy_base = log_base_s - base_entropy / base_s;

	_memoisation_plogp_sum.resize(_wave_size);
	_memoisation_plogp_sum.fill(base_entropy);

	_memoisation_sum.resize(_wave_size);
	_memoisation_sum.fill(base_s);

	_memoisation_log_sum.resize(_wave_size);
	_memoisation_log_sum.fill(log_base_s);

	_memoisation_nb_patterns.resize(_wave_size);
	_memoisation_nb_patterns.fill(_patterns_frequencies.size());

	_memoisation_entropy.resize(_wave_size);
	_memoisation_entropy.fill(entropy_base);

	//propagator
	_compatible.resize(_wave_height, _wave_width, _propagator_state.size());
	init_compatible();
}

WaveFormCollapse::WaveFormCollapse() {
	_periodic_output = true;
	_is_impossible = false;

	_nb_patterns = 0;

	_wave_width = 0;
	_wave_height = 0;
	_wave_size = 0;

	_min_abs_half_plogp = 0;
}

WaveFormCollapse::~WaveFormCollapse() {
}

void WaveFormCollapse::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_wave_width"), &WaveFormCollapse::get_wave_width);
	ClassDB::bind_method(D_METHOD("set_wave_width", "value"), &WaveFormCollapse::set_wave_width);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "wave_width"), "set_wave_width", "get_wave_width");

	ClassDB::bind_method(D_METHOD("get_wave_height"), &WaveFormCollapse::get_wave_height);
	ClassDB::bind_method(D_METHOD("set_wave_height", "value"), &WaveFormCollapse::set_wave_height);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "wave_height"), "set_wave_height", "get_wave_height");

	ClassDB::bind_method(D_METHOD("get_periodic_output"), &WaveFormCollapse::get_periodic_output);
	ClassDB::bind_method(D_METHOD("set_periodic_output", "value"), &WaveFormCollapse::set_periodic_output);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "periodic_output"), "set_periodic_output", "get_periodic_output");

	ClassDB::bind_method(D_METHOD("set_seed", "seed"), &WaveFormCollapse::set_seed);
	//ClassDB::bind_method(D_METHOD("set_wave_size", "width", "height"), &WaveFormCollapse::set_wave_size);

	ClassDB::bind_method(D_METHOD("propagate"), &WaveFormCollapse::propagate);
	ClassDB::bind_method(D_METHOD("initialize"), &WaveFormCollapse::initialize);

	ClassDB::bind_method(D_METHOD("set_input", "data", "width", "height"), &WaveFormCollapse::set_input);

	ClassDB::bind_method(D_METHOD("generate_image_index_data"), &WaveFormCollapse::generate_image_index_data);

	BIND_ENUM_CONSTANT(SYMMETRY_X);
	BIND_ENUM_CONSTANT(SYMMETRY_T);
	BIND_ENUM_CONSTANT(SYMMETRY_I);
	BIND_ENUM_CONSTANT(SYMMETRY_L);
	BIND_ENUM_CONSTANT(SYMMETRY_BACKSLASH);
	BIND_ENUM_CONSTANT(SYMMETRY_P);
}
