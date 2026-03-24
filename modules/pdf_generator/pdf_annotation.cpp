/*************************************************************************/
/*  pdf_annotation.cpp                                                   */
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

#include "pdf_annotation.h"

#include "pdf_3d_view.h"
#include "pdf_javascript.h"

#include "hpdf.h"

uint32_t PDFAnnotation::set_rgb_color(const Color &p_color) {
	HPDF_RGBColor color;
	color.r = p_color.r;
	color.g = p_color.g;
	color.b = p_color.b;

	_status = HPDF_Annot_SetRGBColor((HPDF_Annotation)_annotation, color);

	return _status;
}
uint32_t PDFAnnotation::set_cmyk_color(const Vector4 &p_color) {
	HPDF_CMYKColor color;
	color.c = p_color.x;
	color.m = p_color.y;
	color.y = p_color.z;
	color.k = p_color.w;

	_status = HPDF_Annot_SetCMYKColor((HPDF_Annotation)_annotation, color);

	return _status;
}
uint32_t PDFAnnotation::set_gray_color(const float p_color) {
	_status = HPDF_Annot_SetGrayColor((HPDF_Annotation)_annotation, p_color);

	return _status;
}
uint32_t PDFAnnotation::set_no_color() {
	_status = HPDF_Annot_SetNoColor((HPDF_Annotation)_annotation);

	return _status;
}

uint32_t PDFAnnotation::set_border_style(const BorderStyleSubtype p_sub_type, const float p_width, const uint16_t p_dash_on, const uint16_t p_dash_off, const uint16_t p_dash_phase) {
	HPDF_BSSubtype subtype = static_cast<HPDF_BSSubtype>(p_sub_type);

	_status = HPDF_Annotation_SetBorderStyle((HPDF_Annotation)_annotation, subtype, p_width, p_dash_on, p_dash_off, p_dash_phase);

	return _status;
}

uint32_t PDFAnnotation::get_status() {
	return _status;
}

PDFAnnotation::PDFAnnotation() {
	_annotation = NULL;
}

PDFAnnotation::~PDFAnnotation() {
}

void *PDFAnnotation::_get_hpdf_annotation() const {
	return _annotation;
}

void PDFAnnotation::_set_hpdf_annotation(void *p_annotation) {
	_annotation = p_annotation;
}

void PDFAnnotation::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_rgb_color", "color"), &PDFAnnotation::set_rgb_color);
	ClassDB::bind_method(D_METHOD("set_cmyk_color", "color"), &PDFAnnotation::set_cmyk_color);
	ClassDB::bind_method(D_METHOD("set_gray_color", "color"), &PDFAnnotation::set_gray_color);
	ClassDB::bind_method(D_METHOD("set_no_color"), &PDFAnnotation::set_no_color);

	ClassDB::bind_method(D_METHOD("set_border_style", "sub_type", "width", "dash_on", "dash_off", "dash_phase"), &PDFAnnotation::set_border_style);

	ClassDB::bind_method(D_METHOD("get_status"), &PDFAnnotation::get_status);

	BIND_ENUM_CONSTANT(BORDER_STYLE_SOLID);
	BIND_ENUM_CONSTANT(BORDER_STYLE_DASHED);
	BIND_ENUM_CONSTANT(BORDER_STYLE_BEVELED);
	BIND_ENUM_CONSTANT(BORDER_STYLE_INSET);
	BIND_ENUM_CONSTANT(BORDER_STYLE_UNDERLINED);
}

// PDFAnnotation3D

uint32_t PDFAnnotation3D::set_3d_view(const Ref<PDFAnnotation> &p_annot, const Ref<PDF3DView> &p_view) {
	HPDF_Annotation annot = NULL;

	if (p_annot.is_valid()) {
		annot = (HPDF_Annotation)p_annot->_get_hpdf_annotation();
	}

	HPDF_Dict view = NULL;

	if (p_view.is_valid()) {
		view = (HPDF_Dict)p_view->_get_hpdf_3d_view();
	}

	_status = HPDF_Annot_Set3DView(HPDF_GetPageMMgr((HPDF_Page)_hdpf_page), annot, (HPDF_Annotation)_annotation, view);

	return _status;
}

PDFAnnotation3D::PDFAnnotation3D() {
	_hdpf_page = NULL;
}
PDFAnnotation3D::~PDFAnnotation3D() {
}

void PDFAnnotation3D::_set_page(void *p_hdpf_page) {
	_hdpf_page = p_hdpf_page;
}

void PDFAnnotation3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_3d_view", "annot", "view"), &PDFAnnotation3D::set_3d_view);
}

// PDFAnnotationMarkup

PDFAnnotationMarkup::PDFAnnotationMarkup() {
}
PDFAnnotationMarkup::~PDFAnnotationMarkup() {
}

void PDFAnnotationMarkup::_bind_methods() {
}

