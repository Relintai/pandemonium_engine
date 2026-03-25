/*************************************************************************/
/*  pdf_u3d.cpp                                                          */
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

#include "pdf_u3d.h"

#include "hpdf.h"
#include "hpdf_u3d.h"

#include "pdf_3d_view.h"
#include "pdf_javascript.h"

uint32_t PDFU3D::add_3d_view(const Ref<PDF3DView> &p_view) {
	HPDF_Dict view = NULL;

	if (p_view.is_valid()) {
		view = (HPDF_Dict)p_view->_get_hpdf_3d_view();
	}

	_status = HPDF_U3D_Add3DView((HPDF_U3D)_u3d, view);

	return _status;
}

uint32_t PDFU3D::set_default_3d_view(const String &p_name) {
	_status = HPDF_U3D_SetDefault3DView((HPDF_U3D)_u3d, p_name.utf8().get_data());

	return _status;
}

uint32_t PDFU3D::add_on_instantiate(const Ref<PDFJavascript> &p_javascript) {
	HPDF_JavaScript javaScript = NULL;

	if (p_javascript.is_valid()) {
		javaScript = (HPDF_Dict)p_javascript->_get_hpdf_javascript();
	}

	_status = HPDF_U3D_AddOnInstanciate((HPDF_U3D)_u3d, javaScript);

	return _status;
}

uint32_t PDFU3D::get_status() {
	return _status;
}

PDFU3D::PDFU3D() {
	_status = 0;

	_u3d = NULL;
}

PDFU3D::~PDFU3D() {
}

void *PDFU3D::_get_hpdf_u3d() const {
	return _u3d;
}

void PDFU3D::_set_hpdf_u3d(void *p_u3d) {
	_u3d = p_u3d;
}

void PDFU3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_3d_view", "view"), &PDFU3D::add_3d_view);
	ClassDB::bind_method(D_METHOD("set_default_3d_view", "name"), &PDFU3D::set_default_3d_view);
	ClassDB::bind_method(D_METHOD("add_on_instantiate", "javascript"), &PDFU3D::add_on_instantiate);

	ClassDB::bind_method(D_METHOD("get_status"), &PDFU3D::get_status);
}
