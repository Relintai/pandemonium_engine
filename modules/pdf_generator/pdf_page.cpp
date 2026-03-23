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
#include "pdf_ext_g_state.h"
#include "pdf_font.h"
#include "pdf_image.h"
#include "pdf_shading.h"

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
void PDFPage::line_width_set(const float p_value) {
	_status = HPDF_Page_SetLineWidth((HPDF_Page)_page, p_value);
}

PDFPage::LineCap PDFPage::line_cap_get() {
	HPDF_LineCap lc = HPDF_Page_GetLineCap((HPDF_Page)_page);

	return static_cast<LineCap>(lc);
}
void PDFPage::line_cap_set(const LineCap p_value) {
	HPDF_LineCap lc = static_cast<HPDF_LineCap>(p_value);

	_status = HPDF_Page_SetLineCap((HPDF_Page)_page, lc);
}

PDFPage::LineJoin PDFPage::line_join_get() {
	HPDF_LineJoin lj = HPDF_Page_GetLineJoin((HPDF_Page)_page);

	return static_cast<LineJoin>(lj);
}
void PDFPage::line_join_set(const LineJoin p_value) {
	HPDF_LineJoin lj = static_cast<HPDF_LineJoin>(p_value);

	_status = HPDF_Page_SetLineJoin((HPDF_Page)_page, lj);
}

float PDFPage::miter_limit_get() {
	return HPDF_Page_GetMiterLimit((HPDF_Page)_page);
}
void PDFPage::miter_limit_set(const float p_value) {
	_status = HPDF_Page_SetMiterLimit((HPDF_Page)_page, p_value);
}

Ref<PDFDashMode> PDFPage::dash_get() {
	HPDF_DashMode dm = HPDF_Page_GetDash((HPDF_Page)_page);

	Ref<PDFDashMode> r;
	r.instance();
	r->_setup(dm.ptn, dm.num_ptn, dm.phase);
	return r;
}
void PDFPage::dash_set(const Ref<PDFDashMode> &p_mode) {
	uint32_t num_ptn = 0;
	float phase = 0;
	float *ptn = NULL;

	if (p_mode.is_valid()) {
		num_ptn = p_mode->get_num_elements();
		phase = p_mode->get_phase();
		ptn = (float *)p_mode->_get_ptn();
	}

	_status = HPDF_Page_SetDash((HPDF_Page)_page, ptn, num_ptn, phase);
}

float PDFPage::flat_get() {
	return HPDF_Page_GetFlat((HPDF_Page)_page);
}
void PDFPage::flat_set(const float p_value) {
	_status = HPDF_Page_SetFlat((HPDF_Page)_page, p_value);
}

float PDFPage::char_space_get() {
	return HPDF_Page_GetCharSpace((HPDF_Page)_page);
}
void PDFPage::char_space_set(const float p_value) {
	_status = HPDF_Page_SetCharSpace((HPDF_Page)_page, p_value);
}

float PDFPage::word_space_get() {
	return HPDF_Page_GetWordSpace((HPDF_Page)_page);
}
void PDFPage::word_space_set(const float p_value) {
	_status = HPDF_Page_SetWordSpace((HPDF_Page)_page, p_value);
}

float PDFPage::horizontal_scalling_get() {
	return HPDF_Page_GetHorizontalScalling((HPDF_Page)_page);
}
void PDFPage::horizontal_scalling_set(const float p_value) {
	_status = HPDF_Page_SetHorizontalScalling((HPDF_Page)_page, p_value);
}

float PDFPage::text_leading_get() {
	return HPDF_Page_GetTextLeading((HPDF_Page)_page);
}
void PDFPage::text_leading_set(const float p_value) {
	_status = HPDF_Page_SetTextLeading((HPDF_Page)_page, p_value);
}

PDFPage::TextRenderingMode PDFPage::text_rendering_mode_get() {
	HPDF_TextRenderingMode trm = HPDF_Page_GetTextRenderingMode((HPDF_Page)_page);

	return static_cast<TextRenderingMode>(trm);
}
void PDFPage::text_rendering_mode_set(const TextRenderingMode p_value) {
	HPDF_TextRenderingMode trm = static_cast<HPDF_TextRenderingMode>(p_value);

	_status = HPDF_Page_SetTextRenderingMode((HPDF_Page)_page, trm);
}

