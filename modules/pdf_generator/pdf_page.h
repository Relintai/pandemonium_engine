#ifndef PDF_PAGE_H
#define PDF_PAGE_H

/*************************************************************************/
/*  pdf_page.h                                                           */
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

#include "pdf_annotation.h"
#include "pdf_document.h"

class PDFFont;
class PDFImage;
class PDFDashMode;
class PDFExtGState;
class PDFShading;
class PDFXObject;
class PDF3DView;
class PDF3DMeasure;
class PDFExData;
class PDFDestination;

class PDFPage : public Reference {
	GDCLASS(PDFPage, Reference);

public:
	enum PageBoundary {
		PAGE_BOUNDARY_MEDIABOX = 0,
		PAGE_BOUNDARY_CROPBOX,
		PAGE_BOUNDARY_BLEEDBOX,
		PAGE_BOUNDARY_TRIMBOX,
		PAGE_BOUNDARY_ARTBOX
	};

	enum PageSizes {
		PAGE_SIZE_LETTER = 0,
		PAGE_SIZE_LEGAL,
		PAGE_SIZE_A3,
		PAGE_SIZE_A4,
		PAGE_SIZE_A5,
		PAGE_SIZE_B4,
		PAGE_SIZE_B5,
		PAGE_SIZE_EXECUTIVE,
		PAGE_SIZE_US4x6,
		PAGE_SIZE_US4x8,
		PAGE_SIZE_US5x7,
		PAGE_SIZE_COMM10,
		PAGE_SIZE_EOF
	};

	enum PageDirection {
		PAGE_DIRECTION_PORTRAIT = 0,
		PAGE_DIRECTION_LANDSCAPE
	};

	enum LineCap {
		LINE_CAP_BUTT_END = 0,
		LINE_CAP_ROUND_END,
		LINE_CAP_PROJECTING_SQUARE_END,
		LINE_CAP_LINECAP_EOF
	};

	enum LineJoin {
		LINE_JOIN_MITER_JOIN = 0,
		LINE_JOIN_ROUND_JOIN,
		LINE_JOIN_BEVEL_JOIN,
		LINE_JOIN_EOF
	};

	enum TextRenderingMode {
		TEXT_RENDERING_MODE_FILL = 0,
		TEXT_RENDERING_MODE_STROKE,
		TEXT_RENDERING_MODE_FILL_THEN_STROKE,
		TEXT_RENDERING_MODE_INVISIBLE,
		TEXT_RENDERING_MODE_FILL_CLIPPING,
		TEXT_RENDERING_MODE_STROKE_CLIPPING,
		TEXT_RENDERING_MODE_FILL_STROKE_CLIPPING,
		TEXT_RENDERING_MODE_CLIPPING,
		TEXT_RENDERING_MODE_EOF
	};

	enum TextAlignment {
		TEXT_ALIGN_LEFT = 0,
		TEXT_ALIGN_RIGHT,
		TEXT_ALIGN_CENTER,
		TEXT_ALIGN_JUSTIFY
	};

	enum TransitionStyle {
		TRANSITION_STYLE_WIPE_RIGHT = 0,
		TRANSITION_STYLE_WIPE_UP,
		TRANSITION_STYLE_WIPE_LEFT,
		TRANSITION_STYLE_WIPE_DOWN,
		TRANSITION_STYLE_BARN_DOORS_HORIZONTAL_OUT,
		TRANSITION_STYLE_BARN_DOORS_HORIZONTAL_IN,
		TRANSITION_STYLE_BARN_DOORS_VERTICAL_OUT,
		TRANSITION_STYLE_BARN_DOORS_VERTICAL_IN,
		TRANSITION_STYLE_BOX_OUT,
		TRANSITION_STYLE_BOX_IN,
		TRANSITION_STYLE_BLINDS_HORIZONTAL,
		TRANSITION_STYLE_BLINDS_VERTICAL,
		TRANSITION_STYLE_DISSOLVE,
		TRANSITION_STYLE_GLITTER_RIGHT,
		TRANSITION_STYLE_GLITTER_DOWN,
		TRANSITION_STYLE_GLITTER_TOP_LEFT_TO_BOTTOM_RIGHT,
		TRANSITION_STYLE_REPLACE,
		TRANSITION_STYLE_EOF
	};

	float get_width();
	void set_width(const float p_width);

	float get_height();
	void set_height(const float p_height);

