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

#include "prop_mesher_job_step.h"

#include "modules/modules_enabled.gen.h"

const String PropMesherJobStep::BINDING_STRING_PROP_MESHER_JOB_STEP_TYPE = "Normal,Normal LOD,Drop UV2,Merge Verts,Bake Texture,Simplify Mesh";

PropMesherJobStep::PropMesherJobStepType PropMesherJobStep::get_job_type() const {
	return _job_type;
}
void PropMesherJobStep::set_job_type(const PropMesherJobStep::PropMesherJobStepType value) {
	_job_type = value;
}

int PropMesherJobStep::get_lod_index() const {
	return _lod_index;
}
void PropMesherJobStep::set_lod_index(const int value) {
	_lod_index = value;
}

#ifdef MODULE_MESH_UTILS_ENABLED
Ref<FastQuadraticMeshSimplifier> PropMesherJobStep::get_fqms() {
	return _fqms;
}
void PropMesherJobStep::set_fqms(const Ref<FastQuadraticMeshSimplifier> &val) {
	_fqms = val;
}

float PropMesherJobStep::get_simplification_step_ratio() const {
	return _simplification_step_ratio;
}
void PropMesherJobStep::set_simplification_step_ratio(const float value) {
	_simplification_step_ratio = value;
}

int PropMesherJobStep::get_simplification_steps() const {
	return _simplification_steps;
}
void PropMesherJobStep::set_simplification_steps(const int value) {
	_simplification_steps = value;
}

float PropMesherJobStep::get_simplification_agressiveness() const {
	return _simplification_agressiveness;
}
void PropMesherJobStep::set_simplification_agressiveness(const float value) {
	_simplification_agressiveness = value;
}

#endif

PropMesherJobStep::PropMesherJobStep() {
	_job_type = TYPE_NORMAL;
	_lod_index = 0;

#ifdef MODULE_MESH_UTILS_ENABLED
	_simplification_step_ratio = 0.8;
	_simplification_steps = 2;
	_simplification_agressiveness = 7;
#endif
}

PropMesherJobStep::~PropMesherJobStep() {
#ifdef MODULE_MESH_UTILS_ENABLED
	_fqms.unref();
#endif
}

void PropMesherJobStep::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_job_type"), &PropMesherJobStep::get_job_type);
	ClassDB::bind_method(D_METHOD("set_job_type", "value"), &PropMesherJobStep::set_job_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "job_type", PROPERTY_HINT_ENUM, PropMesherJobStep::BINDING_STRING_PROP_MESHER_JOB_STEP_TYPE), "set_job_type", "get_job_type");

	ClassDB::bind_method(D_METHOD("get_lod_index"), &PropMesherJobStep::get_lod_index);
	ClassDB::bind_method(D_METHOD("set_lod_index", "value"), &PropMesherJobStep::set_lod_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_index"), "set_lod_index", "get_lod_index");

#ifdef MODULE_MESH_UTILS_ENABLED
	ClassDB::bind_method(D_METHOD("get_fqms"), &PropMesherJobStep::get_fqms);
	ClassDB::bind_method(D_METHOD("set_fqms", "value"), &PropMesherJobStep::set_fqms);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fqms", PROPERTY_HINT_RESOURCE_TYPE, "FastQuadraticMeshSimplifier"), "set_fqms", "get_fqms");

	ClassDB::bind_method(D_METHOD("get_simplification_step_ratio"), &PropMesherJobStep::get_simplification_step_ratio);
	ClassDB::bind_method(D_METHOD("set_simplification_step_ratio", "value"), &PropMesherJobStep::set_simplification_step_ratio);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "simplification_step_ratio"), "set_simplification_step_ratio", "get_simplification_step_ratio");

	ClassDB::bind_method(D_METHOD("get_simplification_steps"), &PropMesherJobStep::get_simplification_steps);
	ClassDB::bind_method(D_METHOD("set_simplification_steps", "value"), &PropMesherJobStep::set_simplification_steps);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "simplification_steps"), "set_simplification_steps", "get_simplification_steps");

	ClassDB::bind_method(D_METHOD("get_simplification_agressiveness"), &PropMesherJobStep::get_simplification_agressiveness);
	ClassDB::bind_method(D_METHOD("set_simplification_agressiveness", "value"), &PropMesherJobStep::set_simplification_agressiveness);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "simplification_agressiveness"), "set_simplification_agressiveness", "get_simplification_agressiveness");
#endif

	BIND_ENUM_CONSTANT(TYPE_NORMAL);
	BIND_ENUM_CONSTANT(TYPE_NORMAL_LOD);
	BIND_ENUM_CONSTANT(TYPE_DROP_UV2);
	BIND_ENUM_CONSTANT(TYPE_MERGE_VERTS);
	BIND_ENUM_CONSTANT(TYPE_BAKE_TEXTURE);
	BIND_ENUM_CONSTANT(TYPE_SIMPLIFY_MESH);
	BIND_ENUM_CONSTANT(TYPE_OTHER);
}
