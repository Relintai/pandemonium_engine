#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

/*************************************************************************/
/*  collision_shape.h                                                    */
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

class CollisionShape : public Spatial {
	GDCLASS(CollisionShape, Spatial);
	OBJ_CATEGORY("3D Physics Nodes");

	Ref<Shape> shape;

	uint32_t owner_id;
	CollisionObject *parent;

	void resource_changed(RES res);
	bool disabled;

protected:
	void _update_in_shape_owner(bool p_xform_only = false);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void make_convex_from_brothers();

	void set_shape(const Ref<Shape> &p_shape);
	Ref<Shape> get_shape() const;

	void set_disabled(bool p_disabled);
	bool is_disabled() const;

	String get_configuration_warning() const;

#ifdef TOOLS_ENABLED
	virtual AABB get_fallback_gizmo_aabb() const;
#endif

	CollisionShape();
	~CollisionShape();
};

#endif // BODY_VOLUME_H
