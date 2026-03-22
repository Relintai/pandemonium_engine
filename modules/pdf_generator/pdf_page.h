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

#include "pdf_document.h"

class PDFFont;
class PDFImage;
class PDFDashMode;
class PDFExtGState;
class PDFShading;

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
	float word_space_get();
	float horizontal_scalling_get();
	float text_leading_get();

	TextRenderingMode text_rendering_mode_get();

	float text_rise_get();

	Color rgb_fill_get();
	Color rgb_stroke_get();

	Vector4 cmyk_fill_get();
	Vector4 cmyk_stroke_get();

	float gray_fill_get();
	float gray_stroke_get();

	PDFDocument::ColorSpace stroking_color_space_get();
	PDFDocument::ColorSpace filling_color_space_get();

	Transform2D text_matrix_get();

	uint32_t g_state_depth_get();

	uint32_t ext_g_state_set(const Ref<PDFExtGState> &p_ext_g_state);
	uint32_t shading_set(const Ref<PDFShading> &p_shading);

#if 0
	/*--- Special graphic state operator --------------------------------------*/

	/* q */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_GSave(HPDF_Page page);

	/* Q */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_GRestore(HPDF_Page page);

	/* cm */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_Concat(HPDF_Page page,
			HPDF_REAL a,
			HPDF_REAL b,
			HPDF_REAL c,
			HPDF_REAL d,
			HPDF_REAL x,
			HPDF_REAL y);

#endif

#if 0
	/*--- Path construction operator ------------------------------------------*/

	/* m */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_MoveTo(HPDF_Page page,
			HPDF_REAL x,
			HPDF_REAL y);

	/* l */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_LineTo(HPDF_Page page,
			HPDF_REAL x,
			HPDF_REAL y);

	/* c */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_CurveTo(HPDF_Page page,
			HPDF_REAL x1,
			HPDF_REAL y1,
			HPDF_REAL x2,
			HPDF_REAL y2,
			HPDF_REAL x3,
			HPDF_REAL y3);

	/* v */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_CurveTo2(HPDF_Page page,
			HPDF_REAL x2,
			HPDF_REAL y2,
			HPDF_REAL x3,
			HPDF_REAL y3);

	/* y */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_CurveTo3(HPDF_Page page,
			HPDF_REAL x1,
			HPDF_REAL y1,
			HPDF_REAL x3,
			HPDF_REAL y3);

	/* h */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_ClosePath(HPDF_Page page);

	/* re */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_Rectangle(HPDF_Page page,
			HPDF_REAL x,
			HPDF_REAL y,
			HPDF_REAL width,
			HPDF_REAL height);

#endif

#if 0
	/*--- Path painting operator ---------------------------------------------*/

	/* S */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_Stroke(HPDF_Page page);

	/* s */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_ClosePathStroke(HPDF_Page page);

	/* f */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_Fill(HPDF_Page page);

	/* f* */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_Eofill(HPDF_Page page);

	/* B */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_FillStroke(HPDF_Page page);

	/* B* */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_EofillStroke(HPDF_Page page);

	/* b */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_ClosePathFillStroke(HPDF_Page page);

	/* b* */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_ClosePathEofillStroke(HPDF_Page page);

	/* n */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_EndPath(HPDF_Page page);

	/*--- Clipping paths operator --------------------------------------------*/

	/* W */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_Clip(HPDF_Page page);

	/* W* */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_Eoclip(HPDF_Page page);

#endif

	uint32_t begin_text();
	uint32_t end_text();

	uint32_t set_font_and_size(const Ref<PDFFont> &p_font, float p_size);

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
#endif

	/*--- Text positioning ---------------------------------------------------*/

	uint32_t move_text_pos(const real_t p_x, const real_t p_y);
	uint32_t move_text_posv(const Vector2 &p_move);

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

	uint32_t show_text(const String &p_text);

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

#endif

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

	uint32_t draw_image(const Ref<PDFImage> &p_image, const Rect2 &p_rect);

#if 0
	/*--- XObjects -----------------------------------------------------------*/

	/* Do */
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Page_ExecuteXObject(HPDF_Page page,
			HPDF_XObject obj);

#endif

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

	void
	HPDF_Page_SetFilter(HPDF_Page page,
			HPDF_UINT filter);

	HPDF_STATUS
	HPDF_Page_CheckState(HPDF_Page page,
			HPDF_UINT mode);

#endif

