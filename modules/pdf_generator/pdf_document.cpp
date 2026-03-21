/*************************************************************************/
/*  pdf_document.cpp                                                     */
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

#include "pdf_document.h"

#include "core/io/image.h"
#include "core/log/logger.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"

#include "pdf_date.h"
#include "pdf_destination.h"
#include "pdf_dict.h"
#include "pdf_embedded_file.h"
#include "pdf_encoder.h"
#include "pdf_ext_g_state.h"
#include "pdf_font.h"
#include "pdf_image.h"
#include "pdf_outline.h"
#include "pdf_page.h"
#include "pdf_shading.h"
#include "pdf_x_object.h"

#include "hpdf.h"
#include "hpdf_doc.h"

uint32_t PDFDocument::viewer_preference_get() const {
	return HPDF_GetViewerPreference(_doc);
}
void PDFDocument::viewer_preference_set(const uint32_t p_preference) {
	_status = HPDF_SetViewerPreference(_doc, p_preference);
}

uint32_t PDFDocument::document_new() {
	_status = HPDF_NewDoc(_doc);
	return _status;
}
void PDFDocument::document_free() {
	HPDF_FreeDoc(_doc);
}
bool PDFDocument::document_has() {
	return HPDF_HasDoc(_doc);
}
void PDFDocument::document_free_all() {
	HPDF_FreeDocAll(_doc);
}

Ref<PDFPage> PDFDocument::page_get_current() {
	HPDF_Page hpdf_page = HPDF_GetCurrentPage(_doc);

	if (!hpdf_page) {
		return Ref<PDFPage>();
	}

	Ref<PDFPage> page;
	page.instance();

	page->_set_hpdf_page(hpdf_page);

	return page;
}
Ref<PDFPage> PDFDocument::page_get_index(const uint32_t p_index) {
	HPDF_Page hpdf_page = HPDF_GetPageByIndex(_doc, p_index);

	if (!hpdf_page) {
		return Ref<PDFPage>();
	}

	Ref<PDFPage> page;
	page.instance();

	page->_set_hpdf_page(hpdf_page);

	return page;
}
Ref<PDFPage> PDFDocument::page_add() {
	HPDF_Page hpdf_page = HPDF_AddPage(_doc);

	if (!hpdf_page) {
		return Ref<PDFPage>();
	}

	Ref<PDFPage> page;
	page.instance();

	page->_set_hpdf_page(hpdf_page);

	return page;
}
Ref<PDFPage> PDFDocument::page_insert(const Ref<PDFPage> &p_page) {
	if (!p_page.is_valid()) {
		return Ref<PDFPage>();
	}

	HPDF_Page hpdf_page = HPDF_InsertPage(_doc, (HPDF_Page)p_page->_get_hpdf_page());

	if (!hpdf_page) {
		return Ref<PDFPage>();
	}

	Ref<PDFPage> page;
	page.instance();

	page->_set_hpdf_page(hpdf_page);

	return page;
}

PDFDocument::PageLayout PDFDocument::page_get_layout() const {
	HPDF_PageLayout layout = HPDF_GetPageLayout(_doc);

	return static_cast<PageLayout>(layout);
}
void PDFDocument::page_set_layout(const PageLayout p_layout) {
	HPDF_PageLayout layout = static_cast<HPDF_PageLayout>(p_layout);

	_status = HPDF_SetPageLayout(_doc, layout);
}

PDFDocument::PageMode PDFDocument::page_get_mode() const {
	HPDF_PageMode mode = HPDF_GetPageMode(_doc);

	return static_cast<PageMode>(mode);
}
void PDFDocument::page_set_mode(const PageMode p_mode) {
	HPDF_PageMode mode = static_cast<HPDF_PageMode>(p_mode);

	_status = HPDF_SetPageMode(_doc, mode);
}

uint32_t PDFDocument::page_add_label(const uint32_t p_page_num, const PageNumStyle p_style, const uint32_t p_first_page, const String &p_prefix) {
	HPDF_PageNumStyle style = static_cast<HPDF_PageNumStyle>(p_style);

	if (p_prefix.empty()) {
		_status = HPDF_AddPageLabel(_doc, p_page_num, style, p_first_page, NULL);
	} else {
		_status = HPDF_AddPageLabel(_doc, p_page_num, style, p_first_page, p_prefix.utf8().get_data());
	}

	return _status;
}

uint32_t PDFDocument::pages_set_configuration(const uint32_t p_page_per_pages) {
	_status = HPDF_SetPagesConfiguration(_doc, (HPDF_UINT)p_page_per_pages);
	return _status;
}

uint32_t PDFDocument::compression_mode_set(const uint32_t p_mode) {
	_status = HPDF_SetCompressionMode(_doc, p_mode);

	return _status;
}

