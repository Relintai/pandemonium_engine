#ifndef PDF_SHADING_H
#define PDF_SHADING_H

/*************************************************************************/
/*  pdf_shading.h                                                        */
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

class PDFShading : public Reference {
	GDCLASS(PDFShading, Reference);

public:
	enum FreeFormTriangleMeshEdgeFlag {
		FREE_FORM_TRI_MESH_EDGEFLAG_NO_CONNECTION = 0,
		FREE_FORM_TRI_MESH_EDGEFLAG_BC,
		FREE_FORM_TRI_MESH_EDGEFLAG_AC
	};

	uint32_t add_vertex_rgb(const FreeFormTriangleMeshEdgeFlag p_edge_flag, const Vector2 &p_position, const Color &p_color);

	uint32_t get_status();

	PDFShading();
	~PDFShading();

	void *_get_hpdf_shading() const;
	void _set_hpdf_shading(void *p_shading);

protected:
	static void _bind_methods();

	void *_shading;

	uint32_t _status;
};

VARIANT_ENUM_CAST(PDFShading::FreeFormTriangleMeshEdgeFlag);

#endif
