#ifndef EDITOR_ZOOM_WIDGET_H
#define EDITOR_ZOOM_WIDGET_H

/*************************************************************************/
/*  editor_zoom_widget.h                                                 */
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

#include "scene/gui/box_container.h"

class Button;

class EditorZoomWidget : public HBoxContainer {
	GDCLASS(EditorZoomWidget, HBoxContainer);

	Button *zoom_minus = nullptr;
	Button *zoom_reset = nullptr;
	Button *zoom_plus = nullptr;

	float zoom = 1.0;
	void _update_zoom_label();
	void _button_zoom_minus();
	void _button_zoom_reset();
	void _button_zoom_plus();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	EditorZoomWidget();

	float get_zoom();
	void set_zoom(float p_zoom);
	void set_zoom_by_increments(int p_increment_count, bool p_integer_only = false);
};

#endif // EDITOR_ZOOM_WIDGET_H
