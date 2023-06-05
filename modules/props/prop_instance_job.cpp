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

	BIND_VMETHOD(MethodInfo("_execute_phase"));

	ClassDB::bind_method(D_METHOD("execute_phase"), &PropInstanceJob::execute_phase);
	ClassDB::bind_method(D_METHOD("_execute_phase"), &PropInstanceJob::_execute_phase);

	ClassDB::bind_method(D_METHOD("prop_instance_exit_tree"), &PropInstanceJob::prop_instance_exit_tree);

	BIND_ENUM_CONSTANT(BUILD_PHASE_TYPE_NORMAL);
	BIND_ENUM_CONSTANT(BUILD_PHASE_TYPE_PROCESS);
	BIND_ENUM_CONSTANT(BUILD_PHASE_TYPE_PHYSICS_PROCESS);
}
