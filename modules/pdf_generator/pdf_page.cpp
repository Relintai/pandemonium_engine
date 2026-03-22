/*************************************************************************/
/*  pdf_page.cpp                                                         */
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

#include "pdf_page.h"

#include "hpdf.h"
#include "hpdf_doc.h"
#include "hpdf_font.h"
#include "hpdf_pages.h"

#include "pdf_dash_mode.h"
#include "pdf_font.h"
#include "pdf_image.h"

float PDFPage::get_width() {
	if (!_page) {
		return 0;
	}

	return HPDF_Page_GetWidth((HPDF_Page)_page);
}
void PDFPage::set_width(const float p_width) {
	if (!_page) {
		return;
	}

	HPDF_Page_SetWidth((HPDF_Page)_page, p_width);
}

float PDFPage::get_height() {
	if (!_page) {
		return 0;
	}

	return HPDF_Page_GetHeight((HPDF_Page)_page);
}
void PDFPage::set_height(const float p_height) {
	if (!_page) {
		return;
	}

	HPDF_Page_SetHeight((HPDF_Page)_page, p_height);
}

uint32_t PDFPage::boundary_set(const PageBoundary p_boundary, const float p_left, const float p_bottom, const float p_right, const float p_top) {
	HPDF_PageBoundary boundary = static_cast<HPDF_PageBoundary>(p_boundary);

	_status = HPDF_Page_SetBoundary((HPDF_Page)_page, boundary, p_left, p_bottom, p_right, p_top);

	return _status;
}
uint32_t PDFPage::size_set(const PageSizes p_size, const PageDirection p_direction) {
	HPDF_PageSizes size = static_cast<HPDF_PageSizes>(p_size);
	HPDF_PageDirection direction = static_cast<HPDF_PageDirection>(p_direction);

	_status = HPDF_Page_SetSize((HPDF_Page)_page, size, direction);

	return _status;
}

uint32_t PDFPage::rotate_set(const uint16_t p_angle) {
	_status = HPDF_Page_SetRotate((HPDF_Page)_page, p_angle);

	return _status;
}
uint32_t PDFPage::zoom_set(const float p_zoom) {
	_status = HPDF_Page_SetZoom((HPDF_Page)_page, p_zoom);

	return _status;
}

float PDFPage::text_width(const String &p_text) {
	return HPDF_Page_TextWidth((HPDF_Page)_page, p_text.utf8().get_data());
}

float PDFPage::measure_text(const String &p_text, float p_width, bool p_word_wrap) {
	float result = 0;

	_status = HPDF_Page_MeasureText((HPDF_Page)_page, p_text.utf8().get_data(), p_width, p_word_wrap, &result);

	return result;
}

uint16_t PDFPage::g_mode_get() {
	return HPDF_Page_GetGMode((HPDF_Page)_page);
}

Vector2 PDFPage::current_pos_get() {
	HPDF_Point p = HPDF_Page_GetCurrentPos((HPDF_Page)_page);

	return Vector2(p.x, p.y);
}
Vector2 PDFPage::current_text_pos_get() {
	HPDF_Point p = HPDF_Page_GetCurrentTextPos((HPDF_Page)_page);

	return Vector2(p.x, p.y);
}

Ref<PDFFont> PDFPage::current_font_get() {
	HPDF_Font hpdf_font = HPDF_Page_GetCurrentFont((HPDF_Page)_page);

	if (!hpdf_font) {
		return Ref<PDFFont>();
	}

	Ref<PDFFont> font;
	font.instance();
	font->_set_hpdf_font(hpdf_font);
	return font;
}

float PDFPage::current_font_size_get() {
	return HPDF_Page_GetCurrentFontSize((HPDF_Page)_page);
}

Transform2D PDFPage::trans_matrix_get() {
	HPDF_TransMatrix m = HPDF_Page_GetTransMatrix((HPDF_Page)_page);

	Transform2D t = Transform2D(m.a, m.b, m.c, m.d, m.x, m.y);
	return t;
}

float PDFPage::line_width_get() {
	return HPDF_Page_GetLineWidth((HPDF_Page)_page);
}

PDFPage::LineCap PDFPage::line_cap_get() {
	HPDF_LineCap lc = HPDF_Page_GetLineCap((HPDF_Page)_page);

	return static_cast<LineCap>(lc);
}
PDFPage::LineJoin PDFPage::line_join_get() {
	HPDF_LineJoin lj = HPDF_Page_GetLineJoin((HPDF_Page)_page);

	return static_cast<LineJoin>(lj);
}
float PDFPage::miter_limit_get() {
	return HPDF_Page_GetMiterLimit((HPDF_Page)_page);
}

