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
class PDFXObject;
class PDFDate;
class PDFDict;
class PDFEmbeddedFile;
class PDFExtGState;
class PDFShading;
class PDFU3D;
class PDFJavascript;
class PDF3DView;

class PDFDocument : public Reference {
	GDCLASS(PDFDocument, Reference);

public:
	enum CompressionMode {
		COMPRESSION_MODE_NONE = 0x00,
		COMPRESSION_MODE_TEXT = 0x01,
		COMPRESSION_MODE_IMAGE = 0x02,
		COMPRESSION_MODE_METADATA = 0x04,
		COMPRESSION_MODE_ALL = 0x0F,
		//COMPRESSION_MODE_BEST_COMPRESS = =0x10,
		//COMPRESSION_MODE_BEST_SPEED = =0x20,
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

	enum InfoType {
		/* date-time type parameters */
		INFO_CREATION_DATE = 0,
		INFO_MOD_DATE,

		/* string type parameters */
		INFO_AUTHOR,
		INFO_CREATOR,
		INFO_PRODUCER,
		INFO_TITLE,
		INFO_SUBJECT,
		INFO_KEYWORDS,
		INFO_TRAPPED,
		INFO_GTS_PDFX,
		INFO_EOF
	};

	enum PDFAType {
		NON_PDFA = -1,

		PDFA_1A = 0,
		PDFA_1B,
		PDFA_2A,
		PDFA_2B,
		PDFA_2U,
		PDFA_3A,
		PDFA_3B,
		PDFA_3U,
		PDFA_4,
		PDFA_4E,
		PDFA_4F
	};

	enum EncryptMode {
		ENCRYPT_MODE_R2 = 2,
		ENCRYPT_MODE_R3 = 3
	};

