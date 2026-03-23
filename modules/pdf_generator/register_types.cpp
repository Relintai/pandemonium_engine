/*************************************************************************/
/*  register_types.cpp                                                   */
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

#include "register_types.h"

#include "pdf_3d_measure.h"
#include "pdf_3d_view.h"
#include "pdf_3d_view_node.h"
#include "pdf_annotation.h"
#include "pdf_dash_mode.h"
#include "pdf_date.h"
#include "pdf_destination.h"
#include "pdf_dict.h"
#include "pdf_document.h"
#include "pdf_embedded_file.h"
#include "pdf_encoder.h"
#include "pdf_ex_data.h"
#include "pdf_ext_g_state.h"
#include "pdf_font.h"
#include "pdf_image.h"
#include "pdf_javascript.h"
#include "pdf_outline.h"
#include "pdf_page.h"
#include "pdf_shading.h"
#include "pdf_u3d.h"
#include "pdf_x_object.h"

void register_pdf_generator_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<PDFDocument>();
		ClassDB::register_class<PDFPage>();
		ClassDB::register_class<PDFFont>();
		ClassDB::register_class<PDFImage>();
		ClassDB::register_class<PDFOutline>();
		ClassDB::register_class<PDFDestination>();
		ClassDB::register_class<PDF3DMeasure>();
		ClassDB::register_class<PDFEmbeddedFile>();
		ClassDB::register_class<PDFExData>();
		ClassDB::register_class<PDFExtGState>();
		ClassDB::register_class<PDFShading>();
		ClassDB::register_class<PDFXObject>();
		ClassDB::register_class<PDFEncoder>();
		ClassDB::register_class<PDFDict>();
		ClassDB::register_class<PDFDate>();
		ClassDB::register_class<PDFDashMode>();

		ClassDB::register_class<PDFU3D>();
		ClassDB::register_class<PDFJavascript>();
		ClassDB::register_class<PDF3DView>();
		ClassDB::register_class<PDF3DViewNode>();

		ClassDB::register_class<PDFAnnotation>();
		ClassDB::register_class<PDFAnnotation3D>();
		ClassDB::register_class<PDFAnnotationText>();
		ClassDB::register_class<PDFAnnotationFreeText>();
		ClassDB::register_class<PDFAnnotationLine>();
		ClassDB::register_class<PDFAnnotationWidgetWhitePrint>();
		ClassDB::register_class<PDFAnnotationWidget>();
		ClassDB::register_class<PDFAnnotationLink>();
		ClassDB::register_class<PDFAnnotationURILink>();
		ClassDB::register_class<PDFAnnotationTextMarkup>();
		ClassDB::register_class<PDFAnnotationHighlight>();
		ClassDB::register_class<PDFAnnotationUnderline>();
		ClassDB::register_class<PDFAnnotationSquiggly>();
		ClassDB::register_class<PDFAnnotationStrikeOut>();
		ClassDB::register_class<PDFAnnotationPopup>();
		ClassDB::register_class<PDFAnnotationStamp>();
		ClassDB::register_class<PDFAnnotationProjection>();
		ClassDB::register_class<PDFAnnotationSquare>();
		ClassDB::register_class<PDFAnnotationCircle>();
	}
}

void unregister_pdf_generator_types(ModuleRegistrationLevel p_level) {
}
