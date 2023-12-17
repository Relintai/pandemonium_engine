#ifndef PROP_2D_INSTANCE_MERGER_H
#define PROP_2D_INSTANCE_MERGER_H

/*************************************************************************/
/*  prop_2d_instance_merger.h                                            */
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

#include "prop_2d_instance.h"

#include "scene/main/node_2d.h"
#include "scene/resources/shapes_2d/shape_2d.h"

#include "core/math/vector3.h"

#include "prop_2d_instance_prop_job.h"

#include "props/prop_2d_data.h"

class MeshDataInstance;

class Prop2DInstanceMerger : public Prop2DInstance {
	GDCLASS(Prop2DInstanceMerger, Prop2DInstance);

public:
	bool get_building();
	void set_building(const bool value);

	Ref<Prop2DInstanceJob> get_job();
	void set_job(const Ref<Prop2DInstanceJob> &job);

	//Meshes
	RID mesh_get(const int index);
	void mesh_add(const RID mesh);
	int mesh_get_num() const;
	void meshes_clear();
	void meshes_create(const int num);

	Vector<Variant> meshes_get();
	void meshes_set(const Vector<Variant> &meshes);

	//Colliders
	Transform2D collider_local_transform_get(const int index);
	RID collider_body_get(const int index);
	Ref<Shape2D> collider_shape_get(const int index);
	RID collider_shape_rid_get(const int index);
	int collider_add(const Transform2D &local_transform, const Ref<Shape2D> &shape, const RID &shape_rid, const RID &body, const bool owns_shape = false);
	int collider_get_num() const;
	void colliders_clear();

	Vector<Variant> colliders_get();
	void colliders_set(const Vector<Variant> &colliders);

	//Debug
	void debug_mesh_allocate();
	void debug_mesh_free();
	bool debug_mesh_has();
	void debug_mesh_clear();
	void debug_mesh_array_clear();
	void debug_mesh_add_vertices_to(const PoolVector2Array &arr);
	void debug_mesh_send();
	void draw_debug_mdr_colliders();

	void free_meshes();
	void free_colliders();

	virtual void _init_materials();

	virtual void _build();
	virtual void _build_finished();

	void _prop_preprocess(Transform2D tarnsform, const Ref<Prop2DData> &prop);

	void collision_layer_changed();
	void collision_mask_changed();

	virtual void _create_job();

	Prop2DInstanceMerger();
	~Prop2DInstanceMerger();

protected:
	void _notification(int p_what);
	static void _bind_methods();

protected:
	struct ColliderBody {
		Transform2D transform;
		RID body;
		Ref<Shape2D> shape;
		RID shape_rid;
		bool owns_shape;

		ColliderBody() {
			owns_shape = false;
		}
	};

private:
	bool _build_queued;
	bool _building;

	Transform2D _last_transform;

	Ref<Prop2DInstanceProp2DJob> _job;

	Vector<RID> _meshes;
	Vector<ColliderBody> _colliders;

	//debug
	RID _debug_mesh_rid;
	PoolVector2Array _debug_mesh_array;
};

#endif