	enum ErrorCodes {
		EC_ARRAY_COUNT_ERR = 0x1001,
		EC_ARRAY_ITEM_NOT_FOUND = 0x1002,
		EC_ARRAY_ITEM_UNEXPECTED_TYPE = 0x1003,
		EC_BINARY_LENGTH_ERR = 0x1004,
		EC_CANNOT_GET_PALLET = 0x1005,
		EC_DICT_COUNT_ERR = 0x1007,
		EC_DICT_ITEM_NOT_FOUND = 0x1008,
		EC_DICT_ITEM_UNEXPECTED_TYPE = 0x1009,
		EC_DICT_STREAM_LENGTH_NOT_FOUND = 0x100A,
		EC_DOC_ENCRYPTDICT_NOT_FOUND = 0x100B,
		EC_DOC_INVALID_OBJECT = 0x100C,
		EC_DUPLICATE_REGISTRATION = 0x100E,
		EC_EXCEED_JWW_CODE_NUM_LIMIT = 0x100F,
		EC_ENCRYPT_INVALID_PASSWORD = 0x1011,
		EC_ERR_UNKNOWN_CLASS = 0x1013,
		EC_EXCEED_GSTATE_LIMIT = 0x1014,
		EC_FAILED_TO_ALLOC_MEM = 0x1015,
		EC_FILE_IO_ERROR = 0x1016,
		EC_FILE_OPEN_ERROR = 0x1017,
		EC_FONT_EXISTS = 0x1019,
		EC_FONT_INVALID_WIDTHS_TABLE = 0x101A,
		EC_INVALID_AFM_HEADER = 0x101B,
		EC_INVALID_ANNOTATION = 0x101C,
		EC_INVALID_BIT_PER_COMPONENT = 0x101E,
		EC_INVALID_CHAR_MATRICS_DATA = 0x101F,
		EC_INVALID_COLOR_SPACE = 0x1020,
		EC_INVALID_COMPRESSION_MODE = 0x1021,
		EC_INVALID_DATE_TIME = 0x1022,
		EC_INVALID_DESTINATION = 0x1023,
		EC_INVALID_DOCUMENT = 0x1025,
		EC_INVALID_DOCUMENT_STATE = 0x1026,
		EC_INVALID_ENCODER = 0x1027,
		EC_INVALID_ENCODER_TYPE = 0x1028,
		EC_INVALID_ENCODING_NAME = 0x102B,
		EC_INVALID_ENCRYPT_KEY_LEN = 0x102C,
		EC_INVALID_FONTDEF_DATA = 0x102D,
		EC_INVALID_FONTDEF_TYPE = 0x102E,
		EC_INVALID_FONT_NAME = 0x102F,
		EC_INVALID_IMAGE = 0x1030,
		EC_INVALID_JPEG_DATA = 0x1031,
		EC_INVALID_N_DATA = 0x1032,
		EC_INVALID_OBJECT = 0x1033,
		EC_INVALID_OBJ_ID = 0x1034,
		EC_INVALID_OPERATION = 0x1035,
		EC_INVALID_OUTLINE = 0x1036,
		EC_INVALID_PAGE = 0x1037,
		EC_INVALID_PAGES = 0x1038,
		EC_INVALID_PARAMETER = 0x1039,
		EC_INVALID_PNG_IMAGE = 0x103B,
		EC_INVALID_STREAM = 0x103C,
		EC_MISSING_FILE_NAME_ENTRY = 0x103D,
		EC_INVALID_TTC_FILE = 0x103F,
		EC_INVALID_TTC_INDEX = 0x1040,
		EC_INVALID_WX_DATA = 0x1041,
		EC_ITEM_NOT_FOUND = 0x1042,
		EC_LIBPNG_ERROR = 0x1043,
		EC_NAME_INVALID_VALUE = 0x1044,
		EC_NAME_OUT_OF_RANGE = 0x1045,
		EC_PAGE_INVALID_PARAM_COUNT = 0x1048,
		EC_PAGES_MISSING_KIDS_ENTRY = 0x1049,
		EC_PAGE_CANNOT_FIND_OBJECT = 0x104A,
		EC_PAGE_CANNOT_GET_ROOT_PAGES = 0x104B,
		EC_PAGE_CANNOT_RESTORE_GSTATE = 0x104C,
		EC_PAGE_CANNOT_SET_PARENT = 0x104D,
		EC_PAGE_FONT_NOT_FOUND = 0x104E,
		EC_PAGE_INVALID_FONT = 0x104F,
		EC_PAGE_INVALID_FONT_SIZE = 0x1050,
		EC_PAGE_INVALID_GMODE = 0x1051,
		EC_PAGE_INVALID_INDEX = 0x1052,
		EC_PAGE_INVALID_ROTATE_VALUE = 0x1053,
		EC_PAGE_INVALID_SIZE = 0x1054,
		EC_PAGE_INVALID_XOBJECT = 0x1055,
		EC_PAGE_OUT_OF_RANGE = 0x1056,
		EC_REAL_OUT_OF_RANGE = 0x1057,
		EC_STREAM_EOF = 0x1058,
		EC_STREAM_READLN_CONTINUE = 0x1059,
		EC_STRING_OUT_OF_RANGE = 0x105B,
		EC_THIS_FUNC_WAS_SKIPPED = 0x105C,
		EC_TTF_CANNOT_EMBEDDING_FONT = 0x105D,
		EC_TTF_INVALID_CMAP = 0x105E,
		EC_TTF_INVALID_FOMAT = 0x105F,
		EC_TTF_MISSING_TABLE = 0x1060,
		EC_UNSUPPORTED_FONT_TYPE = 0x1061,
		EC_UNSUPPORTED_FUNC = 0x1062,
		EC_UNSUPPORTED_JPEG_FORMAT = 0x1063,
		EC_UNSUPPORTED_TYPE1_FONT = 0x1064,
		EC_XREF_COUNT_ERR = 0x1065,
		EC_ZLIB_ERROR = 0x1066,
		EC_INVALID_PAGE_INDEX = 0x1067,
		EC_INVALID_URI = 0x1068,
		EC_PAGE_LAYOUT_OUT_OF_RANGE = 0x1069,
		EC_PAGE_MODE_OUT_OF_RANGE = 0x1070,
		EC_PAGE_NUM_STYLE_OUT_OF_RANGE = 0x1071,
		EC_ANNOT_INVALID_ICON = 0x1072,
		EC_ANNOT_INVALID_BORDER_STYLE = 0x1073,
		EC_PAGE_INVALID_DIRECTION = 0x1074,
		EC_INVALID_FONT = 0x1075,
		EC_PAGE_INSUFFICIENT_SPACE = 0x1076,
		EC_PAGE_INVALID_DISPLAY_TIME = 0x1077,
		EC_PAGE_INVALID_TRANSITION_TIME = 0x1078,
		EC_INVALID_PAGE_SLIDESHOW_TYPE = 0x1079,
		EC_EXT_GSTATE_OUT_OF_RANGE = 0x1080,
		EC_INVALID_EXT_GSTATE = 0x1081,
		EC_EXT_GSTATE_READ_ONLY = 0x1082,
		EC_INVALID_U3D_DATA = 0x1083,
		EC_NAME_CANNOT_GET_NAMES = 0x1084,
		EC_INVALID_ICC_COMPONENT_NUM = 0x1085,
		EC_PAGE_INVALID_BOUNDARY = 0x1086,
		EC_INVALID_SHADING_TYPE = 0x1088,
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

