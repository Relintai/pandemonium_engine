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

#include "core/reference.h"
#include "scene/gui/margin_container.h"

#include "core/vector.h"

class PaintCanvasLayer;
class TextureRect;
class Control;
class PaintCanvasOutline;
class PaintVisualGrid;
class PaintCanvasLayer;

//class_name GECanvas
class PaintCanvas : public MarginContainer {
	GDCLASS(PaintCanvas, MarginContainer);

public:
	void _enter_tree();
	void _process(float delta);
	void _draw();

	int get_pixel_size() const;
	void set_pixel_size(const int size);

	int get_grid_size() const;
	void set_grid_size(const int size);

	int get_big_grid_size() const;
	void set_big_grid_size(const int size);

	int get_canvas_width() const;
	void set_canvas_width(const int val);

	int get_canvas_height() const;
	void set_canvas_height(const int val);

	void toggle_alpha_locked(const String &layer_name);
	bool is_alpha_locked();

	Rect2 get_content_margin();
	void crop_to_content();

	Ref<PaintCanvasLayer> get_active_layer();
	Ref<PaintCanvasLayer> get_preview_layer();
	void clear_active_layer();
	void clear_preview_layer();
	void clear_layer(const String &layer_name);
	Ref<PaintCanvasLayer> remove_layer(const String &layer_name);
	Ref<PaintCanvasLayer> add_new_layer(const String &layer_name);
	Ref<PaintCanvasLayer> duplicate_layer(const String &layer_name, const String &new_layer_name);
	void toggle_layer_visibility(const String &layer_name);
	Ref<PaintCanvasLayer> find_layer_by_name(const String &layer_name);
	void toggle_lock_layer(const String &layer_name);
	bool is_active_layer_locked();
	void move_layer_forward(const String &layer_name);
	void move_layer_back(const String &layer_name);
	void select_layer(const String &layer_name);

	void _on_mouse_entered();
	void _on_mouse_exited();

	bool is_inside_canvas(const int x, const int y);
	void set_pixel_arr(const PoolVector2iArray &pixels, const Color &color);
	void set_pixel_v(const Vector2i &pos, const Color &color);
	void set_pixel(const int x, const int y, const Color &color);
	void _set_pixel_v(Ref<PaintCanvasLayer> layer, const Vector2i &v, const Color &color);
	void _set_pixel(Ref<PaintCanvasLayer> layer, const int x, const int y, const Color &color);
	Color get_pixel_v(const Vector2i &pos);
	Color get_pixel(const int x, const int y);
	void set_preview_pixel_v(const Vector2i &pos, const Color &color);
	void set_preview_pixel(const int x, const int y, const Color &color);
	Color get_preview_pixel_v(const Vector2i &pos);
	Color get_preview_pixel(const int x, const int y);

	void toggle_grid();
	void show_grid();
	void hide_grid();

	PoolVector2iArray select_color(const int x, const int y);
	PoolVector2iArray select_same_color(const int x, const int y);
	PoolVector2iArray get_neighbouring_pixels(const int pos_x, const int pos_y);

	void resize(int width, int height);

	PaintCanvas();
	~PaintCanvas();

	Vector<Ref<PaintCanvasLayer>> layers;
	Ref<PaintCanvasLayer> active_layer;
	Ref<PaintCanvasLayer> preview_layer;
	Ref<PaintCanvasLayer> tool_layer;

	TextureRect *canvas_background_rect;
	Control *canvas_layers;
	TextureRect *preview_layer_rect;
	TextureRect *tool_preview_layer_rect;
	PaintVisualGrid *grid;
	PaintCanvasOutline *canvas_outline;

	//not used
	PaintVisualGrid *big_grid;

	bool symmetry_x;
	bool symmetry_y;

	bool mouse_in_region;
	bool mouse_on_top;

	Array selected_pixels;

protected:
	void _notification(int p_what);
	static void _bind_methods();

	int _pixel_size;
	int _canvas_width;
	int _canvas_height;
	int _grid_size;
	int _big_grid_size;
	bool _can_draw;
};

#endif
