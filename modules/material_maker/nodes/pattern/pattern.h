#ifndef MM_PATTERN_H
#define MM_PATTERN_H

/*************************************************************************/
/*  pattern.h                                                            */
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

class MMPattern : public MMNode {
	GDCLASS(MMPattern, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	int get_combiner_type() const;
	void set_combiner_type(const int val);

	int get_combiner_axis_type_x() const;
	void set_combiner_axis_type_x(const int val);

	int get_combiner_axis_type_y() const;
	void set_combiner_axis_type_y(const int val);

	Vector2 get_repeat();
	void set_repeat(const Vector2 &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);

	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMPattern();
	~MMPattern();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	//export(int, "Multiply,Add,Max,Min,Xor,Pow")
	int combiner_type = 0;
	//export(int, "Sine,Triangle,Square,Sawtooth,Constant,Bounce")
	int combiner_axis_type_x = 0;
	//export(int, "Sine,Triangle,Square,Sawtooth,Constant,Bounce")
	int combiner_axis_type_y = 0;
	//export(Vector2)
	Vector2 repeat = Vector2(4, 4);
};

#endif
