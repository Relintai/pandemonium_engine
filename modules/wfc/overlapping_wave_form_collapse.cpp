
#include "overlapping_wave_form_collapse.h"

#include "core/containers/rb_set.h"

bool OverlappingWaveFormCollapse::get_periodic_input() const {
	return _periodic_input;
}
void OverlappingWaveFormCollapse::set_periodic_input(const bool val) {
	_periodic_input = val;
}

int OverlappingWaveFormCollapse::get_out_height() const {
	return _out_height;
}
void OverlappingWaveFormCollapse::set_out_height(const int val) {
	_out_height = val;
}

int OverlappingWaveFormCollapse::get_out_width() const {
	return _out_width;
}
void OverlappingWaveFormCollapse::set_out_width(const int val) {
	_out_width = val;
}

int OverlappingWaveFormCollapse::get_symmetry() const {
	return _symmetry;
}
void OverlappingWaveFormCollapse::set_symmetry(const int val) {
	_symmetry = val;
}

bool OverlappingWaveFormCollapse::get_ground() const {
	return _ground;
}
void OverlappingWaveFormCollapse::set_ground(const bool val) {
	_ground = val;
}

int OverlappingWaveFormCollapse::get_pattern_size() const {
	return _pattern_size;
}
void OverlappingWaveFormCollapse::set_pattern_size(const int val) {
	_pattern_size = val;
}

int OverlappingWaveFormCollapse::get_wave_height() const {
	return _periodic_output ? _out_height : _out_height - _pattern_size + 1;
}

//Get the wave width given these
int OverlappingWaveFormCollapse::get_wave_width() const {
	return _periodic_output ? _out_width : _out_width - _pattern_size + 1;
}

// Run the WFC algorithm, and return the result if the algorithm succeeded.
Array2D<int> OverlappingWaveFormCollapse::run() {
	Array2D<int> result = WaveFormCollapse::run();

	if (result.width == 0 && result.height == 0) {
		return Array2D<int>(0, 0);
	}

	return to_image(result);
}

void OverlappingWaveFormCollapse::init_ground() {
	int ground_pattern_id = get_ground_pattern_id();

	ERR_FAIL_COND(ground_pattern_id == -1);

	for (int j = 0; j < get_wave_width(); j++) {
		set_pattern(ground_pattern_id, get_wave_height() - 1, j);
	}

	for (int i = 0; i < get_wave_height() - 1; i++) {
		for (int j = 0; j < get_wave_width(); j++) {
			remove_wave_pattern(i, j, ground_pattern_id);
		}
	}

	propagate();
}

// Set the pattern at a specific position.
// Returns false if the given pattern does not exist, or if the
// coordinates are not in the wave
bool OverlappingWaveFormCollapse::set_pattern(const Array2D<int> &pattern, int i, int j) {
	int pattern_id = get_pattern_id(pattern);

	if (pattern_id == -1 || i >= get_wave_height() || j >= get_wave_width()) {
		return false;
	}

	set_pattern(pattern_id, i, j);
	return true;
}

int OverlappingWaveFormCollapse::get_ground_pattern_id() {
	// Get the pattern.
	Array2D<int> ground_pattern = _input.get_sub_array(_input.height - 1, _input.width / 2, _pattern_size, _pattern_size);

	// Retrieve the id of the pattern.
	for (int i = 0; i < _patterns.size(); i++) {
		if (ground_pattern == _patterns[i]) {
			return i;
		}
	}

	ERR_FAIL_V(-1);
}

int OverlappingWaveFormCollapse::get_pattern_id(const Array2D<int> &pattern) {
	for (int i = 0; i < _patterns.size(); ++i) {
		if (_patterns[i] == pattern) {
			return i;
		}
	}

	return -1;
}

// Set the pattern at a specific position, given its pattern id
// pattern_id needs to be a valid pattern id, and i and j needs to be in the wave range
void OverlappingWaveFormCollapse::set_pattern(int pattern_id, int i, int j) {
	for (int p = 0; p < _patterns.size(); p++) {
		if (pattern_id != p) {
			remove_wave_pattern(i, j, p);
		}
	}
}

