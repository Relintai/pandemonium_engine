#ifndef OVERLAPPING_WAVE_FORM_COLLAPSE_H
#define OVERLAPPING_WAVE_FORM_COLLAPSE_H

/*************************************************************************/
/*  overlapping_wave_form_collapse.h                                     */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/containers/vector.h"

#include "array_2d.h"
#include "wave_form_collapse.h"

class OverlappingWaveFormCollapse : public WaveFormCollapse {
	GDCLASS(OverlappingWaveFormCollapse, WaveFormCollapse);

public:
	bool get_periodic_input() const;
	void set_periodic_input(const bool val);

	int get_out_height() const;
	void set_out_height(const int val);

	int get_out_width() const;
	void set_out_width(const int val);

	int get_symmetry() const;
	void set_symmetry(const int val);

	bool get_ground() const;
	void set_ground(const bool val);

	int get_pattern_size() const;
	void set_pattern_size(const int val);

	int get_wave_height() const;
	int get_wave_width() const;

	Array2D<int> run();

	void init_ground();

	bool set_pattern(const Array2D<int> &pattern, int i, int j);
	int get_ground_pattern_id();
	int get_pattern_id(const Array2D<int> &pattern);
	void set_pattern(int pattern_id, int i, int j);

	void init_patterns();

	bool agrees(const Array2D<int> &pattern1, const Array2D<int> &pattern2, int dy, int dx);

	void generate_compatible();

	Array2D<int> to_image(const Array2D<int> &output_patterns) const;

	void initialize();

	OverlappingWaveFormCollapse();
	~OverlappingWaveFormCollapse();

protected:
	static void _bind_methods();

private:
	Vector<Array2D<int>> _patterns;

	bool _periodic_input;
	int _out_height;
	int _out_width;
	int _symmetry;
	bool _ground;
	int _pattern_size;
};

#endif
