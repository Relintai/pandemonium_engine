#ifndef PDF_3D_MEASURE_H
#define PDF_3D_MEASURE_H

/*************************************************************************/
/*  pdf_3d_measure.h                                                     */
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

class PDF3DMeasure : public Reference {
	GDCLASS(PDF3DMeasure, Reference);

public:
#if 0
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

#endif

	uint32_t get_status();

	PDF3DMeasure();
	~PDF3DMeasure();

	void *_get_hpdf_3d_measure() const;
	void _set_hpdf_3d_measure(void *p_3d_measure);

protected:
	static void _bind_methods();

	void *_3d_measure;

	uint32_t _status;
};

#endif
