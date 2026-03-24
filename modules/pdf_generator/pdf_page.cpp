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
#include "hpdf_3dmeasure.h"
#include "hpdf_annotation.h"
#include "hpdf_doc.h"
#include "hpdf_font.h"
#include "hpdf_pages.h"
#include "hpdf_u3d.h"

#include "pdf_3d_measure.h"
#include "pdf_3d_view.h"
#include "pdf_dash_mode.h"
#include "pdf_destination.h"
#include "pdf_document.h"
#include "pdf_encoder.h"
#include "pdf_ex_data.h"
#include "pdf_ext_g_state.h"
#include "pdf_font.h"
#include "pdf_image.h"
#include "pdf_shading.h"
#include "pdf_u3d.h"
#include "pdf_x_object.h"

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
void PDFPage::rgb_fill_set(const Color &p_color) {
	_status = HPDF_Page_SetRGBFill((HPDF_Page)_page, p_color.r, p_color.g, p_color.b);
}

Color PDFPage::rgb_stroke_get() {
	HPDF_RGBColor hc = HPDF_Page_GetRGBStroke((HPDF_Page)_page);

	return Color(hc.r, hc.g, hc.b);
}
void PDFPage::rgb_stroke_set(const Color &p_color) {
	_status = HPDF_Page_SetRGBStroke((HPDF_Page)_page, p_color.r, p_color.g, p_color.b);
}

Vector4 PDFPage::cmyk_fill_get() {
	HPDF_CMYKColor hc = HPDF_Page_GetCMYKFill((HPDF_Page)_page);

	return Vector4(hc.c, hc.m, hc.y, hc.k);
}
void PDFPage::cmyk_fill_set(const Vector4 &p_color) {
	_status = HPDF_Page_SetCMYKFill((HPDF_Page)_page, p_color.x, p_color.y, p_color.z, p_color.w);
}

Vector4 PDFPage::cmyk_stroke_get() {
	HPDF_CMYKColor hc = HPDF_Page_GetCMYKFill((HPDF_Page)_page);

	return Vector4(hc.c, hc.m, hc.y, hc.k);
}
void PDFPage::cmyk_stroke_set(const Vector4 &p_color) {
	_status = HPDF_Page_SetCMYKStroke((HPDF_Page)_page, p_color.x, p_color.y, p_color.z, p_color.w);
}

float PDFPage::gray_fill_get() {
	return HPDF_Page_GetGrayFill((HPDF_Page)_page);
}
void PDFPage::gray_fill_set(const float p_color) {
	_status = HPDF_Page_SetGrayFill((HPDF_Page)_page, p_color);
}

