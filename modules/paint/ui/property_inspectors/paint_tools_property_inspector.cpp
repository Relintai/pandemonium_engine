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

#include "paint_tools_property_inspector.h"

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/flow_container.h"
#include "scene/gui/label.h"
#include "scene/gui/slider.h"
#include "scene/gui/texture_button.h"

#include "../../bush_prefabs.h"
#include "../../paint_icons/icons.h"
#include "scene/resources/style_box.h"
#include "scene/resources/texture.h"

#include "../../nodes/paint_canvas.h"
#include "../../nodes/paint_node.h"
#include "../../nodes/paint_project.h"

#ifdef TOOLS_ENABLED
#include "core/config/engine.h"
#include "editor/editor_node.h"
#endif

void PaintToolsPropertyInspector::add_tool_button(int id, const String &hint, const String &icon, const String &theme_type) {
	Button *button = memnew(Button);

	Ref<Texture> icon_tex;

#ifdef TOOLS_ENABLED
	if (EditorNode::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		icon_tex = EditorNode::get_singleton()->get_gui_base()->get_theme_icon(icon, theme_type);
	} else
#endif
	{
		icon_tex = get_theme_icon(icon, theme_type);
	}

	button->set_icon(icon_tex);
	button->set_tooltip(hint);
	button->set_toggle_mode(true);
	button->set_button_group(_group);
	button->set_meta("button_id", id);
	button->connect("toggled", this, "_on_button_toggled", varray(id));

	_grid->add_child(button);
}

void PaintToolsPropertyInspector::add_action_button(const String &callback, const String &hint, const String &icon, const String &theme_type) {
	Button *button = memnew(Button);

	Ref<Texture> icon_tex;

#ifdef TOOLS_ENABLED
	if (EditorNode::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		icon_tex = EditorNode::get_singleton()->get_gui_base()->get_theme_icon(icon, theme_type);
	} else
#endif
	{
		icon_tex = get_theme_icon(icon, theme_type);
	}

	button->set_icon(icon_tex);
	button->set_tooltip(hint);
	button->connect("pressed", this, callback);

	_grid->add_child(button);
}

void PaintToolsPropertyInspector::add_brush_prefab(int id, const Ref<Texture> &normal_texture, const Ref<Texture> &hover_texture) {
	TextureButton *brush_button = memnew(TextureButton);
	brush_button->set_normal_texture(normal_texture);
	brush_button->set_hover_texture(hover_texture);
	brush_button->set_custom_minimum_size(Size2(25, 25));
	brush_button->connect("pressed", this, "_on_brush_prefab_button_pressed", varray(id));
	_brush_prefabs->add_child(brush_button);
}

void PaintToolsPropertyInspector::_on_paint_node_selected(Node *p_paint_node) {
	PaintCanvas *paint_canvas = Object::cast_to<PaintCanvas>(p_paint_node);

	_paint_canvas = 0;

	ERR_FAIL_COND(!paint_canvas);

	_paint_canvas = paint_canvas->get_instance_id();

	_on_tool_changed();
	_on_brush_size_changed();

	paint_canvas->connect("current_tool_changed", this, "_on_tool_changed");
	paint_canvas->connect("brush_size_changed", this, "_on_brush_size_changed");
}

PaintToolsPropertyInspector::PaintToolsPropertyInspector() {
	_ignore_signal = false;
	_paint_canvas = 0;
	_group.instance();

	VBoxContainer *box_container = memnew(VBoxContainer);
	add_child(box_container);

	_grid = memnew(HFlowContainer);
	box_container->add_child(_grid);
	//_grid->set_custom_minimum_size(Size2(0, 145));
	_grid->set_h_size_flags(SIZE_EXPAND_FILL);
	_grid->set_v_size_flags(SIZE_EXPAND_FILL);

	add_tool_button(PaintCanvas::TOOL_PENCIL, "Pencil", "Edit", "EditorIcons");
	add_tool_button(PaintCanvas::TOOL_BRUSH, "Brush", "CanvasItem", "EditorIcons");
	add_tool_button(PaintCanvas::TOOL_BUCKET, "Bucket", "Bucket", "EditorIcons");
	add_tool_button(PaintCanvas::TOOL_RAINBOW, "Rainbow", "StyleBoxLine", "EditorIcons");
	add_tool_button(PaintCanvas::TOOL_LINE, "Line", "CurveLinear", "EditorIcons");
	add_tool_button(PaintCanvas::TOOL_RECT, "Rect", "Panels1", "EditorIcons");
	add_tool_button(PaintCanvas::TOOL_DARKEN, "Darken", "ArrowDown", "EditorIcons");
	add_tool_button(PaintCanvas::TOOL_BRIGHTEN, "Brighten", "ArrowUp", "EditorIcons");
	add_tool_button(PaintCanvas::TOOL_COLORPICKER, "Colorpicker", "ColorPick", "EditorIcons");
	add_tool_button(PaintCanvas::TOOL_CUT, "Cut", "ActionCut", "EditorIcons");
	add_tool_button(PaintCanvas::TOOL_PASTECUT, "Pastecut", "ActionCopy", "EditorIcons");
	add_action_button("_on_undo_pressed", "Undo", "ArrowLeft", "EditorIcons");
	add_action_button("_on_redo_pressed", "Redo", "ArrowRight", "EditorIcons");

	_brush_prefabs = memnew(HFlowContainer);
	box_container->add_child(_brush_prefabs);

	add_brush_prefab(BrushPrefabs::RECT, PaintIcons::make_icon_brush_rect_png(), PaintIcons::make_icon_brush_rect_hovered_png());
	add_brush_prefab(BrushPrefabs::CIRCLE, PaintIcons::make_icon_brush_circle_png(), PaintIcons::make_icon_brush_circle_hovered_png());
	add_brush_prefab(BrushPrefabs::V_LINE, PaintIcons::make_icon_brush_v_line_png(), PaintIcons::make_icon_brush_v_line_hovered_png());
	add_brush_prefab(BrushPrefabs::H_LINE, PaintIcons::make_icon_brush_h_line_png(), PaintIcons::make_icon_brush_h_line_hovered_png());

	HBoxContainer *brush_size_container = memnew(HBoxContainer);
	box_container->add_child(brush_size_container);
	brush_size_container->set_h_size_flags(SIZE_EXPAND_FILL);

	_brush_size_label = memnew(Label);
	brush_size_container->add_child(_brush_size_label);
	_brush_size_label->set_text("1");

	_brush_size_slider = memnew(HSlider);
	brush_size_container->add_child(_brush_size_slider);
	_brush_size_slider->set_min(1);
	_brush_size_slider->set_max(100);
	_brush_size_slider->set_use_rounded_values(true);
	_brush_size_slider->set_value(1);
	_brush_size_slider->set_allow_greater(true);
	_brush_size_slider->connect("value_changed", this, "_on_brush_size_slider_value_changed");
	_brush_size_slider->set_h_size_flags(SIZE_EXPAND_FILL);
}

