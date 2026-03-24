/*************************************************************************/
/*  pdf_destination.cpp                                                  */
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

#include "pdf_destination.h"

#include "hpdf.h"
#include "hpdf_doc.h"
#include "hpdf_pages.h"

uint32_t PDFDestination::set_xyz(const float p_left, const float p_top, const float p_zoom) {
	_status = HPDF_Destination_SetXYZ((HPDF_Destination)_destination, p_left, p_top, p_zoom);

	return _status;
}

uint32_t PDFDestination::set_fit() {
	_status = HPDF_Destination_SetFit((HPDF_Destination)_destination);

	return _status;
}
uint32_t PDFDestination::set_fit_h(const float p_top) {
	_status = HPDF_Destination_SetFitH((HPDF_Destination)_destination, p_top);

	return _status;
}
uint32_t PDFDestination::set_fit_v(const float p_left) {
	_status = HPDF_Destination_SetFitV((HPDF_Destination)_destination, p_left);

	return _status;
}
uint32_t PDFDestination::set_fit_r(const float p_left, const float p_bottom, const float p_right, const float p_top) {
	_status = HPDF_Destination_SetFitR((HPDF_Destination)_destination, p_left, p_bottom, p_right, p_top);

	return _status;
}
uint32_t PDFDestination::set_fit_b() {
	_status = HPDF_Destination_SetFitB((HPDF_Destination)_destination);

	return _status;
}
uint32_t PDFDestination::set_fit_bh(const float p_top) {
	_status = HPDF_Destination_SetFitBH((HPDF_Destination)_destination, p_top);

	return _status;
}
uint32_t PDFDestination::set_fit_bv(const float p_left) {
	_status = HPDF_Destination_SetFitBV((HPDF_Destination)_destination, p_left);

	return _status;
}

uint32_t PDFDestination::get_status() {
	return _status;
}

PDFDestination::PDFDestination() {
	_status = 0;

	_destination = NULL;
}

PDFDestination::~PDFDestination() {
}

void *PDFDestination::_get_hpdf_destination() const {
	return _destination;
}

void PDFDestination::_set_hpdf_destination(void *p_destination) {
	_destination = p_destination;
}

void PDFDestination::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_xyz", "left", "top", "zoom"), &PDFDestination::set_xyz);

	ClassDB::bind_method(D_METHOD("set_fit"), &PDFDestination::set_fit);
	ClassDB::bind_method(D_METHOD("set_fit_h", "top"), &PDFDestination::set_fit_h);
	ClassDB::bind_method(D_METHOD("set_fit_v", "top"), &PDFDestination::set_fit_v);
	ClassDB::bind_method(D_METHOD("set_fit_r", "left", "bottom", "right", "top"), &PDFDestination::set_fit_r);
	ClassDB::bind_method(D_METHOD("set_fit_b"), &PDFDestination::set_fit_b);
	ClassDB::bind_method(D_METHOD("set_fit_bh", "top"), &PDFDestination::set_fit_bh);
	ClassDB::bind_method(D_METHOD("set_fit_bv", "left"), &PDFDestination::set_fit_bv);

	ClassDB::bind_method(D_METHOD("get_status"), &PDFDestination::get_status);
}
