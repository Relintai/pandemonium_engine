/*************************************************************************/
/*  pdf_encoder.cpp                                                      */
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

#include "pdf_encoder.h"

#include "hpdf.h"
#include "hpdf_doc.h"
#include "hpdf_pages.h"

String PDFEncoder::get_encoding_string(const Encoding p_encoding) {
	switch (p_encoding) {
		case ENCODING_FONT_SPECIFIC:
			return String("FontSpecific");
		case ENCODING_STANDARD:
			return String("StandardEncoding");
		case ENCODING_MAC_ROMAN:
			return String("MacRomanEncoding");
		case ENCODING_WIN_ANSI:
			return String("WinAnsiEncoding");
		case ENCODING_ISO8859_2:
			return String("ISO8859-2");
		case ENCODING_ISO8859_3:
			return String("ISO8859-3");
		case ENCODING_ISO8859_4:
			return String("ISO8859-4");
		case ENCODING_ISO8859_5:
			return String("ISO8859-5");
		case ENCODING_ISO8859_6:
			return String("ISO8859-6");
		case ENCODING_ISO8859_7:
			return String("ISO8859-7");
		case ENCODING_ISO8859_8:
			return String("ISO8859-8");
		case ENCODING_ISO8859_9:
			return String("ISO8859-9");
		case ENCODING_ISO8859_10:
			return String("ISO8859-10");
		case ENCODING_ISO8859_11:
			return String("ISO8859-11");
		case ENCODING_ISO8859_13:
			return String("ISO8859-13");
		case ENCODING_ISO8859_14:
			return String("ISO8859-14");
		case ENCODING_ISO8859_15:
			return String("ISO8859-15");
		case ENCODING_ISO8859_16:
			return String("ISO8859-16");
		case ENCODING_CP1250:
			return String("CP1250");
		case ENCODING_CP1251:
			return String("CP1251");
		case ENCODING_CP1252:
			return String("CP1252");
		case ENCODING_CP1253:
			return String("CP1253");
		case ENCODING_CP1254:
			return String("CP1254");
		case ENCODING_CP1255:
			return String("CP1255");
		case ENCODING_CP1256:
			return String("CP1256");
		case ENCODING_CP1257:
			return String("CP1257");
		case ENCODING_CP1258:
			return String("CP1258");
		case ENCODING_KOI8_R:
			return String("KOI8-R");
		default:
			return String();
	}
}

PDFEncoder::EncoderType PDFEncoder::get_type() {
	return static_cast<EncoderType>(HPDF_Encoder_GetType((HPDF_Encoder)_encoder));
}
PDFEncoder::ByteType PDFEncoder::get_byte_type(const String &p_text, const uint32_t p_index) {
	return static_cast<ByteType>(HPDF_Encoder_GetByteType((HPDF_Encoder)_encoder, p_text.utf8().get_data(), p_index));
}
uint16_t PDFEncoder::get_unicode(const uint16_t p_code) {
	return HPDF_Encoder_GetUnicode((HPDF_Encoder)_encoder, p_code);
}
PDFEncoder::WritingMode PDFEncoder::get_writing_mode() {
	return static_cast<WritingMode>(HPDF_Encoder_GetWritingMode((HPDF_Encoder)_encoder));
}

PDFEncoder::PDFEncoder() {
	_encoder = NULL;
}

PDFEncoder::~PDFEncoder() {
}

void *PDFEncoder::_get_hpdf_encoder() const {
	return _encoder;
}

void PDFEncoder::_set_hpdf_encoder(void *p_encoder) {
	_encoder = p_encoder;
}

void PDFEncoder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_encoding_string", "encoding"), &PDFEncoder::get_encoding_string);

	ClassDB::bind_method(D_METHOD("get_type"), &PDFEncoder::get_type);
	ClassDB::bind_method(D_METHOD("get_byte_type", "text", "index"), &PDFEncoder::get_byte_type);
	ClassDB::bind_method(D_METHOD("get_unicode", "code"), &PDFEncoder::get_unicode);
	ClassDB::bind_method(D_METHOD("get_writing_mode"), &PDFEncoder::get_writing_mode);

	BIND_ENUM_CONSTANT(ENCODING_FONT_SPECIFIC);
	BIND_ENUM_CONSTANT(ENCODING_STANDARD);
	BIND_ENUM_CONSTANT(ENCODING_MAC_ROMAN);
	BIND_ENUM_CONSTANT(ENCODING_WIN_ANSI);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_2);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_3);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_4);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_5);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_6);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_7);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_8);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_9);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_10);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_11);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_13);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_14);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_15);
	BIND_ENUM_CONSTANT(ENCODING_ISO8859_16);
	BIND_ENUM_CONSTANT(ENCODING_CP1250);
	BIND_ENUM_CONSTANT(ENCODING_CP1251);
	BIND_ENUM_CONSTANT(ENCODING_CP1252);
	BIND_ENUM_CONSTANT(ENCODING_CP1253);
	BIND_ENUM_CONSTANT(ENCODING_CP1254);
	BIND_ENUM_CONSTANT(ENCODING_CP1255);
	BIND_ENUM_CONSTANT(ENCODING_CP1256);
	BIND_ENUM_CONSTANT(ENCODING_CP1257);
	BIND_ENUM_CONSTANT(ENCODING_CP1258);
	BIND_ENUM_CONSTANT(ENCODING_KOI8_R);

	BIND_ENUM_CONSTANT(ENCODER_TYPE_SINGLE_BYTE);
	BIND_ENUM_CONSTANT(ENCODER_TYPE_DOUBLE_BYTE);
	BIND_ENUM_CONSTANT(ENCODER_TYPE_UNINITIALIZED);
	BIND_ENUM_CONSTANT(ENCODER_UNKNOWN);

	BIND_ENUM_CONSTANT(BYTE_TYPE_SINGLE);
	BIND_ENUM_CONSTANT(BYTE_TYPE_LEAD);
	BIND_ENUM_CONSTANT(BYTE_TYPE_TRAIL);
	BIND_ENUM_CONSTANT(BYTE_TYPE_UNKNOWN);

	BIND_ENUM_CONSTANT(WRITING_MODE_HORIZONTAL);
	BIND_ENUM_CONSTANT(WRITING_MODE_VERTICAL);
	BIND_ENUM_CONSTANT(WRITING_MODE_EOF);
}
