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
	int out_height;
	int out_width;
	int symmetry;
	bool ground;
	int pattern_size;

	void set_input(const Array2D<int> &data);

	int get_wave_height() const;
	int get_wave_width() const;

	Array2D<int> orun();

	void init_ground();

	bool set_pattern(const Array2D<int> &pattern, int i, int j);
	int get_ground_pattern_id();
	int get_pattern_id(const Array2D<int> &pattern);
	void set_pattern(int pattern_id, int i, int j);
	void get_patterns();

	bool agrees(const Array2D<int> &pattern1, const Array2D<int> &pattern2, int dy, int dx);

	Vector<WaveFormCollapse::PropagatorStateEntry> generate_compatible();

	Array2D<int> to_image(const Array2D<int> &output_patterns) const;

	void initialize();

	OverlappingWaveFormCollapse();
	~OverlappingWaveFormCollapse();

protected:
	static void _bind_methods();

private:
	Array2D<int> input;

	Vector<Array2D<int>> patterns;
};

#endif