	uint32_t boundary_set(const PageBoundary p_boundary, const float p_left, const float p_bottom, const float p_right, const float p_top);
	uint32_t size_set(const PageSizes p_size, const PageDirection p_direction);

	uint32_t rotate_set(const uint16_t p_angle);
	uint32_t zoom_set(const float p_zoom);

	float text_width(const String &p_text);
	float measure_text(const String &p_text, float p_width, bool p_word_wrap);

	uint16_t g_mode_get();

	Vector2 current_pos_get();
	Vector2 current_text_pos_get();

	Ref<PDFFont> current_font_get();
	float current_font_size_get();

	Transform2D trans_matrix_get();

	float line_width_get();
	void line_width_set(const float p_value);

	LineCap line_cap_get();
	void line_cap_set(const LineCap p_value);

	LineJoin line_join_get();
	void line_join_set(const LineJoin p_value);

	float miter_limit_get();
	void miter_limit_set(const float p_value);

	Ref<PDFDashMode> dash_get();
	void dash_set(const Ref<PDFDashMode> &p_mode);

	float flat_get();
	void flat_set(const float p_value);

	float char_space_get();
	void char_space_set(const float p_value);

	float word_space_get();
	void word_space_set(const float p_value);

	float horizontal_scalling_get();
	void horizontal_scalling_set(const float p_value);

	float text_leading_get();
	void text_leading_set(const float p_value);

	TextRenderingMode text_rendering_mode_get();
	void text_rendering_mode_set(const TextRenderingMode p_value);

	float text_rise_get();
	void text_rise_set(const float p_value);

	Color rgb_fill_get();
	void rgb_fill_set(const Color &p_color);

	Color rgb_stroke_get();
	void rgb_stroke_set(const Color &p_color);

	Vector4 cmyk_fill_get();
	void cmyk_fill_set(const Vector4 &p_color);

	Vector4 cmyk_stroke_get();
	void cmyk_stroke_set(const Vector4 &p_color);

	float gray_fill_get();
	void gray_fill_set(const float p_color);

	float gray_stroke_get();
	void gray_stroke_set(const float p_color);

	PDFDocument::ColorSpace stroking_color_space_get();
	PDFDocument::ColorSpace filling_color_space_get();

	Transform2D text_matrix_get();
	void text_matrix_set(const Transform2D &p_matrix);

	uint32_t g_state_depth_get();

	uint32_t ext_g_state_set(const Ref<PDFExtGState> &p_ext_g_state);
	uint32_t shading_set(const Ref<PDFShading> &p_shading);

	uint32_t g_save();
	uint32_t g_restore();

	uint32_t concat(const Transform2D &p_transform);

	uint32_t path_move_to(const Vector2 &p_position);
	uint32_t path_line_to(const Vector2 &p_position);
	uint32_t path_curve_to(const Vector2 &p_cp_start, const Vector2 &p_cp_end, const Vector2 &p_end_point);
	uint32_t path_curve_to_2(const Vector2 &p_cp_end, const Vector2 &p_end_point);
	uint32_t path_curve_to_3(const Vector2 &p_cp_start, const Vector2 &p_end_point);
	uint32_t path_rectangle(const Rect2 &p_rect);
	uint32_t path_close();

	uint32_t path_circle(const Vector2 &p_position, const float p_radius);
	uint32_t path_ellipse(const Vector2 &p_position, const Vector2 &p_radius);
	uint32_t path_arc(const Vector2 &p_position, const float p_radius, const float p_angle_1, const float p_angle_2);

	uint32_t path_stroke();
	uint32_t path_stroke_close();
	uint32_t path_fill();
	uint32_t path_eo_fill();
	uint32_t path_fill_stroke();
	uint32_t path_eo_fill_stroke();
	uint32_t path_fill_stroke_close();
	uint32_t path_eo_fill_stroke_close();
	uint32_t path_end();

	uint32_t clip();
	uint32_t eo_clip();

	uint32_t begin_text();
	uint32_t end_text();

	uint32_t set_font_and_size(const Ref<PDFFont> &p_font, float p_size);

	uint32_t move_text_pos(const real_t p_x, const real_t p_y);
	uint32_t move_text_posv(const Vector2 &p_move);

	uint32_t move_text_pos_2(const real_t p_x, const real_t p_y);
	uint32_t move_text_posv_2(const Vector2 &p_move);

	uint32_t move_to_next_line();

