#ifndef MM_WEAVE_H
#define MM_WEAVE_H

/*************************************************************************/
/*  weave.h                                                              */
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

class MMWeave : public MMNode {
	GDCLASS(MMWeave, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_out_main();
	void set_out_main(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_horizontal_map();
	void set_out_horizontal_map(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_vertical_map();
	void set_out_vertical_map(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_size();
	void set_size(const Vector2 &val);

	Ref<MMNodeUniversalProperty> get_width();
	void set_width(const Ref<MMNodeUniversalProperty> &val);

	int get_stitch() const;
	void set_stitch(const int val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMWeave();
	~MMWeave();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> out_main;
	Ref<MMNodeUniversalProperty> out_horizontal_map;
	Ref<MMNodeUniversalProperty> out_vertical_map;
	Vector2 size;
	Ref<MMNodeUniversalProperty> width;
	int stitch;
};

#endif