PaintToolsPropertyInspector::~PaintToolsPropertyInspector() {
}

void PaintToolsPropertyInspector::_on_button_toggled(bool on, int id) {
	if (!on) {
		return;
	}

	PaintCanvas *paint_canvas = Object::cast_to<PaintCanvas>(ObjectDB::get_instance(_paint_canvas));

	ERR_FAIL_COND(!paint_canvas);

	paint_canvas->set_current_tool(id);
}

void PaintToolsPropertyInspector::_on_tool_changed() {
	PaintCanvas *paint_canvas = Object::cast_to<PaintCanvas>(ObjectDB::get_instance(_paint_canvas));

	ERR_FAIL_COND(!paint_canvas);

	int tool_id = paint_canvas->get_current_tool();

	for (int i = 0; i < _grid->get_child_count(); ++i) {
		Button *b = Object::cast_to<Button>(_grid->get_child(i));

		if (!b) {
			continue;
		}

		int bid = b->get_meta("button_id");

		if (bid == tool_id) {
			b->set_pressed(true);
			return;
		}
	}
}

void PaintToolsPropertyInspector::_on_brush_prefab_button_pressed(const int id) {
	PaintCanvas *paint_canvas = Object::cast_to<PaintCanvas>(ObjectDB::get_instance(_paint_canvas));

	ERR_FAIL_COND(!paint_canvas);

	paint_canvas->set_brush_prefab(id);
}

void PaintToolsPropertyInspector::_on_brush_size_slider_value_changed(const float value) {
	PaintCanvas *paint_canvas = Object::cast_to<PaintCanvas>(ObjectDB::get_instance(_paint_canvas));

	ERR_FAIL_COND(!paint_canvas);

	_ignore_signal = true;
	paint_canvas->set_brush_size(static_cast<int>(value));
	_ignore_signal = false;

	_brush_size_label->set_text(itos(paint_canvas->get_brush_size()));
}

void PaintToolsPropertyInspector::_on_brush_size_changed() {
	if (_ignore_signal) {
		return;
	}

	PaintCanvas *paint_canvas = Object::cast_to<PaintCanvas>(ObjectDB::get_instance(_paint_canvas));

	ERR_FAIL_COND(!paint_canvas);

	_brush_size_slider->set_value(paint_canvas->get_brush_size());
	_brush_size_label->set_text(itos(paint_canvas->get_brush_size()));
}

void PaintToolsPropertyInspector::_on_undo_pressed() {
	PaintCanvas *paint_canvas = Object::cast_to<PaintCanvas>(ObjectDB::get_instance(_paint_canvas));

	ERR_FAIL_COND(!paint_canvas);

	paint_canvas->undo_action();
}
void PaintToolsPropertyInspector::_on_redo_pressed() {
	PaintCanvas *paint_canvas = Object::cast_to<PaintCanvas>(ObjectDB::get_instance(_paint_canvas));

	ERR_FAIL_COND(!paint_canvas);

	paint_canvas->redo_action();
}

void PaintToolsPropertyInspector::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_on_button_toggled"), &PaintToolsPropertyInspector::_on_button_toggled);
	ClassDB::bind_method(D_METHOD("_on_tool_changed"), &PaintToolsPropertyInspector::_on_tool_changed);
	ClassDB::bind_method(D_METHOD("_on_brush_prefab_button_pressed"), &PaintToolsPropertyInspector::_on_brush_prefab_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_brush_size_slider_value_changed"), &PaintToolsPropertyInspector::_on_brush_size_slider_value_changed);
	ClassDB::bind_method(D_METHOD("_on_brush_size_changed"), &PaintToolsPropertyInspector::_on_brush_size_changed);

	ClassDB::bind_method(D_METHOD("_on_undo_pressed"), &PaintToolsPropertyInspector::_on_undo_pressed);
	ClassDB::bind_method(D_METHOD("_on_redo_pressed"), &PaintToolsPropertyInspector::_on_redo_pressed);
}
