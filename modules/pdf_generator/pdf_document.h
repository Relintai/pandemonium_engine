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
class PDFOutline;
class PDFEncoder;
class PDFDestination;

class PDFDocument : public Reference {
	GDCLASS(PDFDocument, Reference);

public:
	enum CompressionMode {
		COMPRESSION_MODE_NONE = 0x00,
		COMPRESSION_MODE_TEXT = 0x01,
		COMPRESSION_MODE_IMAGE = 0x02,
		COMPRESSION_MODE_METADATA = 0x04,
		COMPRESSION_MODE_ALL = 0x0F,
		//COMPRESSION_MODE_BEST_COMPRESS = 0x10,
		//COMPRESSION_MODE_BEST_SPEED = 0x20,
		COMPRESSION_MODE_MASK = 0xFF,
	};

	enum PageLayout {
		PAGE_LAYOUT_SINGLE = 0,
		PAGE_LAYOUT_ONE_COLUMN,
		PAGE_LAYOUT_TWO_COLUMN_LEFT,
		PAGE_LAYOUT_TWO_COLUMN_RIGHT,
		PAGE_LAYOUT_TWO_PAGE_LEFT,
		PAGE_LAYOUT_TWO_PAGE_RIGHT,
		PAGE_LAYOUT_EOF
	};

	enum PageMode {
		PAGE_MODE_USE_NONE = 0,
		PAGE_MODE_USE_OUTLINE,
		PAGE_MODE_USE_THUMBS,
		PAGE_MODE_FULL_SCREEN,
		//PAGE_MODE_USE_OC,
		//PAGE_MODE_USE_ATTACHMENTS,
		PAGE_MODE_EOF
	};

	enum PageNumStyle {
		PAGE_NUM_STYLE_DECIMAL = 0,
		PAGE_NUM_STYLE_UPPER_ROMAN,
		PAGE_NUM_STYLE_LOWER_ROMAN,
		PAGE_NUM_STYLE_UPPER_LETTERS,
		PAGE_NUM_STYLE_LOWER_LETTERS,
		PAGE_NUM_STYLE_EOF
	};

	enum ColorSpace {
		COLOR_SPACE_DEVICE_GRAY = 0,
		COLOR_SPACE_DEVICE_RGB,
		COLOR_SPACE_DEVICE_CMYK,
		COLOR_SPACE_CAL_GRAY,
		COLOR_SPACE_CAL_RGB,
		COLOR_SPACE_LAB,
		COLOR_SPACE_ICC_BASED,
		COLOR_SPACE_SEPARATION,
		COLOR_SPACE_DEVICE_N,
		COLOR_SPACE_INDEXED,
		COLOR_SPACE_PATTERN,
		COLOR_SPACE_EOF
	};

	uint32_t viewer_preference_get() const;
	void viewer_preference_set(const uint32_t p_preference);

	uint32_t document_new();
	void document_free();
	bool document_has();
	void document_free_all();

	Ref<PDFPage> page_get_current();
	Ref<PDFPage> page_get_index(const uint32_t p_index);
	Ref<PDFPage> page_add();
	Ref<PDFPage> page_insert(const Ref<PDFPage> &p_page);

	PageLayout page_get_layout() const;
	void page_set_layout(const PageLayout p_layout);

	PageMode page_get_mode() const;
	void page_set_mode(const PageMode p_mode);

	uint32_t page_add_label(const uint32_t p_page_num, const PageNumStyle p_style, const uint32_t p_first_page, const String &p_prefix = String());

	uint32_t pages_set_configuration(const uint32_t p_page_per_pages);

	uint32_t compression_mode_set(const uint32_t p_mode);

	Ref<PDFFont> font_get(const String &p_font_name, const String &p_encoding_name = String());

	String font_load_type_1_from_file(const String &p_afm_file_name, const String &p_data_file_name);
	String font_load_ttf_from_file(const String &p_file_name, const bool p_embed_into_document);
	String font_load_ttf_from_collection_file(const String &p_file_name, const int p_index, const bool p_embed_into_document);
	String font_load_ttf_from_mem(const PoolByteArray &p_data, const bool p_embed_into_document);

	uint32_t fonts_use_jp();
	uint32_t fonts_use_kr();
	uint32_t fonts_use_cns();
	uint32_t fonts_use_cnt();

	Ref<PDFOutline> outline_create(const Ref<PDFOutline> &p_parent, const String &p_title, const Ref<PDFEncoder> &p_encoder);

	uint32_t open_action_set(const Ref<PDFDestination> &p_destination);

	Ref<PDFEncoder> encoder_get(const String &p_encoding_name);

	Ref<PDFEncoder> encoder_current_get();
	uint32_t encoder_current_set(const String &p_encoding_name);

	uint32_t encodings_use_jp();
	uint32_t encodings_use_kr();
	uint32_t encodings_use_cns();
	uint32_t encodings_use_cnt();
	uint32_t encodings_use_utf();

#if 0
	/*----- XObject ------------------------------------------------------------*/

