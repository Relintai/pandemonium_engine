#ifndef MM_SHAPE_H
#define MM_SHAPE_H

/*************************************************************************/
/*  shape.h                                                              */
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

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMShape : public MMNode {
	GDCLASS(MMShape, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	int get_shape_type() const;
	void set_shape_type(const int val);

	int get_sides() const;
	void set_sides(const int val);

	Ref<MMNodeUniversalProperty> get_radius();
	void set_radius(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_edge();
	void set_edge(const Ref<MMNodeUniversalProperty> &val);

	enum MMShapeType {
		SHAPE_TYPE_CIRCLE = 0,
		SHAPE_TYPE_POLYGON = 1,
		SHAPE_TYPE_STAR = 2,
		SHAPE_TYPE_CURVED_STAR = 3,
		SHAPE_TYPE_RAYS = 4,
	};

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMShape();
	~MMShape();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	//export(int, "Circle,Polygon,Star,Curved Star,Rays")
	int shape_type;
	int sides;
	Ref<MMNodeUniversalProperty> radius;
	Ref<MMNodeUniversalProperty> edge;
};

#endif
