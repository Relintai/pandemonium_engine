#ifndef PAINT_CANVAS_H
#define PAINT_CANVAS_H

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

class PaintCanvasLayer;

//class_name GECanvas
class PaintCanvas : public Control {
	GDCLASS(PaintCanvas, Control);

public:
	void _enter_tree();
	void _process(float delta);
	void _draw();

	void resize(int width, int height);

	void set_pixel_size(int size);
	void set_grid_size(int size);
	void set_big_grid_size(int size);
	void set_canvas_width(int val);
	void set_canvas_height(int val);

	void toggle_alpha_locked(String layer_name);
	bool is_alpha_locked();

	Rect2 get_content_margin();
	void crop_to_content();

	Node *get_active_layer();
	Node *get_preview_layer();
	void clear_active_layer();
	void clear_preview_layer();
	void clear_layer(String layer_name);
	Node *remove_layer(String layer_name);
	Node *add_new_layer(String layer_name);
	Node *duplicate_layer(String layer_name, String new_layer_name);
	void toggle_layer_visibility(String layer_name);
	Node *find_layer_by_name(String layer_name);
	void toggle_lock_layer(String layer_name);
	bool is_active_layer_locked();
	void move_layer_forward(String layer_name);
	void move_layer_back(String layer_name);
	void select_layer(String layer_name);

	void _on_mouse_entered();
	void _on_mouse_exited();

	bool is_inside_canvas(int x, int y);
	void set_pixel_arr(Array pixels, Color color);
	void set_pixel_v(Vector2 pos, Color color);
	void set_pixel(int x, int y, Color color);
	void _set_pixel_v(PaintCanvasLayer *layer, Vector2 v, Color color);
	void _set_pixel(PaintCanvasLayer *layer, int x, int y, Color color);
	Color get_pixel_v(Vector2 pos);
	Color get_pixel(int x, int y);
	void set_preview_pixel_v(Vector2 pos, Color color);
	void set_preview_pixel(int x, int y, Color color);
	Color get_preview_pixel_v(Vector2 pos);
	Color get_preview_pixel(int x, int y);

	void toggle_grid();
	void show_grid();
	void hide_grid();

	Array select_color(int x, int y);
	Array select_same_color(int x, int y);
	Array get_neighbouring_pixels(int pos_x, int pos_y);

	PaintCanvas();
	~PaintCanvas();

protected:
	static void _bind_methods();

	/*
	export var pixel_size: int = 16 setget set_pixel_size
	export(int, 1, 2500) var canvas_width = 48 setget set_canvas_width # == pixels
	export(int, 1, 2500) var canvas_height = 28 setget set_canvas_height # == pixels
	export var grid_size = 16 setget set_grid_size
	export var big_grid_size = 10 setget set_big_grid_size
	export var can_draw = true

	var mouse_in_region
	var mouse_on_top

	var layers : Array = [] # Key: layer_name, val: GELayer
	var active_layer: GELayer
	var preview_layer: GELayer
	var tool_layer: GELayer
	var canvas_layers: Control

	var canvas
	var grid
	var big_grid
	var selected_pixels = []

	var symmetry_x = false
	var symmetry_y = false
	*/
};

#endif
