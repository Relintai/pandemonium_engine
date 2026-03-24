#ifndef PDF_ANNOTATION_H
#define PDF_ANNOTATION_H

/*************************************************************************/
/*  pdf_annotation.h                                                     */
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

class PDF3DView;
class PDFJavascript;

class PDFAnnotation : public Reference {
	GDCLASS(PDFAnnotation, Reference);

public:
	enum BorderStyleSubtype {
		BORDER_STYLE_SOLID,
		BORDER_STYLE_DASHED,
		BORDER_STYLE_BEVELED,
		BORDER_STYLE_INSET,
		BORDER_STYLE_UNDERLINED
	};

	uint32_t set_rgb_color(const Color &p_color);
	uint32_t set_cmyk_color(const Vector4 &p_color);
	uint32_t set_gray_color(const float p_color);
	uint32_t set_no_color();

	uint32_t set_border_style(const BorderStyleSubtype p_sub_type, const float p_width, const uint16_t p_dash_on, const uint16_t p_dash_off, const uint16_t p_dash_phase);

	uint32_t get_status();

	PDFAnnotation();
	~PDFAnnotation();

	void *_get_hpdf_annotation() const;
	void _set_hpdf_annotation(void *p_annotation);

protected:
	static void _bind_methods();

	void *_annotation;

	uint32_t _status;
};

VARIANT_ENUM_CAST(PDFAnnotation::BorderStyleSubtype);

class PDFAnnotation3D : public PDFAnnotation {
	GDCLASS(PDFAnnotation3D, PDFAnnotation);

public:
	uint32_t set_3d_view(const Ref<PDFAnnotation> &p_annot, const Ref<PDF3DView> &p_view);

	PDFAnnotation3D();
	~PDFAnnotation3D();

	void _set_page(void *p_hdpf_page);

protected:
	static void _bind_methods();

	void *_hdpf_page;
};

class PDFAnnotationText : public PDFAnnotation {
	GDCLASS(PDFAnnotationText, PDFAnnotation);

public:
	enum AnnotIcon {
		ICON_COMMENT = 0,
		ICON_KEY,
		ICON_NOTE,
		ICON_HELP,
		ICON_NEW_PARAGRAPH,
		ICON_PARAGRAPH,
		ICON_INSERT,
		ICON_EOF
	};

	uint32_t set_icon(const AnnotIcon p_icon);
	uint32_t set_opened(const bool p_opened);

	PDFAnnotationText();
	~PDFAnnotationText();

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(PDFAnnotationText::AnnotIcon);

class PDFAnnotationLine : public PDFAnnotation {
	GDCLASS(PDFAnnotationLine, PDFAnnotation);

public:
	enum LineAnnotEndingStyle {
		LINE_ANNOT_NONE = 0,
		LINE_ANNOT_SQUARE,
		LINE_ANNOT_CIRCLE,
		LINE_ANNOT_DIAMOND,
		LINE_ANNOT_OPENARROW,
		LINE_ANNOT_CLOSEDARROW,
		LINE_ANNOT_BUTT,
		LINE_ANNOT_ROPENARROW,
		LINE_ANNOT_RCLOSEDARROW,
		LINE_ANNOT_SLASH
	};

	enum LineAnnotCapPosition {
		LINE_ANNOT_CAP_INLINE = 0,
		LINE_ANNOT_CAP_TOP
	};

	uint32_t set_position(const Vector2 &p_start_point, const LineAnnotEndingStyle p_start_style, const Vector2 &p_end_point, const LineAnnotEndingStyle p_end_style);
	uint32_t set_leader(const int p_leader_length, const int p_leader_ext_length, const int p_leader_offset_length);
	uint32_t set_caption(bool p_show_caption, const LineAnnotCapPosition p_position, const int p_horizontal_offset, const int p_vertical_offset);

	PDFAnnotationLine();
	~PDFAnnotationLine();

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(PDFAnnotationLine::LineAnnotEndingStyle);
VARIANT_ENUM_CAST(PDFAnnotationLine::LineAnnotCapPosition);

class PDFAnnotationFreeText : public PDFAnnotation {
	GDCLASS(PDFAnnotationFreeText, PDFAnnotation);

public:
	uint32_t set_line_ending_style(const PDFAnnotationLine::LineAnnotEndingStyle p_start_style, const PDFAnnotationLine::LineAnnotEndingStyle p_end_style);
	uint32_t set_3_point_callout_line(const Vector2 &p_start_point, const Vector2 &p_knee_point, const Vector2 &p_end_point);
	uint32_t set_2_point_callout_line(const Vector2 &p_start_point, const Vector2 &p_end_point);
	uint32_t set_default_style(const String &p_style);

