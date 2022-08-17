#include "thread_pool.h"

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

#include "core/config/engine.h"
#include "core/config/project_settings.h"

#include "core/os/os.h"
#include "scene/main/scene_tree.h"

ThreadPool *ThreadPool::_instance;

ThreadPool *ThreadPool::get_singleton() {
	return _instance;
}

bool ThreadPool::get_use_threads() const {
	return _use_threads;
}
void ThreadPool::set_use_threads(const bool value) {
	_use_threads = value;
}

int ThreadPool::get_thread_count() const {
	return _thread_count;
}
void ThreadPool::set_thread_count(const bool value) {
	_thread_count = value;
}

int ThreadPool::get_thread_fallback_count() const {
	return _thread_fallback_count;
}
void ThreadPool::set_thread_fallback_count(const bool value) {
	_thread_fallback_count = value;
}

float ThreadPool::get_max_work_per_frame_percent() const {
	return _max_work_per_frame_percent;
}
void ThreadPool::set_max_work_per_frame_percent(const bool value) {
	_max_work_per_frame_percent = value;
}

float ThreadPool::get_max_time_per_frame() const {
	return _max_time_per_frame;
}
void ThreadPool::set_max_time_per_frame(const bool value) {
	_max_time_per_frame = value;
}

bool ThreadPool::has_job(const Ref<ThreadPoolJob> &job) {
	for (int i = 0; i < _threads.size(); ++i) {
		ThreadPoolContext *context = _threads.get(i);

		if (context->job == job) {
			return true;
		}
	}

	_THREAD_SAFE_LOCK_

	for (int i = _current_queue_head; i < _current_queue_tail; ++i) {
		if (_queue[i] == job) {
			_THREAD_SAFE_UNLOCK_

			return true;
		}
	}

	_THREAD_SAFE_UNLOCK_

	return false;
}

void ThreadPool::add_job(const Ref<ThreadPoolJob> &job) {
	_THREAD_SAFE_LOCK_

	if (_use_threads) {
		for (int i = 0; i < _threads.size(); ++i) {
			ThreadPoolContext *context = _threads.get(i);

			if (!context->job.is_valid()) {
				context->job = job;
				context->semaphore->post();
				_THREAD_SAFE_UNLOCK_
				return;
			}
		}
	}

	if (_current_queue_tail == _queue.size()) {
		if (_current_queue_head == 0) {
			_queue.resize(_queue.size() + _queue_grow_size);
		} else {
			int j = 0;

			for (int i = _current_queue_head; i < _current_queue_tail; ++i) {
				_queue.write[j++] = _queue[i];
			}

			_current_queue_head = 0;
			_current_queue_tail = j;
		}
	}

	_queue.write[_current_queue_tail++] = job;

	_THREAD_SAFE_UNLOCK_
}

void ThreadPool::cancel_job(Ref<ThreadPoolJob> job) {
	ERR_FAIL_COND(!job.is_valid());

	job->set_cancelled(true);

	_THREAD_SAFE_LOCK_

	//it it's in the queue remove it
	for (int i = _current_queue_head; i < _current_queue_tail; ++i) {
		Ref<ThreadPoolJob> cjob = _queue[i];

		if (cjob == job) {
			_queue.write[i].unref();

			for (int j = i; j + 1 < _current_queue_tail; ++j) {
				_queue.write[j] = _queue[j + 1];
			}

			--_current_queue_tail;

			_THREAD_SAFE_UNLOCK_
			return;
		}
	}

	_THREAD_SAFE_UNLOCK_
}

void ThreadPool::cancel_job_wait(Ref<ThreadPoolJob> job) {
	ERR_FAIL_COND(!job.is_valid());

	job->set_cancelled(true);

	_THREAD_SAFE_LOCK_

	for (int i = _current_queue_head; i < _current_queue_tail; ++i) {
		Ref<ThreadPoolJob> j = _queue[i];

		if (j == job) {
			_queue.write[i].unref();
			_THREAD_SAFE_UNLOCK_
			return;
		}
	}

	_THREAD_SAFE_UNLOCK_

	for (int i = 0; i < _threads.size(); ++i) {
		Ref<ThreadPoolJob> j = _threads[i]->job;

		if (j == job) {
			//wait until it's done
			while (_threads[i]->job == job) {
				OS::get_singleton()->delay_usec(100);
			}

			return;
		}
	}
}

void ThreadPool::_thread_finished(ThreadPoolContext *context) {
	_THREAD_SAFE_LOCK_

	context->job.unref();

	while (_current_queue_head != _current_queue_tail) {
		context->job = _queue.get(_current_queue_head);

		if (!context->job.is_valid()) {
			++_current_queue_head;
			continue;
		}

		context->semaphore->post();
		_queue.write[_current_queue_head].unref();

		++_current_queue_head;
		break;
	}

	_THREAD_SAFE_UNLOCK_
}

void ThreadPool::_worker_thread_func(void *user_data) {
	ThreadPoolContext *context = reinterpret_cast<ThreadPoolContext *>(user_data);

	while (context->running) {
		context->semaphore->wait();

		if (!context->job.is_valid()) {
			ThreadPool::get_singleton()->_thread_finished(context);
			continue;
		}

		if (context->job->get_cancelled()) {
			ThreadPool::get_singleton()->_thread_finished(context);
			continue;
		}

		context->job->execute();

		ThreadPool::get_singleton()->_thread_finished(context);
	}
}

void ThreadPool::register_update() {
	SceneTree::get_singleton()->connect("idle_frame", this, "update");
}

