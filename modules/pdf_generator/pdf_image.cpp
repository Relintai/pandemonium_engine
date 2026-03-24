/*************************************************************************/
/*  pdf_image.cpp                                                        */
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

#include "pdf_image.h"

#include "hpdf.h"
#include "hpdf_doc.h"
#include "hpdf_pages.h"

uint32_t PDFImage::get_width() {
	return HPDF_Image_GetWidth((HPDF_Image)_image);
}
uint32_t PDFImage::get_height() {
	return HPDF_Image_GetHeight((HPDF_Image)_image);
}

uint32_t PDFImage::get_status() {
	return _status;
}

PDFImage::PDFImage() {
	_status = 0;

	_image = NULL;
}

PDFImage::~PDFImage() {
}

void *PDFImage::_get_hpdf_image() const {
	return _image;
}

void PDFImage::_set_hpdf_image(void *p_image) {
	_image = p_image;
}

void PDFImage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_width"), &PDFImage::get_width);
	ClassDB::bind_method(D_METHOD("get_height"), &PDFImage::get_height);

	//ClassDB::bind_method(D_METHOD("get_width"), &PDFImage::get_width);
	//ClassDB::bind_method(D_METHOD("set_width", "val"), &PDFImage::set_width);
	//ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");
}
