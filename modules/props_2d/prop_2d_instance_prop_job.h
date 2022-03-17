#ifndef PROP_2D_JOB_H
#define PROP_2D_JOB_H
/*
Copyright (c) 2019-2022 Péter Magyar

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




#include "prop_2d_instance_job.h"

#include "scene/resources/shape_2d.h"

class Prop2DMesher;
class Prop2DInstance;
class Prop2DInstanceMerger;
class Prop2DMesherJobStep;
class Prop2DMaterialCache;
class Shape;
class Prop2DLight;
class Prop2DDataTiledWall2D;

#if MESH_DATA_RESOURCE_PRESENT
class Prop2DDataMeshData;
#endif

class Prop2DInstanceProp2DJob : public Prop2DInstanceJob {
	GDCLASS(Prop2DInstanceProp2DJob, Prop2DInstanceJob);

public:
	Ref<Prop2DMaterialCache> get_material_cache();
	void set_material_cache(const Ref<Prop2DMaterialCache> &cache);

	void add_collision_shape(const Ref<Shape2D> &shape, const Transform2D &transform, const bool owns_shape = false);
	void clear_collision_shapes();

	Prop2DInstanceMerger *get_prop_instace();
	void set_prop_instace(Prop2DInstanceMerger *prop);
	void set_prop_instace_bind(Node *prop);

	Ref<Prop2DMesher> get_prop_mesher() const;
	void set_prop_mesher(const Ref<Prop2DMesher> &mesher);

#if MESH_DATA_RESOURCE_PRESENT
	void add_mesh(const Ref<Prop2DDataMeshData> &mesh_data, const Transform2D &base_transform);
	void clear_meshes();
#endif

	void add_tiled_wall(const Ref<Prop2DDataTiledWall2D> &data, const Transform2D &base_transform);
	void clear_tiled_walls();

	void add_light(const Ref<Prop2DLight> &light);
	void clear_lights();

	void _physics_process(float delta);
	void _execute_phase();
	void _reset();

	void phase_setup_cache();
	void phase_physics_process();
	void phase_prop();
	void phase_steps();

	void step_type_normal();

	Array merge_mesh_array(Array arr) const;
	Array bake_mesh_array_uv(Array arr, Ref<Texture> tex, float mul_color = 0.7) const;

	void reset_meshes();

	Prop2DInstanceProp2DJob();
	~Prop2DInstanceProp2DJob();

protected:
	static void _bind_methods();

protected:
#if MESH_DATA_RESOURCE_PRESENT
	struct PMDREntry {
		Ref<Prop2DDataMeshData> mesh_data;
		Transform2D base_transform;
	};
#endif

	struct PTWEntry {
		Ref<Prop2DDataTiledWall2D> data;
		Transform2D base_transform;
	};

	struct CollisionShapeEntry {
		Ref<Shape2D> shape;
		Transform2D transform;
		bool owns_shape;

		CollisionShapeEntry() {
			owns_shape = false;
		}
	};

	Ref<Prop2DMaterialCache> _material_cache;

	Ref<Prop2DMesher> _prop_mesher;
	Prop2DInstanceMerger *_prop_instace;

#if MESH_DATA_RESOURCE_PRESENT
	Vector<PMDREntry> _prop_mesh_datas;
#endif

	Vector<CollisionShapeEntry> _collision_shapes;

	Vector<PTWEntry> _prop_tiled_wall_datas;

	Array temp_mesh_arr;
};

#endif
