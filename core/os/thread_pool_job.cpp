/*************************************************************************/
/*  thread_pool_job.cpp                                                  */
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

#include "thread_pool_job.h"

#include "core/variant/variant.h"

#include "core/os/os.h"

bool ThreadPoolJob::get_complete() const {
	return _complete;
}
void ThreadPoolJob::set_complete(const bool value) {
	bool call_signal = false;
	if (!_complete && value) {
		call_signal = true;
	}

	_complete = value;

	if (call_signal) {
		call_deferred("emit_signal", "completed");
	}
}

bool ThreadPoolJob::get_cancelled() const {
	return _cancelled;
}
void ThreadPoolJob::set_cancelled(const bool value) {
	_cancelled = value;
}

float ThreadPoolJob::get_max_allocated_time() const {
	return _max_allocated_time;
}
void ThreadPoolJob::set_max_allocated_time(const float value) {
	_max_allocated_time = value;
}

int ThreadPoolJob::get_start_time() const {
	return _start_time;
}
void ThreadPoolJob::set_start_time(const int value) {
	_start_time = value;
}

int ThreadPoolJob::get_current_run_stage() const {
	return _current_run_stage;
}
void ThreadPoolJob::set_current_run_stage(const int value) {
	_current_run_stage = value;
}

int ThreadPoolJob::get_stage() const {
	return _stage;
}
void ThreadPoolJob::set_stage(const int value) {
	_stage = value;
}

void ThreadPoolJob::reset_stages() {
	_current_run_stage = 0;
	_stage = 0;
}

Variant ThreadPoolJob::get_object() const {
	return _object;
}
void ThreadPoolJob::set_object(const Variant &value) {
	_object = value;
}

StringName ThreadPoolJob::get_method() const {
	return _method;
}
void ThreadPoolJob::set_method(const StringName &value) {
	_method = value;
}

float ThreadPoolJob::get_current_execution_time() {
	return (OS::get_singleton()->get_system_time_msecs() - _start_time) / 1000.0;
}

bool ThreadPoolJob::should_do(const bool just_check) {
	if (just_check) {
		return _current_run_stage == _stage;
	}

	if (_current_run_stage < _stage) {
		++_current_run_stage;
		return false;
	}

	++_current_run_stage;
	++_stage;

	return true;
}
bool ThreadPoolJob::should_return() {
	if (_cancelled) {
		return true;
	}

	if (_max_allocated_time < 0.00001) {
		return false;
	}

	return get_current_execution_time() >= _max_allocated_time;
}

void ThreadPoolJob::execute() {
	_current_run_stage = 0;

	_start_time = OS::get_singleton()->get_system_time_msecs();

	call("_execute");
}

void ThreadPoolJob::_execute() {
	set_complete(true);
}

ThreadPoolJob::ThreadPoolJob() {
	_complete = true;
	_cancelled = false;

	_max_allocated_time = 0;
	_start_time = 0;

	_current_run_stage = 0;
	_stage = 0;

	_object = NULL;

	_argcount = 0;

	_argptr = memnew_arr(Variant, 5);
}
ThreadPoolJob::~ThreadPoolJob() {
	memdelete_arr(_argptr);
}

void ThreadPoolJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_complete"), &ThreadPoolJob::get_complete);
	ClassDB::bind_method(D_METHOD("set_complete", "value"), &ThreadPoolJob::set_complete);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "complete"), "set_complete", "get_complete");

	ClassDB::bind_method(D_METHOD("get_cancelled"), &ThreadPoolJob::get_cancelled);
	ClassDB::bind_method(D_METHOD("set_cancelled", "value"), &ThreadPoolJob::set_cancelled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cancelled"), "set_cancelled", "get_cancelled");

	ClassDB::bind_method(D_METHOD("get_max_allocated_time"), &ThreadPoolJob::get_max_allocated_time);
	ClassDB::bind_method(D_METHOD("set_max_allocated_time", "value"), &ThreadPoolJob::set_max_allocated_time);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_allocated_time"), "set_max_allocated_time", "get_max_allocated_time");

	ClassDB::bind_method(D_METHOD("get_start_time"), &ThreadPoolJob::get_start_time);
	ClassDB::bind_method(D_METHOD("set_start_time", "value"), &ThreadPoolJob::set_start_time);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "start_time"), "set_start_time", "get_start_time");

	ClassDB::bind_method(D_METHOD("get_current_run_stage"), &ThreadPoolJob::get_current_run_stage);
	ClassDB::bind_method(D_METHOD("set_current_run_stage", "value"), &ThreadPoolJob::set_current_run_stage);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_run_stage"), "set_current_run_stage", "get_current_run_stage");

	ClassDB::bind_method(D_METHOD("get_stage"), &ThreadPoolJob::get_stage);
	ClassDB::bind_method(D_METHOD("set_stage", "value"), &ThreadPoolJob::set_stage);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stage"), "set_stage", "get_stage");

	ClassDB::bind_method(D_METHOD("reset_stages"), &ThreadPoolJob::reset_stages);

	ClassDB::bind_method(D_METHOD("get_current_execution_time"), &ThreadPoolJob::get_current_execution_time);

	ClassDB::bind_method(D_METHOD("should_do", "just_check"), &ThreadPoolJob::should_do, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("should_return"), &ThreadPoolJob::should_return);

	BIND_VMETHOD(MethodInfo("_execute"));

	ClassDB::bind_method(D_METHOD("execute"), &ThreadPoolJob::execute);
	ClassDB::bind_method(D_METHOD("_execute"), &ThreadPoolJob::_execute);

	ADD_SIGNAL(MethodInfo("completed"));
}
