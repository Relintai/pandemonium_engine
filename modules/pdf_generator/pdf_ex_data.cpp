/*************************************************************************/
/*  pdf_ex_data.cpp                                                      */
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

#include "pdf_ex_data.h"

#include "pdf_3d_measure.h"

#include "hpdf.h"

uint32_t PDFExData::set_3d_measurement(const Ref<PDF3DMeasure> &p_measure) {
	HPDF_3DMeasure measure = NULL;

	if (p_measure.is_valid()) {
		measure = (HPDF_3DMeasure)p_measure->_get_hpdf_3d_measure();
	}

	_status = HPDF_3DAnnotExData_Set3DMeasurement((HPDF_ExData)_ex_data, measure);

	return _status;
}

uint32_t PDFExData::get_status() {
	return _status;
}

PDFExData::PDFExData() {
	_status = 0;

	_ex_data = NULL;
}

PDFExData::~PDFExData() {
}

void *PDFExData::_get_hpdf_ex_data() const {
	return _ex_data;
}

void PDFExData::_set_hpdf_ex_data(void *p_ex_data) {
	_ex_data = p_ex_data;
}

void PDFExData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_3d_measurement", "measure"), &PDFExData::set_3d_measurement);

	ClassDB::bind_method(D_METHOD("get_status"), &PDFExData::get_status);
}
