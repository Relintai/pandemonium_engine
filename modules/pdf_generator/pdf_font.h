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
#if 0
	HPDF_EXPORT(const char *)
	HPDF_Font_GetFontName(HPDF_Font font);

	HPDF_EXPORT(const char *)
	HPDF_Font_GetEncodingName(HPDF_Font font);

	HPDF_EXPORT(HPDF_INT)
	HPDF_Font_GetUnicodeWidth(HPDF_Font font,
			HPDF_UNICODE code);

	HPDF_EXPORT(HPDF_Box)
	HPDF_Font_GetBBox(HPDF_Font font);

	HPDF_EXPORT(HPDF_INT)
	HPDF_Font_GetAscent(HPDF_Font font);

	HPDF_EXPORT(HPDF_INT)
	HPDF_Font_GetDescent(HPDF_Font font);

	HPDF_EXPORT(HPDF_UINT)
	HPDF_Font_GetXHeight(HPDF_Font font);

	HPDF_EXPORT(HPDF_UINT)
	HPDF_Font_GetCapHeight(HPDF_Font font);

	HPDF_EXPORT(HPDF_TextWidth)
	HPDF_Font_TextWidth(HPDF_Font font,
			const HPDF_BYTE *text,
			HPDF_UINT len);

	HPDF_EXPORT(HPDF_UINT)
	HPDF_Font_MeasureText(HPDF_Font font,
			const HPDF_BYTE *text,
			HPDF_UINT len,
			HPDF_REAL width,
			HPDF_REAL font_size,
			HPDF_REAL char_space,
			HPDF_REAL word_space,
			HPDF_BOOL wordwrap,
			HPDF_REAL *real_width);
#endif

	uint32_t get_status();

	PDFFont();
	~PDFFont();

	void *_get_hpdf_font() const;
	void _set_hpdf_font(void *p_font);

protected:
	static void _bind_methods();

	void *_font;

	uint32_t _status;
};

#endif