float PDFPage::gray_stroke_get() {
	return HPDF_Page_GetGrayStroke((HPDF_Page)_page);
}
void PDFPage::gray_stroke_set(const float p_color) {
	_status = HPDF_Page_SetGrayStroke((HPDF_Page)_page, p_color);
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

uint32_t PDFPage::path_circle(const Vector2 &p_position, const float p_radius) {
	_status = HPDF_Page_Circle((HPDF_Page)_page, p_position.x, p_position.y, p_radius);

	return _status;
}
uint32_t PDFPage::path_ellipse(const Vector2 &p_position, const Vector2 &p_radius) {
	_status = HPDF_Page_Ellipse((HPDF_Page)_page, p_position.x, p_position.y, p_radius.x, p_radius.y);

	return _status;
}
uint32_t PDFPage::path_arc(const Vector2 &p_position, const float p_radius, const float p_angle_1, const float p_angle_2) {
	_status = HPDF_Page_Arc((HPDF_Page)_page, p_position.x, p_position.y, p_radius, p_angle_1, p_angle_2);

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

/*--- XObjects -----------------------------------------------------------*/

uint32_t PDFPage::execute_x_object(const Ref<PDFXObject> &p_x_object) {
	HPDF_XObject hpdf_x_object = NULL;

	if (p_x_object.is_valid()) {
		hpdf_x_object = (HPDF_XObject)p_x_object->_get_hpdf_x_object();
	}

	_status = HPDF_Page_ExecuteXObject((HPDF_Page)_page, hpdf_x_object);

	return _status;
}

/*--- Compatibility ----------------------------------------------------*/

uint32_t PDFPage::draw_image(const Ref<PDFImage> &p_image, const Rect2 &p_rect) {
	HPDF_Image hpdf_image = NULL;

	if (p_image.is_valid()) {
		hpdf_image = (HPDF_Image)p_image->_get_hpdf_image();
	}

	_status = HPDF_Page_DrawImage((HPDF_Page)_page, hpdf_image, p_rect.position.x, p_rect.position.y, p_rect.size.width, p_rect.size.height);

	return _status;
}

uint32_t PDFPage::draw_text_out(const Vector2 &p_position, const String &p_text) {
	_status = HPDF_Page_TextOut((HPDF_Page)_page, p_position.x, p_position.y, p_text.utf8().get_data());

	return _status;
}
uint32_t PDFPage::draw_text_rect(const Rect2 &p_rect, const String &p_text, const TextAlignment p_align) {
	Vector2 rect_end = p_rect.get_end();
	HPDF_TextAlignment align = static_cast<HPDF_TextAlignment>(p_align);

	uint32_t length = 0;
	_status = HPDF_Page_TextRect((HPDF_Page)_page, p_rect.position.x, p_rect.position.y, rect_end.x, rect_end.y, p_text.utf8().get_data(), align, &length);

	return length;
}

uint32_t PDFPage::slide_show_set(const TransitionStyle p_type, const float p_disp_time, const float p_trans_time) {
	HPDF_TransitionStyle type = static_cast<HPDF_TransitionStyle>(p_type);

	_status = HPDF_Page_SetSlideShow((HPDF_Page)_page, type, p_disp_time, p_trans_time);

	return _status;
}

Ref<PDFAnnotation3D> PDFPage::annotation_3d_create(const Rect2 &p_rect, const bool p_tb, const bool p_np, const Ref<PDFU3D> &p_u3d, const Ref<PDFImage> &p_ap) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_U3D hpdf_u3d = NULL;

	if (p_u3d.is_valid()) {
		hpdf_u3d = (HPDF_U3D)p_u3d->_get_hpdf_u3d();
	}

	HPDF_Image hpdf_image = NULL;

	if (p_ap.is_valid()) {
		hpdf_image = (HPDF_Image)p_ap->_get_hpdf_image();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_Create3DAnnot((HPDF_Page)_page, rect, p_tb, p_np, hpdf_u3d, hpdf_image);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotation3D>();
	}

	Ref<PDFAnnotation3D> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationText> PDFPage::annotation_text_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateTextAnnot((HPDF_Page)_page, rect, p_text.utf8().get_data(), hpdf_encoder);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationText>();
	}

	Ref<PDFAnnotationText> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationFreeText> PDFPage::annotation_free_text_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateFreeTextAnnot((HPDF_Page)_page, rect, p_text.utf8().get_data(), hpdf_encoder);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationFreeText>();
	}

	Ref<PDFAnnotationFreeText> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationLine> PDFPage::annotation_line_create(const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateLineAnnot((HPDF_Page)_page, p_text.utf8().get_data(), hpdf_encoder);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationLine>();
	}

	Ref<PDFAnnotationLine> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationWidgetWhitePrint> PDFPage::annotation_widget_white_print_create(const Rect2 &p_rect) {
	Ref<PDFDocument> doc = document_get();

	if (!doc.is_valid()) {
		return Ref<PDFAnnotationWidgetWhitePrint>();
	}

	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateWidgetAnnot_WhiteOnlyWhilePrint(doc->_get_hpdf_doc(), (HPDF_Page)_page, rect);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationWidgetWhitePrint>();
	}

	Ref<PDFAnnotationWidgetWhitePrint> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationWidget> PDFPage::annotation_widget_create(const Rect2 &p_rect) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateWidgetAnnot((HPDF_Page)_page, rect);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationWidget>();
	}

	Ref<PDFAnnotationWidget> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationLink> PDFPage::annotation_link_create(const Rect2 &p_rect, const Ref<PDFDestination> &p_destination) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Destination hpdf_dst = NULL;

	if (p_destination.is_valid()) {
		hpdf_dst = (HPDF_Destination)p_destination->_get_hpdf_destination();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateLinkAnnot((HPDF_Page)_page, rect, hpdf_dst);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationLink>();
	}

	Ref<PDFAnnotationLink> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationURILink> PDFPage::annotation_uri_link_create(const Rect2 &p_rect, const String &p_uri) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateURILinkAnnot((HPDF_Page)_page, rect, p_uri.utf8().get_data());

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationURILink>();
	}

	Ref<PDFAnnotationURILink> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationTextMarkup> PDFPage::annotation_text_markup_create(const Rect2 &p_rect, const String &p_text, const PDFAnnotationTextMarkup::AnnotSubType p_sub_type, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_AnnotType sub_type = static_cast<HPDF_AnnotType>(p_sub_type);

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateTextMarkupAnnot((HPDF_Page)_page, rect, p_text.utf8().get_data(), hpdf_encoder, sub_type);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationTextMarkup>();
	}

	Ref<PDFAnnotationTextMarkup> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationHighlight> PDFPage::annotation_highlight_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateHighlightAnnot((HPDF_Page)_page, rect, p_text.utf8().get_data(), hpdf_encoder);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationHighlight>();
	}

	Ref<PDFAnnotationHighlight> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationUnderline> PDFPage::annotation_underline_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateUnderlineAnnot((HPDF_Page)_page, rect, p_text.utf8().get_data(), hpdf_encoder);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationUnderline>();
	}

	Ref<PDFAnnotationUnderline> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationSquiggly> PDFPage::annotation_squiggly_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateSquigglyAnnot((HPDF_Page)_page, rect, p_text.utf8().get_data(), hpdf_encoder);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationSquiggly>();
	}

	Ref<PDFAnnotationSquiggly> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationStrikeOut> PDFPage::annotation_strike_out_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateStrikeOutAnnot((HPDF_Page)_page, rect, p_text.utf8().get_data(), hpdf_encoder);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationStrikeOut>();
	}

	Ref<PDFAnnotationStrikeOut> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}