float PDFPage::text_rise_get() {
	return HPDF_Page_GetTextRise((HPDF_Page)_page);
}
void PDFPage::text_rise_set(const float p_value) {
	_status = HPDF_Page_SetTextRise((HPDF_Page)_page, p_value);
}

Color PDFPage::rgb_fill_get() {
	HPDF_RGBColor hc = HPDF_Page_GetRGBFill((HPDF_Page)_page);

	return Color(hc.r, hc.g, hc.b);
}
Color PDFPage::rgb_stroke_get() {
	HPDF_RGBColor hc = HPDF_Page_GetRGBStroke((HPDF_Page)_page);

	return Color(hc.r, hc.g, hc.b);
}

Vector4 PDFPage::cmyk_fill_get() {
	HPDF_CMYKColor hc = HPDF_Page_GetCMYKFill((HPDF_Page)_page);

	return Vector4(hc.c, hc.m, hc.y, hc.k);
}
Vector4 PDFPage::cmyk_stroke_get() {
	HPDF_CMYKColor hc = HPDF_Page_GetCMYKFill((HPDF_Page)_page);

	return Vector4(hc.c, hc.m, hc.y, hc.k);
}

float PDFPage::gray_fill_get() {
	return HPDF_Page_GetGrayFill((HPDF_Page)_page);
}
float PDFPage::gray_stroke_get() {
	return HPDF_Page_GetGrayStroke((HPDF_Page)_page);
}

PDFDocument::ColorSpace PDFPage::stroking_color_space_get() {
	HPDF_ColorSpace cs = HPDF_Page_GetStrokingColorSpace((HPDF_Page)_page);

	return static_cast<PDFDocument::ColorSpace>(cs);
}
PDFDocument::ColorSpace PDFPage::filling_color_space_get() {
	HPDF_ColorSpace cs = HPDF_Page_GetFillingColorSpace((HPDF_Page)_page);

	return static_cast<PDFDocument::ColorSpace>(cs);
}

Transform2D PDFPage::text_matrix_get() {
	HPDF_TransMatrix m = HPDF_Page_GetTextMatrix((HPDF_Page)_page);

	Transform2D t = Transform2D(m.a, m.b, m.c, m.d, m.x, m.y);
	return t;
}
void PDFPage::text_matrix_set(const Transform2D &p_transform) {
	_status = HPDF_Page_SetTextMatrix((HPDF_Page)_page,
			p_transform.columns[0][0],
			p_transform.columns[0][1],
			p_transform.columns[1][0],
			p_transform.columns[1][1],
			p_transform.columns[2][0],
			p_transform.columns[2][1]);
}

uint32_t PDFPage::g_state_depth_get() {
	return HPDF_Page_GetGStateDepth((HPDF_Page)_page);
}

uint32_t PDFPage::ext_g_state_set(const Ref<PDFExtGState> &p_ext_g_state) {
	HPDF_ExtGState ext_gstate = NULL;

	if (p_ext_g_state.is_valid()) {
		ext_gstate = (HPDF_ExtGState)p_ext_g_state->_get_hpdf_ext_g_state();
	}

	_status = HPDF_Page_SetExtGState((HPDF_Page)_page, ext_gstate);

	return _status;
}
uint32_t PDFPage::shading_set(const Ref<PDFShading> &p_shading) {
	HPDF_Shading shading = NULL;

	if (p_shading.is_valid()) {
		shading = (HPDF_Shading)p_shading->_get_hpdf_shading();
	}

	_status = HPDF_Page_SetShading((HPDF_Page)_page, shading);

	return _status;
}

/*--- Path construction operator ------------------------------------------*/

