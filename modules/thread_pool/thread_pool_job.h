#ifndef THREAD_POOL_JOB_H
#define THREAD_POOL_JOB_H
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

#include "core/reference.h"

class ThreadPoolJob : public Reference {
	GDCLASS(ThreadPoolJob, Reference);

public:
	//is_running, is queued?
	//job status -> none, running, queued, done?

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

	Variant get_object() const;
	void set_object(const Variant &value);

	StringName get_method() const;
	void set_method(const StringName &value);

	float get_current_execution_time();

	bool should_do(const bool just_check = false);
	bool should_return();

	void execute();

	ThreadPoolJob();
	~ThreadPoolJob();

protected:
	static void _bind_methods();

private:
	bool _complete;
	bool _cancelled;

	float _max_allocated_time;
	uint64_t _start_time;

	int _current_run_stage;
	int _stage;

	Object *_object;
	StringName _method;
	int _argcount;
	Variant *_argptr;
};

#endif
