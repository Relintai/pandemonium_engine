#ifndef THREAD_POOL_H
#define THREAD_POOL_H

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

#include "core/object.h"
#include "core/vector.h"

#include "core/os/semaphore.h"
#include "core/os/thread.h"
#include "core/os/thread_safe.h"
#include "thread_pool_execute_job.h"
#include "thread_pool_job.h"

class ThreadPool : public Object {
	GDCLASS(ThreadPool, Object);

	_THREAD_SAFE_CLASS_

protected:
	struct ThreadPoolContext {
		Thread *thread;
		Semaphore *semaphore;
		Ref<ThreadPoolJob> job;
		bool running;

		ThreadPoolContext() {
			thread = NULL;
			semaphore = NULL;
			running = false;
		}
	};

public:
	static ThreadPool *get_singleton();

	bool get_use_threads() const;
	void set_use_threads(const bool value);

	int get_thread_count() const;
	void set_thread_count(const bool value);

	int get_thread_fallback_count() const;
	void set_thread_fallback_count(const bool value);

	float get_max_work_per_frame_percent() const;
	void set_max_work_per_frame_percent(const bool value);

	float get_max_time_per_frame() const;
	void set_max_time_per_frame(const bool value);

	bool has_job(const Ref<ThreadPoolJob> &job);
	void add_job(const Ref<ThreadPoolJob> &job);

	void cancel_job(Ref<ThreadPoolJob> job);
	void cancel_job_wait(Ref<ThreadPoolJob> job);

	void _thread_finished(ThreadPoolContext *context);
	static void _worker_thread_func(void *user_data);

	void register_update();
	void update();

	ThreadPool();
	~ThreadPool();

protected:
	static void _bind_methods();

private:
	static ThreadPool *_instance;

	bool _use_threads;
	int _thread_count;
	int _thread_fallback_count;
	float _max_work_per_frame_percent;
	float _max_time_per_frame;

	Vector<ThreadPoolContext *> _threads;

	Vector<Ref<ThreadPoolJob> > _queue;
	int _current_queue_head;
	int _current_queue_tail;

	int _queue_start_size;
	int _queue_grow_size;

	//todo
	//Vector<Ref<ThreadPoolJob> > _job_pool;
};

#endif
