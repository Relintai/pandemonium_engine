#ifndef PROP_2D_TEXTURE_JOB
#define PROP_2D_TEXTURE_JOB
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

#include "scene/resources/texture.h"

#if THREAD_POOL_PRESENT
#include "../../thread_pool/thread_pool_job.h"
#endif

#include "core/object/reference.h"

#if TEXTURE_PACKER_PRESENT
class TexturePacker;
#endif

#if THREAD_POOL_PRESENT
class Prop2DTextureJob : public ThreadPoolJob {
	GDCLASS(Prop2DTextureJob, ThreadPoolJob);
#else
class Prop2DTextureJob : public Reference {
	GDCLASS(Prop2DTextureJob, Reference);
#endif

public:
#if TEXTURE_PACKER_PRESENT
	Ref<TexturePacker> get_merger();
	void set_merger(const Ref<TexturePacker> &merger);
#endif

	void _execute();

	Prop2DTextureJob();
	~Prop2DTextureJob();

protected:
	static void _bind_methods();

#if TEXTURE_PACKER_PRESENT
	Ref<TexturePacker> _merger;
#endif

public:
#if !THREAD_POOL_PRESENT
	bool get_complete() const;
	void set_complete(const bool value);

	bool get_cancelled() const;
	void set_cancelled(const bool value);

	float get_max_allocated_time() const;
	void set_max_allocated_time(const float value);

	int get_start_time() const;
	void set_start_time(const int value);

	int get_current_run_stage() const;
	void set_current_run_stage(const int value);

	int get_stage() const;
	void set_stage(const int value);

	void reset_stages();

	float get_current_execution_time();

	bool should_do(const bool just_check = false);
	bool should_return();

	void execute();

private:
	bool _complete;
	bool _cancelled;

	float _max_allocated_time;
	uint64_t _start_time;

	int _current_run_stage;
	int _stage;
#endif
};

#endif
