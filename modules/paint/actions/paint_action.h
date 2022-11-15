#ifndef PAINT_ACTIONS_H
#define PAINT_ACTIONS_H

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

#include "core/object/resource.h"

#include "core/variant/array.h"
#include "core/variant/dictionary.h"
#include "core/math/vector2i.h"
#include "core/containers/pool_vector.h"
#include "core/variant/variant.h"

class PaintCanvasOld;
class PaintCanvas;
class PaintCanvasLayer;
class Node;

class PaintAction : public Resource {
	GDCLASS(PaintAction, Resource);

public:
	PaintCanvas *get_paint_canvas();
	void set_paint_canvas(PaintCanvas *paint_canvas);
	void set_paint_canvas_bind(Node *paint_canvas);

	Dictionary get_action_data_undo();
	void set_action_data_undo(const Dictionary &val);

	Dictionary get_action_data_redo();
	void set_action_data_redo(const Dictionary &val);

	Dictionary get_action_data_preview();
	void set_action_data_preview(const Dictionary &val);

	PoolVector2iArray get_undo_cells();
	void set_undo_cells(const PoolVector2iArray &val);

	PoolColorArray get_undo_colors();
	void set_undo_colors(const PoolColorArray &val);

	PoolVector2iArray get_redo_cells();
	void set_redo_cells(const PoolVector2iArray &val);

	PoolColorArray get_redo_colors();
	void set_redo_colors(const PoolColorArray &val);

	PoolVector2iArray get_preview_cells();
	void set_preview_cells(const PoolVector2iArray &val);

	PoolColorArray get_preview_colors();
	void set_preview_colors(const PoolColorArray &val);

	//deprecated
	Ref<PaintCanvasLayer> get_layer();
	void set_layer(const Ref<PaintCanvasLayer> &val);

	Dictionary get_action_data();
	void set_action_data(const Dictionary &val);

	virtual void do_action_old(PaintCanvasOld *canvas, const Array &data);
	virtual void commit_action_old(PaintCanvasOld *canvas);

	virtual void undo_action_old(PaintCanvasOld *canvas);
	virtual void redo_action_old(PaintCanvasOld *canvas);

	void do_action(const Array &data);
	void commit_action();

	void undo_action();
	void redo_action();

	virtual void _do_action(const Array &data);
	virtual void _commit_action();

	virtual void _undo_action();
	virtual void _redo_action();

	bool can_commit();
	virtual bool _can_commit();

	PoolVector2iArray get_x_sym_points(const int canvas_width, const Vector2i &pixel);
	PoolVector2iArray get_y_sym_points(const int canvas_height, const Vector2i &pixel);
	PoolVector2iArray get_xy_sym_points(const int canvas_width, const int canvas_height, const Vector2i &pixel);
	PoolVector2iArray get_points(const Vector2i &pixel);
	PoolVector2iArray get_points_old(PaintCanvasOld *canvas, const Vector2i &pixel);

	void draw_points(const PoolVector2iArray &point_arr, const PoolColorArray &color_arr);
	void draw_points_old(PaintCanvasOld *canvas, const PoolVector2iArray &point_arr, const PoolColorArray &color_arr);

	PaintAction();
	~PaintAction();

	Dictionary action_data_undo;
	Dictionary action_data_redo;
	Dictionary action_data_preview;

	PoolVector2iArray undo_cells;
	PoolColorArray undo_colors;

	PoolVector2iArray redo_cells;
	PoolColorArray redo_colors;

	PoolVector2iArray preview_cells;
	PoolColorArray preview_colors;

	Ref<PaintCanvasLayer> layer;

	Dictionary action_data;

protected:
	static void _bind_methods();

	PaintCanvas *_paint_canvas;
};

#endif
