#ifndef TERRAIN_2D_PROP_JOB_H
#define TERRAIN_2D_PROP_JOB_H
/*
Copyright (c) 2019-2023 Péter Magyar

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

#include "terrain_2d_job.h"

class Terrain2DMesher;

class Terrain2DProp2DJob : public Terrain2DJob {
	GDCLASS(Terrain2DProp2DJob, Terrain2DJob);

public:
	Ref<Terrain2DMesher> get_prop_mesher() const;
	void set_prop_mesher(const Ref<Terrain2DMesher> &mesher);

	void phase_physics_process();
	void phase_prop();

	void _physics_process(float delta);
	void _execute_phase();
	void _reset();

	void phase_setup();

	void phase_steps();

	void step_type_normal();

	Terrain2DProp2DJob();
	~Terrain2DProp2DJob();

protected:
	static void _bind_methods();

	Ref<Terrain2DMesher> _prop_mesher;

	PoolVector<Vector2> temp_arr_collider;
	Array temp_mesh_arr;
};

#endif