	Ref<PDFXObject> x_object_create_from_image(const Ref<PDFPage> &p_page, const Rect2 &p_rect, const Ref<PDFImage> &p_image, const bool p_zoom);
	Ref<PDFXObject> x_object_create_as_white_rect(const Ref<PDFPage> &p_page, const Rect2 &p_rect);

	uint32_t pdf_a_conformance_set(const PDFAType p_type);
	uint32_t pdf_a_xmp_extension(const String &p_xmp_description);

	uint32_t output_intents_append(const String &p_icc_name, const Ref<PDFDict> &p_icc_dict);

	Ref<PDFImage> image_load_png_from_mem(const PoolByteArray &p_data);
	Ref<PDFImage> image_load_png_from_file(const String &p_path);

	Ref<PDFImage> image_load_jpg_from_mem(const PoolByteArray &p_data);
	Ref<PDFImage> image_load_jpg_from_file(const String &p_path);

	Ref<PDFImage> image_load_raw_1_bit_image_from_mem(const PoolByteArray &p_data, const Vector2i &p_size, const uint32_t p_line_width, const bool p_black_is1, const bool p_top_is_first);

	Ref<PDFImage> image_load_raw_image_from_mem(const PoolByteArray &p_data, const Vector2i &p_size, const ColorSpace p_color_space, const uint32_t p_bits_per_component);
	Ref<PDFImage> image_load_raw_image_from_file(const String &p_path, const Vector2i &p_size, const ColorSpace p_color_space);

	Ref<PDFImage> image_create_pdf_from_image(const Ref<Image> &p_image);

	String info_attr_get(const InfoType p_info_type);
	uint32_t info_attr_set(const InfoType p_info_type, const String &p_value);
	uint32_t info_attr_date_set(const InfoType p_info_type, const Ref<PDFDate> &p_date);

	uint32_t password_set(const String &p_owner_passwd, const String &p_user_passwd);
	uint32_t permission_set(const uint32_t p_permission);
	uint32_t encryption_mode_set(const EncryptMode p_encryption_mode, const uint32_t p_key_length);

	Ref<PDFEmbeddedFile> attach_file(const String &p_file);

	Ref<PDFExtGState> ext_graphic_state_create();

	Ref<PDFShading> shading_new(const Vector2 &p_min, const Vector2 &p_max);

	Ref<PDFJavascript> javascript_load_from_mem(const String &p_data);
	Ref<PDFJavascript> javascript_load_from_file(const String &p_path);

	Ref<PDFU3D> u3d_load_from_mem(const PoolByteArray &p_data);
	Ref<PDFU3D> u3d_load_from_file(const String &p_path);

	Ref<PDF3DView> create_3d_view(const String &p_name);

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

	HPDF_Doc _get_hpdf_doc() const {
		return _doc;
	}

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
VARIANT_ENUM_CAST(PDFDocument::InfoType);
VARIANT_ENUM_CAST(PDFDocument::PDFAType);
VARIANT_ENUM_CAST(PDFDocument::EncryptMode);
VARIANT_ENUM_CAST(PDFDocument::ErrorCodes);

#endif
