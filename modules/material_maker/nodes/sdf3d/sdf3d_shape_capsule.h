#ifndef MM_SDF3D_SHAPE_CAPSULE_H
#define MM_SDF3D_SHAPE_CAPSULE_H

/*************************************************************************/
/*  sdf3d_shape_capsule.h                                                */
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

#include "../bases/curve_base.h"
#include "../mm_node_universal_property.h"

class MMSdf3dShapeCapsule : public CurveBase {
	GDCLASS(MMSdf3dShapeCapsule, CurveBase);

public:
	Ref<MMNodeUniversalProperty> get_output();
	void set_output(const Ref<MMNodeUniversalProperty> &val);

	int get_axis() const;
	void set_axis(const int val);

	float get_length() const;
	void set_length(const float val);

	float get_radius() const;
	void set_radius(const float val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	Vector2 sdf3d_capsule_y(const Vector3 &p, const float r, const float l);
	Vector2 sdf3d_capsule_x(const Vector3 &p, const float r, const float l);
	Vector2 sdf3d_capsule_z(const Vector3 &p, const float r, const float l);

	void _curve_changed();

	MMSdf3dShapeCapsule();
	~MMSdf3dShapeCapsule();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Ref<MMNodeUniversalProperty> output;
	//export(int, "X,Y,Z")
	int axis;
	float length;
	float radius;
};

#endif
