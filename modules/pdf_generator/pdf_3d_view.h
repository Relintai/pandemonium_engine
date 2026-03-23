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

class PDF3DView : public Reference {
	GDCLASS(PDF3DView, Reference);

public:
#if 0
	HPDF_EXPORT(HPDF_Dict)
	HPDF_3DView_CreateNode(HPDF_Dict view, const char *name);

	HPDF_3DView_AddNode(HPDF_Dict view, HPDF_Dict node);
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DView_SetLighting(HPDF_Dict view, const char *scheme);
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DView_SetBackgroundColor(HPDF_Dict view, HPDF_REAL r, HPDF_REAL g, HPDF_REAL b);
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DView_SetPerspectiveProjection(HPDF_Dict view, HPDF_REAL fov);
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DView_SetOrthogonalProjection(HPDF_Dict view, HPDF_REAL mag);
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DView_SetCamera(HPDF_Dict view, HPDF_REAL coox, HPDF_REAL cooy, HPDF_REAL cooz, HPDF_REAL c2cx, HPDF_REAL c2cy, HPDF_REAL c2cz, HPDF_REAL roo, HPDF_REAL roll);
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DView_SetCameraByMatrix(HPDF_Dict view, HPDF_3DMatrix Mat3D, HPDF_REAL co);
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DView_SetCrossSectionOn(HPDF_Dict view, HPDF_Point3D center, HPDF_REAL Roll, HPDF_REAL Pitch, HPDF_REAL opacity, HPDF_BOOL showintersection);
	HPDF_EXPORT(HPDF_STATUS)
	HPDF_3DView_SetCrossSectionOff(HPDF_Dict view);
#endif

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