Ref<PDFDashMode> PDFPage::dash_get() {
	HPDF_DashMode dm = HPDF_Page_GetDash((HPDF_Page)_page);

	Ref<PDFDashMode> r;
	r.instance();
	r->_setup(dm.ptn, dm.num_ptn, dm.phase);
	return r;
}

uint32_t PDFPage::begin_text() {
	_status = HPDF_Page_BeginText((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::end_text() {
	_status = HPDF_Page_EndText((HPDF_Page)_page);

	return _status;
}

uint32_t PDFPage::set_font_and_size(const Ref<PDFFont> &p_font, float p_size) {
	ERR_FAIL_COND_V(!p_font.is_valid(), -1);

	_status = HPDF_Page_SetFontAndSize((HPDF_Page)_page, (HPDF_Font)p_font->_get_hpdf_font(), p_size);

	return _status;
}

#if 0
/*--- Text state ---------------------------------------------------------*/

/* Tc */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetCharSpace(HPDF_Page page,
		HPDF_REAL value);

/* Tw */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetWordSpace(HPDF_Page page,
		HPDF_REAL value);

/* Tz */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetHorizontalScalling(HPDF_Page page,
		HPDF_REAL value);

/* TL */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetTextLeading(HPDF_Page page,
		HPDF_REAL value);

/* Tf */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetFontAndSize(HPDF_Page page,
		HPDF_Font font,
		HPDF_REAL size);

/* Tr */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetTextRenderingMode(HPDF_Page page,
		HPDF_TextRenderingMode mode);

/* Ts */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetTextRise(HPDF_Page page,
		HPDF_REAL value);

/* This function is obsolete. Use HPDF_Page_SetTextRise.  */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetTextRaise(HPDF_Page page,
		HPDF_REAL value);

#endif

/*--- Text positioni ---------------------------------------------------*/

uint32_t PDFPage::move_text_pos(const real_t p_x, const real_t p_y) {
	_status = HPDF_Page_MoveTextPos((HPDF_Page)_page, p_x, p_y);

	return _status;
}
uint32_t PDFPage::move_text_posv(const Vector2 &p_move) {
	_status = HPDF_Page_MoveTextPos((HPDF_Page)_page, p_move.x, p_move.y);

	return _status;
}

#if 0

/* Td */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_MoveTextPos(HPDF_Page page,
		HPDF_REAL x,
		HPDF_REAL y);

/* TD */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_MoveTextPos2(HPDF_Page page,
		HPDF_REAL x,
		HPDF_REAL y);

/* Tm */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetTextMatrix(HPDF_Page page,
		HPDF_REAL a,
		HPDF_REAL b,
		HPDF_REAL c,
		HPDF_REAL d,
		HPDF_REAL x,
		HPDF_REAL y);

/* T* */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_MoveToNextLine(HPDF_Page page);

#endif

/*--- Text showing -------------------------------------------------------*/

uint32_t PDFPage::show_text(const String &p_text) {
	_status = HPDF_Page_ShowText((HPDF_Page)_page, p_text.utf8().get_data());

	return _status;
}

#if 0

/* Tj */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_ShowText(HPDF_Page page,
		const char *text);

/* TJ */

/* ' */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_ShowTextNextLine(HPDF_Page page,
		const char *text);

/* " */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_ShowTextNextLineEx(HPDF_Page page,
		HPDF_REAL word_space,
		HPDF_REAL char_space,
		const char *text);

/*--- Color showing ------------------------------------------------------*/

/* cs --not implemented yet */
/* CS --not implemented yet */
/* sc --not implemented yet */
/* scn --not implemented yet */
/* SC --not implemented yet */
/* SCN --not implemented yet */

/* g */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetGrayFill(HPDF_Page page,
		HPDF_REAL gray);

/* G */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetGrayStroke(HPDF_Page page,
		HPDF_REAL gray);

/* rg */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetRGBFill(HPDF_Page page,
		HPDF_REAL r,
		HPDF_REAL g,
		HPDF_REAL b);

/* RG */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetRGBStroke(HPDF_Page page,
		HPDF_REAL r,
		HPDF_REAL g,
		HPDF_REAL b);

/* k */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetCMYKFill(HPDF_Page page,
		HPDF_REAL c,
		HPDF_REAL m,
		HPDF_REAL y,
		HPDF_REAL k);

/* K */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetCMYKStroke(HPDF_Page page,
		HPDF_REAL c,
		HPDF_REAL m,
		HPDF_REAL y,
		HPDF_REAL k);

/*--- Compatibility ------------------------------------------------------*/
#endif

uint32_t PDFPage::draw_image(const Ref<PDFImage> &p_image, const Rect2 &p_rect) {
	HPDF_Image hpdf_image = NULL;

	if (p_image.is_valid()) {
		hpdf_image = (HPDF_Image)p_image->_get_hpdf_image();
	}

	_status = HPDF_Page_DrawImage((HPDF_Page)_page, hpdf_image, p_rect.position.x, p_rect.position.y, p_rect.size.width, p_rect.size.height);

	return _status;
}

#if 0
/* BX --not implemented yet */
/* EX --not implemented yet */

HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_DrawImage(HPDF_Page page,
		HPDF_Image image,
		HPDF_REAL x,
		HPDF_REAL y,
		HPDF_REAL width,
		HPDF_REAL height);

HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Circle(HPDF_Page page,
		HPDF_REAL x,
		HPDF_REAL y,
		HPDF_REAL ray);

HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Ellipse(HPDF_Page page,
		HPDF_REAL x,
		HPDF_REAL y,
		HPDF_REAL xray,
		HPDF_REAL yray);

HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Arc(HPDF_Page page,
		HPDF_REAL x,
		HPDF_REAL y,
		HPDF_REAL ray,
		HPDF_REAL ang1,
		HPDF_REAL ang2);

HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_TextOut(HPDF_Page page,
		HPDF_REAL xpos,
		HPDF_REAL ypos,
		const char *text);

HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_TextRect(HPDF_Page page,
		HPDF_REAL left,
		HPDF_REAL top,
		HPDF_REAL right,
		HPDF_REAL bottom,
		const char *text,
		HPDF_TextAlignment align,
		HPDF_UINT *len);

HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetSlideShow(HPDF_Page page,
		HPDF_TransitionStyle type,
		HPDF_REAL disp_time,
		HPDF_REAL trans_time);
#endif

#if 0
HPDF_STATUS
HPDF_Page_InsertBefore(HPDF_Page page,
		HPDF_Page target);

HPDF_BOOL
HPDF_Page_Validate(HPDF_Page page);

HPDF_Page
HPDF_Page_New(HPDF_MMgr mmgr,
		HPDF_Xref xref);

void *
HPDF_Page_GetInheritableItem(HPDF_Page page,
		const char *key,
		HPDF_UINT16 obj_class);

const char *
HPDF_Page_GetXObjectName(HPDF_Page page,
		HPDF_XObject xobj);

const char *
HPDF_Page_GetLocalFontName(HPDF_Page page,
		HPDF_Font font);

const char *
HPDF_Page_GetExtGStateName(HPDF_Page page,
		HPDF_ExtGState gstate);

const char *
HPDF_Page_GetShadingName(HPDF_Page page,
		HPDF_Shading shading);

HPDF_Box
HPDF_Page_GetMediaBox(HPDF_Page page);

HPDF_STATUS
HPDF_Page_SetBoxValue(HPDF_Page page,
		const char *name,
		HPDF_UINT index,
		HPDF_REAL value);

void HPDF_Page_SetFilter(HPDF_Page page,
		HPDF_UINT filter);

HPDF_STATUS
HPDF_Page_CheckState(HPDF_Page page,
		HPDF_UINT mode);

#endif

uint32_t PDFPage::get_status() {
	return _status;
}

PDFPage::PDFPage() {
	_page = NULL;
}

PDFPage::~PDFPage() {
}

void *PDFPage::_get_hpdf_page() const {
	return _page;
}

void PDFPage::_set_hpdf_page(void *p_page) {
	_page = p_page;
}

void PDFPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_width"), &PDFPage::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "val"), &PDFPage::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");

	ClassDB::bind_method(D_METHOD("get_height"), &PDFPage::get_height);
	ClassDB::bind_method(D_METHOD("set_height", "val"), &PDFPage::set_height);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "height"), "set_height", "get_height");

	ClassDB::bind_method(D_METHOD("boundary_set", "boundary", "left", "bottom", "right", "top"), &PDFPage::boundary_set);
	ClassDB::bind_method(D_METHOD("size_set", "size", "direction"), &PDFPage::size_set);

	ClassDB::bind_method(D_METHOD("rotate_set", "angle"), &PDFPage::rotate_set);
	ClassDB::bind_method(D_METHOD("zoom_set", "zoom"), &PDFPage::zoom_set);

	ClassDB::bind_method(D_METHOD("text_width", "text"), &PDFPage::text_width);
	ClassDB::bind_method(D_METHOD("measure_text", "text", "width", "word_wrap"), &PDFPage::measure_text);

	ClassDB::bind_method(D_METHOD("g_mode_get"), &PDFPage::g_mode_get);

	ClassDB::bind_method(D_METHOD("current_pos_get"), &PDFPage::current_pos_get);
	ClassDB::bind_method(D_METHOD("current_text_pos_get"), &PDFPage::current_text_pos_get);

	ClassDB::bind_method(D_METHOD("current_font_get"), &PDFPage::current_font_get);
	ClassDB::bind_method(D_METHOD("current_font_size_get"), &PDFPage::current_font_size_get);

	ClassDB::bind_method(D_METHOD("trans_matrix_get"), &PDFPage::trans_matrix_get);

	ClassDB::bind_method(D_METHOD("line_width_get"), &PDFPage::line_width_get);

	ClassDB::bind_method(D_METHOD("line_cap_get"), &PDFPage::line_cap_get);
	ClassDB::bind_method(D_METHOD("line_join_get"), &PDFPage::line_join_get);

	ClassDB::bind_method(D_METHOD("miter_limit_get"), &PDFPage::miter_limit_get);

	ClassDB::bind_method(D_METHOD("dash_get"), &PDFPage::dash_get);

	ClassDB::bind_method(D_METHOD("begin_text"), &PDFPage::begin_text);
	ClassDB::bind_method(D_METHOD("end_text"), &PDFPage::end_text);
	ClassDB::bind_method(D_METHOD("set_font_and_size", "font", "size"), &PDFPage::set_font_and_size);

	ClassDB::bind_method(D_METHOD("move_text_pos", "x", "y"), &PDFPage::move_text_pos);
	ClassDB::bind_method(D_METHOD("move_text_posv", "move"), &PDFPage::move_text_posv);

	ClassDB::bind_method(D_METHOD("show_text", "text"), &PDFPage::show_text);

	ClassDB::bind_method(D_METHOD("draw_image", "image", "rect"), &PDFPage::draw_image);

	ClassDB::bind_method(D_METHOD("get_status"), &PDFPage::get_status);

	BIND_ENUM_CONSTANT(PAGE_BOUNDARY_MEDIABOX);
	BIND_ENUM_CONSTANT(PAGE_BOUNDARY_CROPBOX);
	BIND_ENUM_CONSTANT(PAGE_BOUNDARY_BLEEDBOX);
	BIND_ENUM_CONSTANT(PAGE_BOUNDARY_TRIMBOX);
	BIND_ENUM_CONSTANT(PAGE_BOUNDARY_ARTBOX);

	BIND_ENUM_CONSTANT(PAGE_SIZE_LETTER);
	BIND_ENUM_CONSTANT(PAGE_SIZE_LEGAL);
	BIND_ENUM_CONSTANT(PAGE_SIZE_A3);
	BIND_ENUM_CONSTANT(PAGE_SIZE_A4);
	BIND_ENUM_CONSTANT(PAGE_SIZE_A5);
	BIND_ENUM_CONSTANT(PAGE_SIZE_B4);
	BIND_ENUM_CONSTANT(PAGE_SIZE_B5);
	BIND_ENUM_CONSTANT(PAGE_SIZE_EXECUTIVE);
	BIND_ENUM_CONSTANT(PAGE_SIZE_US4x6);
	BIND_ENUM_CONSTANT(PAGE_SIZE_US4x8);
	BIND_ENUM_CONSTANT(PAGE_SIZE_US5x7);
	BIND_ENUM_CONSTANT(PAGE_SIZE_COMM10);
	BIND_ENUM_CONSTANT(PAGE_SIZE_EOF);

	BIND_ENUM_CONSTANT(PAGE_DIRECTION_PORTRAIT);
	BIND_ENUM_CONSTANT(PAGE_DIRECTION_LANDSCAPE);

	BIND_ENUM_CONSTANT(HPDF_BUTT_END);
	BIND_ENUM_CONSTANT(HPDF_ROUND_END);
	BIND_ENUM_CONSTANT(HPDF_PROJECTING_SQUARE_END);
	BIND_ENUM_CONSTANT(HPDF_LINECAP_EOF);

	BIND_ENUM_CONSTANT(HPDF_MITER_JOIN);
	BIND_ENUM_CONSTANT(HPDF_ROUND_JOIN);
	BIND_ENUM_CONSTANT(HPDF_BEVEL_JOIN);
	BIND_ENUM_CONSTANT(HPDF_LINEJOIN_EOF);
}