//Return the list of patterns, as well as their probabilities of apparition.
void OverlappingWaveFormCollapse::init_patterns() {
	LocalVector<Array2D<int>> patterns_id;

	_patterns.clear();

	// The number of time a pattern is seen in the input image.
	Vector<double> patterns_weight;

	Vector<Array2D<int>> symmetries;
	symmetries.resize(8);

	for (int i = 0; i < 8; ++i) {
		symmetries.write[i].resize(_pattern_size, _pattern_size);
	}

	int max_i = _periodic_input ? _input.height : _input.height - _pattern_size + 1;
	int max_j = _periodic_input ? _input.width : _input.width - _pattern_size + 1;

	for (int i = 0; i < max_i; i++) {
		for (int j = 0; j < max_j; j++) {
			// Compute the symmetries of every pattern in the image.
			symmetries.write[0].data = _input.get_sub_array(i, j, _pattern_size, _pattern_size).data;
			symmetries.write[1].data = symmetries[0].reflected().data;
			symmetries.write[2].data = symmetries[0].rotated().data;
			symmetries.write[3].data = symmetries[2].reflected().data;
			symmetries.write[4].data = symmetries[2].rotated().data;
			symmetries.write[5].data = symmetries[4].reflected().data;
			symmetries.write[6].data = symmetries[4].rotated().data;
			symmetries.write[7].data = symmetries[6].reflected().data;

			// The number of symmetries in the option class define which symetries will be used.
			for (int k = 0; k < _symmetry; k++) {
				int indx = _patterns.size();

				for (uint32_t h = 0; h < patterns_id.size(); ++h) {
					if (patterns_id[h] == symmetries[k]) {
						indx = h;
						break;
					}
				}

				if (indx != _patterns.size()) {
					// If the pattern already exist, we just have to increase its number of appearance.
					patterns_weight.write[indx] += 1;
				} else {
					_patterns.push_back(symmetries[k]);
					patterns_weight.push_back(1);
					patterns_id.push_back(symmetries[k]);
				}
			}
		}
	}

	set_pattern_frequencies(patterns_weight);
}

//Return true if the pattern1 is compatible with pattern2 when pattern2 is at a distance (dy,dx) from pattern1.
bool OverlappingWaveFormCollapse::agrees(const Array2D<int> &pattern1, const Array2D<int> &pattern2, int dy, int dx) {
	int xmin = dx < 0 ? 0 : dx;
	int xmax = dx < 0 ? dx + pattern2.width : pattern1.width;
	int ymin = dy < 0 ? 0 : dy;
	int ymax = dy < 0 ? dy + pattern2.height : pattern1.width;

	// Iterate on every pixel contained in the intersection of the two pattern.
	for (int y = ymin; y < ymax; y++) {
		for (int x = xmin; x < xmax; x++) {
			// Check if the color is the same in the two patterns in that pixel.
			if (pattern1.get(y, x) != pattern2.get(y - dy, x - dx)) {
				return false;
			}
		}
	}

	return true;
}

// Precompute the function agrees(pattern1, pattern2, dy, dx).
// If agrees(pattern1, pattern2, dy, dx), then compatible[pattern1][direction]
// contains pattern2, where direction is the direction defined by (dy, dx)
// (see direction.hpp).
void OverlappingWaveFormCollapse::generate_compatible() {
	Vector<WaveFormCollapse::PropagatorStateEntry> compatible;
	compatible.resize(_patterns.size());

	// Iterate on every dy, dx, pattern1 and pattern2
	for (int pattern1 = 0; pattern1 < _patterns.size(); pattern1++) {
		for (int direction = 0; direction < 4; direction++) {
			for (int pattern2 = 0; pattern2 < _patterns.size(); pattern2++) {
				if (agrees(_patterns[pattern1], _patterns[pattern2], DIRECTIONS_Y[direction], DIRECTIONS_X[direction])) {
					compatible.write[pattern1].directions[direction].push_back(pattern2);
				}
			}
		}
	}

	set_propagator_state(compatible);
}

