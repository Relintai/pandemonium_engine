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

#include "prop_instance_job.h"

#include "prop_instance.h"

#include "./props/prop_data.h"

#include "../opensimplex/open_simplex_noise.h"

const String PropInstanceJob::BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE = "Normal,Process,Physics Process";

PropInstanceJob::ActiveBuildPhaseType PropInstanceJob::get_build_phase_type() {
	return _build_phase_type;
}
void PropInstanceJob::set_build_phase_type(PropInstanceJob::ActiveBuildPhaseType build_phase_type) {
	_build_phase_type = build_phase_type;
}

void PropInstanceJob::set_prop(const Ref<PropData> &prop) {
	_prop = prop;

	_in_tree = true;
}

void PropInstanceJob::set_prop_instance(PropInstance *instance) {
	_instance = instance;
}

void PropInstanceJob::set_prop_instance_bind(Node *instance) {
	set_prop_instance(Object::cast_to<PropInstance>(instance));
}

int PropInstanceJob::get_phase() {
	return _phase;
}
void PropInstanceJob::set_phase(const int phase) {
	_phase = phase;
}
void PropInstanceJob::next_phase() {
	++_phase;
}

bool PropInstanceJob::get_build_done() {
	return _build_done;
}
void PropInstanceJob::set_build_done(const bool val) {
	_build_done = val;
}

void PropInstanceJob::finished() {
	set_build_done(true);

	if (_instance) {
		_instance->build_finished();
	}
}

void PropInstanceJob::reset() {
	call("_reset");
}
void PropInstanceJob::_reset() {
	_build_done = false;
	_phase = 0;
}

void PropInstanceJob::_execute() {
	ActiveBuildPhaseType origpt = _build_phase_type;

	while (!get_cancelled() && _in_tree && !_build_done && origpt == _build_phase_type && !should_return()) {
		execute_phase();
	}
}

void PropInstanceJob::execute_phase() {
	call("_execute_phase");
}

void PropInstanceJob::_execute_phase() {
	finished();
}

void PropInstanceJob::process(const float delta) {
	if (has_method("_process"))
		call("_process", delta);
}
void PropInstanceJob::physics_process(const float delta) {
	if (has_method("_physics_process"))
		call("_physics_process", delta);
}

void PropInstanceJob::prop_instance_enter_tree() {
	_in_tree = true;
}

void PropInstanceJob::prop_instance_exit_tree() {
	_in_tree = false;

	if (get_complete()) {
		_prop.unref();
	} else {
		set_cancelled(true);
	}
}

PropInstanceJob::PropInstanceJob() {
	_instance = NULL;
	_in_tree = false;

	_build_phase_type = BUILD_PHASE_TYPE_NORMAL;
	_build_done = true;
	_phase = 0;

#if !THREAD_POOL_PRESENT
	_complete = true;
	_cancelled = false;

	_max_allocated_time = 0;
	_start_time = 0;

	_current_run_stage = 0;
	_stage = 0;
#endif
}

PropInstanceJob::~PropInstanceJob() {
	_prop.unref();
	_instance = NULL;
}

