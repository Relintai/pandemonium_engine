#ifndef FLOAT_EDIT_H
#define FLOAT_EDIT_H

/*************************************************************************/
/*  float_edit.h                                                         */
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
#include "scene/gui/line_edit.h"

class ColorRect;

class FloatEdit : public LineEdit {
	GDCLASS(FloatEdit, LineEdit);

public:
	float get_value() const;
	void set_value_float(const float val);
	void set_value(const Variant &val);

	float get_min_value() const;
	void set_min_value(const float val);

	float get_max_value() const;
	void set_max_value(const float val);

	float get_step() const;
	void set_step(const float val);

	bool get_float_only() const;
	void set_float_only(const bool val);

	bool get_sliding() const;
	void set_sliding(const bool val);

	float get_start_position() const;
	void set_start_position(const float val);

	float get_last_position() const;
	void set_last_position(const float val);

	float get_start_value() const;
	void set_start_value(const float val);

	bool get_from_lower_bound() const;
	void set_from_lower_bound(const bool val);

	bool get_from_upper_bound() const;
	void set_from_upper_bound(const bool val);

	void do_update(const bool update_text = true);

	int get_modifiers(const Ref<InputEventMouseButton> &event);

	void _on_LineEdit_gui_input(const Ref<InputEvent> &event);
	void _on_LineEdit_text_changed(const String &new_text);
	void _on_LineEdit_text_entered(const String &new_text, const Variant &release = true);
	void _on_FloatEdit_focus_entered();
	void _on_LineEdit_focus_exited();

	FloatEdit();
	~FloatEdit();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	float value;
	float min_value;
	float max_value;
	float step;
	bool float_only;
	bool sliding;
	float start_position;
	float last_position;
	float start_value;
	int modifiers;
	bool from_lower_bound;
	bool from_upper_bound;
	ColorRect *slider;
	ColorRect *cursor;
};

#endif
