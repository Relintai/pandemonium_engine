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

#include "bush_prefabs.h"
#include "core/containers/hash_map.h"
#include "core/os/keyboard.h"
#include "core/object/reference.h"

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
class PaintLayerButton;

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

	Color get_selected_color();
	void set_selected_color(const Color &color);

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
	void _on_ChangeGridSizeDialog_confirmed();
	void _on_ChangeCanvasSizeDialog_confirmed();

	void highlight_layer(const String &layer_name);
	void toggle_layer_visibility(Node *button, const String &layer_name);
	void select_layer(const String &layer_name);
	void lock_layer(Node *button, const String &layer_name);

	Ref<PaintCanvasLayer> add_new_layer();
	void remove_active_layer();
	void duplicate_active_layer();
	void delete_all_layers();

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

	bool allow_drawing;

	bool mouse_in_region;
	bool mouse_on_top;

	Vector2 _middle_mouse_pressed_pos;
	Vector2 _middle_mouse_pressed_start_pos;
	bool _middle_mouse_pressed;
	Vector2 _left_mouse_pressed_start_pos;
	PaintWindow::Tools _previous_tool;
	PaintWindow::Tools brush_mode;

	HashMap<String, PaintLayerButton *> _layer_button_ref;
	int _total_added_layers;

	BrushPrefabs::Type selected_brush_prefab;
	Vector2 _last_drawn_pixel;
	Vector2 _last_preview_draw_cell_pos;

	PoolVector2iArray _selection_cells;
	PoolColorArray _selection_colors;

	Vector2i _cut_pos;
	Vector2i _cut_size;

	Vector<Ref<PaintAction>> _actions_history;
	Vector<Ref<PaintAction>> _redo_history;
	Ref<PaintAction> _current_action;

	Vector2 _last_mouse_pos_canvas_area;

	bool _picked_color;

	Vector2 mouse_position;
	Vector2 canvas_position;
	Vector2 canvas_mouse_position;
	Vector2 cell_mouse_position;
	Color cell_color;

	Vector2 last_mouse_position;
	Vector2 last_canvas_position;
	Vector2 last_canvas_mouse_position;
	Vector2 last_cell_mouse_position;
	Color last_cell_color;

	Color current_layer_highlight;
	Color other_layer_highlight;
	Color locked_layer_highlight;

	int big_grid_pixels = 4;

protected:
	void _notification(int p_what);
	static void _bind_methods();

	Color _selected_color;
};

#endif
