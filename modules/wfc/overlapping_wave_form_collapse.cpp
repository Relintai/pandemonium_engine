
#include "overlapping_wave_form_collapse.h"

#include "core/set.h"

void OverlappingWaveFormCollapse::set_input(const Array2D<int> &data) {
	input = data;
}

int OverlappingWaveFormCollapse::get_wave_height() const {
	return periodic_output ? out_height : out_height - pattern_size + 1;
}

//Get the wave width given these
int OverlappingWaveFormCollapse::get_wave_width() const {
	return periodic_output ? out_width : out_width - pattern_size + 1;
}

// Run the WFC algorithm, and return the result if the algorithm succeeded.
Array2D<int> OverlappingWaveFormCollapse::orun() {
	Array2D<int> result = run();

	if (result.width == 0 && result.height == 0) {
		return Array2D<int>(0, 0);
	}

	return to_image(result);
}

void OverlappingWaveFormCollapse::init_ground() {
	int ground_pattern_id = get_ground_pattern_id();

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

	if (pattern_id == static_cast<int>(-1) || i >= get_wave_height() || j >= get_wave_width()) {
		return false;
	}

	set_pattern(pattern_id, i, j);
	return true;
}

int OverlappingWaveFormCollapse::get_ground_pattern_id() {
	// Get the pattern.
	Array2D<int> ground_pattern = input.get_sub_array(input.height - 1, input.width / 2, pattern_size, pattern_size);

	// Retrieve the id of the pattern.
	for (int i = 0; i < patterns.size(); i++) {
		if (ground_pattern == patterns[i]) {
			return i;
		}
	}

	ERR_FAIL_V(0);
}

int OverlappingWaveFormCollapse::get_pattern_id(const Array2D<int> &pattern) {
	for (int i = 0; i < patterns.size(); ++i) {
		if (patterns[i] == pattern) {
			return i;
		}
	}

	return -1;
}

// Set the pattern at a specific position, given its pattern id
// pattern_id needs to be a valid pattern id, and i and j needs to be in the wave range
void OverlappingWaveFormCollapse::set_pattern(int pattern_id, int i, int j) {
	for (int p = 0; p < patterns.size(); p++) {
		if (pattern_id != static_cast<int>(p)) {
			remove_wave_pattern(i, j, p);
		}
	}
}

//Return the list of patterns, as well as their probabilities of apparition.
void OverlappingWaveFormCollapse::get_patterns() {
	//OAHashMap<Array2D<int>, int> patterns_id;

	LocalVector<Array2D<int>> patterns_id;

	patterns.clear();

	// The number of time a pattern is seen in the input image.
	Vector<double> patterns_weight;

	Vector<Array2D<int>> symmetries;
	symmetries.resize(8);

	for (int i = 0; i < 8; ++i) {
		symmetries.write[i].resize(pattern_size, pattern_size);
	}

	int max_i = periodic_input ? input.height : input.height - pattern_size + 1;
	int max_j = periodic_input ? input.width : input.width - pattern_size + 1;

	for (int i = 0; i < max_i; i++) {
		for (int j = 0; j < max_j; j++) {
			// Compute the symmetries of every pattern in the image.
			symmetries.write[0].data = input.get_sub_array(i, j, pattern_size, pattern_size).data;
			symmetries.write[1].data = symmetries[0].reflected().data;
			symmetries.write[2].data = symmetries[0].rotated().data;
			symmetries.write[3].data = symmetries[2].reflected().data;
			symmetries.write[4].data = symmetries[2].rotated().data;
			symmetries.write[5].data = symmetries[4].reflected().data;
			symmetries.write[6].data = symmetries[4].rotated().data;
			symmetries.write[7].data = symmetries[6].reflected().data;

			// The number of symmetries in the option class define which symetries will be used.
			for (int k = 0; k < symmetry; k++) {
				int indx = patterns.size();

				for (uint32_t h = 0; h < patterns_id.size(); ++h) {
					if (patterns_id[h] == symmetries[k]) {
						indx = h;
						break;
					}
				}

				if (indx != patterns.size()) {
					// If the pattern already exist, we just have to increase its number of appearance.
					patterns_weight.write[indx] += 1;
				} else {
					patterns.push_back(symmetries[k]);
					patterns_weight.push_back(1);
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
Vector<WaveFormCollapse::PropagatorStateEntry> OverlappingWaveFormCollapse::generate_compatible() {
	Vector<WaveFormCollapse::PropagatorStateEntry> compatible;
	compatible.resize(patterns.size());

	// Iterate on every dy, dx, pattern1 and pattern2
	for (int pattern1 = 0; pattern1 < patterns.size(); pattern1++) {
		for (int direction = 0; direction < 4; direction++) {
			for (int pattern2 = 0; pattern2 < patterns.size(); pattern2++) {
				if (agrees(patterns[pattern1], patterns[pattern2], DIRECTIONS_Y[direction], DIRECTIONS_X[direction])) {
					compatible.write[pattern1].directions[direction].push_back(pattern2);
				}
			}
		}
	}

	return compatible;
}

// Transform a 2D array containing the patterns id to a 2D array containing the pixels.
Array2D<int> OverlappingWaveFormCollapse::to_image(const Array2D<int> &output_patterns) const {
	Array2D<int> output = Array2D<int>(out_height, out_width);

	if (periodic_output) {
		for (int y = 0; y < get_wave_height(); y++) {
			for (int x = 0; x < get_wave_width(); x++) {
				output.get(y, x) = patterns[output_patterns.get(y, x)].get(0, 0);
			}
		}
	} else {
		for (int y = 0; y < get_wave_height(); y++) {
			for (int x = 0; x < get_wave_width(); x++) {
				output.get(y, x) = patterns[output_patterns.get(y, x)].get(0, 0);
			}
		}

		for (int y = 0; y < get_wave_height(); y++) {
			const Array2D<int> &pattern = patterns[output_patterns.get(y, get_wave_width() - 1)];
			for (int dx = 1; dx < pattern_size; dx++) {
				output.get(y, get_wave_width() - 1 + dx) = pattern.get(0, dx);
			}
		}

		for (int x = 0; x < get_wave_width(); x++) {
			const Array2D<int> &pattern = patterns[output_patterns.get(get_wave_height() - 1, x)];
			for (int dy = 1; dy < pattern_size; dy++) {
				output.get(get_wave_height() - 1 + dy, x) =
						pattern.get(dy, 0);
			}
		}

		const Array2D<int> &pattern = patterns[output_patterns.get(get_wave_height() - 1, get_wave_width() - 1)];

		for (int dy = 1; dy < pattern_size; dy++) {
			for (int dx = 1; dx < pattern_size; dx++) {
				output.get(get_wave_height() - 1 + dy, get_wave_width() - 1 + dx) = pattern.get(dy, dx);
			}
		}
	}

	return output;
}

void OverlappingWaveFormCollapse::initialize() {
	// If necessary, the ground is set.
	if (ground) {
		init_ground();
	}

	set_propagator_state(generate_compatible());

	WaveFormCollapse::initialize();
}

OverlappingWaveFormCollapse::OverlappingWaveFormCollapse() {
	periodic_input = false;
	periodic_output = false;
	out_height = 0;
	out_width = 0;
	symmetry = 0;
	ground = false;
	pattern_size = 0;
}
OverlappingWaveFormCollapse::~OverlappingWaveFormCollapse() {
}

void OverlappingWaveFormCollapse::_bind_methods() {
}