void ThreadPool::update() {
	if (_current_queue_head == _current_queue_tail)
		return;

	float remaining_time = _max_time_per_frame;

	while (remaining_time > 0 && _current_queue_head != _current_queue_tail) {
		Ref<ThreadPoolJob> job = _queue.get(_current_queue_head);

		if (!job.is_valid()) {
			++_current_queue_head;
			continue;
		}

		job->set_max_allocated_time(remaining_time);
		job->execute();

		remaining_time -= job->get_current_execution_time();

		if (job->get_complete() || job->get_cancelled()) {
			_queue.write[_current_queue_head++].unref();
		}
	}
}

ThreadPool::ThreadPool() {
	_instance = this;

	_current_queue_head = 0;
	_current_queue_tail = 0;

	_use_threads = GLOBAL_DEF("thread_pool/use_threads", true);
	_thread_count = GLOBAL_DEF("thread_pool/thread_count", -1);
	_thread_fallback_count = GLOBAL_DEF("thread_pool/thread_fallback_count", 4);

	if (_thread_fallback_count <= 0) {
		print_error("ThreadPool: thread_fallback_count is invalid! Check ProjectSettings/ThreadPool/thread_fallback_count! Needs to be > 0! Set to 1!");

		_thread_fallback_count = 1;
	}

	//Todo Add help text, as this will only come into play if threading is disabled, or not available
	_max_work_per_frame_percent = GLOBAL_DEF("thread_pool/max_work_per_frame_percent", 25);

	_queue_start_size = GLOBAL_DEF("thread_pool/queue_start_size", 20);
	_queue_grow_size = GLOBAL_DEF("thread_pool/queue_grow_size", 10);
	_queue.resize(_queue_start_size);

	//Todo this should be recalculated constantly to smooth out performance better
	_max_time_per_frame = (1 / 60.0) * (_max_work_per_frame_percent / 100.0);

	if (!OS::get_singleton()->can_use_threads()) {
		_use_threads = false;
	}

	if (_use_threads) {
		if (_thread_count <= 0) {
			_thread_count = OS::get_singleton()->get_processor_count() + _thread_count;
		}

		//a.k.a OS::get_singleton()->get_processor_count() is not implemented, or returns something unexpected, or too high negative number
		if (_thread_count <= 0) {
			_thread_count = _thread_fallback_count;
		}

		_threads.resize(_thread_count);

		for (int i = 0; i < _threads.size(); ++i) {
			ThreadPoolContext *context = memnew(ThreadPoolContext);

			context->running = true;
			context->semaphore = memnew(Semaphore);

			context->thread = memnew(Thread());
			context->thread->start(ThreadPool::_worker_thread_func, context);

			_threads.write[i] = context;
		}
	} else {
		call_deferred("register_update");
	}
}

ThreadPool::~ThreadPool() {
	for (int i = 0; i < _threads.size(); ++i) {
		ThreadPoolContext *context = _threads.get(i);

		context->running = false;
		context->semaphore->post();
	}

	for (int i = 0; i < _threads.size(); ++i) {
		ThreadPoolContext *context = _threads.get(i);

		context->thread->wait_to_finish();

		memdelete(context->thread);
		memdelete(context->semaphore);
		context->job.unref();
		memdelete(context);
	}

	_threads.clear();

	_queue.clear();
	//_job_pool.clear();
}

void ThreadPool::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_use_threads"), &ThreadPool::get_use_threads);
	ClassDB::bind_method(D_METHOD("set_use_threads", "value"), &ThreadPool::set_use_threads);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_threads"), "set_use_threads", "get_use_threads");

	ClassDB::bind_method(D_METHOD("get_thread_count"), &ThreadPool::get_thread_count);
	ClassDB::bind_method(D_METHOD("set_thread_count", "value"), &ThreadPool::set_thread_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "thread_count"), "set_thread_count", "get_thread_count");

	ClassDB::bind_method(D_METHOD("get_thread_fallback_count"), &ThreadPool::get_thread_fallback_count);
	ClassDB::bind_method(D_METHOD("set_thread_fallback_count", "value"), &ThreadPool::set_thread_fallback_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "thread_fallback_count"), "set_thread_fallback_count", "get_thread_fallback_count");

	ClassDB::bind_method(D_METHOD("get_max_work_per_frame_percent"), &ThreadPool::get_max_work_per_frame_percent);
	ClassDB::bind_method(D_METHOD("set_max_work_per_frame_percent", "value"), &ThreadPool::set_max_work_per_frame_percent);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_work_per_frame_percent"), "set_max_work_per_frame_percent", "get_max_work_per_frame_percent");

	ClassDB::bind_method(D_METHOD("get_max_time_per_frame"), &ThreadPool::get_max_time_per_frame);
	ClassDB::bind_method(D_METHOD("set_max_time_per_frame", "value"), &ThreadPool::set_max_time_per_frame);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_time_per_frame"), "set_max_time_per_frame", "get_max_time_per_frame");

	ClassDB::bind_method(D_METHOD("has_job", "job"), &ThreadPool::has_job);
	ClassDB::bind_method(D_METHOD("add_job", "job"), &ThreadPool::add_job);

	ClassDB::bind_method(D_METHOD("cancel_job", "job"), &ThreadPool::cancel_job);
	ClassDB::bind_method(D_METHOD("cancel_job_wait", "job"), &ThreadPool::cancel_job_wait);

	ClassDB::bind_method(D_METHOD("register_update"), &ThreadPool::register_update);
	ClassDB::bind_method(D_METHOD("update"), &ThreadPool::update);
}
