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

#ifndef THREAD_POOL_EXECUTE_JOB_H
#define THREAD_POOL_EXECUTE_JOB_H

#include "thread_pool_job.h"

class ThreadPoolExecuteJob : public ThreadPoolJob {
	GDCLASS(ThreadPoolExecuteJob, ThreadPoolJob);

public:
	Variant get_object() const;
	void set_object(const Variant &value);

	StringName get_method() const;
	void set_method(const StringName &value);

	void _execute();
	void setup(const Variant &obj, const StringName &p_method, VARIANT_ARG_LIST);
#if VERSION_MAJOR < 4
	Variant _setup_bind(const Variant **p_args, int p_argcount, Variant::CallError &r_error);
#else
	Variant _setup_bind(const Variant **p_args, int p_argcount, Callable::CallError &r_error);
#endif

	ThreadPoolExecuteJob();
	~ThreadPoolExecuteJob();

protected:
	static void _bind_methods();

private:
	Object *_object;
	StringName _method;
	int _argcount;
	Variant *_argptr;
};

#endif
