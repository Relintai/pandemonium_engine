#ifndef TERRAIN_2D_LIGHT_JOB_H
#define TERRAIN_2D_LIGHT_JOB_H
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

#include "terrain_2d_job.h"

class Terrain2DMesher;

class Terrain2DLightJob : public Terrain2DJob {
	GDCLASS(Terrain2DLightJob, Terrain2DJob);

public:
	int get_rao_octaves();
	void set_rao_octaves(const int val);

	int get_rao_period();
	void set_rao_period(const int val);

	float get_rao_persistence();
	void set_rao_persistence(const float val);

	float get_rao_scale_factor();
	void set_rao_scale_factor(const float val);

	void phase_light();

	void _execute_phase();

	Terrain2DLightJob();
	~Terrain2DLightJob();

protected:
	static void _bind_methods();

	int _rao_octaves;
	int _rao_period;
	float _rao_persistence;
	float _rao_scale_factor;
};

#endif
