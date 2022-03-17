#ifndef PROP_INSTANCE_JOB
#define PROP_INSTANCE_JOB
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
#include "../thread_pool/thread_pool_job.h"
#else

#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/object/reference.h"
#define Texture Texture2D
#else
#include "core/reference.h"
#endif

#endif

class PropData;
class PropInstance;

#if THREAD_POOL_PRESENT
class PropInstanceJob : public ThreadPoolJob {
	GDCLASS(PropInstanceJob, ThreadPoolJob);
#else
class PropInstanceJob : public Reference {
	GDCLASS(PropInstanceJob, Reference);
#endif

public:
	static const String BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE;

	enum ActiveBuildPhaseType {
		BUILD_PHASE_TYPE_NORMAL = 0,
		BUILD_PHASE_TYPE_PROCESS,
		BUILD_PHASE_TYPE_PHYSICS_PROCESS,
	};

public:
	ActiveBuildPhaseType get_build_phase_type();
	void set_build_phase_type(PropInstanceJob::ActiveBuildPhaseType build_phase_type);

	void set_prop(const Ref<PropData> &prop);
	void set_prop_instance(PropInstance *instance);
	void set_prop_instance_bind(Node *instance);

	int get_phase();
	void set_phase(const int phase);
	void next_phase();

	bool get_build_done();
	void set_build_done(const bool val);

	void finished();

	void reset();
	virtual void _reset();

	void _execute();

	void execute_phase();
	virtual void _execute_phase();

	void process(const float delta);
	void physics_process(const float delta);

	void prop_instance_enter_tree();
	void prop_instance_exit_tree();

	PropInstanceJob();
	~PropInstanceJob();

protected:
	static void _bind_methods();

	ActiveBuildPhaseType _build_phase_type;
	bool _build_done;
	int _phase;
	bool _in_tree;
	Ref<PropData> _prop;
	PropInstance *_instance;

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

VARIANT_ENUM_CAST(PropInstanceJob::ActiveBuildPhaseType);

#endif