Ref<PDFFont> PDFDocument::font_get(const String &p_font_name, const String &p_encoding_name) {
	HPDF_Font hpdf_font = NULL;

	if (p_encoding_name.empty()) {
		hpdf_font = HPDF_GetFont(_doc, p_font_name.utf8().get_data(), NULL);
	} else {
		hpdf_font = HPDF_GetFont(_doc, p_font_name.utf8().get_data(), p_encoding_name.utf8().get_data());
	}

	if (!hpdf_font) {
		return Ref<PDFFont>();
	}

	Ref<PDFFont> font;
	font.instance();

	font->_set_hpdf_font(hpdf_font);

	return font;
}

String PDFDocument::font_load_type_1_from_file(const String &p_afm_file_name, const String &p_data_file_name) {
	String afm_file_name = FileAccess::get_filesystem_abspath_for(p_afm_file_name);
	String data_file_name = FileAccess::get_filesystem_abspath_for(p_data_file_name);

	String font_name = String::utf8(HPDF_LoadType1FontFromFile(_doc, afm_file_name.utf8().get_data(), data_file_name.utf8().get_data()));

	return font_name;
}
String PDFDocument::font_load_ttf_from_file(const String &p_file_name, const bool p_embed_into_document) {
	String file_name = FileAccess::get_filesystem_abspath_for(p_file_name);

	String font_name = String::utf8(HPDF_LoadTTFontFromFile(_doc, file_name.utf8().get_data(), p_embed_into_document));

	return font_name;
}
String PDFDocument::font_load_ttf_from_collection_file(const String &p_file_name, const int p_index, const bool p_embed_into_document) {
	String file_name = FileAccess::get_filesystem_abspath_for(p_file_name);

	String font_name = String::utf8(HPDF_LoadTTFontFromFile2(_doc, file_name.utf8().get_data(), p_index, p_embed_into_document));

	return font_name;
}
String PDFDocument::font_load_ttf_from_mem(const PoolByteArray &p_data, const bool p_embed_into_document) {
	PoolByteArray::Read r = p_data.read();

	String font_name = String::utf8(HPDF_LoadTTFontFromMemory(_doc, r.ptr(), p_data.size(), p_embed_into_document));

	return font_name;
}

uint32_t PDFDocument::fonts_use_jp() {
	_status = HPDF_UseJPFonts(_doc);

	return _status;
}
uint32_t PDFDocument::fonts_use_kr() {
	_status = HPDF_UseKRFonts(_doc);

	return _status;
}
uint32_t PDFDocument::fonts_use_cns() {
	_status = HPDF_UseCNSFonts(_doc);

	return _status;
}
uint32_t PDFDocument::fonts_use_cnt() {
	_status = HPDF_UseCNTFonts(_doc);

	return _status;
}

/*----- outline ------------------------------------------------------------*/

