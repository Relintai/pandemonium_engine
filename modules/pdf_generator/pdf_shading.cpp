/*************************************************************************/
/*  pdf_shading.cpp                                                      */
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

#include "pdf_shading.h"

#include "hpdf.h"
#include "hpdf_doc.h"
#include "hpdf_pages.h"

uint32_t PDFShading::get_status() {
	return _status;
}

PDFShading::PDFShading() {
	_status = 0;

	_shading = NULL;
}

PDFShading::~PDFShading() {
}

void *PDFShading::_get_hpdf_shading() const {
	return _shading;
}

void PDFShading::_set_hpdf_shading(void *p_shading) {
	_shading = p_shading;
}

void PDFShading::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("get_width"), &PDFShading::get_width);
	//ClassDB::bind_method(D_METHOD("set_width", "val"), &PDFShading::set_width);
	//ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");
}
