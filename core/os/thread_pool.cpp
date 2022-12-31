#include "thread_pool.h"

/*

Copyright (c) 2020-2023 Péter Magyar

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
	// Will be applied later in update, so current jobs can be finished first
	_use_threads_new = value;
	_dirty = true;
}

int ThreadPool::get_thread_count() const {
	return _thread_count;
}
void ThreadPool::set_thread_count(const int value) {
	_thread_count = value;
	_dirty = true;
}

int ThreadPool::get_thread_fallback_count() const {
	return _thread_fallback_count;
}
void ThreadPool::set_thread_fallback_count(const int value) {
	_thread_fallback_count = value;
	_dirty = true;
}

float ThreadPool::get_max_time_per_frame() const {
	return _max_time_per_frame;
}
void ThreadPool::set_max_time_per_frame(const float value) {
	_max_time_per_frame = value;
}

float ThreadPool::get_max_work_per_frame_percent() const {
	return _max_work_per_frame_percent;
}
void ThreadPool::set_max_work_per_frame_percent(const float value) {
	_max_work_per_frame_percent = value;

	apply_max_work_per_frame_percent();
}

float ThreadPool::get_target_fps() const {
	return _target_fps;
}
void ThreadPool::set_target_fps(const float value) {
	_target_fps = value;

	apply_max_work_per_frame_percent();
}

void ThreadPool::apply_max_work_per_frame_percent() {
	_max_time_per_frame = (1.0 / _target_fps) * (_max_work_per_frame_percent / 100.0);
}

bool ThreadPool::is_working() const {
	_THREAD_SAFE_LOCK_

	if (_queue.size() > 0) {
		_THREAD_SAFE_UNLOCK_

		return true;
	}

	for (int i = 0; i < _threads.size(); ++i) {
		if (_threads[i]->job.is_valid()) {
			_THREAD_SAFE_UNLOCK_
			return true;
		}
	}

	_THREAD_SAFE_UNLOCK_

	return false;
}

bool ThreadPool::is_working_no_lock() const {
	if (_queue.size() > 0) {
		return true;
	}

	for (int i = 0; i < _threads.size(); ++i) {
		if (_threads[i]->job.is_valid()) {
			return true;
		}
	}

	return false;
}

bool ThreadPool::has_job(const Ref<ThreadPoolJob> &job) {
	_THREAD_SAFE_LOCK_

	for (int i = 0; i < _threads.size(); ++i) {
		ThreadPoolContext *context = _threads.get(i);

		if (context->job == job) {
			_THREAD_SAFE_UNLOCK_
			return true;
		}
	}

	List<Ref<ThreadPoolJob>>::Element *E = _queue.find(job);

	_THREAD_SAFE_UNLOCK_

	return E;
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

	_queue.push_back(job);

	_THREAD_SAFE_UNLOCK_
}

void ThreadPool::cancel_job(Ref<ThreadPoolJob> job) {
	ERR_FAIL_COND(!job.is_valid());

	job->set_cancelled(true);

	_THREAD_SAFE_LOCK_

	_queue.erase(job);

	_THREAD_SAFE_UNLOCK_
}

void ThreadPool::cancel_job_wait(Ref<ThreadPoolJob> job) {
	ERR_FAIL_COND(!job.is_valid());

	job->set_cancelled(true);

	_THREAD_SAFE_LOCK_

	if (_queue.erase(job)) {
		_THREAD_SAFE_UNLOCK_
		return;
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

	while (_queue.size() > 0 && !context->job.is_valid()) {
		context->job = _queue.front()->get();
		_queue.pop_front();
	}

	if (context->job.is_valid()) {
		context->semaphore->post();
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

void ThreadPool::update() {
	if (_dirty) {
		apply_settings();
	}

	if (_use_threads) {
		return;
	}

	if (_queue.size() == 0) {
		return;
	}

	float remaining_time = _max_time_per_frame;

	while (remaining_time > 0 && _queue.size() > 0) {
		Ref<ThreadPoolJob> job = _queue.front()->get();

		if (!job.is_valid()) {
			_queue.pop_front();
			continue;
		}

		job->set_max_allocated_time(remaining_time);
		job->execute();

		remaining_time -= job->get_current_execution_time();

		if (job->get_complete() || job->get_cancelled()) {
			_queue.pop_front();
		}
	}
}

void ThreadPool::register_core_settings() {
	_use_threads = GLOBAL_DEF("thread_pool/use_threads", true);
	_thread_count = GLOBAL_DEF("thread_pool/thread_count", -1);
	_thread_fallback_count = GLOBAL_DEF("thread_pool/thread_fallback_count", 4);

	if (_thread_fallback_count <= 0) {
		print_error("ThreadPool: thread_fallback_count is invalid! Check ProjectSettings/ThreadPool/thread_fallback_count! Needs to be > 0! Set to 1!");

		_thread_fallback_count = 1;
	}

	_target_fps = GLOBAL_DEF("thread_pool/target_fps", 60);
	//Todo Add help text, as this will only come into play if threading is disabled, or not available
	_max_work_per_frame_percent = GLOBAL_DEF("thread_pool/max_work_per_frame_percent", 25);

	apply_max_work_per_frame_percent();

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
	}

	_use_threads_new = _use_threads;

	_dirty = true;

	apply_settings();
}

void ThreadPool::apply_settings() {
	if (!_dirty) {
		return;
	}

	_THREAD_SAFE_LOCK_

	if (is_working_no_lock()) {
		_THREAD_SAFE_UNLOCK_
		return;
	}

	_dirty = false;

	for (int i = 0; i < _threads.size(); ++i) {
		ThreadPoolContext *context = _threads[i];

		CRASH_COND(context->job.is_valid());

		context->running = false;
		context->semaphore->post();
		context->thread->wait_to_finish();
		memdelete(context->thread);
		memdelete(context->semaphore);
		memdelete(context);
	}

	_threads.resize(0);

	_use_threads = _use_threads_new;

	if (_use_threads) {
		_threads.resize(_thread_count);

		for (int i = 0; i < _threads.size(); ++i) {
			ThreadPoolContext *context = memnew(ThreadPoolContext);

			context->running = true;
			context->semaphore = memnew(Semaphore);

			context->thread = memnew(Thread());
			context->thread->start(ThreadPool::_worker_thread_func, context);

			_threads.write[i] = context;
		}
	}

	_THREAD_SAFE_UNLOCK_
}

ThreadPool::ThreadPool() {
	_instance = this;
	_dirty = false;
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

	ClassDB::bind_method(D_METHOD("get_max_time_per_frame"), &ThreadPool::get_max_time_per_frame);
	ClassDB::bind_method(D_METHOD("set_max_time_per_frame", "value"), &ThreadPool::set_max_time_per_frame);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_time_per_frame"), "set_max_time_per_frame", "get_max_time_per_frame");

	ClassDB::bind_method(D_METHOD("get_max_work_per_frame_percent"), &ThreadPool::get_max_work_per_frame_percent);
	ClassDB::bind_method(D_METHOD("set_max_work_per_frame_percent", "value"), &ThreadPool::set_max_work_per_frame_percent);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_work_per_frame_percent"), "set_max_work_per_frame_percent", "get_max_work_per_frame_percent");

	ClassDB::bind_method(D_METHOD("get_target_fps"), &ThreadPool::get_target_fps);
	ClassDB::bind_method(D_METHOD("set_target_fps", "value"), &ThreadPool::set_target_fps);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "target_fps"), "set_target_fps", "get_target_fps");

	ClassDB::bind_method(D_METHOD("apply_max_work_per_frame_percent"), &ThreadPool::apply_max_work_per_frame_percent);

	ClassDB::bind_method(D_METHOD("is_working"), &ThreadPool::is_working);
	ClassDB::bind_method(D_METHOD("is_working_no_lock"), &ThreadPool::is_working_no_lock);

	ClassDB::bind_method(D_METHOD("has_job", "job"), &ThreadPool::has_job);
	ClassDB::bind_method(D_METHOD("add_job", "job"), &ThreadPool::add_job);

	ClassDB::bind_method(D_METHOD("cancel_job", "job"), &ThreadPool::cancel_job);
	ClassDB::bind_method(D_METHOD("cancel_job_wait", "job"), &ThreadPool::cancel_job_wait);

	ClassDB::bind_method(D_METHOD("update"), &ThreadPool::update);
}