void PropInstanceJob::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_process", PropertyInfo(Variant::REAL, "delta")));
	BIND_VMETHOD(MethodInfo("_physics_process", PropertyInfo(Variant::REAL, "delta")));

	ClassDB::bind_method(D_METHOD("get_build_phase_type"), &PropInstanceJob::get_build_phase_type);
	ClassDB::bind_method(D_METHOD("set_build_phase_type", "value"), &PropInstanceJob::set_build_phase_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_phase_type", PROPERTY_HINT_ENUM, BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE), "set_build_phase_type", "get_build_phase_type");

	ClassDB::bind_method(D_METHOD("set_prop", "prop"), &PropInstanceJob::set_prop);
	ClassDB::bind_method(D_METHOD("set_prop_instance", "instance"), &PropInstanceJob::set_prop_instance_bind);

	ClassDB::bind_method(D_METHOD("get_phase"), &PropInstanceJob::get_phase);
	ClassDB::bind_method(D_METHOD("set_phase", "phase"), &PropInstanceJob::set_phase);
	ClassDB::bind_method(D_METHOD("next_phase"), &PropInstanceJob::next_phase);

	ClassDB::bind_method(D_METHOD("get_build_done"), &PropInstanceJob::get_build_done);
	ClassDB::bind_method(D_METHOD("set_build_done", "val"), &PropInstanceJob::set_build_done);

	ClassDB::bind_method(D_METHOD("finished"), &PropInstanceJob::finished);

	BIND_VMETHOD(MethodInfo("_reset"));

	ClassDB::bind_method(D_METHOD("reset"), &PropInstanceJob::reset);
	ClassDB::bind_method(D_METHOD("_reset"), &PropInstanceJob::_reset);

	ClassDB::bind_method(D_METHOD("_execute"), &PropInstanceJob::_execute);

	BIND_VMETHOD(MethodInfo("_execute_phase"));

	ClassDB::bind_method(D_METHOD("execute_phase"), &PropInstanceJob::execute_phase);
	ClassDB::bind_method(D_METHOD("_execute_phase"), &PropInstanceJob::_execute_phase);

	ClassDB::bind_method(D_METHOD("prop_instance_exit_tree"), &PropInstanceJob::prop_instance_exit_tree);

#if !THREAD_POOL_PRESENT
	ClassDB::bind_method(D_METHOD("get_complete"), &PropInstanceJob::get_complete);
	ClassDB::bind_method(D_METHOD("set_complete", "value"), &PropInstanceJob::set_complete);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "complete"), "set_complete", "get_complete");

	ClassDB::bind_method(D_METHOD("get_start_time"), &PropInstanceJob::get_start_time);
	ClassDB::bind_method(D_METHOD("set_start_time", "value"), &PropInstanceJob::set_start_time);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "start_time"), "set_start_time", "get_start_time");

	ClassDB::bind_method(D_METHOD("get_current_run_stage"), &PropInstanceJob::get_current_run_stage);
	ClassDB::bind_method(D_METHOD("set_current_run_stage", "value"), &PropInstanceJob::set_current_run_stage);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_run_stage"), "set_current_run_stage", "get_current_run_stage");

	ClassDB::bind_method(D_METHOD("get_stage"), &PropInstanceJob::get_stage);
	ClassDB::bind_method(D_METHOD("set_stage", "value"), &PropInstanceJob::set_stage);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stage"), "set_stage", "get_stage");

	ClassDB::bind_method(D_METHOD("get_current_execution_time"), &PropInstanceJob::get_current_execution_time);

	ClassDB::bind_method(D_METHOD("should_do", "just_check"), &PropInstanceJob::should_do, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("should_return"), &PropInstanceJob::should_return);

	BIND_VMETHOD(MethodInfo("_execute"));
	ClassDB::bind_method(D_METHOD("execute"), &PropInstanceJob::execute);

	ADD_SIGNAL(MethodInfo("completed"));
#endif
}

#if !THREAD_POOL_PRESENT
bool PropInstanceJob::get_complete() const {
	return _complete;
}
void PropInstanceJob::set_complete(const bool value) {
	_complete = value;
}

bool PropInstanceJob::get_cancelled() const {
	return _cancelled;
}
void PropInstanceJob::set_cancelled(const bool value) {
	_cancelled = value;
}

float PropInstanceJob::get_max_allocated_time() const {
	return _max_allocated_time;
}
void PropInstanceJob::set_max_allocated_time(const float value) {
	_max_allocated_time = value;
}

int PropInstanceJob::get_start_time() const {
	return _start_time;
}
void PropInstanceJob::set_start_time(const int value) {
	_start_time = value;
}

int PropInstanceJob::get_current_run_stage() const {
	return _current_run_stage;
}
void PropInstanceJob::set_current_run_stage(const int value) {
	_current_run_stage = value;
}

int PropInstanceJob::get_stage() const {
	return _stage;
}
void PropInstanceJob::set_stage(const int value) {
	_stage = value;
}

void PropInstanceJob::reset_stages() {
	_current_run_stage = 0;
	_stage = 0;
}

float PropInstanceJob::get_current_execution_time() {
	return 0;
}

bool PropInstanceJob::should_do(const bool just_check) {
	return true;
}
bool PropInstanceJob::should_return() {
	if (_cancelled)
		return true;

	return false;
}

void PropInstanceJob::execute() {
	ERR_FAIL_COND(!has_method("_execute"));

	call("_execute");
}

#endif