	PDFAnnotationFreeText();
	~PDFAnnotationFreeText();

protected:
	static void _bind_methods();
};

class PDFAnnotationWidgetWhitePrint : public PDFAnnotation {
	GDCLASS(PDFAnnotationWidgetWhitePrint, PDFAnnotation);

public:
	PDFAnnotationWidgetWhitePrint();
	~PDFAnnotationWidgetWhitePrint();

protected:
	static void _bind_methods();
};

class PDFAnnotationWidget : public PDFAnnotation {
	GDCLASS(PDFAnnotationWidget, PDFAnnotation);

public:
	PDFAnnotationWidget();
	~PDFAnnotationWidget();

protected:
	static void _bind_methods();
};

class PDFAnnotationLink : public PDFAnnotation {
	GDCLASS(PDFAnnotationLink, PDFAnnotation);

public:
	enum HighlightMode {
		HIGHLIGHT_MODE_NO_HIGHTLIGHT = 0,
		HIGHLIGHT_MODE_INVERT_BOX,
		HIGHLIGHT_MODE_INVERT_BORDER,
		HIGHLIGHT_MODE_DOWN_APPEARANCE,
		HIGHLIGHT_MODE_HIGHTLIGHT_MODE_EOF
	};

	uint32_t set_highlight_mode(const HighlightMode p_highlight_mode);
	uint32_t set_javascript(const Ref<PDFJavascript> &p_javascript);
	uint32_t set_border_style(const float p_width, const uint16_t p_dash_on, const uint16_t p_dash_off);

	PDFAnnotationLink();
	~PDFAnnotationLink();

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(PDFAnnotationLink::HighlightMode);

class PDFAnnotationURILink : public PDFAnnotation {
	GDCLASS(PDFAnnotationURILink, PDFAnnotation);

public:
	PDFAnnotationURILink();
	~PDFAnnotationURILink();

protected:
	static void _bind_methods();
};

// TODO
// Need to add PDFAnnotationMarkup
// Lots of things inherit from it

class PDFAnnotationTextMarkup : public PDFAnnotation {
	GDCLASS(PDFAnnotationTextMarkup, PDFAnnotation);

public:
	enum AnnotSubType {
		ANNOTATION_TYPE_TEXT_NOTES,
		ANNOTATION_TYPE_LINK,
		ANNOTATION_TYPE_SOUND,
		ANNOTATION_TYPE_FREE_TEXT,
		ANNOTATION_TYPE_STAMP,
		ANNOTATION_TYPE_SQUARE,
		ANNOTATION_TYPE_CIRCLE,
		ANNOTATION_TYPE_STRIKE_OUT,
		ANNOTATION_TYPE_HIGHTLIGHT,
		ANNOTATION_TYPE_UNDERLINE,
		ANNOTATION_TYPE_INK,
		ANNOTATION_TYPE_FILE_ATTACHMENT,
		ANNOTATION_TYPE_POPUP,
		ANNOTATION_TYPE_3D,
		ANNOTATION_TYPE_SQUIGGLY,
		ANNOTATION_TYPE_LINE,
		ANNOTATION_TYPE_PROJECTION,
		ANNOTATION_TYPE_WIDGET
	};

#if 0
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetTitle(HPDF_Annotation annot, const char *name);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetSubject(HPDF_Annotation annot, const char *name);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetCreationDate(HPDF_Annotation annot, HPDF_Date value);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetTransparency(HPDF_Annotation annot, HPDF_REAL value);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetIntent(HPDF_Annotation annot, HPDF_AnnotIntent intent);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetPopup(HPDF_Annotation annot, HPDF_Annotation popup);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetRectDiff(HPDF_Annotation annot, HPDF_Rect rect); /* RD entry */

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetCloudEffect(HPDF_Annotation annot, HPDF_INT cloudIntensity); /* BE entry */

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetInteriorRGBColor(HPDF_Annotation annot, HPDF_RGBColor color); /* IC with RGB entry */

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetInteriorCMYKColor(HPDF_Annotation annot, HPDF_CMYKColor color); /* IC with CMYK entry */

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetInteriorGrayColor(HPDF_Annotation annot, HPDF_REAL color); /* IC with Gray entry */

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_MarkupAnnot_SetInteriorTransparent(HPDF_Annotation annot); /* IC with No Color entry */
#endif

#if 0
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_TextMarkupAnnot_SetQuadPoints(HPDF_Annotation annot, HPDF_Point lb, HPDF_Point rb, HPDF_Point rt, HPDF_Point lt); /* l-left, r-right, b-bottom, t-top positions */
#endif

