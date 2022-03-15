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

#include "prop_2d_texture_job.h"

#if TEXTURE_PACKER_PRESENT
#include "../../texture_packer/texture_packer.h"
#endif

#if TEXTURE_PACKER_PRESENT
Ref<TexturePacker> Prop2DTextureJob::get_merger() {
	return _merger;
}

void Prop2DTextureJob::set_merger(const Ref<TexturePacker> &merger) {
	_merger = merger;
}
#endif

void Prop2DTextureJob::_execute() {
#if TEXTURE_PACKER_PRESENT
	if (!_merger.is_valid()) {
		set_complete(true);
		return;
	}

	_merger->merge();
#endif

	set_complete(true);
}

Prop2DTextureJob::Prop2DTextureJob() {
#if !THREAD_POOL_PRESENT
	_complete = true;
	_cancelled = false;

	_max_allocated_time = 0;
	_start_time = 0;

	_current_run_stage = 0;
	_stage = 0;
#endif
}

Prop2DTextureJob::~Prop2DTextureJob() {
}

void Prop2DTextureJob::_bind_methods() {
#if TEXTURE_PACKER_PRESENT
	ClassDB::bind_method(D_METHOD("get_merger"), &Prop2DTextureJob::get_merger);
	ClassDB::bind_method(D_METHOD("set_merger", "value"), &Prop2DTextureJob::set_merger);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "merger", PROPERTY_HINT_RESOURCE_TYPE, "TexturePacker"), "set_merger", "get_merger");
#endif

	ClassDB::bind_method(D_METHOD("_execute"), &Prop2DTextureJob::_execute);

#if !THREAD_POOL_PRESENT
	ClassDB::bind_method(D_METHOD("get_complete"), &Prop2DTextureJob::get_complete);
	ClassDB::bind_method(D_METHOD("set_complete", "value"), &Prop2DTextureJob::set_complete);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "complete"), "set_complete", "get_complete");

	ClassDB::bind_method(D_METHOD("get_start_time"), &Prop2DTextureJob::get_start_time);
	ClassDB::bind_method(D_METHOD("set_start_time", "value"), &Prop2DTextureJob::set_start_time);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "start_time"), "set_start_time", "get_start_time");

	ClassDB::bind_method(D_METHOD("get_current_run_stage"), &Prop2DTextureJob::get_current_run_stage);
	ClassDB::bind_method(D_METHOD("set_current_run_stage", "value"), &Prop2DTextureJob::set_current_run_stage);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_run_stage"), "set_current_run_stage", "get_current_run_stage");

	ClassDB::bind_method(D_METHOD("get_stage"), &Prop2DTextureJob::get_stage);
	ClassDB::bind_method(D_METHOD("set_stage", "value"), &Prop2DTextureJob::set_stage);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stage"), "set_stage", "get_stage");

	ClassDB::bind_method(D_METHOD("get_current_execution_time"), &Prop2DTextureJob::get_current_execution_time);

	ClassDB::bind_method(D_METHOD("should_do", "just_check"), &Prop2DTextureJob::should_do, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("should_return"), &Prop2DTextureJob::should_return);

	BIND_VMETHOD(MethodInfo("_execute"));
	ClassDB::bind_method(D_METHOD("execute"), &Prop2DTextureJob::execute);

	ADD_SIGNAL(MethodInfo("completed"));
#endif
}

#if !THREAD_POOL_PRESENT
bool Prop2DTextureJob::get_complete() const {
	return _complete;
}
void Prop2DTextureJob::set_complete(const bool value) {
	_complete = value;
}

bool Prop2DTextureJob::get_cancelled() const {
	return _cancelled;
}
void Prop2DTextureJob::set_cancelled(const bool value) {
	_cancelled = value;
}

float Prop2DTextureJob::get_max_allocated_time() const {
	return _max_allocated_time;
}
void Prop2DTextureJob::set_max_allocated_time(const float value) {
	_max_allocated_time = value;
}

int Prop2DTextureJob::get_start_time() const {
	return _start_time;
}
void Prop2DTextureJob::set_start_time(const int value) {
	_start_time = value;
}

int Prop2DTextureJob::get_current_run_stage() const {
	return _current_run_stage;
}
void Prop2DTextureJob::set_current_run_stage(const int value) {
	_current_run_stage = value;
}

int Prop2DTextureJob::get_stage() const {
	return _stage;
}
void Prop2DTextureJob::set_stage(const int value) {
	_stage = value;
}

void Prop2DTextureJob::reset_stages() {
	_current_run_stage = 0;
	_stage = 0;
}

float Prop2DTextureJob::get_current_execution_time() {
	return 0;
}

bool Prop2DTextureJob::should_do(const bool just_check) {
	return true;
}
bool Prop2DTextureJob::should_return() {
	if (_cancelled)
		return true;

	return false;
}

void Prop2DTextureJob::execute() {
	ERR_FAIL_COND(!has_method("_execute"));

	call("_execute");
}

#endif