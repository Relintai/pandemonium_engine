#ifndef PDF_IMAGE_H
#define PDF_IMAGE_H

/*************************************************************************/
/*  pdf_image.h                                                          */
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

class PDFXObject;

class PDFImage : public Reference {
	GDCLASS(PDFImage, Reference);

public:
	uint32_t add_s_mask(const Ref<PDFImage> &p_smask);

	Vector2 get_size();
	Vector2 get_size_status();

	uint32_t get_width();
	uint32_t get_height();

	uint32_t get_bits_per_component();
	String get_color_space();

	uint32_t set_color_mask(const Vector2i &p_r, const Vector2i &p_g, const Vector2i &p_b);

	uint32_t set_mask_image(const Ref<PDFImage> &p_mask_image);

	Ref<PDFXObject> as_x_object();

	uint32_t get_status();

	PDFImage();
	~PDFImage();

	void *_get_hpdf_image() const;
	void _set_hpdf_image(void *p_image);

protected:
	static void _bind_methods();

	void *_image;

	uint32_t _status;
};

#endif