Ref<PDFAnnotationPopup> PDFPage::annotation_popup_create(const Rect2 &p_rect, const Ref<PDFAnnotation> &p_parent) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Annotation hpdf_parent_annotation = NULL;

	if (p_parent.is_valid()) {
		hpdf_parent_annotation = (HPDF_Annotation)p_parent->_get_hpdf_annotation();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreatePopupAnnot((HPDF_Page)_page, rect, hpdf_parent_annotation);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationPopup>();
	}

	Ref<PDFAnnotationPopup> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationStamp> PDFPage::annotation_stamp_create(const Rect2 &p_rect, const PDFAnnotationStamp::StampAnnotName p_name, const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_StampAnnotName name = static_cast<HPDF_StampAnnotName>(p_name);

	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateStampAnnot((HPDF_Page)_page, rect, name, p_text.utf8().get_data(), hpdf_encoder);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationStamp>();
	}

	Ref<PDFAnnotationStamp> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationProjection> PDFPage::annotation_projection_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateProjectionAnnot((HPDF_Page)_page, rect, p_text.utf8().get_data(), hpdf_encoder);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationProjection>();
	}

	Ref<PDFAnnotationProjection> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationSquare> PDFPage::annotation_square_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateSquareAnnot((HPDF_Page)_page, rect, p_text.utf8().get_data(), hpdf_encoder);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationSquare>();
	}

	Ref<PDFAnnotationSquare> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}
Ref<PDFAnnotationCircle> PDFPage::annotation_circle_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder) {
	HPDF_Rect rect;
	rect.left = p_rect.position.x;
	rect.top = p_rect.position.y;
	Vector2 rect_end = p_rect.get_end();
	rect.right = rect_end.x;
	rect.bottom = rect_end.y;

	HPDF_Encoder hpdf_encoder = NULL;

	if (p_encoder.is_valid()) {
		hpdf_encoder = (HPDF_Encoder)p_encoder->_get_hpdf_encoder();
	}

	HPDF_Annotation hpdf_annotation = HPDF_Page_CreateCircleAnnot((HPDF_Page)_page, rect, p_text.utf8().get_data(), hpdf_encoder);

	if (!hpdf_annotation) {
		return Ref<PDFAnnotationCircle>();
	}

	Ref<PDFAnnotationCircle> a;
	a.instance();
	a->_set_hpdf_annotation(hpdf_annotation);
	return a;
}

