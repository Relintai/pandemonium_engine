
#include "overlapping_wave_form_collapse.h"

uint32_t OverlappingWaveFormCollapse::get_wave_height() const {
	return periodic_output ? out_height : out_height - pattern_size + 1;
}

//Get the wave width given these options.
uint32_t OverlappingWaveFormCollapse::get_wave_width() const {
	return periodic_output ? out_width : out_width - pattern_size + 1;
}

// Run the WFC algorithm, and return the result if the algorithm succeeded.
Array2D<uint32_t> OverlappingWaveFormCollapse::run() {
	Array2D<uint32_t> result = wfc.run();

	if (result.width == 0 && result.height == 0) {
		return Array2D<uint32_t>(0, 0);
	}

	return to_image(result);
}

void OverlappingWaveFormCollapse::init_ground(WFC &wfc, const Array2D<T> &input, const Vector<Array2D<T>> &patterns, const OverlappingWFCOptions &options) {
	uint32_t ground_pattern_id = get_ground_pattern_id(input, patterns, options);

	for (uint32_t j = 0; j < options.get_wave_width(); j++) {
		set_pattern(ground_pattern_id, options.get_wave_height() - 1, j);
	}

	for (uint32_t i = 0; i < options.get_wave_height() - 1; i++) {
		for (uint32_t j = 0; j < options.get_wave_width(); j++) {
			wfc.remove_wave_pattern(i, j, ground_pattern_id);
		}
	}

	wfc.propagate();
}

// Set the pattern at a specific position.
// Returns false if the given pattern does not exist, or if the
// coordinates are not in the wave
bool OverlappingWaveFormCollapse::set_pattern(const Array2D<uint32_t> &pattern, uint32_t i, uint32_t j) {
	auto pattern_id = get_pattern_id(pattern);

	if (pattern_id == std::nullopt || i >= options.get_wave_height() || j >= options.get_wave_width()) {
		return false;
	}

	set_pattern(pattern_id, i, j);
	return true;
}

static uint32_t OverlappingWaveFormCollapse::get_ground_pattern_id(const Array2D<uint32_t> &input, const Vector<Array2D<uint32_t>> &patterns, const OverlappingWFCOptions &options) {
	// Get the pattern.
	Array2D<uint32_t> ground_pattern = input.get_sub_array(input.height - 1, input.width / 2, options.pattern_size, options.pattern_size);

	// Retrieve the id of the pattern.
	for (int i = 0; i < patterns.size(); i++) {
		if (ground_pattern == patterns[i]) {
			return i;
		}
	}

	ERR_FAIL_V(0);
}

uint32_t OverlappingWaveFormCollapse::get_pattern_id(const Array2D<uint32_t> &pattern) {
	uint32_t *pattern_id = std::find(patterns.begin(), patterns.end(), pattern);

	if (pattern_id != patterns.end()) {
		return *pattern_id;
	}

	return -1;
}

// Set the pattern at a specific position, given its pattern id
// pattern_id needs to be a valid pattern id, and i and j needs to be in the wave range
void OverlappingWaveFormCollapse::set_pattern(uint32_t pattern_id, uint32_t i, uint32_t j) {
	for (int p = 0; p < patterns.size(); p++) {
		if (pattern_id != p) {
			wfc.remove_wave_pattern(i, j, p);
		}
	}
}

//Return the list of patterns, as well as their probabilities of apparition.
static std::pair<Vector<Array2D<uint32_t>>, Vector<double>> OverlappingWaveFormCollapse::get_patterns(const Array2D<uint32_t> &input, const OverlappingWFCOptions &options) {
	std::unordered_map<Array2D<uint32_t>, uint32_t> patterns_id;
	Vector<Array2D<uint32_t>> patterns;

	// The number of time a pattern is seen in the input image.
	Vector<double> patterns_weight;

	Vector<Array2D<uint32_t>> symmetries(8, Array2D<uint32_t>(options.pattern_size, options.pattern_size));
	uint32_t max_i = options.periodic_input ? input.height : input.height - options.pattern_size + 1;
	uint32_t max_j = options.periodic_input ? input.width : input.width - options.pattern_size + 1;

	for (uint32_t i = 0; i < max_i; i++) {
		for (uint32_t j = 0; j < max_j; j++) {
			// Compute the symmetries of every pattern in the image.
			symmetries[0].data = input.get_sub_array(i, j, options.pattern_size, options.pattern_size).data;
			symmetries[1].data = symmetries[0].reflected().data;
			symmetries[2].data = symmetries[0].rotated().data;
			symmetries[3].data = symmetries[2].reflected().data;
			symmetries[4].data = symmetries[2].rotated().data;
			symmetries[5].data = symmetries[4].reflected().data;
			symmetries[6].data = symmetries[4].rotated().data;
			symmetries[7].data = symmetries[6].reflected().data;

			// The number of symmetries in the option class define which symetries
			// will be used.
			for (uint32_t k = 0; k < options.symmetry; k++) {
				auto res = patterns_id.insert(std::make_pair(symmetries[k], patterns.size()));

				// If the pattern already exist, we just have to increase its number
				// of appearance.
				if (!res.second) {
					patterns_weight[res.first->second] += 1;
				} else {
					patterns.push_back(symmetries[k]);
					patterns_weight.push_back(1);
				}
			}
		}
	}

	return { patterns, patterns_weight };
}

