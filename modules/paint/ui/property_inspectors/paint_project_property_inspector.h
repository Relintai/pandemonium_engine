#ifndef PAINT_PROJECT_PROPERTY_INSPECTOR_H
#define PAINT_PROJECT_PROPERTY_INSPECTOR_H

/*
Copyright (c) 2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "core/object/object_id.h"
#include "core/object/reference.h"
#include "paint_custom_property_inspector.h"

class GridContainer;
class PaintNode;
class PaintProject;
class HFlowContainer;
class ColorSelectorButton;
class Button;

class PaintProjectPropertyInspector : public PaintCustomPropertyInspector {
	GDCLASS(PaintProjectPropertyInspector, PaintCustomPropertyInspector);

public:
	void add_grid_button(const Color &color, const int color_index);
	void create_grid_buttons();
	void clear_grid_buttons();

	void _set_paint_node(Node *paint_node);

	PaintProjectPropertyInspector();
	~PaintProjectPropertyInspector();

protected:
	void _on_grid_color_button_changed(const Color &color, Node *button);
	void _on_grid_color_button_pressed(Node *button);
	void _on_main_color_changed(const Color &color);
	void _on_main_color_selected();
	void _on_add_color_button_pressed();
	void _on_project_color_changed(const Color &color);
	void _on_project_color_preset_changed();

	void _notification(int p_what);

	static void _bind_methods();

	HFlowContainer *_color_grid;
	ColorSelectorButton *_main_color_button;
	Button *_add_color_button;

	ObjectID _current_paint_node;
	ObjectID _current_paint_project;

	bool _ignore_preset_changed_event;
	bool _ignore_color_event;
};

#endif
