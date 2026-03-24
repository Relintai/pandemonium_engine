#ifndef PDF_FONT_H
#define PDF_FONT_H

/*************************************************************************/
/*  pdf_font.h                                                           */
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

class PDFFont : public Reference {
	GDCLASS(PDFFont, Reference);

public:
	String get_font_name();
	String get_encoding_name();

	int get_unicode_width(const uint16_t p_code);

	Rect2 get_bbox();
	int get_ascent();
	int get_descent();
	uint32_t get_x_height();
	uint32_t get_cap_height();

	Dictionary get_text_width(const String &p_text);
	Array measure_text(const String &p_text, const float p_width, const float p_font_size, const float p_char_space, const float p_word_space, const bool p_wordwrap);

	PDFFont();
	~PDFFont();

	void *_get_hpdf_font() const;
	void _set_hpdf_font(void *p_font);

protected:
	static void _bind_methods();

	void *_font;
};

#endif