//  PDFAnnotationText

uint32_t PDFAnnotationText::set_icon(const AnnotIcon p_icon) {
	HPDF_AnnotIcon icon = static_cast<HPDF_AnnotIcon>(p_icon);

	_status = HPDF_TextAnnot_SetIcon((HPDF_Annotation)_annotation, icon);

	return _status;
}
uint32_t PDFAnnotationText::set_opened(const bool p_opened) {
	_status = HPDF_TextAnnot_SetOpened((HPDF_Annotation)_annotation, p_opened);

	return _status;
}

PDFAnnotationText::PDFAnnotationText() {
}
PDFAnnotationText::~PDFAnnotationText() {
}

void PDFAnnotationText::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_icon", "icon"), &PDFAnnotationText::set_icon);
	ClassDB::bind_method(D_METHOD("set_opened", "opened"), &PDFAnnotationText::set_opened);

	BIND_ENUM_CONSTANT(ICON_COMMENT);
	BIND_ENUM_CONSTANT(ICON_KEY);
	BIND_ENUM_CONSTANT(ICON_NOTE);
	BIND_ENUM_CONSTANT(ICON_HELP);
	BIND_ENUM_CONSTANT(ICON_NEW_PARAGRAPH);
	BIND_ENUM_CONSTANT(ICON_PARAGRAPH);
	BIND_ENUM_CONSTANT(ICON_INSERT);
	BIND_ENUM_CONSTANT(ICON_EOF);
}

// PDFAnnotationLine

uint32_t PDFAnnotationLine::set_position(const Vector2 &p_start_point, const LineAnnotEndingStyle p_start_style, const Vector2 &p_end_point, const LineAnnotEndingStyle p_end_style) {
	HPDF_Point startPoint;
	startPoint.x = p_start_point.x;
	startPoint.y = p_start_point.y;
	HPDF_LineAnnotEndingStyle startStyle = static_cast<HPDF_LineAnnotEndingStyle>(p_start_style);
	HPDF_Point endPoint;
	endPoint.x = p_end_point.x;
	endPoint.y = p_end_point.y;
	HPDF_LineAnnotEndingStyle endStyle = static_cast<HPDF_LineAnnotEndingStyle>(p_end_style);

	_status = HPDF_LineAnnot_SetPosition((HPDF_Annotation)_annotation, startPoint, startStyle, endPoint, endStyle);

	return _status;
}
uint32_t PDFAnnotationLine::set_leader(const int p_leader_length, const int p_leader_ext_length, const int p_leader_offset_length) {
	_status = HPDF_LineAnnot_SetLeader((HPDF_Annotation)_annotation, p_leader_length, p_leader_ext_length, p_leader_offset_length);

	return _status;
}
uint32_t PDFAnnotationLine::set_caption(bool p_show_caption, const LineAnnotCapPosition p_position, const int p_horizontal_offset, const int p_vertical_offset) {
	HPDF_LineAnnotCapPosition position = static_cast<HPDF_LineAnnotCapPosition>(p_position);

	_status = HPDF_LineAnnot_SetCaption((HPDF_Annotation)_annotation, p_show_caption, position, p_horizontal_offset, p_vertical_offset);

	return _status;
}

PDFAnnotationLine::PDFAnnotationLine() {
}
PDFAnnotationLine::~PDFAnnotationLine() {
}

void PDFAnnotationLine::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_position", "start_point", "start_style", "end_point", "end_style"), &PDFAnnotationLine::set_position);
	ClassDB::bind_method(D_METHOD("set_leader", "leader_length", "leader_ext_length", "leader_offset_length"), &PDFAnnotationLine::set_leader);
	ClassDB::bind_method(D_METHOD("set_caption", "show_caption", "position", "horizontal_offset", "vertical_offset"), &PDFAnnotationLine::set_caption);

	BIND_ENUM_CONSTANT(LINE_ANNOT_NONE);
	BIND_ENUM_CONSTANT(LINE_ANNOT_SQUARE);
	BIND_ENUM_CONSTANT(LINE_ANNOT_CIRCLE);
	BIND_ENUM_CONSTANT(LINE_ANNOT_DIAMOND);
	BIND_ENUM_CONSTANT(LINE_ANNOT_OPENARROW);
	BIND_ENUM_CONSTANT(LINE_ANNOT_CLOSEDARROW);
	BIND_ENUM_CONSTANT(LINE_ANNOT_BUTT);
	BIND_ENUM_CONSTANT(LINE_ANNOT_ROPENARROW);
	BIND_ENUM_CONSTANT(LINE_ANNOT_RCLOSEDARROW);
	BIND_ENUM_CONSTANT(LINE_ANNOT_SLASH);

	BIND_ENUM_CONSTANT(LINE_ANNOT_CAP_INLINE);
	BIND_ENUM_CONSTANT(LINE_ANNOT_CAP_TOP);
}