	uint32_t show_text(const String &p_text);
	uint32_t show_text_next_line(const String &p_text);
	uint32_t show_text_next_line_ex(const String &p_text, const float p_word_space, const float p_char_space);

	uint32_t execute_x_object(const Ref<PDFXObject> &p_x_object);

	uint32_t draw_image(const Ref<PDFImage> &p_image, const Rect2 &p_rect);
	uint32_t draw_text_out(const Vector2 &p_position, const String &p_text);
	uint32_t draw_text_rect(const Rect2 &p_rect, const String &p_text, const TextAlignment p_align = TEXT_ALIGN_LEFT);

	uint32_t slide_show_set(const TransitionStyle p_type, const float p_disp_time, const float p_trans_time);

	Ref<PDFAnnotation3D> annotation_3d_create(const Rect2 &p_rect, const bool p_tb, const bool p_np, const Ref<PDFU3D> &p_u3d, const Ref<PDFImage> &p_ap);
	Ref<PDFAnnotationText> annotation_text_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());
	Ref<PDFAnnotationFreeText> annotation_free_text_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());
	Ref<PDFAnnotationLine> annotation_line_create(const String &p_text, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());
	Ref<PDFAnnotationWidgetWhitePrint> annotation_widget_white_print_create(const Rect2 &p_rect);
	Ref<PDFAnnotationWidget> annotation_widget_create(const Rect2 &p_rect);
	Ref<PDFAnnotationLink> annotation_link_create(const Rect2 &p_rect, const Ref<PDFDestination> &p_destination);
	Ref<PDFAnnotationURILink> annotation_uri_link_create(const Rect2 &p_rect, const String &p_uri);
	Ref<PDFAnnotationTextMarkup> annotation_text_markup_create(const Rect2 &p_rect, const String &p_text, const PDFAnnotationTextMarkup::AnnotSubType p_sub_type, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());
	Ref<PDFAnnotationHighlight> annotation_highlight_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());
	Ref<PDFAnnotationUnderline> annotation_underline_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());
	Ref<PDFAnnotationSquiggly> annotation_squiggly_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());
	Ref<PDFAnnotationStrikeOut> annotation_strike_out_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());
	Ref<PDFAnnotationPopup> annotation_popup_create(const Rect2 &p_rect, const Ref<PDFAnnotation> &p_parent);
	Ref<PDFAnnotationStamp> annotation_stamp_create(const Rect2 &p_rect, const PDFAnnotationStamp::StampAnnotName p_name, const String &p_text, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());
	Ref<PDFAnnotationProjection> annotation_projection_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());
	Ref<PDFAnnotationSquare> annotation_square_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());
	Ref<PDFAnnotationCircle> annotation_circle_create(const Rect2 &p_rect, const String &p_text, const Ref<PDFEncoder> &p_encoder = Ref<PDFEncoder>());

	Ref<PDF3DMeasure> create_c3d_3d_measure(const Vector3 &p_first_anchor_point, const Vector3 &p_text_anchor_point);
	Ref<PDF3DMeasure> create_pd3_3d_measure(const PoolVector3Array &p_points, const float p_value, const String &p_units_string);

	Ref<PDFExData> create_3d_annot_ex_data();

	Ref<PDF3DView> create_3d_view_name(const String &p_name);
	Ref<PDF3DView> create_3d_view(const Ref<PDFU3D> &p_u3d, const Ref<PDFAnnotation3D> &p_annot_3d, const String &p_name);

	Ref<PDFDestination> create_destination();

	uint32_t get_status();

	Ref<PDFDocument> document_get();

	PDFPage();
	~PDFPage();

	void *_get_hpdf_page() const;
	void _set_hpdf_page(void *p_page);

	void _set_document(const Ref<PDFDocument> &p_document);

protected:
	static void _bind_methods();

	WRef<PDFDocument> _document;

	void *_page;

	uint32_t _status;
};

VARIANT_ENUM_CAST(PDFPage::PageBoundary);
VARIANT_ENUM_CAST(PDFPage::PageSizes);
VARIANT_ENUM_CAST(PDFPage::PageDirection);
VARIANT_ENUM_CAST(PDFPage::LineCap);
VARIANT_ENUM_CAST(PDFPage::LineJoin);
VARIANT_ENUM_CAST(PDFPage::TextRenderingMode);
VARIANT_ENUM_CAST(PDFPage::TextAlignment);
VARIANT_ENUM_CAST(PDFPage::TransitionStyle);

#endif
