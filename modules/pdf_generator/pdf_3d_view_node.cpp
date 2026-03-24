/*************************************************************************/
/*  pdf_3d_view_node.cpp                                                      */
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

#include "pdf_3d_view_node.h"

#include "hpdf.h"
#include "hpdf_u3d.h"

uint32_t PDF3DViewNode::set_opacity(const float p_opacity) {
	_status = HPDF_3DViewNode_SetOpacity((HPDF_Dict)_3d_view_node, p_opacity);

	return _status;
}
uint32_t PDF3DViewNode::set_visibility(const bool p_visible) {
	_status = HPDF_3DViewNode_SetVisibility((HPDF_Dict)_3d_view_node, p_visible);

	return _status;
}
uint32_t PDF3DViewNode::set_matrix(const Transform &p_matrix) {
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

	_status = HPDF_3DViewNode_SetMatrix((HPDF_Dict)_3d_view_node, m);

	return _status;
}

uint32_t PDF3DViewNode::get_status() {
	return _status;
}

PDF3DViewNode::PDF3DViewNode() {
	_status = 0;

	_3d_view_node = NULL;
}

PDF3DViewNode::~PDF3DViewNode() {
}

void *PDF3DViewNode::_get_hpdf_3d_view_node() const {
	return _3d_view_node;
}

void PDF3DViewNode::_set_hpdf_3d_view_node(void *p_3d_view_node) {
	_3d_view_node = p_3d_view_node;
}

void PDF3DViewNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_opacity", "opacity"), &PDF3DViewNode::set_opacity);
	ClassDB::bind_method(D_METHOD("set_visibility", "visible"), &PDF3DViewNode::set_visibility);
	ClassDB::bind_method(D_METHOD("set_matrix", "matrix"), &PDF3DViewNode::set_matrix);

	ClassDB::bind_method(D_METHOD("get_status"), &PDF3DViewNode::get_status);
}