// PDFAnnotationFreeText

uint32_t PDFAnnotationFreeText::set_line_ending_style(const PDFAnnotationLine::LineAnnotEndingStyle p_start_style, const PDFAnnotationLine::LineAnnotEndingStyle p_end_style) {
	HPDF_LineAnnotEndingStyle startStyle = static_cast<HPDF_LineAnnotEndingStyle>(p_start_style);
	HPDF_LineAnnotEndingStyle endStyle = static_cast<HPDF_LineAnnotEndingStyle>(p_end_style);

	_status = HPDF_FreeTextAnnot_SetLineEndingStyle((HPDF_Annotation)_annotation, startStyle, endStyle);

	return _status;
}
uint32_t PDFAnnotationFreeText::set_3_point_callout_line(const Vector2 &p_start_point, const Vector2 &p_knee_point, const Vector2 &p_end_point) {
	HPDF_Point startPoint;
	startPoint.x = p_start_point.x;
	startPoint.y = p_start_point.y;
	HPDF_Point kneePoint;
	kneePoint.x = p_knee_point.x;
	kneePoint.y = p_knee_point.y;
	HPDF_Point endPoint;
	endPoint.x = p_end_point.x;
	endPoint.y = p_end_point.y;

	_status = HPDF_FreeTextAnnot_Set3PointCalloutLine((HPDF_Annotation)_annotation, startPoint, kneePoint, endPoint);

	return _status;
}
uint32_t PDFAnnotationFreeText::set_2_point_callout_line(const Vector2 &p_start_point, const Vector2 &p_end_point) {
	HPDF_Point startPoint;
	startPoint.x = p_start_point.x;
	startPoint.y = p_start_point.y;
	HPDF_Point endPoint;
	endPoint.x = p_end_point.x;
	endPoint.y = p_end_point.y;

	_status = HPDF_FreeTextAnnot_Set2PointCalloutLine((HPDF_Annotation)_annotation, startPoint, endPoint);

	return _status;
}
uint32_t PDFAnnotationFreeText::set_default_style(const String &p_style) {
	_status = HPDF_FreeTextAnnot_SetDefaultStyle((HPDF_Annotation)_annotation, p_style.utf8().get_data());

	return _status;
}

PDFAnnotationFreeText::PDFAnnotationFreeText() {
}
PDFAnnotationFreeText::~PDFAnnotationFreeText() {
}

void PDFAnnotationFreeText::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_line_ending_style", "start_style", "end_style"), &PDFAnnotationFreeText::set_line_ending_style);
	ClassDB::bind_method(D_METHOD("set_3_point_callout_line", "start_point", "knee_point", "end_point"), &PDFAnnotationFreeText::set_3_point_callout_line);
	ClassDB::bind_method(D_METHOD("set_2_point_callout_line", "start_point", "end_point"), &PDFAnnotationFreeText::set_2_point_callout_line);
	ClassDB::bind_method(D_METHOD("set_default_style", "style"), &PDFAnnotationFreeText::set_default_style);
}

// PDFAnnotationWidgetWhitePrint

PDFAnnotationWidgetWhitePrint::PDFAnnotationWidgetWhitePrint() {
}
PDFAnnotationWidgetWhitePrint::~PDFAnnotationWidgetWhitePrint() {
}

void PDFAnnotationWidgetWhitePrint::_bind_methods() {
}

// PDFAnnotationWidget

PDFAnnotationWidget::PDFAnnotationWidget() {
}
PDFAnnotationWidget::~PDFAnnotationWidget() {
}

void PDFAnnotationWidget::_bind_methods() {
}

// PDFAnnotationLink

uint32_t PDFAnnotationLink::set_highlight_mode(const HighlightMode p_highlight_mode) {
	HPDF_AnnotHighlightMode mode = static_cast<HPDF_AnnotHighlightMode>(p_highlight_mode);

	_status = HPDF_LinkAnnot_SetHighlightMode((HPDF_Annotation)_annotation, mode);

	return _status;
}
uint32_t PDFAnnotationLink::set_javascript(const Ref<PDFJavascript> &p_javascript) {
	HPDF_JavaScript javascript = NULL;

	if (p_javascript.is_valid()) {
		javascript = (HPDF_JavaScript)p_javascript->_get_hpdf_javascript();
	}

	_status = HPDF_LinkAnnot_SetJavaScript((HPDF_Annotation)_annotation, javascript);

	return _status;
}
uint32_t PDFAnnotationLink::set_border_style(const float p_width, const uint16_t p_dash_on, const uint16_t p_dash_off) {
	_status = HPDF_LinkAnnot_SetBorderStyle((HPDF_Annotation)_annotation, p_width, p_dash_on, p_dash_off);

	return _status;
}

