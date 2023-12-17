#ifndef PAINT_PROJECT_H
#define PAINT_PROJECT_H

/*************************************************************************/
/*  paint_project.h                                                      */
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

#include "core/containers/pool_vector.h"

#include "paint_node.h"

class PaintProject : public PaintNode {
	GDCLASS(PaintProject, PaintNode);

public:
	String get_save_file_name();
	void set_save_file_name(const String &fn);

	Color get_current_color();
	void set_current_color(const Color &color);

	void add_preset_color(const Color &color);
	void remove_preset_color(const int index);
	Color get_preset_color(const int index);
	void set_preset_color(const int index, const Color &color);
	int get_preset_color_count();

	PoolColorArray get_color_presets();
	void set_color_presets(const PoolColorArray &colors);

	void set_colors_as_default();

	void save_image_to_file();

	void add_paint_canvas_backgorund();
	void add_paint_visual_grid();

	bool _render_should_evaluate_children();

	PaintProject();
	~PaintProject();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	String _save_file_name;
	Color _current_color;
	PoolColorArray _color_presets;
};

#endif
