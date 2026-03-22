/*************************************************************************/
/*  pdf_dash_mode.cpp                                                    */
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

#include "pdf_dash_mode.h"

uint32_t PDFDashMode::get_num_elements() const {
	return _num_pattern;
}
void PDFDashMode::set_num_elements(const uint32_t p_num) {
	ERR_FAIL_INDEX(p_num, MAX_PATTERN_ELEMENT_COUNT);

	_num_pattern = p_num;
}

float PDFDashMode::get_phase() const {
	return _phase;
}
void PDFDashMode::set_phase(const float p_phase) {
	_phase = p_phase;
}

float PDFDashMode::get_pattern_element(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, MAX_PATTERN_ELEMENT_COUNT, 0);

	return _pattern[p_index];
}
void PDFDashMode::set_pattern_element(const int p_index, const float p_value) {
	ERR_FAIL_INDEX(p_index, MAX_PATTERN_ELEMENT_COUNT);

	_pattern[p_index] = p_value;
}

PDFDashMode::PDFDashMode() {
	_num_pattern = 0;
	_phase = 0;
}

PDFDashMode::~PDFDashMode() {
}

const float *PDFDashMode::_get_ptn() const {
	return _pattern;
}
void PDFDashMode::_setup(float *p_ptn, uint32_t p_num, float p_phase) {
	ERR_FAIL_INDEX(p_num, MAX_PATTERN_ELEMENT_COUNT);

	_phase = p_phase;
	_num_pattern = p_num;

	for (uint32_t i = 0; i < p_num; ++i) {
		_pattern[i] = p_ptn[i];
	}
}

void PDFDashMode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_num_elements"), &PDFDashMode::get_num_elements);
	ClassDB::bind_method(D_METHOD("set_num_elements", "val"), &PDFDashMode::set_num_elements);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_elements"), "set_num_elements", "get_num_elements");

	ClassDB::bind_method(D_METHOD("get_phase"), &PDFDashMode::get_phase);
	ClassDB::bind_method(D_METHOD("set_phase", "val"), &PDFDashMode::set_phase);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "phase"), "set_phase", "get_phase");

	ClassDB::bind_method(D_METHOD("get_pattern_element", "index"), &PDFDashMode::get_pattern_element);
	ClassDB::bind_method(D_METHOD("set_pattern_element", "index", "value"), &PDFDashMode::set_pattern_element);

	BIND_CONSTANT(MAX_PATTERN_ELEMENT_COUNT);
}
