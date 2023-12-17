#ifndef PAINT_TOOLS_PROPERTY_INSPECTOR_H
#define PAINT_TOOLS_PROPERTY_INSPECTOR_H

/*************************************************************************/
/*  paint_tools_property_inspector.h                                     */
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

#include "core/object/object_id.h"
#include "core/object/reference.h"
#include "paint_custom_property_inspector.h"

class HFlowContainer;
class PaintNode;
class PaintCanvas;
class PaintProject;
class ButtonGroup;
class HSlider;
class Label;
class FileDialog;
class ConfirmationDialog;
class SpinBox;
class OptionButton;

class PaintToolsPropertyInspector : public PaintCustomPropertyInspector {
	GDCLASS(PaintToolsPropertyInspector, PaintCustomPropertyInspector);

public:
	void add_tool_button(int id, const String &hint, const String &icon, const String &theme_type);
	void add_action_button(const String &callback, const String &hint, const String &icon, const String &theme_type);
	void add_brush_prefab(int id, const Ref<Texture> &normal_texture, const Ref<Texture> &hover_texture);

	void _set_paint_node(Node *paint_node);

	PaintToolsPropertyInspector();
	~PaintToolsPropertyInspector();

protected:
	void _on_button_toggled(bool on, int id);
	void _on_tool_changed();
	void _on_brush_prefab_button_pressed(const int id);
	void _on_brush_size_slider_value_changed(const float value);
	void _on_brush_size_changed();
	void _on_undo_pressed();
	void _on_redo_pressed();
	void _on_import_pressed();
	void _on_export_pressed();
	void _on_import_dialog_file_selected(const String &f);
	void _on_export_dialog_file_selected(const String &f);
	void _on_pixel_scale_canvas_pressed();
	void _on_pixel_scale_canvas_confirmed();

	static void _bind_methods();

	FileDialog *_import_file_dialog;
	FileDialog *_export_file_dialog;
	ConfirmationDialog *_pixel_resize_canvas;
	SpinBox *_pixel_resize_x_spinbox;
	SpinBox *_pixel_resize_y_spinbox;
	OptionButton *_pixel_resize_interpolation_option_button;

	HFlowContainer *_grid;
	HFlowContainer *_brush_prefabs;
	HSlider *_brush_size_slider;
	Label *_brush_size_label;

	ObjectID _paint_canvas;

	Ref<ButtonGroup> _group;

	bool _ignore_signal;
};

#endif
