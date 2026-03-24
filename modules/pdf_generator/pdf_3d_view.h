#ifndef PDF_3D_VIEW_H
#define PDF_3D_VIEW_H

/*************************************************************************/
/*  pdf_3d_view.h                                                        */
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

class PDF3DViewNode;
class PDF3DMeasure;

class PDF3DView : public Reference {
	GDCLASS(PDF3DView, Reference);

public:
	Ref<PDF3DViewNode> create_node(const String &p_name);

	uint32_t add_node(const Ref<PDF3DViewNode> &p_node);

	uint32_t set_lighting(const String &p_scheme);
	uint32_t set_background_color(const Color &p_color);

	uint32_t set_perspective_projection(const float p_fov);
	uint32_t set_orthogonal_projection(const float p_mag);

	uint32_t set_camera(const Vector3 &p_coo, const Vector3 &p_c2c, const float p_roo, const float p_roll);
	uint32_t set_camera_by_matrix(const Transform &p_matrix, const float p_co);

	uint32_t set_cross_section_on(const Vector3 &p_center, const float p_roll, const float p_pitch, const float p_opacity, bool p_showintersection);
	uint32_t set_cross_section_off();

	uint32_t add_3d_c3d_measure(const Ref<PDF3DMeasure> &p_measure);

	uint32_t get_status();

	PDF3DView();
	~PDF3DView();

	void *_get_hpdf_3d_view() const;
	void _set_hpdf_3d_view(void *p_3d_view);

protected:
	static void _bind_methods();

	void *_3d_view;

	uint32_t _status;
};

#endif
