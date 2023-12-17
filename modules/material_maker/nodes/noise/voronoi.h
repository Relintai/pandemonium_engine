#ifndef MM_VORONOI_H
#define MM_VORONOI_H

/*************************************************************************/
/*  voronoi.h                                                            */
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

class MMVoronoi : public MMNode {
	GDCLASS(MMVoronoi, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_out_nodes();
	void set_out_nodes(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_borders();
	void set_out_borders(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_random_color();
	void set_out_random_color(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_fill();
	void set_out_fill(const Ref<MMNodeUniversalProperty> &val);

	Vector2 get_scale();
	void set_scale(const Vector2 &val);

	Vector2 get_stretch();
	void set_stretch(const Vector2 &val);

	float get_intensity() const;
	void set_intensity(const float val);

	float get_randomness() const;
	void set_randomness(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);

	Color _get_value_for(const Vector2 &uv, const int pseed);

	MMVoronoi();
	~MMVoronoi();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> out_nodes;
	Ref<MMNodeUniversalProperty> out_borders;
	Ref<MMNodeUniversalProperty> out_random_color;
	Ref<MMNodeUniversalProperty> out_fill;
	Vector2 scale;
	Vector2 stretch;
	float intensity;
	float randomness;
};

#endif
