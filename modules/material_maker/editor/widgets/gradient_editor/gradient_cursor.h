#ifndef GRADIENT_CURSOR_H
#define GRADIENT_CURSOR_H

/*************************************************************************/
/*  gradient_cursor.h                                                    */
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

#include "core/input/input_event.h"
#include "core/math/color.h"
#include "core/variant/variant.h"

#include "scene/main/control.h"

class Label;

class GradientCursor : public Control {
	GDCLASS(GradientCursor, Control);

public:
	Color get_cursor_color();
	void set_cursor_color(const Color &val);

	void set_cursor_color_notify(const Color &val);

	bool get_sliding() const;
	void set_sliding(const bool val);

	void set_label(Label *label);

	void _draw();
	void _gui_input(const Ref<InputEvent> &ev);

	float get_cursor_position();

	bool operator<(const GradientCursor &b) const;

	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

	GradientCursor();
	~GradientCursor();

	static const int WIDTH;

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Color color;
	bool sliding;
	Label *label;
};

#endif
