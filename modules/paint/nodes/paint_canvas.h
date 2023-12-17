#ifndef PAINT_CANVAS_H
#define PAINT_CANVAS_H

/*************************************************************************/
/*  paint_canvas.h                                                       */
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

#include "core/object/reference.h"

#include "paint_node.h"

class Image;
class ImageTexture;
class PaintAction;

class PaintCanvas : public PaintNode {
	GDCLASS(PaintCanvas, PaintNode);

public:
	enum Tools {
		TOOL_PENCIL = 0,
		TOOL_BRUSH,
		TOOL_BUCKET,
		TOOL_RAINBOW,
		TOOL_LINE,
		TOOL_RECT,
		TOOL_DARKEN,
		TOOL_BRIGHTEN,
		TOOL_COLORPICKER,
		TOOL_CUT,
		TOOL_PASTECUT,
	};

	bool get_symmetry_x() const;
	void set_symmetry_x(const bool val);

	bool get_symmetry_y() const;
	void set_symmetry_y(const bool val);

	bool get_alpha_locked() const;
	void set_alpha_locked(const bool val);

	int get_brush_prefab() const;
	void set_brush_prefab(const int val);

	int get_brush_size() const;
	void set_brush_size(const int val);

	int get_current_tool() const;
	void set_current_tool(const int val);

	int get_previous_tool() const;

	bool is_inside_canvas(const int x, const int y);

	void set_pixel_arr(const PoolVector2iArray &pixels, const Color &color);

	void set_pixel_v(const Vector2i &pos, const Color &color);
	void set_pixel(const int x, const int y, const Color &color);

	Color get_pixel_v(const Vector2i &pos);
	Color get_pixel(const int x, const int y);

	void set_preview_pixel_v(const Vector2i &pos, const Color &color);
	void set_preview_pixel(const int x, const int y, const Color &color);

	Color get_preview_pixel_v(const Vector2i &pos);
	Color get_preview_pixel(const int x, const int y);

	bool validate_pixel_v(const Vector2i &pos) const;

	void clear();
	void clear_preview();
	void update_textures();

	void resize_crop(const int width, const int height);
	void resize_interpolate(const int width, const int height, Image::Interpolation p_interpolation = Image::INTERPOLATE_BILINEAR);

	PoolVector2iArray select_color(const int p_x, const int p_y);
	PoolVector2iArray select_same_color(const int p_x, const int p_y);
	PoolVector2iArray get_neighbouring_pixels(const int pos_x, const int pos_y);

	Ref<Image> get_image();
	Ref<Image> get_preview_image();

	Ref<ImageTexture> get_image_texture();
	Ref<ImageTexture> get_preview_image_texture();

	Ref<Image> _get_rendered_image();

	PoolByteArray get_image_data_compressed();
	void set_image_data_compressed(const PoolByteArray &arr);

	void handle_draw(const Vector2 &local_position, const Ref<InputEvent> &event);
	Color get_current_color();
	void update_mouse_position(const Vector2 &local_position, const Ref<InputEvent> &event);
	void handle_left_mouse_button_down(const Vector2 &local_position, const Ref<InputEvent> &event);
	void handle_left_mouse_button_up(const Vector2 &local_position, const Ref<InputEvent> &event);
	void handle_right_mouse_button_down(const Vector2 &local_position, const Ref<InputEvent> &event);

	void draw_brush_preview();
	void do_action(const Array &arr);
	void commit_action();
	void redo_action();
	void undo_action();

	bool has_point(const Vector2 &pos);

	Ref<PaintAction> get_action();
	void _on_tool_changed();
	void tool_process(const Vector2 &local_position, const Ref<InputEvent> &event);
	bool _forward_canvas_gui_input(const Ref<InputEvent> &event);

	Error load_image(const String &path);

	PaintCanvas();
	~PaintCanvas();

protected:
	void _on_size_changed();

	void _notification(int p_what);

	static void _bind_methods();

	bool _symmetry_x;
	bool _symmetry_y;
	bool _alpha_locked;
	int _brush_prefab;
	int _brush_size;
	int _current_tool;
	int _previous_tool;

	Ref<Image> _image;
	Ref<Image> _preview_image;

	Ref<ImageTexture> _image_texture;
	Ref<ImageTexture> _preview_image_texture;

	bool _mouse_down;
	int _mouse_button_down;

	Vector<Ref<PaintAction>> _actions_history;
	Vector<Ref<PaintAction>> _redo_history;
	Ref<PaintAction> _current_action;

	bool _picked_color;

	PoolVector2iArray _selection_cells;
	PoolColorArray _selection_colors;

	Vector2i _cut_pos;
	Vector2i _cut_size;

	Vector2 _mouse_position;
	Vector2 _canvas_mouse_position;
	Vector2 _cell_mouse_position;

	Vector2 _last_mouse_position;
	Vector2 _last_canvas_mouse_position;
	Vector2 _last_cell_mouse_position;

	bool _was_mouse_outside;
};

VARIANT_ENUM_CAST(PaintCanvas::Tools);

#endif
