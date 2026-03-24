/*************************************************************************/
/*  pdf_3d_measure.cpp                                                   */
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

#include "pdf_3d_measure.h"

#include "pdf_annotation.h"
#include "pdf_encoder.h"

#include "hpdf.h"

uint32_t PDF3DMeasure::set_name(const String &p_name) {
	_status = HPDF_3DMeasure_SetName((HPDF_3DMeasure)_3d_measure, p_name.utf8().get_data());

	return _status;
}
uint32_t PDF3DMeasure::set_color(const Color &p_color) {
	HPDF_RGBColor c;
	c.r = p_color.r;
	c.g = p_color.g;
	c.b = p_color.b;

	_status = HPDF_3DMeasure_SetColor((HPDF_3DMeasure)_3d_measure, c);

	return _status;
}
uint32_t PDF3DMeasure::set_text_size(const float p_size) {
	_status = HPDF_3DMeasure_SetTextSize((HPDF_3DMeasure)_3d_measure, p_size);

	return _status;
}
uint32_t PDF3DMeasure::set_text_box_size(const Vector2i &p_size) {
	_status = HPDF_3DC3DMeasure_SetTextBoxSize((HPDF_3DMeasure)_3d_measure, p_size.x, p_size.y);

	return _status;
}
uint32_t PDF3DMeasure::set_text(const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Encoder encoder = NULL;

	if (p_encoder.is_valid()) {
		encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	_status = HPDF_3DC3DMeasure_SetText((HPDF_3DMeasure)_3d_measure, p_text.utf8().get_data(), encoder);

	return _status;
}
uint32_t PDF3DMeasure::set_projection_annotation(const Ref<PDFAnnotationProjection> &p_value) {
	HPDF_Annotation hdpf_annot = NULL;

	if (p_value.is_valid()) {
		hdpf_annot = (HPDF_Annotation)p_value->_get_hpdf_annotation();
	}

	_status = HPDF_3DC3DMeasure_SetProjectionAnotation((HPDF_3DMeasure)_3d_measure, hdpf_annot);

	return _status;
}

uint32_t PDF3DMeasure::get_status() {
	return _status;
}

PDF3DMeasure::PDF3DMeasure() {
	_3d_measure = NULL;
}

PDF3DMeasure::~PDF3DMeasure() {
}

void *PDF3DMeasure::_get_hpdf_3d_measure() const {
	return _3d_measure;
}

void PDF3DMeasure::_set_hpdf_3d_measure(void *p_3d_measure) {
	_3d_measure = p_3d_measure;
}

void PDF3DMeasure::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_name", "name"), &PDF3DMeasure::set_name);
	ClassDB::bind_method(D_METHOD("set_color", "color"), &PDF3DMeasure::set_color);
	ClassDB::bind_method(D_METHOD("set_text_size", "siz"), &PDF3DMeasure::set_text_size);
	ClassDB::bind_method(D_METHOD("set_text_box_size", "siz"), &PDF3DMeasure::set_text_box_size);
	ClassDB::bind_method(D_METHOD("set_text", "text", "encoder"), &PDF3DMeasure::set_text, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("set_projection_annotation", "value"), &PDF3DMeasure::set_projection_annotation);

	ClassDB::bind_method(D_METHOD("get_status"), &PDF3DMeasure::get_status);
}