uint32_t PDFPage::path_move_to(const Vector2 &p_position) {
	_status = HPDF_Page_MoveTo((HPDF_Page)_page, p_position.x, p_position.y);

	return _status;
}
uint32_t PDFPage::path_line_to(const Vector2 &p_position) {
	_status = HPDF_Page_LineTo((HPDF_Page)_page, p_position.x, p_position.y);

	return _status;
}
uint32_t PDFPage::path_curve_to(const Vector2 &p_cp_start, const Vector2 &p_cp_end, const Vector2 &p_end_point) {
	_status = HPDF_Page_CurveTo((HPDF_Page)_page, p_cp_start.x, p_cp_start.y, p_cp_end.x, p_cp_end.y, p_end_point.x, p_end_point.y);

	return _status;
}
uint32_t PDFPage::path_curve_to_2(const Vector2 &p_cp_end, const Vector2 &p_end_point) {
	_status = HPDF_Page_CurveTo2((HPDF_Page)_page, p_cp_end.x, p_cp_end.y, p_end_point.x, p_end_point.y);

	return _status;
}
uint32_t PDFPage::path_curve_to_3(const Vector2 &p_cp_start, const Vector2 &p_end_point) {
	_status = HPDF_Page_CurveTo3((HPDF_Page)_page, p_cp_start.x, p_cp_start.y, p_end_point.x, p_end_point.y);

	return _status;
}
uint32_t PDFPage::path_rectangle(const Rect2 &p_rect) {
	_status = HPDF_Page_Rectangle((HPDF_Page)_page, p_rect.position.x, p_rect.position.y, p_rect.size.width, p_rect.size.height);

	return _status;
}
uint32_t PDFPage::path_close() {
	_status = HPDF_Page_ClosePath((HPDF_Page)_page);

	return _status;
}

/*--- Path painting operator ---------------------------------------------*/