	HPDF_EXPORT(HPDF_XObject)
	HPDF_Page_CreateXObjectFromImage(HPDF_Doc pdf,
			HPDF_Page page,
			HPDF_Rect rect,
			HPDF_Image image,
			HPDF_BOOL zoom);

	HPDF_EXPORT(HPDF_XObject)
	HPDF_Page_CreateXObjectAsWhiteRect(HPDF_Doc pdf,
			HPDF_Page page,
			HPDF_Rect rect);

#endif

#if 0
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetPDFAConformance(HPDF_Doc pdf,
			HPDF_PDFAType pdfa_type);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_AddPDFAXmpExtension(HPDF_Doc pdf,
			const char *xmp_description);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_AppendOutputIntents(HPDF_Doc pdf, const char *iccname, HPDF_Dict iccdict);
#endif

	Ref<PDFImage> image_load_png_from_mem(const PoolByteArray &p_data);
	Ref<PDFImage> image_load_png_from_file(const String &p_path);

	Ref<PDFImage> image_load_jpg_from_mem(const PoolByteArray &p_data);
	Ref<PDFImage> image_load_jpg_from_file(const String &p_path);

	Ref<PDFImage> image_load_u3d_from_mem(const PoolByteArray &p_data);
	Ref<PDFImage> image_load_u3d_from_file(const String &p_path);

	Ref<PDFImage> image_load_raw_1_bit_image_from_mem(const PoolByteArray &p_data, const Vector2i &p_size, const uint32_t p_line_width, const bool p_black_is1, const bool p_top_is_first);

	Ref<PDFImage> image_load_raw_image_from_mem(const PoolByteArray &p_data, const Vector2i &p_size, const ColorSpace p_color_space, const uint32_t p_bits_per_component);
	Ref<PDFImage> image_load_raw_image_from_file(const String &p_path, const Vector2i &p_size, const ColorSpace p_color_space);

	Ref<PDFImage> image_create_pdf_from_image(const Ref<Image> &p_image);

#if 0
	/*----- info dictionary ----------------------------------------------------*/

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetInfoAttr(HPDF_Doc pdf,
			HPDF_InfoType type,
			const char *value);

	HPDF_EXPORT(const char *)
	HPDF_GetInfoAttr(HPDF_Doc pdf,
			HPDF_InfoType type);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetInfoDateAttr(HPDF_Doc pdf,
			HPDF_InfoType type,
			HPDF_Date value);

#endif

#if 0
	/*----- encryption ---------------------------------------------------------*/

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetPassword(HPDF_Doc pdf,
			const char *owner_passwd,
			const char *user_passwd);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetPermission(HPDF_Doc pdf,
			HPDF_UINT permission);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetEncryptionMode(HPDF_Doc pdf,
			HPDF_EncryptMode mode,
			HPDF_UINT key_len);

#endif

#if 0
	/*----- attachments -------------------------------------------------------*/

	HPDF_EXPORT(HPDF_EmbeddedFile)
	HPDF_AttachFile(HPDF_Doc pdf,
			const char *file);

#endif

#if 0
	/*----- extended graphics state --------------------------------------------*/

	HPDF_EXPORT(HPDF_ExtGState)
	HPDF_CreateExtGState(HPDF_Doc pdf);

#endif

#if 0
	/*--- Shading patterns ---------------------------------------------------*/

	/* Notes for docs:
	 * - ShadingType must be HPDF_SHADING_FREE_FORM_TRIANGLE_MESH (the only
	 *   defined option...)
	 * - colorSpace must be HPDF_CS_DEVICE_RGB for now.
	 */
	HPDF_EXPORT(HPDF_Shading)
	HPDF_Shading_New(HPDF_Doc pdf,
			HPDF_ShadingType type,
			HPDF_ColorSpace colorSpace,
			HPDF_REAL xMin, HPDF_REAL xMax,
			HPDF_REAL yMin, HPDF_REAL yMax);

#endif

#if 0
	Likely not needed

	HPDF_EXPORT(HPDF_FontDef)
	HPDF_GetTTFontDefFromFile(HPDF_Doc pdf,
			const char *file_name,
			HPDF_BOOL embedding);
#endif

	PoolByteArray save_to_mem();
	uint32_t save_to_file(const String &p_file);

	uint32_t get_status();
	uint32_t get_error_no();
	uint32_t get_error_detail();
	void reset_error();

	PDFDocument();
	~PDFDocument();

protected:
	static void _bind_methods();

	HPDF_Doc _doc;

	uint32_t _status;
};

VARIANT_ENUM_CAST(PDFDocument::CompressionMode);
VARIANT_ENUM_CAST(PDFDocument::PageLayout);
VARIANT_ENUM_CAST(PDFDocument::PageMode);
VARIANT_ENUM_CAST(PDFDocument::PageNumStyle);
VARIANT_ENUM_CAST(PDFDocument::ColorSpace);

#endif
