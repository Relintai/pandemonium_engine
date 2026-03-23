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

class PDFAnnotation : public Reference {
	GDCLASS(PDFAnnotation, Reference);

public:
#if 0
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annot_SetRGBColor(HPDF_Annotation annot, HPDF_RGBColor color);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annot_SetCMYKColor(HPDF_Annotation annot, HPDF_CMYKColor color);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annot_SetGrayColor(HPDF_Annotation annot, HPDF_REAL color);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annot_SetNoColor(HPDF_Annotation annot);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annotation_SetBorderStyle(HPDF_Annotation annot,
			HPDF_BSSubtype subtype,
			HPDF_REAL width,
			HPDF_UINT16 dash_on,
			HPDF_UINT16 dash_off,
			HPDF_UINT16 dash_phase);

#endif

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

class PDFAnnotation3D : public PDFAnnotation {
	GDCLASS(PDFAnnotation3D, PDFAnnotation);

public:
#if 0
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annot_Set3DView(HPDF_MMgr mmgr,
			HPDF_Annotation annot,
			HPDF_Annotation annot3d,
			HPDF_Dict view);
#endif

	PDFAnnotation3D();
	~PDFAnnotation3D();

protected:
	static void _bind_methods();
};

class PDFAnnotationText : public PDFAnnotation {
	GDCLASS(PDFAnnotationText, PDFAnnotation);

public:
#if 0
#endif

	PDFAnnotationText();
	~PDFAnnotationText();

protected:
	static void _bind_methods();
};

class PDFAnnotationFreeText : public PDFAnnotation {
	GDCLASS(PDFAnnotationFreeText, PDFAnnotation);

public:
#if 0
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_FreeTextAnnot_SetLineEndingStyle(HPDF_Annotation annot, HPDF_LineAnnotEndingStyle startStyle, HPDF_LineAnnotEndingStyle endStyle);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_FreeTextAnnot_Set3PointCalloutLine(HPDF_Annotation annot, HPDF_Point startPoint, HPDF_Point kneePoint, HPDF_Point endPoint); /* Callout line will be in default user space */

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_FreeTextAnnot_Set2PointCalloutLine(HPDF_Annotation annot, HPDF_Point startPoint, HPDF_Point endPoint); /* Callout line will be in default user space */

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_FreeTextAnnot_SetDefaultStyle(HPDF_Annotation annot, const char *style);
#endif

	PDFAnnotationFreeText();
	~PDFAnnotationFreeText();

protected:
	static void _bind_methods();
};

class PDFAnnotationLine : public PDFAnnotation {
	GDCLASS(PDFAnnotationLine, PDFAnnotation);

public:
#if 0
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_LineAnnot_SetPosition(HPDF_Annotation annot,
			HPDF_Point startPoint, HPDF_LineAnnotEndingStyle startStyle,
			HPDF_Point endPoint, HPDF_LineAnnotEndingStyle endStyle);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_LineAnnot_SetLeader(HPDF_Annotation annot, HPDF_INT leaderLen, HPDF_INT leaderExtLen, HPDF_INT leaderOffsetLen);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_LineAnnot_SetCaption(HPDF_Annotation annot, HPDF_BOOL showCaption, HPDF_LineAnnotCapPosition position, HPDF_INT horzOffset, HPDF_INT vertOffset);
#endif

	PDFAnnotationLine();
	~PDFAnnotationLine();

protected:
	static void _bind_methods();
};

class PDFAnnotationWidgetWhitePrint : public PDFAnnotation {
	GDCLASS(PDFAnnotationWidgetWhitePrint, PDFAnnotation);

public:
#if 0
#endif

	PDFAnnotationWidgetWhitePrint();
	~PDFAnnotationWidgetWhitePrint();

protected:
#if 0
#endif

	static void _bind_methods();
};

class PDFAnnotationWidget : public PDFAnnotation {
	GDCLASS(PDFAnnotationWidget, PDFAnnotation);

public:
#if 0
#endif

	PDFAnnotationWidget();
	~PDFAnnotationWidget();

protected:
	static void _bind_methods();
};

class PDFAnnotationLink : public PDFAnnotation {
	GDCLASS(PDFAnnotationLink, PDFAnnotation);

public:
#if 0
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_LinkAnnot_SetHighlightMode(HPDF_Annotation annot,
			HPDF_AnnotHighlightMode mode);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_LinkAnnot_SetJavaScript(HPDF_Annotation annot,
			HPDF_JavaScript javascript);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_LinkAnnot_SetBorderStyle(HPDF_Annotation annot,
			HPDF_REAL width,
			HPDF_UINT16 dash_on,
			HPDF_UINT16 dash_off);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_TextAnnot_SetIcon(HPDF_Annotation annot,
			HPDF_AnnotIcon icon);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_TextAnnot_SetOpened(HPDF_Annotation annot,
			HPDF_BOOL opened);
#endif

	PDFAnnotationLink();
	~PDFAnnotationLink();

protected:
	static void _bind_methods();
};

class PDFAnnotationURILink : public PDFAnnotation {
	GDCLASS(PDFAnnotationURILink, PDFAnnotation);

public:
#if 0
#endif

	PDFAnnotationURILink();
	~PDFAnnotationURILink();

protected:
	static void _bind_methods();
};

class PDFAnnotationTextMarkup : public PDFAnnotation {
	GDCLASS(PDFAnnotationTextMarkup, PDFAnnotation);

public:
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

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_TextMarkupAnnot_SetQuadPoints(HPDF_Annotation annot, HPDF_Point lb, HPDF_Point rb, HPDF_Point rt, HPDF_Point lt); /* l-left, r-right, b-bottom, t-top positions */
#endif

	PDFAnnotationTextMarkup();
	~PDFAnnotationTextMarkup();

protected:
	static void _bind_methods();
};

class PDFAnnotationHighlight : public PDFAnnotation {
	GDCLASS(PDFAnnotationHighlight, PDFAnnotation);

public:
#if 0
#endif

	PDFAnnotationHighlight();
	~PDFAnnotationHighlight();

protected:
	static void _bind_methods();
};

class PDFAnnotationUnderline : public PDFAnnotation {
	GDCLASS(PDFAnnotationUnderline, PDFAnnotation);

public:
#if 0
#endif

	PDFAnnotationUnderline();
	~PDFAnnotationUnderline();

protected:
	static void _bind_methods();
};

class PDFAnnotationSquiggly : public PDFAnnotation {
	GDCLASS(PDFAnnotationSquiggly, PDFAnnotation);

public:
#if 0
#endif

	PDFAnnotationSquiggly();
	~PDFAnnotationSquiggly();

protected:
	static void _bind_methods();
};

class PDFAnnotationStrikeOut : public PDFAnnotation {
	GDCLASS(PDFAnnotationStrikeOut, PDFAnnotation);

public:
#if 0
#endif

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
#if 0
#endif

	PDFAnnotationStamp();
	~PDFAnnotationStamp();

protected:
	static void _bind_methods();
};

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
#if 0
#endif

	PDFAnnotationSquare();
	~PDFAnnotationSquare();

protected:
	static void _bind_methods();
};

class PDFAnnotationCircle : public PDFAnnotation {
	GDCLASS(PDFAnnotationCircle, PDFAnnotation);

public:
#if 0
#endif

	PDFAnnotationCircle();
	~PDFAnnotationCircle();

protected:
	static void _bind_methods();
};

#endif
