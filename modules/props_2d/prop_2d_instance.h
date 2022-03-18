#ifndef PROP_2D_INSTANCE_H
#define PROP_2D_INSTANCE_H
/*
Copyright (c) 2020-2022 Péter Magyar

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

#include "core/version.h"

#include "scene/2d/node_2d.h"

#include "scene/resources/material.h"

#include "core/math/vector3.h"

#include "props/prop_2d_data.h"

class Prop2DInstance : public Node2D {
	GDCLASS(Prop2DInstance, Node2D);

public:
	Ref<Prop2DData> get_prop_data();
	void set_prop_data(const Ref<Prop2DData> &data);

	uint32_t get_collision_layer() const;
	void set_collision_layer(uint32_t p_layer);

	uint32_t get_collision_mask() const;
	void set_collision_mask(uint32_t p_mask);

	virtual void collision_layer_changed();
	virtual void collision_mask_changed();

	void init_materials();
	virtual void _init_materials();

	void build();
	void queue_build();
	void build_finished();

	virtual void _build();
	virtual void _build_finished();

	void prop_preprocess(Transform2D tarnsform, const Ref<Prop2DData> &prop);
	virtual void _prop_preprocess(Transform2D tarnsform, const Ref<Prop2DData> &prop);

	Prop2DInstance();
	~Prop2DInstance();

protected:
	void _notification(int p_what);
	static void _bind_methods();

protected:
	Ref<Prop2DData> _prop_data;

	uint32_t _collision_layer;
	uint32_t _collision_mask;

	bool _build_queued;
	bool _building;
};

#endif
