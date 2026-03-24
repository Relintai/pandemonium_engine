#ifndef PDF_EXT_G_STATE_H
#define PDF_EXT_G_STATE_H

/*************************************************************************/
/*  pdf_ext_g_state.h                                                    */
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

class PDFExtGState : public Reference {
	GDCLASS(PDFExtGState, Reference);

public:
	enum BlendMode {
		BM_NORMAL,
		BM_MULTIPLY,
		BM_SCREEN,
		BM_OVERLAY,
		BM_DARKEN,
		BM_LIGHTEN,
		BM_COLOR_DODGE,
		BM_COLOR_BUM,
		BM_HARD_LIGHT,
		BM_SOFT_LIGHT,
		BM_DIFFERENCE,
		BM_EXCLUSHON,
		BM_EOF
	};

	uint32_t set_alpha_stroke(const float p_value);
	uint32_t set_alpha_fill(const float p_value);
	uint32_t set_blend_mode(const BlendMode p_mode);

	uint32_t get_status();

	PDFExtGState();
	~PDFExtGState();

	void *_get_hpdf_ext_g_state() const;
	void _set_hpdf_ext_g_state(void *p_ext_g_state);

protected:
	static void _bind_methods();

	void *_ext_g_state;

	uint32_t _status;
};

VARIANT_ENUM_CAST(PDFExtGState::BlendMode);

#endif
