#ifndef PDF_DASH_MODE_H
#define PDF_DASH_MODE_H

/*************************************************************************/
/*  pdf_dash_mode.h                                                      */
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

#include "core/object/reference.h"

class PDFDashMode : public Reference {
	GDCLASS(PDFDashMode, Reference);

public:
	enum {
		MAX_PATTERN_ELEMENT_COUNT = 8,
	};

	uint32_t get_num_elements() const;
	void set_num_elements(const uint32_t p_num);

	float get_phase() const;
	void set_phase(const float p_phase);

	float get_pattern_element(const int p_index) const;
	void set_pattern_element(const int p_index, const float p_value);

	PDFDashMode();
	~PDFDashMode();

	float *_get_ptn();
	void _setup(float *p_ptn, uint32_t p_num, float p_phase);

protected:
	static void _bind_methods();

	float _pattern[MAX_PATTERN_ELEMENT_COUNT];
	uint32_t _num_pattern;
	float _phase;
};

#endif
