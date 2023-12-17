#ifndef MM_TILER_H
#define MM_TILER_H

/*************************************************************************/
/*  tiler.h                                                              */
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

class MMTiler : public MMNode {
	GDCLASS(MMTiler, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_in_mask();
	void set_in_mask(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_instance_map();
	void set_instance_map(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_tile();
	void set_tile(const Vector2 &val);

	float get_overlap() const;
	void set_overlap(const float val);

	int get_select_inputs() const;
	void set_select_inputs(const int val);

	Vector2 get_scale();
	void set_scale(const Vector2 &val);

	float get_fixed_offset() const;
	void set_fixed_offset(const float val);

	float get_rnd_offset() const;
	void set_rnd_offset(const float val);

	float get_rnd_rotate() const;
	void set_rnd_rotate(const float val);

	float get_rnd_scale() const;
	void set_rnd_scale(const float val);

	float get_rnd_value() const;
	void set_rnd_value(const float val);

	bool get_variations() const;
	void set_variations(const bool val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	Color tiler_calc(const Vector2 &uv, const Vector2 &tile, const int overlap, const Vector2 &_seed);

	MMTiler();
	~MMTiler();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> in_mask;
	Ref<MMNodeUniversalProperty> output;
	Ref<MMNodeUniversalProperty> instance_map;

	Vector2 tile = Vector2(4, 4);
	float overlap = 1;
	int select_inputs = 0;
	Vector2 scale = Vector2(0.5, 0.5);
	float fixed_offset = 0;
	float rnd_offset = 0.25;
	float rnd_rotate = 45;
	float rnd_scale = 0.2;
	float rnd_value = 2;
	bool variations = false;
};

#endif
