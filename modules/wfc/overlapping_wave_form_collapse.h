#ifndef OVERLAPPING_WAVE_FORM_COLLAPSE_H
#define OVERLAPPING_WAVE_FORM_COLLAPSE_H

#include "core/vector.h"

#include "array_2d.h"
#include "wave_form_collapse.h"

class OverlappingWaveFormCollapse : public WaveFormCollapse {
	GDCLASS(OverlappingWaveFormCollapse, WaveFormCollapse);

public:
	bool periodic_input;
	bool periodic_output;
	uint32_t out_height;
	uint32_t out_width;
	uint32_t symmetry;
	bool ground;
	uint32_t pattern_size;

	uint32_t get_wave_height() const;
	uint32_t get_wave_width() const;

	Array2D<uint32_t> run();

	void init_ground(const Array2D<uint32_t> &input, const Vector<Array2D<uint32_t>> &patterns, const OverlappingWFCOptions &options);

	bool set_pattern(const Array2D<uint32_t> &pattern, uint32_t i, uint32_t j);
	static uint32_t get_ground_pattern_id(const Array2D<uint32_t> &input, const Vector<Array2D<uint32_t>> &patterns, const OverlappingWFCOptions &options);
	uint32_t get_pattern_id(const Array2D<uint32_t> &pattern);
	void set_pattern(uint32_t pattern_id, uint32_t i, uint32_t j);
	static std::pair<Vector<Array2D<uint32_t>>, Vector<double>> get_patterns(const Array2D<uint32_t> &input, const OverlappingWFCOptions &options);

	static bool agrees(const Array2D<uint32_t> &pattern1, const Array2D<uint32_t> &pattern2, int dy, int dx);

	static Vector<PropagatorStateEntry> generate_compatible(const Vector<Array2D<uint32_t>> &patterns);

	Array2D<uint32_t> to_image(const Array2D<uint32_t> &output_patterns) const;

	OverlappingWaveFormCollapse();
	~OverlappingWaveFormCollapse();

protected:
	static void _bind_methods();

private:
	Array2D<uint32_t> input;

	Vector<Array2D<uint32_t>> patterns;
};

#endif
