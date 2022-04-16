#ifndef PAINT_WINDOW_H
#define PAINT_WINDOW_H

/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
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

#include "scene/gui/control.h"

#include "core/os/keyboard.h"
#include "core/reference.h"

class PaintAction;
class PaintCanvasLayer;
class PaintCanvas;
class InputEvent;
class PaintCanvasDialog;
class PaintChangeGridSizeDialog;
class PaintLoadFileDialog;
class PaintSaveFileDialog;
class PaintSettings;
class RichTextLabel;
class PaintNavbar;
class PaintColorGrid;
class CheckButton;
class Label;
class HSlider;
class Control;
class Button;
class ColorPickerButton;
class VBoxContainer;

class PaintWindow : public Control {
	GDCLASS(PaintWindow, Control);

public:
	enum Tools {
		PAINT = 0,
		BRUSH,
		BUCKET,
		RAINBOW,
		LINE,
		RECT,
		DARKEN,
		BRIGHTEN,
		COLORPICKER,
		CUT,
		PASTECUT,
	};

	enum KeyboardShortcuts {
		K_UNDO = KEY_Z,
		K_REDO = KEY_Y,
		K_PENCIL = KEY_Q,
		K_BRUSH = KEY_W,
		K_BUCKET = KEY_F,
		K_RAINBOW = KEY_R,
		K_LINE = KEY_L,
		K_DARK = KEY_D,
		K_BRIGHT = KEY_B,
		K_CUT = KEY_C,
		K_PICK = KEY_P,
	};

	enum {
		max_zoom_out = 1,
		max_zoom_in = 50,
	};

	void _input(const Ref<InputEvent> &event);
	void _process(float delta);

	void _handle_shortcuts(const int scancode);
	void _draw_tool_brush();

	void _handle_scroll();

	void _handle_zoom(const Ref<InputEvent> &event);

	void _handle_cut();
	void brush_process();
	void update_text_info();

	void _on_Save_pressed();

	void do_action(const Array &data);
	void commit_action();
	void redo_action();
	void undo_action();

	Ref<PaintAction> get_action();

	void set_selected_color(const Color &color);
	void set_brush(const Tools new_mode);

	void change_color(const Color &new_color);

	void _on_ColorPicker_color_changed(const Color &color);
	void _on_PaintTool_pressed();
	void _on_BucketTool_pressed();
	void _on_RainbowTool_pressed();
	void _on_BrushTool_pressed();
	void _on_LineTool_pressed();
	void _on_RectTool_pressed();
	void _on_DarkenTool_pressed();
	void _on_BrightenTool_pressed();
	void _on_ColorPickerTool_pressed();
	void _on_CutTool_pressed();
	void _on_Editor_visibility_changed();

	void highlight_layer(const String &layer_name);
	void toggle_layer_visibility(Node *button, const String &layer_name);
	void select_layer(const String &layer_name);
	void lock_layer(Node *button, const String &layer_name);

	Ref<PaintCanvasLayer> add_new_layer();
	void remove_active_layer();
	void duplicate_active_layer();

	void move_up(Node *layer_btn);
	void move_down(Node *layer_btn);

	void _connect_layer_buttons();

	void _on_Button_pressed();
	void _on_PaintCanvasContainer_mouse_entered();
	void _on_PaintCanvasContainer_mouse_exited();
	void _on_ColorPicker_popup_closed();

	bool is_position_in_canvas(const Vector2 &pos);

	bool is_mouse_in_canvas();

	bool is_any_menu_open();

	void _on_LockAlpha_pressed();
	void _on_BrushRect_pressed();
	void _on_BrushCircle_pressed();
	void _on_BrushVLine_pressed();
	void _on_BrushHLine_pressed();
	void _on_BrushSize_value_changed(float value);
	void _on_XSymmetry_pressed();
	void _on_YSymmetry_pressed();

	PaintWindow();
	~PaintWindow();

	PaintCanvasDialog *paint_canvas_dialog;
	PaintChangeGridSizeDialog *paint_change_grid_size_dialog;
	PaintLoadFileDialog *paint_load_file_dialog;
	PaintSaveFileDialog *paint_save_file_dialog;
	PaintSettings *paint_settings_dialog;

	RichTextLabel *text_info;

	PaintNavbar *navbar;
	PaintColorGrid *color_grid;

	CheckButton *lock_alpha_button;

	Label *brush_size_label;
	HSlider *brush_size_slider;

	CheckButton *x_symmetry_button;
	CheckButton *y_symmetry_button;

	Control *paint_canvas_container;
	PaintCanvas *paint_canvas;

	Button *paint_tool_button;
	Button *brush_tool_button;
	Button *multi_tool_button;
	Button *bucket_tool_button;
	Button *rainbow_tool_button;
	Button *line_tool_button;
	Button *rect_tool_button;
	Button *darken_tool_button;
	Button *brighten_tool_button;
	Button *color_picker_tool_button;
	Button *cut_tool_button;
	ColorPickerButton *color_picker_button;

	VBoxContainer *layers_box_container;
	Button *add_layer_button;

protected:
	static void _bind_methods();
	/*

		var layer_buttons: Control
		var paint_canvas_container_node
		var paint_canvas: GECanvas
		var canvas_background: TextureRect
		var grids_node
		var colors_grid
		var selected_color = Color(1, 1, 1, 1) setget set_selected_color
		var util = preload("res://addons/Godoxel/Util.gd")
		var textinfo
		var allow_drawing = true

		var mouse_in_region = false
		var mouse_on_top = false

		var _middle_mouse_pressed_pos = null
		var _middle_mouse_pressed_start_pos = null
		var _left_mouse_pressed_start_pos = Vector2()
		var _previous_tool
		var brush_mode

		var _layer_button_ref = {}

		var _total_added_layers = 1

		var selected_brush_prefab = 0
		var _last_drawn_pixel = Vector2.ZERO
		var _last_preview_draw_cell_pos = Vector2.ZERO

		var _selection_cells = []
		var _selection_colors = []

		var _cut_pos = Vector2.ZERO
		var _cut_size = Vector2.ZERO

		var _actions_history = [] # for undo
		var _redo_history = []
		var _current_action

		var _last_mouse_pos_canvas_area = Vector2.ZERO

		var _picked_color = false

		var mouse_position = Vector2()
		var canvas_position = Vector2()
		var canvas_mouse_position = Vector2()
		var cell_mouse_position = Vector2()
		var cell_color = Color()

		var last_mouse_position = Vector2()
		var last_canvas_position = Vector2()
		var last_canvas_mouse_position = Vector2()
		var last_cell_mouse_position = Vector2()
		var last_cell_color = Color()

		const current_layer_highlight = Color(0.354706, 0.497302, 0.769531)
		const other_layer_highlight = Color(0.180392, 0.176471, 0.176471)
		const locked_layer_highlight = Color(0.098039, 0.094118, 0.094118)

		var big_grid_pixels = 4 // 1 grid-box is big_grid_pixels big
	*/
};

#endif
