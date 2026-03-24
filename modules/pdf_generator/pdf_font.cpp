/*************************************************************************/
/*  pdf_font.cpp                                                         */
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

#include "pdf_font.h"

#include "core/object/method_bind_ext.gen.inc"

#include "hpdf.h"

String PDFFont::get_font_name() {
	return String::utf8(HPDF_Font_GetFontName((HPDF_Font)_font));
}
String PDFFont::get_encoding_name() {
	return String::utf8(HPDF_Font_GetEncodingName((HPDF_Font)_font));
}

int PDFFont::get_unicode_width(const uint16_t p_code) {
	return HPDF_Font_GetUnicodeWidth((HPDF_Font)_font, p_code);
}

Rect2 PDFFont::get_bbox() {
	HPDF_Box box = HPDF_Font_GetBBox((HPDF_Font)_font);

	Rect2 r;
	r.position.x = box.left;
	r.position.y = box.top;
	r.set_end(Vector2(box.right, box.bottom));

	return r;
}
int PDFFont::get_ascent() {
	return HPDF_Font_GetAscent((HPDF_Font)_font);
}
int PDFFont::get_descent() {
	return HPDF_Font_GetDescent((HPDF_Font)_font);
}
uint32_t PDFFont::get_x_height() {
	return HPDF_Font_GetXHeight((HPDF_Font)_font);
}
uint32_t PDFFont::get_cap_height() {
	return HPDF_Font_GetCapHeight((HPDF_Font)_font);
}

Dictionary PDFFont::get_text_width(const String &p_text) {
	CharString cs = p_text.utf8();

	HPDF_TextWidth tw = HPDF_Font_TextWidth((HPDF_Font)_font, (const HPDF_BYTE *)cs.get_data(), cs.length());

	Dictionary d;
	d["numchars"] = tw.numchars;
	d["numwords"] = tw.numwords;
	d["width"] = tw.width;
	d["numspace"] = tw.numspace;

	return d;
}
Array PDFFont::measure_text(const String &p_text, const float p_width, const float p_font_size, const float p_char_space, const float p_word_space, const bool p_wordwrap) {
	CharString cs = p_text.utf8();
	float real_width = 0;
	uint32_t width = HPDF_Font_MeasureText((HPDF_Font)_font, (const HPDF_BYTE *)cs.get_data(), cs.length(), p_width, p_font_size, p_char_space, p_word_space, p_wordwrap, &real_width);

	Array arr;
	arr.push_back(width);
	arr.push_back(real_width);
	return arr;
}

PDFFont::PDFFont() {
	_font = NULL;
}

PDFFont::~PDFFont() {
}

void *PDFFont::_get_hpdf_font() const {
	return _font;
}

void PDFFont::_set_hpdf_font(void *p_font) {
	_font = p_font;
}

void PDFFont::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_font_name"), &PDFFont::get_font_name);
	ClassDB::bind_method(D_METHOD("get_encoding_name"), &PDFFont::get_encoding_name);

	ClassDB::bind_method(D_METHOD("get_unicode_width", "code"), &PDFFont::get_unicode_width);

	ClassDB::bind_method(D_METHOD("get_bbox"), &PDFFont::get_bbox);
	ClassDB::bind_method(D_METHOD("get_ascent"), &PDFFont::get_ascent);
	ClassDB::bind_method(D_METHOD("get_descent"), &PDFFont::get_descent);
	ClassDB::bind_method(D_METHOD("get_x_height"), &PDFFont::get_x_height);
	ClassDB::bind_method(D_METHOD("get_cap_height"), &PDFFont::get_cap_height);

	ClassDB::bind_method(D_METHOD("get_text_width", "text"), &PDFFont::get_text_width);
	ClassDB::bind_method(D_METHOD("measure_text", "text", "width", "font_size", "char_space", "word_space", "wordwrap"), &PDFFont::measure_text);
}