//Return true if the pattern1 is compatible with pattern2 when pattern2 is at a distance (dy,dx) from pattern1.
static bool OverlappingWaveFormCollapse::agrees(const Array2D<uint32_t> &pattern1, const Array2D<uint32_t> &pattern2, int dy, int dx) {
	uint32_t xmin = dx < 0 ? 0 : dx;
	uint32_t xmax = dx < 0 ? dx + pattern2.width : pattern1.width;
	uint32_t ymin = dy < 0 ? 0 : dy;
	uint32_t ymax = dy < 0 ? dy + pattern2.height : pattern1.width;

	// Iterate on every pixel contained in the intersection of the two pattern.
	for (uint32_t y = ymin; y < ymax; y++) {
		for (uint32_t x = xmin; x < xmax; x++) {
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
static Vector<PropagatorStateEntry> OverlappingWaveFormCollapse::generate_compatible(const Vector<Array2D<uint32_t>> &patterns) {
	Vector<PropagatorStateEntry> compatible;
	compatible.resize(patterns.size());

	// Iterate on every dy, dx, pattern1 and pattern2
	for (int pattern1 = 0; pattern1 < patterns.size(); pattern1++) {
		for (uint32_t direction = 0; direction < 4; direction++) {
			for (int pattern2 = 0; pattern2 < patterns.size(); pattern2++) {
				if (agrees(patterns[pattern1], patterns[pattern2], directions_y[direction], directions_x[direction])) {
					compatible[pattern1][direction].push_back(pattern2);
				}
			}
		}
	}

	return compatible;
}

// Transform a 2D array containing the patterns id to a 2D array containing the pixels.
Array2D<uint32_t> OverlappingWaveFormCollapse::to_image(const Array2D<uint32_t> &output_patterns) const {
	Array2D<uint32_t> output = Array2D<uint32_t>(options.out_height, options.out_width);

	if (options.periodic_output) {
		for (uint32_t y = 0; y < options.get_wave_height(); y++) {
			for (uint32_t x = 0; x < options.get_wave_width(); x++) {
				output.get(y, x) = patterns[output_patterns.get(y, x)].get(0, 0);
			}
		}
	} else {
		for (uint32_t y = 0; y < options.get_wave_height(); y++) {
			for (uint32_t x = 0; x < options.get_wave_width(); x++) {
				output.get(y, x) = patterns[output_patterns.get(y, x)].get(0, 0);
			}
		}

		for (uint32_t y = 0; y < options.get_wave_height(); y++) {
			const Array2D<uint32_t> &pattern = patterns[output_patterns.get(y, options.get_wave_width() - 1)];
			for (uint32_t dx = 1; dx < options.pattern_size; dx++) {
				output.get(y, options.get_wave_width() - 1 + dx) = pattern.get(0, dx);
			}
		}

		for (uint32_t x = 0; x < options.get_wave_width(); x++) {
			const Array2D<uint32_t> &pattern = patterns[output_patterns.get(options.get_wave_height() - 1, x)];
			for (uint32_t dy = 1; dy < options.pattern_size; dy++) {
				output.get(options.get_wave_height() - 1 + dy, x) =
						pattern.get(dy, 0);
			}
		}

		const Array2D<uint32_t> &pattern = patterns[output_patterns.get(options.get_wave_height() - 1, options.get_wave_width() - 1)];

		for (uint32_t dy = 1; dy < options.pattern_size; dy++) {
			for (uint32_t dx = 1; dx < options.pattern_size; dx++) {
				output.get(options.get_wave_height() - 1 + dy, options.get_wave_width() - 1 + dx) = pattern.get(dy, dx);
			}
		}
	}

	return output;
}

/*

	OverlappingWFC(const Array2D<uint32_t> &input, const OverlappingWFCOptions &options, int seed) :
			OverlappingWFC(input, options, seed, get_patterns(input, options)) {}

	OverlappingWFC(
			const Array2D<uint32_t> &input, const OverlappingWFCOptions &options,
			const int &seed,
			const std::pair<Vector<Array2D<uint32_t>>, Vector<double>> &patterns,
			const Vector<PropagatorStateEntry> &propagator) :
			input(input), options(options), patterns(patterns.first), wfc(options.periodic_output, seed, patterns.second, propagator, options.get_wave_height(), options.get_wave_width()) {
		// If necessary, the ground is set.
		if (options.ground) {
			init_ground(wfc, input, patterns.first, options);
		}
	}

	OverlappingWFC(const Array2D<uint32_t> &input, const OverlappingWFCOptions &options,
			const int &seed,
			const std::pair<Vector<Array2D<uint32_t>>, Vector<double>>
					&patterns) :
			OverlappingWFC(input, options, seed, patterns,
					generate_compatible(patterns.first)) {}
*/

OverlappingWaveFormCollapse::OverlappingWaveFormCollapse() {
}
OverlappingWaveFormCollapse::~OverlappingWaveFormCollapse() {
}

void OverlappingWaveFormCollapse::_bind_methods() {
}
