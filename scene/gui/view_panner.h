#ifndef VIEW_PANNER_H
#define VIEW_PANNER_H

/*************************************************************************/
/*  view_panner.h                                                        */
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

#include "core/object/resource.h"

#include "core/object/func_ref.h"

class InputEvent;
class ShortCut;

class ViewPanner : public Resource {
	GDCLASS(ViewPanner, Resource);

public:
	enum ControlScheme {
		SCROLL_ZOOMS,
		SCROLL_PANS,
	};

	enum PanAxis {
		PAN_AXIS_BOTH,
		PAN_AXIS_HORIZONTAL,
		PAN_AXIS_VERTICAL,
	};

public:
	ControlScheme get_control_scheme() const;
	void set_control_scheme(const ControlScheme p_scheme);

	bool get_enable_rmb() const;
	void set_enable_rmb(const bool p_enable);

	Ref<ShortCut> get_pan_shortcut() const;
	void set_pan_shortcut(const Ref<ShortCut> &p_shortcut);

	bool get_simple_panning_enabled() const;
	void set_simple_panning_enabled(const bool p_enabled);

	int get_scroll_speed() const;
	void set_scroll_speed(const int p_scroll_speed);

	float get_scroll_zoom_factor() const;
	void set_scroll_zoom_factor(const float p_scroll_zoom_factor);

	PanAxis get_pan_axis() const;
	void set_pan_axis(const PanAxis p_pan_axis);

	void setup(const ControlScheme p_scheme, const Ref<ShortCut> &p_shortcut, const bool p_simple_panning);

	bool is_panning() const;
	void set_force_drag(bool p_force);

	bool gui_input(const Ref<InputEvent> &p_ev, Rect2 p_canvas_rect = Rect2());
	void release_pan_key();

	ViewPanner();

protected:
	static void _bind_methods();

private:
	int scroll_speed;
	float scroll_zoom_factor;
	PanAxis pan_axis;

	bool is_dragging;
	bool pan_key_pressed;
	bool force_drag;

	bool enable_rmb;
	bool simple_panning_enabled;

	Ref<ShortCut> pan_view_shortcut;

	ControlScheme control_scheme;
};

VARIANT_ENUM_CAST(ViewPanner::ControlScheme);
VARIANT_ENUM_CAST(ViewPanner::PanAxis);

#endif // VIEW_PANNER_H
