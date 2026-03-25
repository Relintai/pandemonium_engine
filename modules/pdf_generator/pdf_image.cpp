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

uint32_t PDFImage::add_s_mask(const Ref<PDFImage> &p_smask) {
	HPDF_Image smask = NULL;

	if (p_smask.is_valid()) {
		smask = (HPDF_Image)p_smask->_get_hpdf_image();
	}

	_status = HPDF_Image_AddSMask((HPDF_Image)_image, smask);

	return _status;
}

Vector2 PDFImage::get_size() {
	HPDF_Point s = HPDF_Image_GetSize((HPDF_Image)_image);

	return Vector2(s.x, s.y);
}
Vector2 PDFImage::get_size_status() {
	HPDF_Point s;

	_status = HPDF_Image_GetSize2((HPDF_Image)_image, &s);

	return Vector2(s.x, s.y);
}

uint32_t PDFImage::get_width() {
	return HPDF_Image_GetWidth((HPDF_Image)_image);
}
uint32_t PDFImage::get_height() {
	return HPDF_Image_GetHeight((HPDF_Image)_image);
}

uint32_t PDFImage::get_bits_per_component() {
	return HPDF_Image_GetBitsPerComponent((HPDF_Image)_image);
}
String PDFImage::get_color_space() {
	return String::utf8(HPDF_Image_GetColorSpace((HPDF_Image)_image));
}

uint32_t PDFImage::set_color_mask(const Vector2i &p_r, const Vector2i &p_g, const Vector2i &p_b) {
	_status = HPDF_Image_SetColorMask((HPDF_Image)_image, p_r.x, p_r.y, p_g.x, p_g.y, p_b.x, p_b.y);

	return _status;
}

uint32_t PDFImage::set_mask_image(const Ref<PDFImage> &p_mask_image) {
	HPDF_Image mask_image = NULL;

	if (p_mask_image.is_valid()) {
		mask_image = (HPDF_Image)p_mask_image->_get_hpdf_image();
	}

	_status = HPDF_Image_SetMaskImage((HPDF_Image)_image, mask_image);

	return _status;
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
	ClassDB::bind_method(D_METHOD("add_s_mask", "smask"), &PDFImage::add_s_mask);

	ClassDB::bind_method(D_METHOD("get_size"), &PDFImage::get_size);
	ClassDB::bind_method(D_METHOD("get_size_status"), &PDFImage::get_size_status);

	ClassDB::bind_method(D_METHOD("get_width"), &PDFImage::get_width);
	ClassDB::bind_method(D_METHOD("get_height"), &PDFImage::get_height);

	ClassDB::bind_method(D_METHOD("get_bits_per_component"), &PDFImage::get_bits_per_component);
	ClassDB::bind_method(D_METHOD("get_color_space"), &PDFImage::get_color_space);

	ClassDB::bind_method(D_METHOD("set_color_mask", "r_min_max", "g_min_max", "b_min_max"), &PDFImage::set_color_mask);

	ClassDB::bind_method(D_METHOD("set_mask_image", "mask_image"), &PDFImage::set_mask_image);

	ClassDB::bind_method(D_METHOD("get_status"), &PDFImage::get_status);
}