	PDFAnnotationTextMarkup();
	~PDFAnnotationTextMarkup();

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(PDFAnnotationTextMarkup::AnnotSubType);

class PDFAnnotationHighlight : public PDFAnnotation {
	GDCLASS(PDFAnnotationHighlight, PDFAnnotation);

public:
	PDFAnnotationHighlight();
	~PDFAnnotationHighlight();

protected:
	static void _bind_methods();
};

class PDFAnnotationUnderline : public PDFAnnotation {
	GDCLASS(PDFAnnotationUnderline, PDFAnnotation);

public:
	PDFAnnotationUnderline();
	~PDFAnnotationUnderline();

protected:
	static void _bind_methods();
};

class PDFAnnotationSquiggly : public PDFAnnotation {
	GDCLASS(PDFAnnotationSquiggly, PDFAnnotation);

public:
	PDFAnnotationSquiggly();
	~PDFAnnotationSquiggly();

protected:
	static void _bind_methods();
};

class PDFAnnotationStrikeOut : public PDFAnnotation {
	GDCLASS(PDFAnnotationStrikeOut, PDFAnnotation);

public:
	PDFAnnotationStrikeOut();
	~PDFAnnotationStrikeOut();

protected:
	static void _bind_methods();
};

class PDFAnnotationPopup : public PDFAnnotation {
	GDCLASS(PDFAnnotationPopup, PDFAnnotation);

public:
#if 0
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_PopupAnnot_SetOpened(HPDF_Annotation annot,
			HPDF_BOOL opened);
#endif

	PDFAnnotationPopup();
	~PDFAnnotationPopup();

protected:
	static void _bind_methods();
};

class PDFAnnotationStamp : public PDFAnnotation {
	GDCLASS(PDFAnnotationStamp, PDFAnnotation);

public:
	enum StampAnnotName {
		STAMP_ANNOT_APPROVED = 0,
		STAMP_ANNOT_EXPERIMENTAL,
		STAMP_ANNOT_NOTAPPROVED,
		STAMP_ANNOT_ASIS,
		STAMP_ANNOT_EXPIRED,
		STAMP_ANNOT_NOTFORPUBLICRELEASE,
		STAMP_ANNOT_CONFIDENTIAL,
		STAMP_ANNOT_FINAL,
		STAMP_ANNOT_SOLD,
		STAMP_ANNOT_DEPARTMENTAL,
		STAMP_ANNOT_FORCOMMENT,
		STAMP_ANNOT_TOPSECRET,
		STAMP_ANNOT_DRAFT,
		STAMP_ANNOT_FORPUBLICRELEASE
	};

	PDFAnnotationStamp();
	~PDFAnnotationStamp();

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(PDFAnnotationStamp::StampAnnotName);

class PDFAnnotationProjection : public PDFAnnotation {
	GDCLASS(PDFAnnotationProjection, PDFAnnotation);

public:
#if 0
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_ProjectionAnnot_SetExData(HPDF_Annotation annot, HPDF_ExData exdata);
#endif

	PDFAnnotationProjection();
	~PDFAnnotationProjection();

protected:
	static void _bind_methods();
};

class PDFAnnotationSquare : public PDFAnnotation {
	GDCLASS(PDFAnnotationSquare, PDFAnnotation);

public:
	PDFAnnotationSquare();
	~PDFAnnotationSquare();

protected:
	static void _bind_methods();
};

class PDFAnnotationCircle : public PDFAnnotation {
	GDCLASS(PDFAnnotationCircle, PDFAnnotation);

public:
	PDFAnnotationCircle();
	~PDFAnnotationCircle();

protected:
	static void _bind_methods();
};

#endif
