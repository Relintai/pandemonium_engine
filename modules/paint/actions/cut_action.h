#ifndef CUT_ACTION_H
#define CUT_ACTION_H

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

#include "paint_action.h"

class PaintCanvasOld;

class CutAction : public PaintAction {
	GDCLASS(CutAction, PaintAction);

public:
	Color get_selection_color();
	void set_selection_color(const Color &val);

	Vector2i get_mouse_start_pos();
	void set_mouse_start_pos(const Vector2i &val);

	Vector2i get_mouse_end_pos();
	void set_mouse_end_pos(const Vector2i &val);

	bool get_mouse_start_pos_set();
	void set_mouse_start_pos_set(const bool val);

	bool _can_commit();

	void do_action_old(PaintCanvasOld *canvas, const Array &data);
	void commit_action_old(PaintCanvasOld *canvas);

	void _do_action(const Array &data);
	void _commit_action();

	CutAction();
	~CutAction();

	Color selection_color;
	Vector2i mouse_start_pos;
	Vector2i mouse_end_pos;
	bool mouse_start_pos_set;

protected:
	static void _bind_methods();
};

#endif