uint32_t PDFPage::path_stroke() {
	_status = HPDF_Page_Stroke((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::path_stroke_close() {
	_status = HPDF_Page_ClosePathStroke((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::path_fill() {
	_status = HPDF_Page_Fill((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::path_eo_fill() {
	_status = HPDF_Page_Eofill((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::path_fill_stroke() {
	_status = HPDF_Page_FillStroke((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::path_eo_fill_stroke() {
	_status = HPDF_Page_EofillStroke((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::path_fill_stroke_close() {
	_status = HPDF_Page_ClosePathFillStroke((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::path_eo_fill_stroke_close() {
	_status = HPDF_Page_ClosePathEofillStroke((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::path_end() {
	_status = HPDF_Page_EndPath((HPDF_Page)_page);

	return _status;
}

/*--- Clipping paths operator --------------------------------------------*/

uint32_t PDFPage::clip() {
	_status = HPDF_Page_Clip((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::eo_clip() {
	_status = HPDF_Page_Eoclip((HPDF_Page)_page);

	return _status;
}

/*--- Text state ---------------------------------------------------------*/

uint32_t PDFPage::begin_text() {
	_status = HPDF_Page_BeginText((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::end_text() {
	_status = HPDF_Page_EndText((HPDF_Page)_page);

	return _status;
}

uint32_t PDFPage::set_font_and_size(const Ref<PDFFont> &p_font, float p_size) {
	HPDF_Font hpdf_font = NULL;

	if (p_font.is_valid()) {
		hpdf_font = (HPDF_Font)p_font->_get_hpdf_font();
	}

	_status = HPDF_Page_SetFontAndSize((HPDF_Page)_page, hpdf_font, p_size);

	return _status;
}

/*--- Special graphic state operator --------------------------------------*/

uint32_t PDFPage::g_save() {
	_status = HPDF_Page_GSave((HPDF_Page)_page);

	return _status;
}
uint32_t PDFPage::g_restore() {
	_status = HPDF_Page_GRestore((HPDF_Page)_page);

	return _status;
}

uint32_t PDFPage::concat(const Transform2D &p_transform) {
	_status = HPDF_Page_Concat((HPDF_Page)_page,
			p_transform.columns[0][0],
			p_transform.columns[0][1],
			p_transform.columns[1][0],
			p_transform.columns[1][1],
			p_transform.columns[2][0],
			p_transform.columns[2][1]);

	return _status;
}

/*--- Text positioni ---------------------------------------------------*/

uint32_t PDFPage::move_text_pos(const real_t p_x, const real_t p_y) {
	_status = HPDF_Page_MoveTextPos((HPDF_Page)_page, p_x, p_y);

	return _status;
}
uint32_t PDFPage::move_text_posv(const Vector2 &p_move) {
	_status = HPDF_Page_MoveTextPos((HPDF_Page)_page, p_move.x, p_move.y);

	return _status;
}

uint32_t PDFPage::move_text_pos_2(const real_t p_x, const real_t p_y) {
	_status = HPDF_Page_MoveTextPos2((HPDF_Page)_page, p_x, p_y);

	return _status;
}
uint32_t PDFPage::move_text_posv_2(const Vector2 &p_move) {
	_status = HPDF_Page_MoveTextPos2((HPDF_Page)_page, p_move.x, p_move.y);

	return _status;
}

uint32_t PDFPage::move_to_next_line() {
	_status = HPDF_Page_MoveToNextLine((HPDF_Page)_page);

	return _status;
}

/*--- Text showing -------------------------------------------------------*/

uint32_t PDFPage::show_text(const String &p_text) {
	_status = HPDF_Page_ShowText((HPDF_Page)_page, p_text.utf8().get_data());

	return _status;
}
uint32_t PDFPage::show_text_next_line(const String &p_text) {
	_status = HPDF_Page_ShowTextNextLine((HPDF_Page)_page, p_text.utf8().get_data());

	return _status;
}
uint32_t PDFPage::show_text_next_line_ex(const String &p_text, const float p_word_space, const float p_char_space) {
	_status = HPDF_Page_ShowTextNextLineEx((HPDF_Page)_page, p_word_space, p_char_space, p_text.utf8().get_data());

	return _status;
}

#if 0

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
	ClassDB::bind_method(D_METHOD("line_width_set", "val"), &PDFPage::line_width_set);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "line_width"), "line_width_set", "line_width_get");

	ClassDB::bind_method(D_METHOD("line_cap_get"), &PDFPage::line_cap_get);
	ClassDB::bind_method(D_METHOD("line_cap_set", "val"), &PDFPage::line_cap_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "line_cap", PROPERTY_HINT_ENUM, "Butt End,Round End,Projection Square End,EOF"), "line_cap_set", "line_cap_get");

	ClassDB::bind_method(D_METHOD("line_join_get"), &PDFPage::line_join_get);
	ClassDB::bind_method(D_METHOD("line_join_set", "val"), &PDFPage::line_join_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "line_join", PROPERTY_HINT_ENUM, "Miter Join,Round Join,Bevel Join,EOF"), "line_join_set", "line_join_get");

	ClassDB::bind_method(D_METHOD("miter_limit_get"), &PDFPage::miter_limit_get);
	ClassDB::bind_method(D_METHOD("miter_limit_set", "val"), &PDFPage::miter_limit_set);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "miter_limit"), "miter_limit_set", "miter_limit_get");

	// Probably shouldn't be a property
	ClassDB::bind_method(D_METHOD("dash_get"), &PDFPage::dash_get);
	ClassDB::bind_method(D_METHOD("dash_set", "val"), &PDFPage::dash_set);

	ClassDB::bind_method(D_METHOD("flat_get"), &PDFPage::flat_get);
	ClassDB::bind_method(D_METHOD("flat_set", "val"), &PDFPage::flat_set);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "flat"), "flat_set", "flat_get");

	ClassDB::bind_method(D_METHOD("char_space_get"), &PDFPage::char_space_get);
	ClassDB::bind_method(D_METHOD("char_space_set", "val"), &PDFPage::char_space_set);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "char_space"), "char_space_set", "char_space_get");

	ClassDB::bind_method(D_METHOD("word_space_get"), &PDFPage::word_space_get);
	ClassDB::bind_method(D_METHOD("word_space_set", "val"), &PDFPage::word_space_set);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "word_space"), "word_space_set", "word_space_get");

	ClassDB::bind_method(D_METHOD("horizontal_scalling_get"), &PDFPage::horizontal_scalling_get);
	ClassDB::bind_method(D_METHOD("horizontal_scalling_set", "val"), &PDFPage::horizontal_scalling_set);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "horizontal_scalling"), "horizontal_scalling_set", "horizontal_scalling_get");

	ClassDB::bind_method(D_METHOD("text_leading_get"), &PDFPage::text_leading_get);
	ClassDB::bind_method(D_METHOD("text_leading_set", "val"), &PDFPage::text_leading_set);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "text_leading"), "text_leading_set", "text_leading_get");

	ClassDB::bind_method(D_METHOD("text_rendering_mode_get"), &PDFPage::text_rendering_mode_get);
	ClassDB::bind_method(D_METHOD("text_rendering_mode_set", "val"), &PDFPage::text_rendering_mode_set);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "text_rendering_mode", PROPERTY_HINT_ENUM, "Fill,Stroke,Fill Then Stroke,invisible,Fill Clipping,Stroke Clipping,Fill Stroke Clipping,Clipping,EOF"), "text_rendering_mode_set", "text_rendering_mode_get");

	ClassDB::bind_method(D_METHOD("text_rise_get"), &PDFPage::text_rise_get);
	ClassDB::bind_method(D_METHOD("text_rise_set", "val"), &PDFPage::text_rise_set);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "text_rise"), "text_rise_set", "text_rise_get");

	ClassDB::bind_method(D_METHOD("rgb_fill_get"), &PDFPage::rgb_fill_get);
	ClassDB::bind_method(D_METHOD("rgb_stroke_get"), &PDFPage::rgb_stroke_get);

	ClassDB::bind_method(D_METHOD("cmyk_fill_get"), &PDFPage::cmyk_fill_get);
	ClassDB::bind_method(D_METHOD("cmyk_stroke_get"), &PDFPage::cmyk_stroke_get);

	ClassDB::bind_method(D_METHOD("gray_fill_get"), &PDFPage::gray_fill_get);
	ClassDB::bind_method(D_METHOD("gray_stroke_get"), &PDFPage::gray_stroke_get);

	ClassDB::bind_method(D_METHOD("stroking_color_space_get"), &PDFPage::stroking_color_space_get);
	ClassDB::bind_method(D_METHOD("filling_color_space_get"), &PDFPage::filling_color_space_get);

	ClassDB::bind_method(D_METHOD("text_matrix_get"), &PDFPage::text_matrix_get);
	ClassDB::bind_method(D_METHOD("text_matrix_set", "matrix"), &PDFPage::text_matrix_set);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "text_matrix"), "text_matrix_set", "text_matrix_get");

	ClassDB::bind_method(D_METHOD("g_state_depth_get"), &PDFPage::g_state_depth_get);

	ClassDB::bind_method(D_METHOD("ext_g_state_set", "ext_g_state"), &PDFPage::ext_g_state_set);
	ClassDB::bind_method(D_METHOD("shading_set", "shading"), &PDFPage::shading_set);

	ClassDB::bind_method(D_METHOD("g_save"), &PDFPage::g_save);
	ClassDB::bind_method(D_METHOD("g_restore"), &PDFPage::g_restore);
	ClassDB::bind_method(D_METHOD("concat", "transform"), &PDFPage::concat);

	ClassDB::bind_method(D_METHOD("path_move_to", "position"), &PDFPage::path_move_to);
	ClassDB::bind_method(D_METHOD("path_line_to", "position"), &PDFPage::path_line_to);
	ClassDB::bind_method(D_METHOD("path_curve_to", "cp_start", "cp_end", "end_point"), &PDFPage::path_curve_to);
	ClassDB::bind_method(D_METHOD("path_curve_to_2", "cp_end", "end_point"), &PDFPage::path_curve_to_2);
	ClassDB::bind_method(D_METHOD("path_curve_to_3", "end_point"), &PDFPage::path_curve_to_3);
	ClassDB::bind_method(D_METHOD("path_rectangle", "rect"), &PDFPage::path_rectangle);
	ClassDB::bind_method(D_METHOD("path_close"), &PDFPage::path_close);

	ClassDB::bind_method(D_METHOD("path_stroke"), &PDFPage::path_stroke);
	ClassDB::bind_method(D_METHOD("path_stroke_close"), &PDFPage::path_stroke_close);
	ClassDB::bind_method(D_METHOD("path_fill"), &PDFPage::path_fill);
	ClassDB::bind_method(D_METHOD("path_eo_fill"), &PDFPage::path_eo_fill);
	ClassDB::bind_method(D_METHOD("path_fill_stroke"), &PDFPage::path_fill_stroke);
	ClassDB::bind_method(D_METHOD("path_eo_fill_stroke"), &PDFPage::path_eo_fill_stroke);
	ClassDB::bind_method(D_METHOD("path_fill_stroke_close"), &PDFPage::path_fill_stroke_close);
	ClassDB::bind_method(D_METHOD("path_eo_fill_stroke_close"), &PDFPage::path_eo_fill_stroke_close);
	ClassDB::bind_method(D_METHOD("path_end"), &PDFPage::path_end);

	ClassDB::bind_method(D_METHOD("clip"), &PDFPage::clip);
	ClassDB::bind_method(D_METHOD("eo_clip"), &PDFPage::eo_clip);

	ClassDB::bind_method(D_METHOD("begin_text"), &PDFPage::begin_text);
	ClassDB::bind_method(D_METHOD("end_text"), &PDFPage::end_text);
	ClassDB::bind_method(D_METHOD("set_font_and_size", "font", "size"), &PDFPage::set_font_and_size);

	ClassDB::bind_method(D_METHOD("move_text_pos", "x", "y"), &PDFPage::move_text_pos);
	ClassDB::bind_method(D_METHOD("move_text_posv", "move"), &PDFPage::move_text_posv);

	ClassDB::bind_method(D_METHOD("move_text_pos_2", "x", "y"), &PDFPage::move_text_pos_2);
	ClassDB::bind_method(D_METHOD("move_text_posv_2", "move"), &PDFPage::move_text_posv_2);

	ClassDB::bind_method(D_METHOD("move_to_next_line"), &PDFPage::move_to_next_line);

	ClassDB::bind_method(D_METHOD("show_text", "text"), &PDFPage::show_text);
	ClassDB::bind_method(D_METHOD("show_text_next_line", "text"), &PDFPage::show_text_next_line);
	ClassDB::bind_method(D_METHOD("show_text_next_line_ex", "text", "word_space", "char_space"), &PDFPage::show_text_next_line_ex);

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

	BIND_ENUM_CONSTANT(LINE_CAP_BUTT_END);
	BIND_ENUM_CONSTANT(LINE_CAP_ROUND_END);
	BIND_ENUM_CONSTANT(LINE_CAP_PROJECTING_SQUARE_END);
	BIND_ENUM_CONSTANT(LINE_CAP_LINECAP_EOF);

	BIND_ENUM_CONSTANT(LINE_JOIN_MITER_JOIN);
	BIND_ENUM_CONSTANT(LINE_JOIN_ROUND_JOIN);
	BIND_ENUM_CONSTANT(LINE_JOIN_BEVEL_JOIN);
	BIND_ENUM_CONSTANT(LINE_JOIN_EOF);

	BIND_ENUM_CONSTANT(TEXT_RENDERING_MODE_FILL);
	BIND_ENUM_CONSTANT(TEXT_RENDERING_MODE_STROKE);
	BIND_ENUM_CONSTANT(TEXT_RENDERING_MODE_FILL_THEN_STROKE);
	BIND_ENUM_CONSTANT(TEXT_RENDERING_MODE_INVISIBLE);
	BIND_ENUM_CONSTANT(TEXT_RENDERING_MODE_FILL_CLIPPING);
	BIND_ENUM_CONSTANT(TEXT_RENDERING_MODE_STROKE_CLIPPING);
	BIND_ENUM_CONSTANT(TEXT_RENDERING_MODE_FILL_STROKE_CLIPPING);
	BIND_ENUM_CONSTANT(TEXT_RENDERING_MODE_CLIPPING);
	BIND_ENUM_CONSTANT(TEXT_RENDERING_MODE_EOF);
}
