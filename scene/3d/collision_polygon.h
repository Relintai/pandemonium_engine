#ifndef COLLISION_POLYGON_H
#define COLLISION_POLYGON_H

/*************************************************************************/
/*  collision_polygon.h                                                  */
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
#include "scene/main/spatial.h"

class CollisionObject;
class Shape;

class CollisionPolygon : public Spatial {
	GDCLASS(CollisionPolygon, Spatial);
	real_t margin = 0.04;

protected:
	float depth;
	AABB aabb;
	Vector<Point2> polygon;

	uint32_t owner_id;
	CollisionObject *parent;

	bool disabled;

	void _build_polygon();

	void _update_in_shape_owner(bool p_xform_only = false);

	bool _is_editable_3d_polygon() const;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_depth(float p_depth);
	float get_depth() const;

	void set_polygon(const Vector<Point2> &p_polygon);
	Vector<Point2> get_polygon() const;

	void set_disabled(bool p_disabled);
	bool is_disabled() const;

	virtual AABB get_item_rect() const;

	real_t get_margin() const;
	void set_margin(real_t p_margin);

	String get_configuration_warning() const;

	CollisionPolygon();
};

#endif // COLLISION_POLYGON_H