/*----- 3D Measure ---------------------------------------------------------*/

inline static HPDF_Point3D _vector3_to_hdpf_point_3d(const Vector3 &p_point) {
	HPDF_Point3D p;
	p.x = p_point.x;
	p.y = p_point.y;
	p.z = p_point.z;
	return p;
}

Ref<PDF3DMeasure> PDFPage::create_c3d_3d_measure(const Vector3 &p_first_anchor_point, const Vector3 &p_text_anchor_point) {
	HPDF_Point3D firstanchorpoint = _vector3_to_hdpf_point_3d(p_first_anchor_point);
	HPDF_Point3D textanchorpoint = _vector3_to_hdpf_point_3d(p_text_anchor_point);

	HPDF_3DMeasure hpdf_3d_measure = HPDF_Page_Create3DC3DMeasure((HPDF_Page)_page, firstanchorpoint, textanchorpoint);

	if (!hpdf_3d_measure) {
		return Ref<PDF3DMeasure>();
	}

	Ref<PDF3DMeasure> a;
	a.instance();
	a->_set_hpdf_3d_measure(hpdf_3d_measure);
	return a;
}
Ref<PDF3DMeasure> PDFPage::create_pd3_3d_measure(const PoolVector3Array &p_points, const float p_value, const String &p_units_string) {
	ERR_FAIL_COND_V(p_points.size() != 6, Ref<PDF3DMeasure>());

	int index = 0;
	HPDF_Point3D annotationPlaneNormal = _vector3_to_hdpf_point_3d(p_points[index++]);
	HPDF_Point3D firstAnchorPoint = _vector3_to_hdpf_point_3d(p_points[index++]);
	HPDF_Point3D secondAnchorPoint = _vector3_to_hdpf_point_3d(p_points[index++]);
	HPDF_Point3D leaderLinesDirection = _vector3_to_hdpf_point_3d(p_points[index++]);
	HPDF_Point3D measurementValuePoint = _vector3_to_hdpf_point_3d(p_points[index++]);
	HPDF_Point3D textYDirection = _vector3_to_hdpf_point_3d(p_points[index++]);

	HPDF_3DMeasure hpdf_3d_measure = HPDF_Page_CreatePD33DMeasure((HPDF_Page)_page,
			annotationPlaneNormal,
			firstAnchorPoint,
			secondAnchorPoint,
			leaderLinesDirection,
			measurementValuePoint,
			textYDirection,
			p_value,
			p_units_string.utf8().get_data());

	if (!hpdf_3d_measure) {
		return Ref<PDF3DMeasure>();
	}

	Ref<PDF3DMeasure> a;
	a.instance();
	a->_set_hpdf_3d_measure(hpdf_3d_measure);
	return a;
}

Ref<PDFExData> PDFPage::create_3d_annot_ex_data() {
	HPDF_ExData hpdf_ex_data = HPDF_Page_Create3DAnnotExData((HPDF_Page)_page);

	if (!hpdf_ex_data) {
		return Ref<PDFExData>();
	}

	Ref<PDFExData> a;
	a.instance();
	a->_set_hpdf_ex_data(hpdf_ex_data);
	return a;
}

Ref<PDF3DView> PDFPage::create_3d_view_name(const String &p_name) {
	HPDF_Dict hpdf_view = HPDF_Create3DView(HPDF_GetPageMMgr((HPDF_Page)_page), p_name.utf8().get_data());

	if (!hpdf_view) {
		return Ref<PDF3DView>();
	}

	Ref<PDF3DView> view;
	view.instance();
	view->_set_hpdf_3d_view(hpdf_view);
	return view;
}
Ref<PDF3DView> PDFPage::create_3d_view(const Ref<PDFU3D> &p_u3d, const Ref<PDFAnnotation3D> &p_annot_3d, const String &p_name) {
	HPDF_U3D hpdf_u3d = NULL;

	if (p_u3d.is_valid()) {
		hpdf_u3d = (HPDF_U3D)p_u3d->_get_hpdf_u3d();
	}

	HPDF_Annotation hpdf_annot3d = NULL;

	if (p_annot_3d.is_valid()) {
		hpdf_annot3d = (HPDF_Annotation)p_annot_3d->_get_hpdf_annotation();
	}

	HPDF_Dict hpdf_view = HPDF_Page_Create3DView((HPDF_Page)_page, hpdf_u3d, hpdf_annot3d, p_name.utf8().get_data());

	if (!hpdf_view) {
		return Ref<PDF3DView>();
	}

	Ref<PDF3DView> view;
	view.instance();
	view->_set_hpdf_3d_view(hpdf_view);
	return view;
}

