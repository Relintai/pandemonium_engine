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

#include "core/reference.h"

#include "core/pool_vector.h"
#include "core/variant.h"
#include "core/dictionary.h"
#include "core/array.h"
#include "core/math/vector2i.h"

class PaintCanvas;

//extends Node  (this should probably not be a node)
//class_name GEAction
class PaintAction : public Reference {
	GDCLASS(PaintAction, Reference);

public:
	virtual void do_action(PaintCanvas *canvas, Array data);
	virtual void commit_action(PaintCanvas *canvas);

	virtual void undo_action(PaintCanvas *canvas);
	virtual void redo_action(PaintCanvas *canvas);
	virtual bool can_commit();

	virtual PoolVector2iArray get_x_sym_points(int canvas_width, Vector2i pixel);
	virtual PoolVector2iArray get_y_sym_points(int canvas_height, Vector2i pixel);
	virtual PoolVector2iArray get_xy_sym_points(int canvas_width, int canvas_height, Vector2i pixel);
	virtual PoolVector2iArray get_points(PaintCanvas *canvas, Vector2i pixel);

	PaintAction();
	~PaintAction();

	Dictionary action_data_undo;
	Dictionary action_data_redo;
	Dictionary action_data_preview;

	Dictionary action_data;

protected:
	static void _bind_methods();
};

#endif
