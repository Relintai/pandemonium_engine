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

#include "hpdf.h"
#include "hpdf_doc.h"
#include "hpdf_pages.h"

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
	//ClassDB::bind_method(D_METHOD("get_width"), &PDFAnnotation::get_width);
	//ClassDB::bind_method(D_METHOD("set_width", "val"), &PDFAnnotation::set_width);
	//ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");
}

// PDFAnnotation3D

PDFAnnotation3D::PDFAnnotation3D() {
}
PDFAnnotation3D::~PDFAnnotation3D() {
}

void PDFAnnotation3D::_bind_methods() {
}

//  PDFAnnotationText

PDFAnnotationText::PDFAnnotationText() {
}
PDFAnnotationText::~PDFAnnotationText() {
}

void PDFAnnotationText::_bind_methods() {
}

// PDFAnnotationFreeText

PDFAnnotationFreeText::PDFAnnotationFreeText() {
}
PDFAnnotationFreeText::~PDFAnnotationFreeText() {
}

void PDFAnnotationFreeText::_bind_methods() {
}

// PDFAnnotationFreeText

PDFAnnotationLine::PDFAnnotationLine() {
}
PDFAnnotationLine::~PDFAnnotationLine() {
}

void PDFAnnotationLine::_bind_methods() {
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

PDFAnnotationLink::PDFAnnotationLink() {
}
PDFAnnotationLink::~PDFAnnotationLink() {
}

void PDFAnnotationLink::_bind_methods() {
}

// PDFAnnotationURILink

PDFAnnotationURILink::PDFAnnotationURILink() {
}
PDFAnnotationURILink::~PDFAnnotationURILink() {
}

void PDFAnnotationURILink::_bind_methods() {
}

// 5DFAnnotationTextMarkup

PDFAnnotationTextMarkup::PDFAnnotationTextMarkup() {
}
PDFAnnotationTextMarkup::~PDFAnnotationTextMarkup() {
}

void PDFAnnotationTextMarkup::_bind_methods() {
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