Ref<PDFDestination> PDFPage::create_destination() {
	HPDF_Destination hpdf_dest = HPDF_Page_CreateDestination((HPDF_Page)_page);

	if (!hpdf_dest) {
		return Ref<PDFDestination>();
	}

	Ref<PDFDestination> dest;
	dest.instance();
	dest->_set_hpdf_destination(hpdf_dest);
	return dest;
}

uint32_t PDFPage::get_status() {
	return _status;
}

Ref<PDFDocument> PDFPage::document_get() {
	return _document.get_ref();
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

void PDFPage::_set_document(const Ref<PDFDocument> &p_document) {
	_document.set_ref(p_document);
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
	ClassDB::bind_method(D_METHOD("rgb_fill_set", "val"), &PDFPage::rgb_fill_set);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "rgb_fill"), "rgb_fill_set", "rgb_fill_get");

	ClassDB::bind_method(D_METHOD("rgb_stroke_get"), &PDFPage::rgb_stroke_get);
	ClassDB::bind_method(D_METHOD("rgb_stroke_set", "val"), &PDFPage::rgb_stroke_set);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "rgb_stroke"), "rgb_stroke_set", "rgb_stroke_get");

	ClassDB::bind_method(D_METHOD("cmyk_fill_get"), &PDFPage::cmyk_fill_get);
	ClassDB::bind_method(D_METHOD("cmyk_fill_set", "val"), &PDFPage::cmyk_fill_set);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR4, "cmyk_fill"), "cmyk_fill_set", "cmyk_fill_get");

	ClassDB::bind_method(D_METHOD("cmyk_stroke_get"), &PDFPage::cmyk_stroke_get);
	ClassDB::bind_method(D_METHOD("cmyk_stroke_set", "val"), &PDFPage::cmyk_stroke_set);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR4, "cmyk_stroke"), "cmyk_stroke_set", "cmyk_stroke_get");

	ClassDB::bind_method(D_METHOD("gray_fill_get"), &PDFPage::gray_fill_get);
	ClassDB::bind_method(D_METHOD("gray_fill_set", "val"), &PDFPage::gray_fill_set);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "gray_fill"), "gray_fill_set", "gray_fill_get");

	ClassDB::bind_method(D_METHOD("gray_stroke_get"), &PDFPage::gray_stroke_get);
	ClassDB::bind_method(D_METHOD("gray_stroke_set", "val"), &PDFPage::gray_stroke_set);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "gray_stroke"), "gray_stroke_set", "gray_stroke_get");

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

	ClassDB::bind_method(D_METHOD("path_circle", "position", "radius"), &PDFPage::path_circle);
	ClassDB::bind_method(D_METHOD("path_ellipse", "position", "radius"), &PDFPage::path_ellipse);
	ClassDB::bind_method(D_METHOD("path_arc", "position", "radius", "angle_1", "angle_2"), &PDFPage::path_arc);

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

	ClassDB::bind_method(D_METHOD("execute_x_object", "x_object"), &PDFPage::execute_x_object);

	ClassDB::bind_method(D_METHOD("draw_image", "image", "rect"), &PDFPage::draw_image);

	ClassDB::bind_method(D_METHOD("draw_text_out", "position", "text"), &PDFPage::draw_text_out);
	ClassDB::bind_method(D_METHOD("draw_text_rect", "rect", "text", "align"), &PDFPage::draw_text_rect, DEFVAL(TEXT_ALIGN_LEFT));

	ClassDB::bind_method(D_METHOD("slide_show_set", "type", "disp_time", "trans_time"), &PDFPage::slide_show_set);

	ClassDB::bind_method(D_METHOD("annotation_3d_create", "rect", "tb", "np", "u3d", "ap"), &PDFPage::annotation_3d_create);
	ClassDB::bind_method(D_METHOD("annotation_text_create", "rect", "text", "encoder"), &PDFPage::annotation_text_create, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("annotation_free_text_create", "rect", "text", "encoder"), &PDFPage::annotation_free_text_create, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("annotation_line_create", "text", "encoder"), &PDFPage::annotation_line_create, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("annotation_widget_white_print_create", "rect"), &PDFPage::annotation_widget_white_print_create);
	ClassDB::bind_method(D_METHOD("annotation_widget_create", "rect"), &PDFPage::annotation_widget_create);
	ClassDB::bind_method(D_METHOD("annotation_link_create", "rect", "destination"), &PDFPage::annotation_link_create);
	ClassDB::bind_method(D_METHOD("annotation_uri_link_create", "rect", "uri"), &PDFPage::annotation_uri_link_create);
	ClassDB::bind_method(D_METHOD("annotation_text_markup_create", "text", "sub_type", "encoder"), &PDFPage::annotation_text_markup_create, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("annotation_highlight_create", "text", "encoder"), &PDFPage::annotation_highlight_create, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("annotation_underline_create", "text", "encoder"), &PDFPage::annotation_underline_create, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("annotation_squiggly_create", "text", "encoder"), &PDFPage::annotation_squiggly_create, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("annotation_strike_out_create", "text", "encoder"), &PDFPage::annotation_strike_out_create, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("annotation_popup_create", "rect", "parent"), &PDFPage::annotation_popup_create);
	ClassDB::bind_method(D_METHOD("annotation_stamp_create", "rect", "name", "text", "encoder"), &PDFPage::annotation_stamp_create, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("annotation_projection_create", "rect", "text", "encoder"), &PDFPage::annotation_projection_create, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("annotation_square_create", "rect", "text", "encoder"), &PDFPage::annotation_square_create, DEFVAL(Ref<PDFEncoder>()));
	ClassDB::bind_method(D_METHOD("annotation_circle_create", "rect", "text", "encoder"), &PDFPage::annotation_circle_create, DEFVAL(Ref<PDFEncoder>()));

	ClassDB::bind_method(D_METHOD("create_c3d_3d_measure", "first_anchor_point", "text_anchor_point"), &PDFPage::create_c3d_3d_measure);
	ClassDB::bind_method(D_METHOD("create_pd3_3d_measure", "points", "value", "units_string"), &PDFPage::create_pd3_3d_measure);

	ClassDB::bind_method(D_METHOD("create_3d_annot_ex_data"), &PDFPage::create_3d_annot_ex_data);

	ClassDB::bind_method(D_METHOD("create_3d_view_name", "name"), &PDFPage::create_3d_view_name);
	ClassDB::bind_method(D_METHOD("create_3d_view", "u3d", "annot_3d", "name"), &PDFPage::create_3d_view);

	ClassDB::bind_method(D_METHOD("create_destination"), &PDFPage::create_destination);

	ClassDB::bind_method(D_METHOD("get_status"), &PDFPage::get_status);

	ClassDB::bind_method(D_METHOD("document_get"), &PDFPage::document_get);

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

	BIND_ENUM_CONSTANT(TEXT_ALIGN_LEFT);
	BIND_ENUM_CONSTANT(TEXT_ALIGN_RIGHT);
	BIND_ENUM_CONSTANT(TEXT_ALIGN_CENTER);
	BIND_ENUM_CONSTANT(TEXT_ALIGN_JUSTIFY);

	BIND_ENUM_CONSTANT(TRANSITION_STYLE_WIPE_RIGHT);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_WIPE_UP);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_WIPE_LEFT);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_WIPE_DOWN);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_BARN_DOORS_HORIZONTAL_OUT);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_BARN_DOORS_HORIZONTAL_IN);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_BARN_DOORS_VERTICAL_OUT);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_BARN_DOORS_VERTICAL_IN);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_BOX_OUT);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_BOX_IN);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_BLINDS_HORIZONTAL);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_BLINDS_VERTICAL);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_DISSOLVE);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_GLITTER_RIGHT);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_GLITTER_DOWN);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_GLITTER_TOP_LEFT_TO_BOTTOM_RIGHT);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_REPLACE);
	BIND_ENUM_CONSTANT(TRANSITION_STYLE_EOF);
}
