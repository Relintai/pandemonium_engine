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

class PDFDocument : public Reference {
	GDCLASS(PDFDocument, Reference);

public:
	enum CompressionMode {
		COMPRESSION_MODE_NONE = 0x00,
		COMPRESSION_MODE_TEXT = 0x01,
		COMPRESSION_MODE_IMAGE = 0x02,
		COMPRESSION_MODE_METADATA = 0x04,
		COMPRESSION_MODE_ALL = 0x0F,
		//COMPRESSION_MODE_BEST_COMPRESS = 0x10,
		//COMPRESSION_MODE_BEST_SPEED = 0x20,
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

	uint32_t pages_set_configuration(const uint32_t p_page_per_pages);

#if 0


	HPDF_EXPORT(HPDF_PageMode)
	HPDF_GetPageMode(HPDF_Doc pdf);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetPageMode(HPDF_Doc pdf,
			HPDF_PageMode mode);


	HPDF_EXPORT(HPDF_STATUS)
	HPDF_AddPageLabel(HPDF_Doc pdf,
			HPDF_UINT page_num,
			HPDF_PageNumStyle style,
			HPDF_UINT first_page,
			const char *prefix);


	HPDF_EXPORT(HPDF_UINT)
	HPDF_GetViewerPreference(HPDF_Doc pdf);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetViewerPreference(HPDF_Doc pdf,
			HPDF_UINT value);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetOpenAction(HPDF_Doc pdf,
			HPDF_Destination open_action);

#endif

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

#if 0
	HPDF_EXPORT(HPDF_FontDef)
	HPDF_GetTTFontDefFromFile(HPDF_Doc pdf,
			const char *file_name,
			HPDF_BOOL embedding);
#endif

#if 0
	/*----- outline ------------------------------------------------------------*/

