#ifndef POLYGON_VIEW_H
#define POLYGON_VIEW_H

/*************************************************************************/
/*  polygon_view.h                                                       */
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

#include "scene/main/control.h"

class PolygonBase;

class PolygonView : public Control {
	GDCLASS(PolygonView, Control);

public:
	Ref<PolygonBase> get_polygon();
	virtual void set_polygon(const Ref<PolygonBase> &val);

	Vector2 get_draw_size();
	void set_draw_size(const Vector2 &val);

	Vector2 get_draw_offset();
	void set_draw_offset(const Vector2 &val);

	bool get_closed() const;
	void set_closed(const bool c = true);

	Vector2 transform_point(const Vector2 &p);
	Vector2 reverse_transform_point(const Vector2 &p);

	void _draw();
	virtual void _on_resize();

	PolygonView();
	~PolygonView();

protected:
	void _notification(int p_what);
	static void _bind_methods();

	Ref<PolygonBase> polygon;
	Vector2 draw_size = Vector2(1, 1);
	Vector2 draw_offset = Vector2(0, 0);
	bool closed = true;
};

#endif
