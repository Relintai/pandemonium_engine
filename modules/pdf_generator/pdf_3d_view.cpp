/*************************************************************************/
/*  pdf_3d_view.cpp                                                      */
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

#include "pdf_3d_view.h"

#include "pdf_3d_measure.h"
#include "pdf_3d_view_node.h"
#include "pdf_encoder.h"

#include "hpdf.h"
#include "hpdf_u3d.h"

Ref<PDF3DViewNode> PDF3DView::create_node(const String &p_name) {
	HPDF_Dict hpdf_dict = HPDF_3DView_CreateNode((HPDF_Dict)_3d_view, p_name.utf8().get_data());

	if (!hpdf_dict) {
		return Ref<PDF3DViewNode>();
	}

	Ref<PDF3DViewNode> n;
	n.instance();
	n->_set_hpdf_3d_view_node(hpdf_dict);
	return n;
}

uint32_t PDF3DView::add_node(const Ref<PDF3DViewNode> &p_node) {
	HPDF_Dict hpdf_dict = NULL;

	if (p_node.is_valid()) {
		hpdf_dict = (HPDF_Dict)p_node->_get_hpdf_3d_view_node();
	}

	_status = HPDF_3DView_AddNode((HPDF_Dict)_3d_view, hpdf_dict);

	return _status;
}

uint32_t PDF3DView::set_lighting(const String &p_scheme) {
	_status = HPDF_3DView_SetLighting((HPDF_Dict)_3d_view, p_scheme.utf8().get_data());

	return _status;
}

uint32_t PDF3DView::set_background_color(const Color &p_color) {
	_status = HPDF_3DView_SetBackgroundColor((HPDF_Dict)_3d_view, p_color.r, p_color.g, p_color.b);

	return _status;
}

uint32_t PDF3DView::set_perspective_projection(const float p_fov) {
	_status = HPDF_3DView_SetPerspectiveProjection((HPDF_Dict)_3d_view, p_fov);

	return _status;
}
uint32_t PDF3DView::set_orthogonal_projection(const float p_mag) {
	_status = HPDF_3DView_SetOrthogonalProjection((HPDF_Dict)_3d_view, p_mag);

	return _status;
}

uint32_t PDF3DView::set_camera(const Vector3 &p_coo, const Vector3 &p_c2c, const float p_roo, const float p_roll) {
	_status = HPDF_3DView_SetCamera((HPDF_Dict)_3d_view, p_coo.x, p_coo.y, p_coo.z, p_c2c.x, p_c2c.y, p_c2c.z, p_roo, p_roll);

	return _status;
}
uint32_t PDF3DView::set_camera_by_matrix(const Transform &p_matrix, const float p_co) {
	const Basis &b = p_matrix.basis;

	HPDF_3DMatrix m;

	m.a = b.rows[0][0];
	m.b = b.rows[0][1];
	m.c = b.rows[0][2];
	m.d = b.rows[1][0];
	m.e = b.rows[1][1];
	m.f = b.rows[1][2];
	m.g = b.rows[2][0];
	m.h = b.rows[2][1];
	m.i = b.rows[2][2];
	m.tx = p_matrix.origin.x;
	m.ty = p_matrix.origin.y;
	m.tz = p_matrix.origin.z;

	_status = HPDF_3DView_SetCameraByMatrix((HPDF_Dict)_3d_view, m, p_co);

	return _status;
}

uint32_t PDF3DView::set_cross_section_on(const Vector3 &p_center, const float p_roll, const float p_pitch, const float p_opacity, bool p_showintersection) {
	HPDF_Point3D center;
	center.x = p_center.x;
	center.y = p_center.y;
	center.z = p_center.z;

	_status = HPDF_3DView_SetCrossSectionOn((HPDF_Dict)_3d_view, center, p_roll, p_pitch, p_opacity, p_showintersection);

	return _status;
}
uint32_t PDF3DView::set_cross_section_off() {
	_status = HPDF_3DView_SetCrossSectionOff((HPDF_Dict)_3d_view);

	return _status;
}

uint32_t PDF3DView::add_3d_c3d_measure(const Ref<PDF3DMeasure> &p_measure) {
	HPDF_3DMeasure hpdf_measure = NULL;

	if (p_measure.is_valid()) {
		hpdf_measure = (HPDF_3DMeasure)p_measure->_get_hpdf_3d_measure();
	}

	_status = HPDF_3DView_Add3DC3DMeasure((HPDF_Dict)_3d_view, hpdf_measure);

	return _status;
}

uint32_t PDF3DView::get_status() {
	return _status;
}

PDF3DView::PDF3DView() {
	_3d_view = NULL;
}

PDF3DView::~PDF3DView() {
}

void *PDF3DView::_get_hpdf_3d_view() const {
	return _3d_view;
}

void PDF3DView::_set_hpdf_3d_view(void *p_3d_view) {
	_3d_view = p_3d_view;
}

void PDF3DView::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create_node", "name"), &PDF3DView::create_node);

	ClassDB::bind_method(D_METHOD("add_node", "node"), &PDF3DView::add_node);

	ClassDB::bind_method(D_METHOD("set_lighting", "scheme"), &PDF3DView::set_lighting);
	ClassDB::bind_method(D_METHOD("set_background_color", "color"), &PDF3DView::set_background_color);

	ClassDB::bind_method(D_METHOD("set_perspective_projection", "fov"), &PDF3DView::set_perspective_projection);
	ClassDB::bind_method(D_METHOD("set_orthogonal_projection", "mag"), &PDF3DView::set_orthogonal_projection);

	ClassDB::bind_method(D_METHOD("set_camera", "coo", "c2c", "roo", "roll"), &PDF3DView::set_camera);
	ClassDB::bind_method(D_METHOD("set_camera_by_matrix", "matrix", "co"), &PDF3DView::set_camera_by_matrix);

	ClassDB::bind_method(D_METHOD("set_cross_section_on", "center", "roll", "pitch", "opacity", "showintersection"), &PDF3DView::set_cross_section_on);
	ClassDB::bind_method(D_METHOD("set_cross_section_off"), &PDF3DView::set_cross_section_off);
	ClassDB::bind_method(D_METHOD("add_3d_c3d_measure", "measure"), &PDF3DView::add_3d_c3d_measure);

	ClassDB::bind_method(D_METHOD("get_status"), &PDF3DView::get_status);
}
