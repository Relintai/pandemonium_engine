#ifndef PDF_ENCODER_H
#define PDF_ENCODER_H

/*************************************************************************/
/*  pdf_encoder.h                                                        */
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

class PDFEncoder : public Reference {
	GDCLASS(PDFEncoder, Reference);

public:
#if 0

	HPDF_EXPORT(HPDF_EncoderType)
	HPDF_Encoder_GetType(HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_ByteType)
	HPDF_Encoder_GetByteType(HPDF_Encoder encoder,
			const char *text,
			HPDF_UINT index);

	HPDF_EXPORT(HPDF_UNICODE)
	HPDF_Encoder_GetUnicode(HPDF_Encoder encoder,
			HPDF_UINT16 code);

	HPDF_EXPORT(HPDF_WritingMode)
	HPDF_Encoder_GetWritingMode(HPDF_Encoder encoder);

#endif


#if 0
#define HPDF_ENCODING_FONT_SPECIFIC "FontSpecific"
#define HPDF_ENCODING_STANDARD "StandardEncoding"
#define HPDF_ENCODING_MAC_ROMAN "MacRomanEncoding"
#define HPDF_ENCODING_WIN_ANSI "WinAnsiEncoding"
#define HPDF_ENCODING_ISO8859_2 "ISO8859-2"
#define HPDF_ENCODING_ISO8859_3 "ISO8859-3"
#define HPDF_ENCODING_ISO8859_4 "ISO8859-4"
#define HPDF_ENCODING_ISO8859_5 "ISO8859-5"
#define HPDF_ENCODING_ISO8859_6 "ISO8859-6"
#define HPDF_ENCODING_ISO8859_7 "ISO8859-7"
#define HPDF_ENCODING_ISO8859_8 "ISO8859-8"
#define HPDF_ENCODING_ISO8859_9 "ISO8859-9"
#define HPDF_ENCODING_ISO8859_10 "ISO8859-10"
#define HPDF_ENCODING_ISO8859_11 "ISO8859-11"
#define HPDF_ENCODING_ISO8859_13 "ISO8859-13"
#define HPDF_ENCODING_ISO8859_14 "ISO8859-14"
#define HPDF_ENCODING_ISO8859_15 "ISO8859-15"
#define HPDF_ENCODING_ISO8859_16 "ISO8859-16"
#define HPDF_ENCODING_CP1250 "CP1250"
#define HPDF_ENCODING_CP1251 "CP1251"
#define HPDF_ENCODING_CP1252 "CP1252"
#define HPDF_ENCODING_CP1253 "CP1253"
#define HPDF_ENCODING_CP1254 "CP1254"
#define HPDF_ENCODING_CP1255 "CP1255"
#define HPDF_ENCODING_CP1256 "CP1256"
#define HPDF_ENCODING_CP1257 "CP1257"
#define HPDF_ENCODING_CP1258 "CP1258"
#define HPDF_ENCODING_KOI8_R "KOI8-R"
#endif

	uint32_t get_status();

	PDFEncoder();
	~PDFEncoder();

	void *_get_hpdf_encoder() const;
	void _set_hpdf_encoder(void *p_encoder);

protected:
	static void _bind_methods();

	void *_encoder;

	uint32_t _status;
};

#endif