// Transform a 2D array containing the patterns id to a 2D array containing the pixels.
Array2D<int> OverlappingWaveFormCollapse::to_image(const Array2D<int> &output_patterns) const {
	Array2D<int> output(_out_height, _out_width);

	if (_periodic_output) {
		for (int y = 0; y < get_wave_height(); y++) {
			for (int x = 0; x < get_wave_width(); x++) {
				output.get(y, x) = _patterns[output_patterns.get(y, x)].get(0, 0);
			}
		}
	} else {
		for (int y = 0; y < get_wave_height(); y++) {
			for (int x = 0; x < get_wave_width(); x++) {
				output.get(y, x) = _patterns[output_patterns.get(y, x)].get(0, 0);
			}
		}

		for (int y = 0; y < get_wave_height(); y++) {
			const Array2D<int> &pattern = _patterns[output_patterns.get(y, get_wave_width() - 1)];
			for (int dx = 1; dx < _pattern_size; dx++) {
				output.get(y, get_wave_width() - 1 + dx) = pattern.get(0, dx);
			}
		}

		for (int x = 0; x < get_wave_width(); x++) {
			const Array2D<int> &pattern = _patterns[output_patterns.get(get_wave_height() - 1, x)];
			for (int dy = 1; dy < _pattern_size; dy++) {
				output.get(get_wave_height() - 1 + dy, x) =
						pattern.get(dy, 0);
			}
		}

		const Array2D<int> &pattern = _patterns[output_patterns.get(get_wave_height() - 1, get_wave_width() - 1)];

		for (int dy = 1; dy < _pattern_size; dy++) {
			for (int dx = 1; dx < _pattern_size; dx++) {
				output.get(get_wave_height() - 1 + dy, get_wave_width() - 1 + dx) = pattern.get(dy, dx);
			}
		}
	}

	return output;
}

void OverlappingWaveFormCollapse::initialize() {
	init_patterns();
	generate_compatible();

	set_wave_size(get_wave_width(), get_wave_height());

	WaveFormCollapse::initialize();

	// If necessary, the ground is set.
	if (_ground) {
		init_ground();
	}
}

OverlappingWaveFormCollapse::OverlappingWaveFormCollapse() {
	_periodic_input = true;
	_out_height = 0;
	_out_width = 0;
	_symmetry = 8;
	_ground = false;
	_pattern_size = 0;
}
OverlappingWaveFormCollapse::~OverlappingWaveFormCollapse() {
}

void OverlappingWaveFormCollapse::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_periodic_input"), &OverlappingWaveFormCollapse::get_periodic_input);
	ClassDB::bind_method(D_METHOD("set_periodic_input", "value"), &OverlappingWaveFormCollapse::set_periodic_input);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "periodic_input"), "set_periodic_input", "get_periodic_input");

	ClassDB::bind_method(D_METHOD("get_out_height"), &OverlappingWaveFormCollapse::get_out_height);
	ClassDB::bind_method(D_METHOD("set_out_height", "value"), &OverlappingWaveFormCollapse::set_out_height);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "out_height"), "set_out_height", "get_out_height");

	ClassDB::bind_method(D_METHOD("get_out_width"), &OverlappingWaveFormCollapse::get_out_width);
	ClassDB::bind_method(D_METHOD("set_out_width", "value"), &OverlappingWaveFormCollapse::set_out_width);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "out_width"), "set_out_width", "get_out_width");

	ClassDB::bind_method(D_METHOD("get_symmetry"), &OverlappingWaveFormCollapse::get_symmetry);
	ClassDB::bind_method(D_METHOD("set_symmetry", "value"), &OverlappingWaveFormCollapse::set_symmetry);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "symmetry"), "set_symmetry", "get_symmetry");

	ClassDB::bind_method(D_METHOD("get_ground"), &OverlappingWaveFormCollapse::get_ground);
	ClassDB::bind_method(D_METHOD("set_ground", "value"), &OverlappingWaveFormCollapse::set_ground);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ground"), "set_ground", "get_ground");

	ClassDB::bind_method(D_METHOD("get_pattern_size"), &OverlappingWaveFormCollapse::get_pattern_size);
	ClassDB::bind_method(D_METHOD("set_pattern_size", "value"), &OverlappingWaveFormCollapse::set_pattern_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "pattern_size"), "set_pattern_size", "get_pattern_size");
}