	HPDF_EXPORT(HPDF_Outline)
	HPDF_CreateOutline(HPDF_Doc pdf,
			HPDF_Outline parent,
			const char *title,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Outline_SetOpened(HPDF_Outline outline,
			HPDF_BOOL opened);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Outline_SetDestination(HPDF_Outline outline,
			HPDF_Destination dst);

#endif

#if 0
	/*----- destination --------------------------------------------------------*/

	HPDF_EXPORT(HPDF_Destination)
	HPDF_Page_CreateDestination(HPDF_Page page);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetXYZ(HPDF_Destination dst,
			HPDF_REAL left,
			HPDF_REAL top,
			HPDF_REAL zoom);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFit(HPDF_Destination dst);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitH(HPDF_Destination dst,
			HPDF_REAL top);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitV(HPDF_Destination dst,
			HPDF_REAL left);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitR(HPDF_Destination dst,
			HPDF_REAL left,
			HPDF_REAL bottom,
			HPDF_REAL right,
			HPDF_REAL top);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitB(HPDF_Destination dst);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitBH(HPDF_Destination dst,
			HPDF_REAL top);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Destination_SetFitBV(HPDF_Destination dst,
			HPDF_REAL left);

#endif

#if 0
	/*----- encoder ------------------------------------------------------------*/

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_UseJPEncodings(HPDF_Doc pdf);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_UseKREncodings(HPDF_Doc pdf);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_UseCNSEncodings(HPDF_Doc pdf);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_UseCNTEncodings(HPDF_Doc pdf);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_UseUTFEncodings(HPDF_Doc pdf);

#endif

#if 0
	/*----- XObject ------------------------------------------------------------*/

	HPDF_EXPORT(HPDF_XObject)
	HPDF_Page_CreateXObjectFromImage(HPDF_Doc pdf,
			HPDF_Page page,
			HPDF_Rect rect,
			HPDF_Image image,
			HPDF_BOOL zoom);

	HPDF_EXPORT(HPDF_XObject)
	HPDF_Page_CreateXObjectAsWhiteRect(HPDF_Doc pdf,
			HPDF_Page page,
			HPDF_Rect rect);

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

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annot_SetRGBColor(HPDF_Annotation annot, HPDF_RGBColor color);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annot_SetCMYKColor(HPDF_Annotation annot, HPDF_CMYKColor color);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annot_SetGrayColor(HPDF_Annotation annot, HPDF_REAL color);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annot_SetNoColor(HPDF_Annotation annot);

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

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annot_Set3DView(HPDF_MMgr mmgr,
			HPDF_Annotation annot,
			HPDF_Annotation annot3d,
			HPDF_Dict view);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_PopupAnnot_SetOpened(HPDF_Annotation annot,
			HPDF_BOOL opened);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_FreeTextAnnot_SetLineEndingStyle(HPDF_Annotation annot, HPDF_LineAnnotEndingStyle startStyle, HPDF_LineAnnotEndingStyle endStyle);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_FreeTextAnnot_Set3PointCalloutLine(HPDF_Annotation annot, HPDF_Point startPoint, HPDF_Point kneePoint, HPDF_Point endPoint); /* Callout line will be in default user space */

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_FreeTextAnnot_Set2PointCalloutLine(HPDF_Annotation annot, HPDF_Point startPoint, HPDF_Point endPoint); /* Callout line will be in default user space */

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_FreeTextAnnot_SetDefaultStyle(HPDF_Annotation annot, const char *style);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_LineAnnot_SetPosition(HPDF_Annotation annot,
			HPDF_Point startPoint, HPDF_LineAnnotEndingStyle startStyle,
			HPDF_Point endPoint, HPDF_LineAnnotEndingStyle endStyle);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_LineAnnot_SetLeader(HPDF_Annotation annot, HPDF_INT leaderLen, HPDF_INT leaderExtLen, HPDF_INT leaderOffsetLen);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_LineAnnot_SetCaption(HPDF_Annotation annot, HPDF_BOOL showCaption, HPDF_LineAnnotCapPosition position, HPDF_INT horzOffset, HPDF_INT vertOffset);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Annotation_SetBorderStyle(HPDF_Annotation annot,
			HPDF_BSSubtype subtype,
			HPDF_REAL width,
			HPDF_UINT16 dash_on,
			HPDF_UINT16 dash_off,
			HPDF_UINT16 dash_phase);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_ProjectionAnnot_SetExData(HPDF_Annotation annot, HPDF_ExData exdata);
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

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DMeasure_SetName(HPDF_3DMeasure measure,
			const char *name);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DMeasure_SetColor(HPDF_3DMeasure measure,
			HPDF_RGBColor color);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DMeasure_SetTextSize(HPDF_3DMeasure measure,
			HPDF_REAL textsize);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DC3DMeasure_SetTextBoxSize(HPDF_3DMeasure measure,
			HPDF_INT32 x,
			HPDF_INT32 y);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DC3DMeasure_SetText(HPDF_3DMeasure measure,
			const char *text,
			HPDF_Encoder encoder);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DC3DMeasure_SetProjectionAnotation(HPDF_3DMeasure measure,
			HPDF_Annotation projectionanotation);

	/*--------------------------------------------------------------------------*/
	/*----- External Data ---------------------------------------------------------*/

	HPDF_EXPORT(HPDF_ExData)
	HPDF_Page_Create3DAnnotExData(HPDF_Page page);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DAnnotExData_Set3DMeasurement(HPDF_ExData exdata, HPDF_3DMeasure measure);

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

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetPDFAConformance(HPDF_Doc pdf,
			HPDF_PDFAType pdfa_type);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_AddPDFAXmpExtension(HPDF_Doc pdf,
			const char *xmp_description);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_AppendOutputIntents(HPDF_Doc pdf, const char *iccname, HPDF_Dict iccdict);
#endif

#if 0
#define HPDF_ENCODING_FONT_SPECIFIC "FontSpecific"
#define HPDF_ENCODING_STANDARD "StandardEncoding"
#define HPDF_ENCODING_MAC_ROMAN "MacRomanEncoding"
#define HPDF_ENCODING_WIN_ANSI "WinAnsiEncoding"
#define HPDF_ENCODING_ISO8859_2 "ISO8859-2"
#define HPDF_ENCODING_ISO8859_3 "ISO8859-3"
#define HPDF_ENCODING_ISO8859_4 "ISO8859-4"
#define HPDF_ENCODING_ISO8859_5 "ISO8859-5"
#define HPDF_ENCODING_ISO8859_6 "ISO8859-6"
#define HPDF_ENCODING_ISO8859_7 "ISO8859-7"
#define HPDF_ENCODING_ISO8859_8 "ISO8859-8"
#define HPDF_ENCODING_ISO8859_9 "ISO8859-9"
#define HPDF_ENCODING_ISO8859_10 "ISO8859-10"
#define HPDF_ENCODING_ISO8859_11 "ISO8859-11"
#define HPDF_ENCODING_ISO8859_13 "ISO8859-13"
#define HPDF_ENCODING_ISO8859_14 "ISO8859-14"
#define HPDF_ENCODING_ISO8859_15 "ISO8859-15"
#define HPDF_ENCODING_ISO8859_16 "ISO8859-16"
#define HPDF_ENCODING_CP1250 "CP1250"
#define HPDF_ENCODING_CP1251 "CP1251"
#define HPDF_ENCODING_CP1252 "CP1252"
#define HPDF_ENCODING_CP1253 "CP1253"
#define HPDF_ENCODING_CP1254 "CP1254"
#define HPDF_ENCODING_CP1255 "CP1255"
#define HPDF_ENCODING_CP1256 "CP1256"
#define HPDF_ENCODING_CP1257 "CP1257"
#define HPDF_ENCODING_CP1258 "CP1258"
#define HPDF_ENCODING_KOI8_R "KOI8-R"
#endif

	Ref<PDFImage> image_load_png_from_mem(const PoolByteArray &p_data);
	Ref<PDFImage> image_load_png_from_file(const String &p_path);
	Ref<PDFImage> image_load_jpg_from_mem(const PoolByteArray &p_data);
	Ref<PDFImage> image_load_jpg_from_file(const String &p_path);
	Ref<PDFImage> image_create_pdf_from_image(const Ref<Image> &p_image);

#if 0
/*----- image data ---------------------------------------------------------*/

HPDF_EXPORT(HPDF_Image)
HPDF_LoadPngImageFromMem  (HPDF_Doc     pdf,
                    const HPDF_BYTE    *buffer,
                          HPDF_UINT     size);

HPDF_EXPORT(HPDF_Image)
HPDF_LoadPngImageFromFile (HPDF_Doc      pdf,
                           const char   *filename);


HPDF_EXPORT(HPDF_Image)
HPDF_LoadPngImageFromFile2 (HPDF_Doc      pdf,
                            const char   *filename);


HPDF_EXPORT(HPDF_Image)
HPDF_LoadJpegImageFromFile (HPDF_Doc      pdf,
                            const char   *filename);

HPDF_EXPORT(HPDF_Image)
HPDF_LoadJpegImageFromMem   (HPDF_Doc      pdf,
                      const HPDF_BYTE     *buffer,
                            HPDF_UINT      size);

HPDF_EXPORT(HPDF_Image)
HPDF_LoadU3DFromFile (HPDF_Doc      pdf,
                      const char   *filename);

HPDF_EXPORT(HPDF_Image)
HPDF_LoadU3DFromMem  (HPDF_Doc      pdf,
               const HPDF_BYTE     *buffer,
                     HPDF_UINT      size);

HPDF_EXPORT(HPDF_Image)
HPDF_Image_LoadRaw1BitImageFromMem  (HPDF_Doc           pdf,
                           const HPDF_BYTE   *buf,
                          HPDF_UINT          width,
                          HPDF_UINT          height,
                          HPDF_UINT          line_width,
                          HPDF_BOOL          black_is1,
                          HPDF_BOOL          top_is_first);


HPDF_EXPORT(HPDF_Image)
HPDF_LoadRawImageFromFile  (HPDF_Doc           pdf,
                            const char        *filename,
                            HPDF_UINT          width,
                            HPDF_UINT          height,
                            HPDF_ColorSpace    color_space);


HPDF_EXPORT(HPDF_Image)
HPDF_LoadRawImageFromMem  (HPDF_Doc           pdf,
                           const HPDF_BYTE   *buf,
                           HPDF_UINT          width,
                           HPDF_UINT          height,
                           HPDF_ColorSpace    color_space,
                           HPDF_UINT          bits_per_component);

HPDF_EXPORT(HPDF_STATUS)
HPDF_Image_AddSMask  (HPDF_Image    image,
                      HPDF_Image    smask);

HPDF_EXPORT(HPDF_Point)
HPDF_Image_GetSize (HPDF_Image  image);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Image_GetSize2 (HPDF_Image  image, HPDF_Point *size);


HPDF_EXPORT(HPDF_UINT)
HPDF_Image_GetWidth  (HPDF_Image   image);


HPDF_EXPORT(HPDF_UINT)
HPDF_Image_GetHeight  (HPDF_Image   image);


HPDF_EXPORT(HPDF_UINT)
HPDF_Image_GetBitsPerComponent (HPDF_Image  image);


HPDF_EXPORT(const char*)
HPDF_Image_GetColorSpace (HPDF_Image  image);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Image_SetColorMask (HPDF_Image   image,
                         HPDF_UINT    rmin,
                         HPDF_UINT    rmax,
                         HPDF_UINT    gmin,
                         HPDF_UINT    gmax,
                         HPDF_UINT    bmin,
                         HPDF_UINT    bmax);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Image_SetMaskImage  (HPDF_Image   image,
                          HPDF_Image   mask_image);

#endif

#if 0
	/*----- info dictionary ----------------------------------------------------*/

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetInfoAttr(HPDF_Doc pdf,
			HPDF_InfoType type,
			const char *value);

	HPDF_EXPORT(const char *)
	HPDF_GetInfoAttr(HPDF_Doc pdf,
			HPDF_InfoType type);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetInfoDateAttr(HPDF_Doc pdf,
			HPDF_InfoType type,
			HPDF_Date value);

	/*----- encryption ---------------------------------------------------------*/

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetPassword(HPDF_Doc pdf,
			const char *owner_passwd,
			const char *user_passwd);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetPermission(HPDF_Doc pdf,
			HPDF_UINT permission);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_SetEncryptionMode(HPDF_Doc pdf,
			HPDF_EncryptMode mode,
			HPDF_UINT key_len);

	/*----- attachments -------------------------------------------------------*/

	HPDF_EXPORT(HPDF_EmbeddedFile)
	HPDF_AttachFile(HPDF_Doc pdf,
			const char *file);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetName(HPDF_EmbeddedFile emfile,
			const char *name);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetDescription(HPDF_EmbeddedFile emfile,
			const char *new_description);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetSubtype(HPDF_EmbeddedFile emfile,
			const char *subtype);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetAFRelationship(HPDF_EmbeddedFile emfile,
			HPDF_AFRelationship relationship);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetSize(HPDF_EmbeddedFile emfile,
			HPDF_UINT64 size);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetCreationDate(HPDF_EmbeddedFile emfile,
			HPDF_Date creationDate);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_EmbeddedFile_SetLastModificationDate(HPDF_EmbeddedFile emfile,
			HPDF_Date lastModificationDate);

	/*----- extended graphics state --------------------------------------------*/

	HPDF_EXPORT(HPDF_ExtGState)
	HPDF_CreateExtGState(HPDF_Doc pdf);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_ExtGState_SetAlphaStroke(HPDF_ExtGState ext_gstate,
			HPDF_REAL value);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_ExtGState_SetAlphaFill(HPDF_ExtGState ext_gstate,
			HPDF_REAL value);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_ExtGState_SetBlendMode(HPDF_ExtGState ext_gstate,
			HPDF_BlendMode mode);

	/*--- Shading patterns ---------------------------------------------------*/

	/* Notes for docs:
	 * - ShadingType must be HPDF_SHADING_FREE_FORM_TRIANGLE_MESH (the only
	 *   defined option...)
	 * - colorSpace must be HPDF_CS_DEVICE_RGB for now.
	 */
	HPDF_EXPORT(HPDF_Shading)
	HPDF_Shading_New(HPDF_Doc pdf,
			HPDF_ShadingType type,
			HPDF_ColorSpace colorSpace,
			HPDF_REAL xMin, HPDF_REAL xMax,
			HPDF_REAL yMin, HPDF_REAL yMax);

	HPDF_EXPORT(HPDF_STATUS)
	HPDF_Shading_AddVertexRGB(HPDF_Shading shading,
			HPDF_Shading_FreeFormTriangleMeshEdgeFlag edgeFlag,
			HPDF_REAL x, HPDF_REAL y,
			HPDF_UINT8 r, HPDF_UINT8 g, HPDF_UINT8 b);

#endif

	PoolByteArray save_to_mem();
	uint32_t save_to_file(const String &p_file);

	uint32_t get_status();
	uint32_t get_error_no();
	uint32_t get_error_detail();
	void reset_error();

	PDFDocument();
	~PDFDocument();

protected:
	static void _bind_methods();

	HPDF_Doc _doc;

	uint32_t _status;
};

VARIANT_ENUM_CAST(PDFDocument::CompressionMode);
VARIANT_ENUM_CAST(PDFDocument::PageLayout);

#endif