Ref<PDFOutline> PDFDocument::outline_create(const Ref<PDFOutline> &p_parent, const String &p_title, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Outline parent = NULL;

	if (p_parent.is_valid()) {
		parent = (HPDF_Outline)p_parent->_get_hpdf_outline();
	}

	HPDF_Encoder encoder = NULL;

	if (p_encoder.is_valid()) {
		encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Outline outline = HPDF_CreateOutline(_doc, parent, p_title.utf8().get_data(), encoder);

	if (!outline) {
		return Ref<PDFEncoder>();
	}

	Ref<PDFOutline> pdf_outline;
	pdf_outline.instance();

	pdf_outline->_set_hpdf_outline(encoder);

	return pdf_outline;
}

/*----- destination --------------------------------------------------------*/

uint32_t PDFDocument::open_action_set(const Ref<PDFDestination> &p_destination) {
	HPDF_Destination dest = NULL;

	if (p_destination.is_valid()) {
		dest = (HPDF_Destination)p_destination->_get_hpdf_destination();
	}

	_status = HPDF_SetOpenAction(_doc, dest);

	return _status;
}

/*----- encoder ------------------------------------------------------------*/

Ref<PDFEncoder> PDFDocument::encoder_get(const String &p_encoding_name) {
	HPDF_Encoder encoder = HPDF_GetEncoder(_doc, p_encoding_name.utf8().get_data());

	if (!encoder) {
		return Ref<PDFEncoder>();
	}

	Ref<PDFEncoder> pdf_encoder;
	pdf_encoder.instance();

	pdf_encoder->_set_hpdf_encoder(encoder);

	return pdf_encoder;
}

Ref<PDFEncoder> PDFDocument::encoder_current_get() {
	HPDF_Encoder encoder = HPDF_GetCurrentEncoder(_doc);

	if (!encoder) {
		return Ref<PDFEncoder>();
	}

	Ref<PDFEncoder> pdf_encoder;
	pdf_encoder.instance();

	pdf_encoder->_set_hpdf_encoder(encoder);

	return pdf_encoder;
}
uint32_t PDFDocument::encoder_current_set(const String &p_encoding_name) {
	_status = HPDF_SetCurrentEncoder(_doc, p_encoding_name.utf8().get_data());

	return _status;
}

uint32_t PDFDocument::encodings_use_jp() {
	_status = HPDF_UseJPEncodings(_doc);

	return _status;
}
uint32_t PDFDocument::encodings_use_kr() {
	_status = HPDF_UseKREncodings(_doc);

	return _status;
}
uint32_t PDFDocument::encodings_use_cns() {
	_status = HPDF_UseCNSEncodings(_doc);

	return _status;
}
uint32_t PDFDocument::encodings_use_cnt() {
	_status = HPDF_UseCNTEncodings(_doc);

	return _status;
}
uint32_t PDFDocument::encodings_use_utf() {
	_status = HPDF_UseUTFEncodings(_doc);

	return _status;
}

/*----- XObject ------------------------------------------------------------*/

Ref<PDFXObject> PDFDocument::x_object_create_from_image(const Ref<PDFPage> &p_page, const Rect2 &p_rect, const Ref<PDFImage> &p_image, const bool p_zoom) {
	HPDF_Page page = NULL;

	if (p_page.is_valid()) {
		page = (HPDF_Page)p_page->_get_hpdf_page();
	}

	HPDF_Rect rect;
	Vector2 end = p_rect.get_end();
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	rect.right = end.x;
	rect.bottom = end.y;

	HPDF_Image image = NULL;

	if (p_image.is_valid()) {
		image = (HPDF_Image)p_image->_get_hpdf_image();
	}

	HPDF_XObject hpdf_xobject = HPDF_Page_CreateXObjectFromImage(_doc, page, rect, image, p_zoom);

	if (!hpdf_xobject) {
		return Ref<PDFXObject>();
	}

	Ref<PDFXObject> pdf_xobject;
	pdf_xobject.instance();

	pdf_xobject->_set_hpdf_x_object(hpdf_xobject);

	return pdf_xobject;
}
Ref<PDFXObject> PDFDocument::x_object_create_as_white_rect(const Ref<PDFPage> &p_page, const Rect2 &p_rect) {
	HPDF_Page page = NULL;

	if (p_page.is_valid()) {
		page = (HPDF_Page)p_page->_get_hpdf_page();
	}

	HPDF_Rect rect;
	Vector2 end = p_rect.get_end();
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	rect.right = end.x;
	rect.bottom = end.y;

	HPDF_XObject hpdf_xobject = HPDF_Page_CreateXObjectAsWhiteRect(_doc, page, rect);

	if (!hpdf_xobject) {
		return Ref<PDFXObject>();
	}

	Ref<PDFXObject> pdf_xobject;
	pdf_xobject.instance();

	pdf_xobject->_set_hpdf_x_object(hpdf_xobject);

	return pdf_xobject;
}

uint32_t PDFDocument::pdf_a_conformance_set(const PDFAType p_type) {
	HPDF_PDFAType type = static_cast<HPDF_PDFAType>(p_type);

	_status = HPDF_SetPDFAConformance(_doc, type);

	return _status;
}
uint32_t PDFDocument::pdf_a_xmp_extension(const String &p_xmp_description) {
	_status = HPDF_AddPDFAXmpExtension(_doc, p_xmp_description.utf8().get_data());

	return _status;
}

uint32_t PDFDocument::output_intents_append(const String &p_icc_name, const Ref<PDFDict> &p_icc_dict) {
	HPDF_Dict iccdict = NULL;

	if (p_icc_dict.is_valid()) {
		iccdict = (HPDF_Dict)p_icc_dict->_get_hpdf_dict();
	}

	_status = HPDF_AppendOutputIntents(_doc, p_icc_name.utf8().get_data(), iccdict);

	return _status;
}

Ref<PDFImage> PDFDocument::image_load_png_from_mem(const PoolByteArray &p_data) {
	PoolByteArray::Read r = p_data.read();

	HPDF_Image hpdf_image = HPDF_LoadPngImageFromMem(_doc, r.ptr(), p_data.size());

	if (!hpdf_image) {
		return Ref<PDFImage>();
	}

	Ref<PDFImage> image;
	image.instance();

	image->_set_hpdf_image(hpdf_image);

	return image;
}
Ref<PDFImage> PDFDocument::image_load_png_from_file(const String &p_path) {
	String abs_path = FileAccess::get_filesystem_abspath_for(p_path);

	HPDF_Font hpdf_image = HPDF_LoadPngImageFromFile(_doc, abs_path.utf8().get_data());

	if (!hpdf_image) {
		return Ref<PDFImage>();
	}

	Ref<PDFImage> image;
	image.instance();

	image->_set_hpdf_image(hpdf_image);

	return image;
}
Ref<PDFImage> PDFDocument::image_load_jpg_from_mem(const PoolByteArray &p_data) {
	PoolByteArray::Read r = p_data.read();

	HPDF_Image hpdf_image = HPDF_LoadJpegImageFromMem(_doc, r.ptr(), p_data.size());

	if (!hpdf_image) {
		return Ref<PDFImage>();
	}

	Ref<PDFImage> image;
	image.instance();

	image->_set_hpdf_image(hpdf_image);

	return image;
}
Ref<PDFImage> PDFDocument::image_load_jpg_from_file(const String &p_path) {
	String abs_path = FileAccess::get_filesystem_abspath_for(p_path);

	HPDF_Font hpdf_image = HPDF_LoadJpegImageFromFile(_doc, abs_path.utf8().get_data());

	if (!hpdf_image) {
		return Ref<PDFImage>();
	}

	Ref<PDFImage> image;
	image.instance();

	image->_set_hpdf_image(hpdf_image);

	return image;
}

Ref<PDFImage> PDFDocument::image_load_u3d_from_mem(const PoolByteArray &p_data) {
	PoolByteArray::Read r = p_data.read();

	HPDF_Image hpdf_image = HPDF_LoadU3DFromMem(_doc, r.ptr(), p_data.size());

	if (!hpdf_image) {
		return Ref<PDFImage>();
	}

	Ref<PDFImage> image;
	image.instance();

	image->_set_hpdf_image(hpdf_image);

	return image;
}
Ref<PDFImage> PDFDocument::image_load_u3d_from_file(const String &p_path) {
	String abs_path = FileAccess::get_filesystem_abspath_for(p_path);

	HPDF_Font hpdf_image = HPDF_LoadU3DFromFile(_doc, abs_path.utf8().get_data());

	if (!hpdf_image) {
		return Ref<PDFImage>();
	}

	Ref<PDFImage> image;
	image.instance();

	image->_set_hpdf_image(hpdf_image);

	return image;
}

Ref<PDFImage> PDFDocument::image_load_raw_1_bit_image_from_mem(const PoolByteArray &p_data, const Vector2i &p_size, const uint32_t p_line_width, const bool p_black_is1, const bool p_top_is_first) {
	PoolByteArray::Read r = p_data.read();

	HPDF_Image hpdf_image = HPDF_Image_LoadRaw1BitImageFromMem(_doc, r.ptr(), p_size.width, p_size.height, p_line_width, p_black_is1, p_top_is_first);

	if (!hpdf_image) {
		return Ref<PDFImage>();
	}

	Ref<PDFImage> image;
	image.instance();

	image->_set_hpdf_image(hpdf_image);

	return image;
}

Ref<PDFImage> PDFDocument::image_load_raw_image_from_mem(const PoolByteArray &p_data, const Vector2i &p_size, const ColorSpace p_color_space, const uint32_t p_bits_per_component) {
	PoolByteArray::Read r = p_data.read();

	HPDF_ColorSpace color_space = static_cast<HPDF_ColorSpace>(p_color_space);

	HPDF_Image hpdf_image = HPDF_LoadRawImageFromMem(_doc, r.ptr(), p_size.width, p_size.height, color_space, p_bits_per_component);

	if (!hpdf_image) {
		return Ref<PDFImage>();
	}

	Ref<PDFImage> image;
	image.instance();

	image->_set_hpdf_image(hpdf_image);

	return image;
}
Ref<PDFImage> PDFDocument::image_load_raw_image_from_file(const String &p_path, const Vector2i &p_size, const ColorSpace p_color_space) {
	String abs_path = FileAccess::get_filesystem_abspath_for(p_path);

	HPDF_ColorSpace color_space = static_cast<HPDF_ColorSpace>(p_color_space);

	HPDF_Font hpdf_image = HPDF_LoadRawImageFromFile(_doc, abs_path.utf8().get_data(), p_size.width, p_size.height, color_space);

	if (!hpdf_image) {
		return Ref<PDFImage>();
	}

	Ref<PDFImage> image;
	image.instance();

	image->_set_hpdf_image(hpdf_image);

	return image;
}
Ref<PDFImage> PDFDocument::image_create_pdf_from_image(const Ref<Image> &p_image) {
	if (!p_image.is_valid()) {
		return Ref<PDFImage>();
	}

	Ref<Image> img = p_image;

	bool duplicated = false;

	if (img->has_mipmaps()) {
		duplicated = true;
		img = img->duplicate();
		img->clear_mipmaps();
	}

	if (img->is_compressed()) {
		if (!duplicated) {
			duplicated = true;
			img = img->duplicate();
		}

		img->decompress();
	}

	Image::Format format = p_image->get_format();

	if (format != Image::FORMAT_L8 && format != Image::FORMAT_R8 && format != Image::FORMAT_RGB8) {
		if (!duplicated) {
			duplicated = true;
			img = img->duplicate();
		}

		if (format == Image::FORMAT_LA8 || format == Image::FORMAT_RF || format == Image::FORMAT_RH) {
			img->convert(Image::FORMAT_L8);
		} else {
			img->convert(Image::FORMAT_RGB8);
		}
	}

	HPDF_ColorSpace color_space = HPDF_CS_DEVICE_GRAY;

	if (format == Image::FORMAT_L8 || format == Image::FORMAT_R8) {
		color_space = HPDF_CS_DEVICE_GRAY;
	} else {
		color_space = HPDF_CS_DEVICE_RGB;
	}

	Vector2i size = img->get_sizei();
	PoolByteArray data = img->get_data();

	PoolByteArray::Read r = data.read();

	HPDF_Font hpdf_image = HPDF_LoadRawImageFromMem(_doc, r.ptr(), size.width, size.height, color_space, 8);

	if (!hpdf_image) {
		return Ref<PDFImage>();
	}

	Ref<PDFImage> image;
	image.instance();

	image->_set_hpdf_image(hpdf_image);

	return image;
}

/*----- info dictionary ----------------------------------------------------*/

String PDFDocument::info_attr_get(const InfoType p_info_type) {
	HPDF_InfoType info_type = static_cast<HPDF_InfoType>(p_info_type);

	return String::utf8(HPDF_GetInfoAttr(_doc, info_type));
}
uint32_t PDFDocument::info_attr_set(const InfoType p_info_type, const String &p_value) {
	HPDF_InfoType info_type = static_cast<HPDF_InfoType>(p_info_type);

	_status = HPDF_SetInfoAttr(_doc, info_type, p_value.utf8().get_data());

	return _status;
}
uint32_t PDFDocument::info_attr_date_set(const InfoType p_info_type, const Ref<PDFDate> &p_date) {
	HPDF_InfoType info_type = static_cast<HPDF_InfoType>(p_info_type);

	HPDF_Date date;

	if (p_date.is_valid()) {
		date.year = p_date->get_year();
		date.month = p_date->get_month();
		date.day = p_date->get_day();
		date.hour = p_date->get_hour();
		date.minutes = p_date->get_minutes();
		date.seconds = p_date->get_seconds();
		date.ind = static_cast<char>(p_date->get_ind());
		date.off_hour = p_date->get_off_hour();
		date.off_minutes = p_date->get_off_minutes();
	}

	_status = HPDF_SetInfoDateAttr(_doc, info_type, date);

	return _status;
}

/*----- encryption ---------------------------------------------------------*/

uint32_t PDFDocument::password_set(const String &p_owner_passwd, const String &p_user_passwd) {
	_status = HPDF_SetPassword(_doc, p_owner_passwd.utf8().get_data(), p_user_passwd.utf8().get_data());

	return _status;
}
uint32_t PDFDocument::permission_set(const uint32_t p_permission) {
	_status = HPDF_SetPermission(_doc, p_permission);

	return _status;
}
uint32_t PDFDocument::encryption_mode_set(const EncryptMode p_encryption_mode, const uint32_t p_key_length) {
	HPDF_EncryptMode info_type = static_cast<HPDF_EncryptMode>(p_encryption_mode);

	_status = HPDF_SetEncryptionMode(_doc, info_type, p_key_length);

	return _status;
}

/*----- attachments -------------------------------------------------------*/

Ref<PDFEmbeddedFile> PDFDocument::attach_file(const String &p_file) {
	String file_name = FileAccess::get_filesystem_abspath_for(p_file);

	HPDF_EmbeddedFile hpdf_embedded_file = HPDF_AttachFile(_doc, file_name.utf8().get_data());

	if (!hpdf_embedded_file) {
		return Ref<PDFEmbeddedFile>();
	}

	Ref<PDFEmbeddedFile> embedded_file;

	embedded_file.instance();

	embedded_file->_set_hpdf_embedded_file(hpdf_embedded_file);

	return embedded_file;
}

/*----- extended graphics state --------------------------------------------*/

Ref<PDFExtGState> PDFDocument::ext_graphic_state_create() {
	HPDF_ExtGState hpdf_ext_g_state = HPDF_CreateExtGState(_doc);

	if (!hpdf_ext_g_state) {
		return Ref<PDFEmbeddedFile>();
	}

	Ref<PDFExtGState> ext_g_state;

	ext_g_state.instance();

	ext_g_state->_set_hpdf_ext_g_state(hpdf_ext_g_state);

	return ext_g_state;
}

/* Notes for docs:
 * - ShadingType must be HPDF_SHADING_FREE_FORM_TRIANGLE_MESH (the only
 *   defined option...)
 * - colorSpace must be HPDF_CS_DEVICE_RGB for now.
 */
Ref<PDFShading> PDFDocument::shading_new(const Vector2 &p_min, const Vector2 &p_max) {
	HPDF_Shading hpdf_shading = HPDF_Shading_New(_doc, HPDF_SHADING_FREE_FORM_TRIANGLE_MESH, HPDF_CS_DEVICE_RGB, p_min.x, p_max.x, p_min.y, p_max.y);

	if (!hpdf_shading) {
		return Ref<PDFShading>();
	}

	Ref<PDFShading> shading;

	shading.instance();

	shading->_set_hpdf_shading(hpdf_shading);

	return shading;
}

PoolByteArray PDFDocument::save_to_mem() {
	HPDF_ResetStream(_doc);

	_status = HPDF_SaveToStream(_doc);

	if (_status != HPDF_OK) {
		return PoolByteArray();
	}

	uint32_t stream_size = HPDF_GetStreamSize(_doc);

	if (stream_size == 0) {
		return PoolByteArray();
	}

	PoolByteArray data;
	data.resize((int)stream_size);

	PoolByteArray::Write w = data.write();

	uint32_t final_size;

	_status = HPDF_ReadFromStream(_doc, w.ptr(), &final_size);

	w.release();

	if (_status != HPDF_OK && _status != HPDF_STREAM_EOF) {
		return PoolByteArray();
	}

	return data;
}
uint32_t PDFDocument::save_to_file(const String &p_file) {
	String abs_path = FileAccess::get_filesystem_abspath_for(p_file);

	_status = HPDF_SaveToFile(_doc, abs_path.utf8().get_data());
	return _status;

	/*
	FileAccess *f = FileAccess::open(p_file, FileAccess::WRITE);

	ERR_FAIL_COND(!f);

	uint32_t stream_size = HPDF_GetStreamSize(_doc);

	Vector<uint8_t> data;
	data.resize((int)stream_size);

	uint32_t final_size;
	_status = HPDF_GetContents(_doc, data.ptrw(), &final_size);
	data.resize(final_size);

	f->store_buffer(data.ptr(), final_size);
	f->close();
	*/
}

uint32_t PDFDocument::get_status() {
	return _status;
}
uint32_t PDFDocument::get_error_no() {
	return HPDF_GetError(_doc);
}
uint32_t PDFDocument::get_error_detail() {
	return HPDF_GetErrorDetail(_doc);
}
void PDFDocument::reset_error() {
	HPDF_ResetError(_doc);
}

PDFDocument::PDFDocument() {
	_doc = HPDF_New(NULL, NULL);

	if (!_doc) {
		PLOG_ERR("Couldn't create PDFDocument object!");
	}
}

PDFDocument::~PDFDocument() {
	if (_doc) {
		HPDF_Free(_doc);
	}
}

void PDFDocument::_bind_methods() {
	ClassDB::bind_method(D_METHOD("viewer_preference_get"), &PDFDocument::viewer_preference_get);
	ClassDB::bind_method(D_METHOD("viewer_preference_set", "val"), &PDFDocument::viewer_preference_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "viewer_preference"), "viewer_preference_set", "viewer_preference_get");

	ClassDB::bind_method(D_METHOD("document_new"), &PDFDocument::document_new);
	ClassDB::bind_method(D_METHOD("document_free"), &PDFDocument::document_free);
	ClassDB::bind_method(D_METHOD("document_has"), &PDFDocument::document_has);
	ClassDB::bind_method(D_METHOD("document_free_all"), &PDFDocument::document_free_all);

	ClassDB::bind_method(D_METHOD("page_get_current"), &PDFDocument::page_get_current);
	ClassDB::bind_method(D_METHOD("page_get_index", "index"), &PDFDocument::page_get_index);
	ClassDB::bind_method(D_METHOD("page_add"), &PDFDocument::page_add);
	ClassDB::bind_method(D_METHOD("page_insert", "page"), &PDFDocument::page_insert);

	ClassDB::bind_method(D_METHOD("page_get_layout"), &PDFDocument::page_get_layout);
	ClassDB::bind_method(D_METHOD("page_set_layout", "val"), &PDFDocument::page_set_layout);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "page_layout", PROPERTY_HINT_ENUM, "Single,One Column,Two Columns Left,Two Columns Right,Two Page Left,Two Page Right,EOF"), "page_set_layout", "page_get_layout");

	ClassDB::bind_method(D_METHOD("page_get_mode"), &PDFDocument::page_get_mode);
	ClassDB::bind_method(D_METHOD("page_set_mode", "val"), &PDFDocument::page_set_mode);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "page_mode", PROPERTY_HINT_ENUM, "None,Outline,Thumbs,Full Screen,EOF"), "page_set_mode", "page_get_mode");

	ClassDB::bind_method(D_METHOD("page_add_label", "page_num", "style", "first_page", "prefix"), &PDFDocument::page_add_label, DEFVAL(String()));

	ClassDB::bind_method(D_METHOD("pages_set_configuration", "page_per_pages"), &PDFDocument::pages_set_configuration);

	ClassDB::bind_method(D_METHOD("compression_mode_set", "mode"), &PDFDocument::compression_mode_set);

	ClassDB::bind_method(D_METHOD("font_get", "font_name", "encoding_name"), &PDFDocument::font_get, DEFVAL(String()));

	ClassDB::bind_method(D_METHOD("font_load_type_1_from_file", "afm_file_name", "data_file_name"), &PDFDocument::font_load_type_1_from_file);
	ClassDB::bind_method(D_METHOD("font_load_ttf_from_file", "file_name", "embed_into_document"), &PDFDocument::font_load_ttf_from_file);
	ClassDB::bind_method(D_METHOD("font_load_ttf_from_collection_file", "index", "embed_into_document"), &PDFDocument::font_load_ttf_from_collection_file);
	ClassDB::bind_method(D_METHOD("font_load_ttf_from_mem", "data", "embed_into_document"), &PDFDocument::font_load_ttf_from_mem);

	ClassDB::bind_method(D_METHOD("fonts_use_jp"), &PDFDocument::fonts_use_jp);
	ClassDB::bind_method(D_METHOD("fonts_use_kr"), &PDFDocument::fonts_use_kr);
	ClassDB::bind_method(D_METHOD("fonts_use_cns"), &PDFDocument::fonts_use_cns);
	ClassDB::bind_method(D_METHOD("fonts_use_cnt"), &PDFDocument::fonts_use_cnt);

	ClassDB::bind_method(D_METHOD("outline_create", "parent", "title", "encoder"), &PDFDocument::outline_create);

	ClassDB::bind_method(D_METHOD("open_action_set", "destination"), &PDFDocument::open_action_set);

	ClassDB::bind_method(D_METHOD("encoder_get", "encoding_name"), &PDFDocument::encoder_get);

	ClassDB::bind_method(D_METHOD("encoder_current_get"), &PDFDocument::encoder_current_get);
	ClassDB::bind_method(D_METHOD("encoder_current_set", "encoding_name"), &PDFDocument::encoder_current_set);

	ClassDB::bind_method(D_METHOD("encodings_use_jp"), &PDFDocument::encodings_use_jp);
	ClassDB::bind_method(D_METHOD("encodings_use_kr"), &PDFDocument::encodings_use_kr);
	ClassDB::bind_method(D_METHOD("encodings_use_cns"), &PDFDocument::encodings_use_cns);
	ClassDB::bind_method(D_METHOD("encodings_use_cnt"), &PDFDocument::encodings_use_cnt);
	ClassDB::bind_method(D_METHOD("encodings_use_utf"), &PDFDocument::encodings_use_utf);

	ClassDB::bind_method(D_METHOD("x_object_create_from_image", "page", "rect", "image", "zoom"), &PDFDocument::x_object_create_from_image);
	ClassDB::bind_method(D_METHOD("x_object_create_as_white_rect", "page", "rect"), &PDFDocument::x_object_create_as_white_rect);

	ClassDB::bind_method(D_METHOD("pdf_a_conformance_set", "type"), &PDFDocument::pdf_a_conformance_set);
	ClassDB::bind_method(D_METHOD("pdf_a_xmp_extension", "xmp_description"), &PDFDocument::pdf_a_xmp_extension);

	ClassDB::bind_method(D_METHOD("output_intents_append", "icc_name", "icc_dict"), &PDFDocument::output_intents_append);

	ClassDB::bind_method(D_METHOD("image_load_png_from_mem", "data"), &PDFDocument::image_load_png_from_mem);
	ClassDB::bind_method(D_METHOD("image_load_png_from_file", "path"), &PDFDocument::image_load_png_from_file);

	ClassDB::bind_method(D_METHOD("image_load_jpg_from_mem", "data"), &PDFDocument::image_load_jpg_from_mem);
	ClassDB::bind_method(D_METHOD("image_load_jpg_from_file", "path"), &PDFDocument::image_load_jpg_from_file);

	ClassDB::bind_method(D_METHOD("image_load_u3d_from_mem", "data"), &PDFDocument::image_load_u3d_from_mem);
	ClassDB::bind_method(D_METHOD("image_load_u3d_from_file", "path"), &PDFDocument::image_load_u3d_from_file);

	ClassDB::bind_method(D_METHOD("image_load_raw_1_bit_image_from_mem", "data", "size", "line_width", "black_is1", "top_is_first"), &PDFDocument::image_load_raw_1_bit_image_from_mem);

	ClassDB::bind_method(D_METHOD("image_load_raw_image_from_mem", "data", "size", "color_space", "bits_per_component"), &PDFDocument::image_load_raw_image_from_mem);
	ClassDB::bind_method(D_METHOD("image_load_raw_image_from_file", "path", "size", "color_space"), &PDFDocument::image_load_raw_image_from_file);

	ClassDB::bind_method(D_METHOD("image_create_pdf_from_image", "image"), &PDFDocument::image_create_pdf_from_image);

	ClassDB::bind_method(D_METHOD("password_set", "owner_passwd", "user_passwd"), &PDFDocument::password_set);
	ClassDB::bind_method(D_METHOD("permission_set", "permission"), &PDFDocument::permission_set);
	ClassDB::bind_method(D_METHOD("encryption_mode_set", "encryption_mode", "key_length"), &PDFDocument::encryption_mode_set);

	ClassDB::bind_method(D_METHOD("attach_file", "file"), &PDFDocument::attach_file);

	ClassDB::bind_method(D_METHOD("ext_graphic_state_create"), &PDFDocument::ext_graphic_state_create);

	ClassDB::bind_method(D_METHOD("shading_new", "min", "max"), &PDFDocument::shading_new);

	ClassDB::bind_method(D_METHOD("save_to_mem"), &PDFDocument::save_to_mem);
	ClassDB::bind_method(D_METHOD("save_to_file", "file"), &PDFDocument::save_to_file);

	ClassDB::bind_method(D_METHOD("get_status"), &PDFDocument::get_status);
	ClassDB::bind_method(D_METHOD("get_error_no"), &PDFDocument::get_error_no);
	ClassDB::bind_method(D_METHOD("get_error_detail"), &PDFDocument::get_error_detail);
	ClassDB::bind_method(D_METHOD("reset_error"), &PDFDocument::reset_error);

	BIND_ENUM_CONSTANT(COMPRESSION_MODE_NONE);
	BIND_ENUM_CONSTANT(COMPRESSION_MODE_TEXT);
	BIND_ENUM_CONSTANT(COMPRESSION_MODE_IMAGE);
	BIND_ENUM_CONSTANT(COMPRESSION_MODE_METADATA);
	BIND_ENUM_CONSTANT(COMPRESSION_MODE_ALL);
	BIND_ENUM_CONSTANT(COMPRESSION_MODE_MASK);

	BIND_ENUM_CONSTANT(PAGE_LAYOUT_SINGLE);
	BIND_ENUM_CONSTANT(PAGE_LAYOUT_ONE_COLUMN);
	BIND_ENUM_CONSTANT(PAGE_LAYOUT_TWO_COLUMN_LEFT);
	BIND_ENUM_CONSTANT(PAGE_LAYOUT_TWO_COLUMN_RIGHT);
	BIND_ENUM_CONSTANT(PAGE_LAYOUT_TWO_PAGE_LEFT);
	BIND_ENUM_CONSTANT(PAGE_LAYOUT_TWO_PAGE_RIGHT);
	BIND_ENUM_CONSTANT(PAGE_LAYOUT_EOF);

	BIND_ENUM_CONSTANT(PAGE_MODE_USE_NONE);
	BIND_ENUM_CONSTANT(PAGE_MODE_USE_OUTLINE);
	BIND_ENUM_CONSTANT(PAGE_MODE_USE_THUMBS);
	BIND_ENUM_CONSTANT(PAGE_MODE_FULL_SCREEN);
	BIND_ENUM_CONSTANT(PAGE_MODE_EOF);

	BIND_ENUM_CONSTANT(PAGE_NUM_STYLE_DECIMAL);
	BIND_ENUM_CONSTANT(PAGE_NUM_STYLE_UPPER_ROMAN);
	BIND_ENUM_CONSTANT(PAGE_NUM_STYLE_LOWER_ROMAN);
	BIND_ENUM_CONSTANT(PAGE_NUM_STYLE_UPPER_LETTERS);
	BIND_ENUM_CONSTANT(PAGE_NUM_STYLE_LOWER_LETTERS);
	BIND_ENUM_CONSTANT(PAGE_NUM_STYLE_EOF);

	BIND_ENUM_CONSTANT(COLOR_SPACE_DEVICE_GRAY);
	BIND_ENUM_CONSTANT(COLOR_SPACE_DEVICE_RGB);
	BIND_ENUM_CONSTANT(COLOR_SPACE_DEVICE_CMYK);
	BIND_ENUM_CONSTANT(COLOR_SPACE_CAL_GRAY);
	BIND_ENUM_CONSTANT(COLOR_SPACE_CAL_RGB);
	BIND_ENUM_CONSTANT(COLOR_SPACE_LAB);
	BIND_ENUM_CONSTANT(COLOR_SPACE_ICC_BASED);
	BIND_ENUM_CONSTANT(COLOR_SPACE_SEPARATION);
	BIND_ENUM_CONSTANT(COLOR_SPACE_DEVICE_N);
	BIND_ENUM_CONSTANT(COLOR_SPACE_INDEXED);
	BIND_ENUM_CONSTANT(COLOR_SPACE_PATTERN);
	BIND_ENUM_CONSTANT(COLOR_SPACE_EOF);

	BIND_ENUM_CONSTANT(INFO_CREATION_DATE);
	BIND_ENUM_CONSTANT(INFO_MOD_DATE);
	BIND_ENUM_CONSTANT(INFO_AUTHOR);
	BIND_ENUM_CONSTANT(INFO_CREATOR);
	BIND_ENUM_CONSTANT(INFO_PRODUCER);
	BIND_ENUM_CONSTANT(INFO_TITLE);
	BIND_ENUM_CONSTANT(INFO_SUBJECT);
	BIND_ENUM_CONSTANT(INFO_KEYWORDS);
	BIND_ENUM_CONSTANT(INFO_TRAPPED);
	BIND_ENUM_CONSTANT(INFO_GTS_PDFX);
	BIND_ENUM_CONSTANT(INFO_EOF);

	BIND_ENUM_CONSTANT(NON_PDFA);
	BIND_ENUM_CONSTANT(PDFA_1A);
	BIND_ENUM_CONSTANT(PDFA_1B);
	BIND_ENUM_CONSTANT(PDFA_2A);
	BIND_ENUM_CONSTANT(PDFA_2B);
	BIND_ENUM_CONSTANT(PDFA_2U);
	BIND_ENUM_CONSTANT(PDFA_3A);
	BIND_ENUM_CONSTANT(PDFA_3B);
	BIND_ENUM_CONSTANT(PDFA_3U);
	BIND_ENUM_CONSTANT(PDFA_4);
	BIND_ENUM_CONSTANT(PDFA_4E);
	BIND_ENUM_CONSTANT(PDFA_4F);

	BIND_ENUM_CONSTANT(ENCRYPT_MODE_R2);
	BIND_ENUM_CONSTANT(ENCRYPT_MODE_R3);
}