#if 0
	/*----- annotation ---------------------------------------------------------*/

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_Create3DAnnot(HPDF_Page page,
			HPDF_Rect rect,
			HPDF_BOOL tb,
			HPDF_BOOL np,
			HPDF_U3D u3d,
			HPDF_Image ap);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateTextAnnot(HPDF_Page page,
			HPDF_Rect rect,
			const char *text,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateFreeTextAnnot(HPDF_Page page,
			HPDF_Rect rect,
			const char *text,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateLineAnnot(HPDF_Page page,
			const char *text,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateWidgetAnnot_WhiteOnlyWhilePrint(HPDF_Doc pdf,
			HPDF_Page page,
			HPDF_Rect rect);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateWidgetAnnot(HPDF_Page page,
			HPDF_Rect rect);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateLinkAnnot(HPDF_Page page,
			HPDF_Rect rect,
			HPDF_Destination dst);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateURILinkAnnot(HPDF_Page page,
			HPDF_Rect rect,
			const char *uri);

	HPDF_Annotation
	HPDF_Page_CreateTextMarkupAnnot(HPDF_Page page,
			HPDF_Rect rect,
			const char *text,
			HPDF_Encoder encoder,
			HPDF_AnnotType subType);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateHighlightAnnot(HPDF_Page page,
			HPDF_Rect rect,
			const char *text,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateUnderlineAnnot(HPDF_Page page,
			HPDF_Rect rect,
			const char *text,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateSquigglyAnnot(HPDF_Page page,
			HPDF_Rect rect,
			const char *text,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateStrikeOutAnnot(HPDF_Page page,
			HPDF_Rect rect,
			const char *text,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreatePopupAnnot(HPDF_Page page,
			HPDF_Rect rect,
			HPDF_Annotation parent);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateStampAnnot(HPDF_Page page,
			HPDF_Rect rect,
			HPDF_StampAnnotName name,
			const char *text,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateProjectionAnnot(HPDF_Page page,
			HPDF_Rect rect,
			const char *text,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateSquareAnnot(HPDF_Page page,
			HPDF_Rect rect,
			const char *text,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_Annotation)
	HPDF_Page_CreateCircleAnnot(HPDF_Page page,
			HPDF_Rect rect,
			const char *text,
			HPDF_Encoder encoder);

#endif

#if 0
	/*----- 3D Measure ---------------------------------------------------------*/
	HPDF_EXPORT(HPDF_3DMeasure)
	HPDF_Page_Create3DC3DMeasure(HPDF_Page page,
			HPDF_Point3D firstanchorpoint,
			HPDF_Point3D textanchorpoint);

	HPDF_EXPORT(HPDF_3DMeasure)
	HPDF_Page_CreatePD33DMeasure(HPDF_Page page,
			HPDF_Point3D annotationPlaneNormal,
			HPDF_Point3D firstAnchorPoint,
			HPDF_Point3D secondAnchorPoint,
			HPDF_Point3D leaderLinesDirection,
			HPDF_Point3D measurementValuePoint,
			HPDF_Point3D textYDirection,
			HPDF_REAL value,
			const char *unitsString);

#endif

#if 0
	/*--------------------------------------------------------------------------*/
	/*----- External Data ---------------------------------------------------------*/

	HPDF_EXPORT(HPDF_ExData)
	HPDF_Page_Create3DAnnotExData(HPDF_Page page);

	/*--------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------*/
	/*----- 3D View ---------------------------------------------------------*/

	HPDF_EXPORT(HPDF_Dict)
	HPDF_Page_Create3DView(HPDF_Page page,
			HPDF_U3D u3d,
			HPDF_Annotation annot3d,
			const char *name);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DView_Add3DC3DMeasure(HPDF_Dict view,
			HPDF_3DMeasure measure);

#endif

#if 0
	/*----- destination --------------------------------------------------------*/

	HPDF_EXPORT(HPDF_Destination)
	HPDF_Page_CreateDestination(HPDF_Page page);

#endif

	uint32_t get_status();

	PDFPage();
	~PDFPage();

	void *_get_hpdf_page() const;
	void _set_hpdf_page(void *p_page);

protected:
	static void _bind_methods();

	void *_page;

	uint32_t _status;
};

VARIANT_ENUM_CAST(PDFPage::PageBoundary);
VARIANT_ENUM_CAST(PDFPage::PageSizes);
VARIANT_ENUM_CAST(PDFPage::PageDirection);
VARIANT_ENUM_CAST(PDFPage::LineCap);
VARIANT_ENUM_CAST(PDFPage::LineJoin);
VARIANT_ENUM_CAST(PDFPage::TextRenderingMode);

#endif