PDFAnnotationLink::PDFAnnotationLink() {
}
PDFAnnotationLink::~PDFAnnotationLink() {
}

void PDFAnnotationLink::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_highlight_mode", "highlight_mode"), &PDFAnnotationLink::set_highlight_mode);
	ClassDB::bind_method(D_METHOD("set_javascript", "javascript"), &PDFAnnotationLink::set_javascript);
	ClassDB::bind_method(D_METHOD("set_border_style", "width", "dash_on", "dash_off"), &PDFAnnotationLink::set_border_style);

	BIND_ENUM_CONSTANT(HIGHLIGHT_MODE_NO_HIGHTLIGHT);
	BIND_ENUM_CONSTANT(HIGHLIGHT_MODE_INVERT_BOX);
	BIND_ENUM_CONSTANT(HIGHLIGHT_MODE_INVERT_BORDER);
	BIND_ENUM_CONSTANT(HIGHLIGHT_MODE_DOWN_APPEARANCE);
	BIND_ENUM_CONSTANT(HIGHLIGHT_MODE_HIGHTLIGHT_MODE_EOF);
}

// PDFAnnotationURILink

PDFAnnotationURILink::PDFAnnotationURILink() {
}
PDFAnnotationURILink::~PDFAnnotationURILink() {
}

void PDFAnnotationURILink::_bind_methods() {
}

// PDFAnnotationTextMarkup

PDFAnnotationTextMarkup::PDFAnnotationTextMarkup() {
}
PDFAnnotationTextMarkup::~PDFAnnotationTextMarkup() {
}

void PDFAnnotationTextMarkup::_bind_methods() {
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_TEXT_NOTES);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_LINK);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_SOUND);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_FREE_TEXT);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_STAMP);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_SQUARE);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_CIRCLE);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_STRIKE_OUT);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_HIGHTLIGHT);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_UNDERLINE);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_INK);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_FILE_ATTACHMENT);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_POPUP);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_3D);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_SQUIGGLY);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_LINE);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_PROJECTION);
	BIND_ENUM_CONSTANT(ANNOTATION_TYPE_WIDGET);
}

// PDFAnnotationHighlight

PDFAnnotationHighlight::PDFAnnotationHighlight() {
}
PDFAnnotationHighlight::~PDFAnnotationHighlight() {
}

void PDFAnnotationHighlight::_bind_methods() {
}

// PDFAnnotationUnderline

PDFAnnotationUnderline::PDFAnnotationUnderline() {
}
PDFAnnotationUnderline::~PDFAnnotationUnderline() {
}

void PDFAnnotationUnderline::_bind_methods() {
}

// PDFAnnotationSquiggly

PDFAnnotationSquiggly::PDFAnnotationSquiggly() {
}
PDFAnnotationSquiggly::~PDFAnnotationSquiggly() {
}

void PDFAnnotationSquiggly::_bind_methods() {
}

// PDFAnnotationStrikeOut

PDFAnnotationStrikeOut::PDFAnnotationStrikeOut() {
}
PDFAnnotationStrikeOut::~PDFAnnotationStrikeOut() {
}

void PDFAnnotationStrikeOut::_bind_methods() {
}

// PDFAnnotationPopup

PDFAnnotationPopup::PDFAnnotationPopup() {
}
PDFAnnotationPopup::~PDFAnnotationPopup() {
}

void PDFAnnotationPopup::_bind_methods() {
}

// PDFAnnotationStamp

PDFAnnotationStamp::PDFAnnotationStamp() {
}
PDFAnnotationStamp::~PDFAnnotationStamp() {
}

void PDFAnnotationStamp::_bind_methods() {
	BIND_ENUM_CONSTANT(STAMP_ANNOT_APPROVED);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_EXPERIMENTAL);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_NOTAPPROVED);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_ASIS);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_EXPIRED);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_NOTFORPUBLICRELEASE);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_CONFIDENTIAL);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_FINAL);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_SOLD);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_DEPARTMENTAL);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_FORCOMMENT);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_TOPSECRET);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_DRAFT);
	BIND_ENUM_CONSTANT(STAMP_ANNOT_FORPUBLICRELEASE);
}

// PDFAnnotationProjection

PDFAnnotationProjection::PDFAnnotationProjection() {
}
PDFAnnotationProjection::~PDFAnnotationProjection() {
}

void PDFAnnotationProjection::_bind_methods() {
}

// PDFAnnotationSquare

PDFAnnotationSquare::PDFAnnotationSquare() {
}
PDFAnnotationSquare::~PDFAnnotationSquare() {
}

void PDFAnnotationSquare::_bind_methods() {
}

// PDFAnnotationCircle

PDFAnnotationCircle::PDFAnnotationCircle() {
}
PDFAnnotationCircle::~PDFAnnotationCircle() {
}

void PDFAnnotationCircle::_bind_methods() {
}
