#ifndef PDF_DOCUMENT_H
#define PDF_DOCUMENT_H

/*************************************************************************/
/*  pdf_document.h                                                       */
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

#include "core/string/ustring.h"

#include "core/object/reference.h"

struct _HPDF_Doc_Rec;
typedef struct _HPDF_Doc_Rec *HPDF_Doc;

class PDFPage;
class PDFFont;
class PDFImage;
class Image;

class PDFDocument : public Reference {
	GDCLASS(PDFDocument, Reference);

public:
	uint32_t new_document();
	void free_document();
	bool has_document();
	void free_document_all();

	Ref<PDFPage> page_get_current();
	Ref<PDFPage> page_get_index(const uint32_t p_index);
	Ref<PDFPage> page_add();
	Ref<PDFPage> page_insert(const Ref<PDFPage> &p_page);

	uint32_t set_pages_configuration(const uint32_t p_page_per_pages);
	//get_page_by_index();

	Ref<PDFFont> get_font(const String &p_font_name, const String &p_encoding_name = String());

	String load_type_1_font_from_file(const String &p_afm_file_name, const String &p_data_file_name);
	String load_ttf_font_from_file(const String &p_file_name, const bool p_embed_into_document);
	String load_ttf_font_from_collection_file(const String &p_file_name, const int p_index, const bool p_embed_into_document);
	String load_ttf_font_from_mem(const PoolByteArray &p_data, const bool p_embed_into_document);

	uint32_t use_jp_fonts();
	uint32_t use_kr_fonts();
	uint32_t use_cns_fonts();
	uint32_t use_cnt_fonts();

#if 0
	HPDF_EXPORT(HPDF_FontDef)
	HPDF_GetTTFontDefFromFile(HPDF_Doc pdf,
			const char *file_name,
			HPDF_BOOL embedding);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_AddPageLabel(HPDF_Doc pdf,
			HPDF_UINT page_num,
			HPDF_PageNumStyle style,
			HPDF_UINT first_page,
			const char *prefix);
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

	Ref<PDFImage> load_png_image_from_mem(const PoolByteArray &p_data);
	Ref<PDFImage> load_png_image_from_file(const String &p_path);
	Ref<PDFImage> load_jpg_image_from_mem(const PoolByteArray &p_data);
	Ref<PDFImage> load_jpg_image_from_file(const String &p_path);
	Ref<PDFImage> create_pdf_image_from_image(const Ref<Image> &p_image);

#if 0

	HPDF_EXPORT(HPDF_Image)
	HPDF_LoadU3DFromFile(HPDF_Doc pdf,
			const char *filename);

	HPDF_EXPORT(HPDF_Image)
	HPDF_LoadU3DFromMem(HPDF_Doc pdf,
			const HPDF_BYTE *buffer,
			HPDF_UINT size);

	HPDF_EXPORT(HPDF_Image)
	HPDF_Image_LoadRaw1BitImageFromMem(HPDF_Doc pdf,
			const HPDF_BYTE *buf,
			HPDF_UINT width,
			HPDF_UINT height,
			HPDF_UINT line_width,
			HPDF_BOOL black_is1,
			HPDF_BOOL top_is_first);

	HPDF_EXPORT(HPDF_Image)
	HPDF_LoadRawImageFromFile(HPDF_Doc pdf,
			const char *filename,
			HPDF_UINT width,
			HPDF_UINT height,
			HPDF_ColorSpace color_space);

#endif

	PoolByteArray get_contents();
	uint32_t save_to_file(const String &p_file);

	uint32_t get_status();
	uint32_t get_error_no();
	uint32_t get_error_detail_no();
	void reset_error();

	PDFDocument();
	~PDFDocument();

protected:
	static void _bind_methods();

	HPDF_Doc _doc;

	uint32_t _status;
};

#endif
