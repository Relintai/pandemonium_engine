#ifndef PROP_INSTANCE_H
#define PROP_INSTANCE_H

/*************************************************************************/
/*  prop_instance.h                                                      */
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

#include "scene/3d/visual_instance.h"

#include "scene/resources/material/material.h"

#include "core/math/vector3.h"

#include "props/prop_data.h"

class PropInstance : public VisualInstance {
	GDCLASS(PropInstance, VisualInstance);

public:
	Ref<PropData> get_prop_data();
	void set_prop_data(const Ref<PropData> &data);

	Ref<Material> get_material();
	void set_material(const Ref<Material> &material);

	uint32_t get_collision_layer() const;
	void set_collision_layer(uint32_t p_layer);

	uint32_t get_collision_mask() const;
	void set_collision_mask(uint32_t p_mask);

	virtual void collision_layer_changed();
	virtual void collision_mask_changed();

	virtual AABB get_aabb() const;
	virtual PoolVector<Face3> get_faces(uint32_t p_usage_flags) const;

	void init_materials();
	virtual void _init_materials();

	void build();
	void queue_build();
	void build_finished();

	virtual void _build();
	virtual void _build_finished();

	void prop_preprocess(Transform transform, const Ref<PropData> &prop);
	virtual void _prop_preprocess(Transform transform, const Ref<PropData> &prop);

	PropInstance();
	~PropInstance();

protected:
	void _notification(int p_what);
	static void _bind_methods();

protected:
	Ref<PropData> _prop_data;
	Ref<Material> _material;

	uint32_t _collision_layer;
	uint32_t _collision_mask;

	bool _build_queued;
	bool _building;

	AABB _aabb;
};

#endif
