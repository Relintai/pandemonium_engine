#ifndef CONTROL_POINT_H
#define CONTROL_POINT_H

/*************************************************************************/
/*  control_point.h                                                      */
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

#include "scene/main/control.h"

#include "../../../nodes/bases/curve_base.h"

class SlopePoint;

class ControlPoint : public Control {
	GDCLASS(ControlPoint, Control);

public:
	bool get_moving() const;
	void set_moving(const bool val);

	float get_min_x() const;
	void set_min_x(const float val);

	float get_max_x() const;
	void set_max_x(const float val);

	float get_min_y() const;
	void set_min_y(const float val);

	float get_max_y() const;
	void set_max_y(const float val);

	void _draw();

	void initialize(const CurveBase::Point &p);
	void set_constraint(const float x, const float X, const float y, const float Y);
	void update_tangents();

	void _on_ControlPoint_gui_input(const Ref<InputEvent> &event);

	ControlPoint();
	~ControlPoint();

	const Vector2 OFFSET = Vector2(3, 3);

	SlopePoint *left_slope_point;
	SlopePoint *right_slope_point;

protected:
	void _notification(int p_what);

	static void _bind_methods();

	bool moving;
	float min_x;
	float max_x;
	float min_y;
	float max_y;
};

#endif
