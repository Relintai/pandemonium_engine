#ifndef MM_FBM_NOISE_H
#define MM_FBM_NOISE_H

/*************************************************************************/
/*  fbm_noise.h                                                          */
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

class MMFbmNoise : public MMNode {
	GDCLASS(MMFbmNoise, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_image();
	void set_image(const Ref<MMNodeUniversalProperty> &val);

	int get_type() const;
	void set_type(const int val);

	Vector2 get_scale();
	void set_scale(const Vector2 &val);

	int get_folds() const;
	void set_folds(const int val);

	int get_iterations() const;
	void set_iterations(const int val);

	float get_persistence() const;
	void set_persistence(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Color _get_value_for(const Vector2 &uv, const int pseed);
	void _render(const Ref<MMMaterial> &material);

	MMFbmNoise();
	~MMFbmNoise();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> image;
	//export(int, "Value,Perlin,Simplex,Cellular1,Cellular2,Cellular3,Cellular4,Cellular5,Cellular6")
	int type;
	Vector2 scale;
	int folds;
	int iterations;
	float persistence;
};

#endif
