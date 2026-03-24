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
	enum Encoding {
		ENCODING_FONT_SPECIFIC,
		ENCODING_STANDARD,
		ENCODING_MAC_ROMAN,
		ENCODING_WIN_ANSI,
		ENCODING_ISO8859_2,
		ENCODING_ISO8859_3,
		ENCODING_ISO8859_4,
		ENCODING_ISO8859_5,
		ENCODING_ISO8859_6,
		ENCODING_ISO8859_7,
		ENCODING_ISO8859_8,
		ENCODING_ISO8859_9,
		ENCODING_ISO8859_10,
		ENCODING_ISO8859_11,
		ENCODING_ISO8859_13,
		ENCODING_ISO8859_14,
		ENCODING_ISO8859_15,
		ENCODING_ISO8859_16,
		ENCODING_CP1250,
		ENCODING_CP1251,
		ENCODING_CP1252,
		ENCODING_CP1253,
		ENCODING_CP1254,
		ENCODING_CP1255,
		ENCODING_CP1256,
		ENCODING_CP1257,
		ENCODING_CP1258,
		ENCODING_KOI8_R,
	};

	enum EncoderType {
		ENCODER_TYPE_SINGLE_BYTE,
		ENCODER_TYPE_DOUBLE_BYTE,
		ENCODER_TYPE_UNINITIALIZED,
		ENCODER_UNKNOWN
	};

	enum ByteType {
		BYTE_TYPE_SINGLE = 0,
		BYTE_TYPE_LEAD,
		BYTE_TYPE_TRAIL,
		BYTE_TYPE_UNKNOWN
	};

	enum WritingMode {
		WRITING_MODE_HORIZONTAL = 0,
		WRITING_MODE_VERTICAL,
		WRITING_MODE_EOF
	};

	String get_encoding_string(const Encoding p_encoding);

	EncoderType get_type();
	ByteType get_byte_type(const String &p_text, const uint32_t p_index);
	uint16_t get_unicode(const uint16_t p_code);
	WritingMode get_writing_mode();

	PDFEncoder();
	~PDFEncoder();

	void *_get_hpdf_encoder() const;
	void _set_hpdf_encoder(void *p_encoder);

protected:
	static void _bind_methods();

	void *_encoder;
};

VARIANT_ENUM_CAST(PDFEncoder::Encoding);
VARIANT_ENUM_CAST(PDFEncoder::EncoderType);
VARIANT_ENUM_CAST(PDFEncoder::ByteType);
VARIANT_ENUM_CAST(PDFEncoder::WritingMode);

#endif
