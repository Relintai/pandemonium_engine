#ifndef PROP_DATA_COLLISION_OBJECT_H
#define PROP_DATA_COLLISION_OBJECT_H

/*************************************************************************/
/*  prop_data_collision_object.h                                         */
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

/*
Copyright (c) 2023-present Péter Magyar

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

#include "prop_data_entry.h"

class Shape;

class PropDataCollisionObject : public PropDataEntry {
	GDCLASS(PropDataCollisionObject, PropDataEntry);

public:
	uint32_t get_collision_layer() const;
	void set_collision_layer(const uint32_t p_layer);

	uint32_t get_collision_mask() const;
	void set_collision_mask(const uint32_t p_mask);

	int get_collision_shape_count() const;
	void set_collision_shape_count(const int p_count);

	Transform get_collision_shape_transform(const int p_index) const;
	void set_collision_shape_transform(const int p_index, const Transform &p_transform);

	Ref<Shape> get_collision_shape(const int p_index) const;
	void set_collision_shape(const int p_index, const Ref<Shape> &p_shape);

	void add_collision_shape(const Transform &p_transform, const Ref<Shape> &p_shape);
	void remove_collision_shape(const int p_index);

	bool _processor_evaluate_children();

	void processor_process_collision_objects(Node *node, const Transform &transform);
	void processor_create_shapes_for(Node *root);

	PropDataCollisionObject();
	~PropDataCollisionObject();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	static void _bind_methods();

	struct ShapeEntry {
		Transform transform;
		Ref<Shape> shape;
	};

	uint32_t _collision_layer;
	uint32_t _collision_mask;
	Vector<ShapeEntry> _shapes;
};

#endif
