#ifndef PROP_INSTANCE_MERGER_H
#define PROP_INSTANCE_MERGER_H
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




#include "prop_instance.h"

#include "core/version.h"

#if VERSION_MAJOR < 4
#include "scene/3d/spatial.h"
#else
#include "scene/3d/node_3d.h"

#define Spatial Node3D
#endif

#include "core/math/vector3.h"

#include "prop_instance_prop_job.h"

#include "props/prop_data.h"

class MeshDataInstance;

class PropInstanceMerger : public PropInstance {
	GDCLASS(PropInstanceMerger, PropInstance);

public:
	static const float LOD_CHECK_INTERVAL;

	bool get_building();
	void set_building(const bool value);

	int get_lod_level();
	void set_lod_level(const int value);

	bool get_auto_lod();
	void set_auto_lod(const bool value);

	float get_first_lod_distance_squared();
	void set_first_lod_distance_squared(const float dist);

	float get_lod_reduction_distance_squared();
	void set_lod_reduction_distance_squared(const float dist);

	Ref<PropInstanceJob> get_job();
	void set_job(const Ref<PropInstanceJob> &job);

	///Materials
	Ref<Material> material_get(const int index);
	void material_add(const Ref<Material> &value);
	int material_get_num() const;
	void materials_clear();

	Vector<Variant> materials_get();
	void materials_set(const Vector<Variant> &materials);

	//Meshes
	RID mesh_get(const int index);
	RID mesh_instance_get(const int index);
	void mesh_add(const RID mesh_instance, const RID mesh);
	int mesh_get_num() const;
	void meshes_clear();
	void meshes_create(const int num);

	Vector<Variant> meshes_get();
	void meshes_set(const Vector<Variant> &meshes);

	//Colliders
	Transform collider_local_transform_get(const int index);
	RID collider_body_get(const int index);
	Ref<Shape> collider_shape_get(const int index);
	RID collider_shape_rid_get(const int index);
	int collider_add(const Transform &local_transform, const Ref<Shape> &shape, const RID &shape_rid, const RID &body, const bool owns_shape = false);
	int collider_get_num() const;
	void colliders_clear();

	Vector<Variant> colliders_get();
	void colliders_set(const Vector<Variant> &colliders);

	void check_auto_lod();
	void apply_lod_level();

	//Debug
	void debug_mesh_allocate();
	void debug_mesh_free();
	bool debug_mesh_has();
	void debug_mesh_clear();
	void debug_mesh_array_clear();
	void debug_mesh_add_vertices_to(const PoolVector3Array &arr);
	void debug_mesh_send();
	void draw_debug_mdr_colliders();

	void free_meshes();
	void free_colliders();

	virtual void _init_materials();

	virtual void _build();
	virtual void _build_finished();

	void _prop_preprocess(Transform tarnsform, const Ref<PropData> &prop);

	void collision_layer_changed();
	void collision_mask_changed();

	virtual void _create_job();

	PropInstanceMerger();
	~PropInstanceMerger();

protected:
	void _notification(int p_what);
	static void _bind_methods();

protected:
	struct ColliderBody {
		Transform transform;
		RID body;
		Ref<Shape> shape;
		RID shape_rid;
		bool owns_shape;

		ColliderBody() {
			owns_shape = false;
		}
	};

	struct MeshEntry {
		RID mesh;
		RID mesh_instance;
	};

private:
	bool _build_queued;
	bool _building;

	int _lod_level;

	Transform _last_transform;

	Ref<PropInstancePropJob> _job;

	Vector<Ref<Material>> _materials;
	Vector<MeshEntry> _meshes;
	Vector<ColliderBody> _colliders;

	bool _auto_lod;
	bool _auto_lod_on;
	float _first_lod_distance_squared;
	float _lod_reduction_distance_squared;
	float _lod_check_timer;

	//debug
	RID _debug_mesh_rid;
	RID _debug_mesh_instance;
	PoolVector3Array _debug_mesh_array;
};

#endif
