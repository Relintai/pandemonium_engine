/*************************************************************************/
/*  pdf_ext_g_state.cpp                                                  */
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

#include "pdf_ext_g_state.h"

#include "hpdf.h"

uint32_t PDFExtGState::set_alpha_stroke(const float p_value) {
	_status = HPDF_ExtGState_SetAlphaStroke((HPDF_ExtGState)_ext_g_state, p_value);

	return _status;
}
uint32_t PDFExtGState::set_alpha_fill(const float p_value) {
	_status = HPDF_ExtGState_SetAlphaFill((HPDF_ExtGState)_ext_g_state, p_value);

	return _status;
}
uint32_t PDFExtGState::set_blend_mode(const BlendMode p_mode) {
	HPDF_BlendMode mode = static_cast<HPDF_BlendMode>(p_mode);

	_status = HPDF_ExtGState_SetBlendMode((HPDF_ExtGState)_ext_g_state, mode);

	return _status;
}

uint32_t PDFExtGState::get_status() {
	return _status;
}

PDFExtGState::PDFExtGState() {
	_status = 0;

	_ext_g_state = NULL;
}

PDFExtGState::~PDFExtGState() {
}

void *PDFExtGState::_get_hpdf_ext_g_state() const {
	return _ext_g_state;
}

void PDFExtGState::_set_hpdf_ext_g_state(void *p_ext_g_state) {
	_ext_g_state = p_ext_g_state;
}

void PDFExtGState::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_alpha_stroke", "value"), &PDFExtGState::set_alpha_stroke);
	ClassDB::bind_method(D_METHOD("set_alpha_fill", "value"), &PDFExtGState::set_alpha_fill);
	ClassDB::bind_method(D_METHOD("set_blend_mode", "mode"), &PDFExtGState::set_blend_mode);

	ClassDB::bind_method(D_METHOD("get_status"), &PDFExtGState::get_status);

	BIND_ENUM_CONSTANT(BM_NORMAL);
	BIND_ENUM_CONSTANT(BM_MULTIPLY);
	BIND_ENUM_CONSTANT(BM_SCREEN);
	BIND_ENUM_CONSTANT(BM_OVERLAY);
	BIND_ENUM_CONSTANT(BM_DARKEN);
	BIND_ENUM_CONSTANT(BM_LIGHTEN);
	BIND_ENUM_CONSTANT(BM_COLOR_DODGE);
	BIND_ENUM_CONSTANT(BM_COLOR_BUM);
	BIND_ENUM_CONSTANT(BM_HARD_LIGHT);
	BIND_ENUM_CONSTANT(BM_SOFT_LIGHT);
	BIND_ENUM_CONSTANT(BM_DIFFERENCE);
	BIND_ENUM_CONSTANT(BM_EXCLUSHON);
	BIND_ENUM_CONSTANT(BM_EOF);
}
