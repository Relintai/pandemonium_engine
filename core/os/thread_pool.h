#ifndef THREAD_POOL_H
#define THREAD_POOL_H

/*************************************************************************/
/*  thread_pool.h                                                        */
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

#include "core/containers/list.h"
#include "core/containers/vector.h"
#include "core/object/object.h"

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
	void set_thread_count(const int value);

	int get_thread_fallback_count() const;
	void set_thread_fallback_count(const int value);

	float get_max_time_per_frame() const;
	void set_max_time_per_frame(const float value);

	float get_max_work_per_frame_percent() const;
	void set_max_work_per_frame_percent(const float value);

	float get_target_fps() const;
	void set_target_fps(const float value);

	void apply_max_work_per_frame_percent();

	bool is_working() const;
	bool is_working_no_lock() const;

	bool has_job(const Ref<ThreadPoolJob> &job);
	void add_job(const Ref<ThreadPoolJob> &job);

	void cancel_job(Ref<ThreadPoolJob> job);
	void cancel_job_wait(Ref<ThreadPoolJob> job);

	void _thread_finished(ThreadPoolContext *context);
	static void _worker_thread_func(void *user_data);

	void update();

	void register_core_settings();

	void apply_settings();

	ThreadPool();
	~ThreadPool();

protected:
	static void _bind_methods();

private:
	static ThreadPool *_instance;

	bool _dirty;
	bool _use_threads;
	bool _use_threads_new;
	int _thread_count;
	int _thread_fallback_count;
	float _max_work_per_frame_percent;
	float _max_time_per_frame;
	float _target_fps;

	Vector<ThreadPoolContext *> _threads;

	List<Ref<ThreadPoolJob>> _queue;
};

#endif
