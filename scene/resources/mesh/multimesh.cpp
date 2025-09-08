/*************************************************************************/
/*  multimesh.cpp                                                        */
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

#include "multimesh.h"

#include "servers/rendering_server.h"

void MultiMesh::_set_transform_array(const PoolVector<Vector3> &p_array) {
	if (transform_format != TRANSFORM_3D) {
		return;
	}

	const PoolVector<Vector3> &xforms = p_array;
	int len = xforms.size();
	ERR_FAIL_COND((len / 4) != instance_count);
	if (len == 0) {
		return;
	}

	PoolVector<Vector3>::Read r = xforms.read();

	for (int i = 0; i < len / 4; i++) {
		Transform t;
		t.basis[0] = r[i * 4 + 0];
		t.basis[1] = r[i * 4 + 1];
		t.basis[2] = r[i * 4 + 2];
		t.origin = r[i * 4 + 3];

		set_instance_transform(i, t);
	}
}

PoolVector<Vector3> MultiMesh::_get_transform_array() const {
	if (transform_format != TRANSFORM_3D) {
		return PoolVector<Vector3>();
	}

	if (instance_count == 0) {
		return PoolVector<Vector3>();
	}

	PoolVector<Vector3> xforms;
	xforms.resize(instance_count * 4);

	PoolVector<Vector3>::Write w = xforms.write();

	for (int i = 0; i < instance_count; i++) {
		Transform t = get_instance_transform(i);
		w[i * 4 + 0] = t.basis[0];
		w[i * 4 + 1] = t.basis[1];
		w[i * 4 + 2] = t.basis[2];
		w[i * 4 + 3] = t.origin;
	}

	return xforms;
}

void MultiMesh::_set_transform_2d_array(const PoolVector<Vector2> &p_array) {
	if (transform_format != TRANSFORM_2D) {
		return;
	}

	const PoolVector<Vector2> &xforms = p_array;
	int len = xforms.size();
	ERR_FAIL_COND((len / 3) != instance_count);
	if (len == 0) {
		return;
	}

	PoolVector<Vector2>::Read r = xforms.read();

	for (int i = 0; i < len / 3; i++) {
		Transform2D t;
		t.columns[0] = r[i * 3 + 0];
		t.columns[1] = r[i * 3 + 1];
		t.columns[2] = r[i * 3 + 2];

		set_instance_transform_2d(i, t);
	}
}

PoolVector<Vector2> MultiMesh::_get_transform_2d_array() const {
	if (transform_format != TRANSFORM_2D) {
		return PoolVector<Vector2>();
	}

	if (instance_count == 0) {
		return PoolVector<Vector2>();
	}

	PoolVector<Vector2> xforms;
	xforms.resize(instance_count * 3);

	PoolVector<Vector2>::Write w = xforms.write();

	for (int i = 0; i < instance_count; i++) {
		Transform2D t = get_instance_transform_2d(i);
		w[i * 3 + 0] = t.columns[0];
		w[i * 3 + 1] = t.columns[1];
		w[i * 3 + 2] = t.columns[2];
	}

	return xforms;
}

void MultiMesh::_set_color_array(const PoolVector<Color> &p_array) {
	const PoolVector<Color> &colors = p_array;
	int len = colors.size();
	if (len == 0) {
		return;
	}
	ERR_FAIL_COND(len != instance_count);

	PoolVector<Color>::Read r = colors.read();

	for (int i = 0; i < len; i++) {
		set_instance_color(i, r[i]);
	}
}

PoolVector<Color> MultiMesh::_get_color_array() const {
	if (instance_count == 0 || color_format == COLOR_NONE) {
		return PoolVector<Color>();
	}

	PoolVector<Color> colors;
	colors.resize(instance_count);

	for (int i = 0; i < instance_count; i++) {
		colors.set(i, get_instance_color(i));
	}

	return colors;
}

void MultiMesh::_set_custom_data_array(const PoolVector<Color> &p_array) {
	const PoolVector<Color> &custom_datas = p_array;
	int len = custom_datas.size();
	if (len == 0) {
		return;
	}
	ERR_FAIL_COND(len != instance_count);

	PoolVector<Color>::Read r = custom_datas.read();

	for (int i = 0; i < len; i++) {
		set_instance_custom_data(i, r[i]);
	}
}

PoolVector<Color> MultiMesh::_get_custom_data_array() const {
	if (instance_count == 0 || custom_data_format == CUSTOM_DATA_NONE) {
		return PoolVector<Color>();
	}

	PoolVector<Color> custom_datas;
	custom_datas.resize(instance_count);

	for (int i = 0; i < instance_count; i++) {
		custom_datas.set(i, get_instance_custom_data(i));
	}

	return custom_datas;
}
void MultiMesh::set_mesh(const Ref<Mesh> &p_mesh) {
	mesh = p_mesh;
	if (!mesh.is_null()) {
		RenderingServer::get_singleton()->multimesh_set_mesh(multimesh, mesh->get_rid());
	} else {
		RenderingServer::get_singleton()->multimesh_set_mesh(multimesh, RID());
	}
}

Ref<Mesh> MultiMesh::get_mesh() const {
	return mesh;
}

void MultiMesh::set_instance_count(int p_count) {
	ERR_FAIL_COND(p_count < 0);
	RenderingServer::get_singleton()->multimesh_allocate(multimesh, p_count, RS::MultimeshTransformFormat(transform_format), RS::MultimeshColorFormat(color_format), RS::MultimeshCustomDataFormat(custom_data_format));
	instance_count = p_count;
}
int MultiMesh::get_instance_count() const {
	return instance_count;
}

void MultiMesh::set_visible_instance_count(int p_count) {
	ERR_FAIL_COND(p_count < -1);
	RenderingServer::get_singleton()->multimesh_set_visible_instances(multimesh, p_count);
	visible_instance_count = p_count;
}

void MultiMesh::set_physics_interpolation_quality(PhysicsInterpolationQuality p_quality) {
	_physics_interpolation_quality = p_quality;
	RenderingServer::get_singleton()->multimesh_set_physics_interpolation_quality(multimesh, (RS::MultimeshPhysicsInterpolationQuality)p_quality);
}

int MultiMesh::get_visible_instance_count() const {
	return visible_instance_count;
}

void MultiMesh::reset_instance_physics_interpolation(int p_instance) {
	RenderingServer::get_singleton()->multimesh_instance_reset_physics_interpolation(multimesh, p_instance);
}

void MultiMesh::reset_instances_physics_interpolation() {
	RenderingServer::get_singleton()->multimesh_instances_reset_physics_interpolation(multimesh);
}

void MultiMesh::set_instance_transform(int p_instance, const Transform &p_transform) {
	RenderingServer::get_singleton()->multimesh_instance_set_transform(multimesh, p_instance, p_transform);
}

void MultiMesh::set_instance_transform_2d(int p_instance, const Transform2D &p_transform) {
	RenderingServer::get_singleton()->multimesh_instance_set_transform_2d(multimesh, p_instance, p_transform);
	emit_changed();
}

Transform MultiMesh::get_instance_transform(int p_instance) const {
	return RenderingServer::get_singleton()->multimesh_instance_get_transform(multimesh, p_instance);
}

Transform2D MultiMesh::get_instance_transform_2d(int p_instance) const {
	return RenderingServer::get_singleton()->multimesh_instance_get_transform_2d(multimesh, p_instance);
}

void MultiMesh::set_instance_color(int p_instance, const Color &p_color) {
	RenderingServer::get_singleton()->multimesh_instance_set_color(multimesh, p_instance, p_color);
}
Color MultiMesh::get_instance_color(int p_instance) const {
	return RenderingServer::get_singleton()->multimesh_instance_get_color(multimesh, p_instance);
}

void MultiMesh::set_instance_custom_data(int p_instance, const Color &p_custom_data) {
	RenderingServer::get_singleton()->multimesh_instance_set_custom_data(multimesh, p_instance, p_custom_data);
}
Color MultiMesh::get_instance_custom_data(int p_instance) const {
	return RenderingServer::get_singleton()->multimesh_instance_get_custom_data(multimesh, p_instance);
}

void MultiMesh::set_as_bulk_array(const PoolVector<float> &p_array) {
	RenderingServer::get_singleton()->multimesh_set_as_bulk_array(multimesh, p_array);
}

void MultiMesh::set_as_bulk_array_interpolated(const PoolVector<float> &p_array_curr, const PoolVector<float> &p_array_prev) {
	RenderingServer::get_singleton()->multimesh_set_as_bulk_array_interpolated(multimesh, p_array_curr, p_array_prev);
}

void MultiMesh::set_physics_interpolated(bool p_interpolated) {
	RenderingServer::get_singleton()->multimesh_set_physics_interpolated(multimesh, p_interpolated);
}

AABB MultiMesh::get_aabb() const {
	return RenderingServer::get_singleton()->multimesh_get_aabb(multimesh);
}

RID MultiMesh::get_rid() const {
	return multimesh;
}

void MultiMesh::set_color_format(ColorFormat p_color_format) {
	ERR_FAIL_COND(instance_count > 0);
	color_format = p_color_format;
}

MultiMesh::ColorFormat MultiMesh::get_color_format() const {
	return color_format;
}

void MultiMesh::set_custom_data_format(CustomDataFormat p_custom_data_format) {
	ERR_FAIL_COND(instance_count > 0);
	custom_data_format = p_custom_data_format;
}

MultiMesh::CustomDataFormat MultiMesh::get_custom_data_format() const {
	return custom_data_format;
}

void MultiMesh::set_transform_format(TransformFormat p_transform_format) {
	ERR_FAIL_COND(instance_count > 0);
	transform_format = p_transform_format;
}
MultiMesh::TransformFormat MultiMesh::get_transform_format() const {
	return transform_format;
}

void MultiMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &MultiMesh::set_mesh);
	ClassDB::bind_method(D_METHOD("get_mesh"), &MultiMesh::get_mesh);
	ClassDB::bind_method(D_METHOD("set_color_format", "format"), &MultiMesh::set_color_format);
	ClassDB::bind_method(D_METHOD("get_color_format"), &MultiMesh::get_color_format);
	ClassDB::bind_method(D_METHOD("set_custom_data_format", "format"), &MultiMesh::set_custom_data_format);
	ClassDB::bind_method(D_METHOD("get_custom_data_format"), &MultiMesh::get_custom_data_format);
	ClassDB::bind_method(D_METHOD("set_transform_format", "format"), &MultiMesh::set_transform_format);
	ClassDB::bind_method(D_METHOD("get_transform_format"), &MultiMesh::get_transform_format);

	ClassDB::bind_method(D_METHOD("set_instance_count", "count"), &MultiMesh::set_instance_count);
	ClassDB::bind_method(D_METHOD("get_instance_count"), &MultiMesh::get_instance_count);
	ClassDB::bind_method(D_METHOD("set_visible_instance_count", "count"), &MultiMesh::set_visible_instance_count);
	ClassDB::bind_method(D_METHOD("get_visible_instance_count"), &MultiMesh::get_visible_instance_count);
	ClassDB::bind_method(D_METHOD("set_physics_interpolation_quality", "quality"), &MultiMesh::set_physics_interpolation_quality);
	ClassDB::bind_method(D_METHOD("get_physics_interpolation_quality"), &MultiMesh::get_physics_interpolation_quality);
	ClassDB::bind_method(D_METHOD("set_instance_transform", "instance", "transform"), &MultiMesh::set_instance_transform);
	ClassDB::bind_method(D_METHOD("set_instance_transform_2d", "instance", "transform"), &MultiMesh::set_instance_transform_2d);
	ClassDB::bind_method(D_METHOD("get_instance_transform", "instance"), &MultiMesh::get_instance_transform);
	ClassDB::bind_method(D_METHOD("get_instance_transform_2d", "instance"), &MultiMesh::get_instance_transform_2d);
	ClassDB::bind_method(D_METHOD("set_instance_color", "instance", "color"), &MultiMesh::set_instance_color);
	ClassDB::bind_method(D_METHOD("get_instance_color", "instance"), &MultiMesh::get_instance_color);
	ClassDB::bind_method(D_METHOD("set_instance_custom_data", "instance", "custom_data"), &MultiMesh::set_instance_custom_data);
	ClassDB::bind_method(D_METHOD("get_instance_custom_data", "instance"), &MultiMesh::get_instance_custom_data);
	ClassDB::bind_method(D_METHOD("reset_instance_physics_interpolation", "instance"), &MultiMesh::reset_instance_physics_interpolation);
	ClassDB::bind_method(D_METHOD("reset_instances_physics_interpolation"), &MultiMesh::reset_instances_physics_interpolation);
	ClassDB::bind_method(D_METHOD("set_as_bulk_array", "array"), &MultiMesh::set_as_bulk_array);
	ClassDB::bind_method(D_METHOD("set_as_bulk_array_interpolated", "array_current", "array_previous"), &MultiMesh::set_as_bulk_array_interpolated);
	ClassDB::bind_method(D_METHOD("get_aabb"), &MultiMesh::get_aabb);

	ClassDB::bind_method(D_METHOD("_set_transform_array"), &MultiMesh::_set_transform_array);
	ClassDB::bind_method(D_METHOD("_get_transform_array"), &MultiMesh::_get_transform_array);
	ClassDB::bind_method(D_METHOD("_set_transform_2d_array"), &MultiMesh::_set_transform_2d_array);
	ClassDB::bind_method(D_METHOD("_get_transform_2d_array"), &MultiMesh::_get_transform_2d_array);
	ClassDB::bind_method(D_METHOD("_set_color_array"), &MultiMesh::_set_color_array);
	ClassDB::bind_method(D_METHOD("_get_color_array"), &MultiMesh::_get_color_array);
	ClassDB::bind_method(D_METHOD("_set_custom_data_array"), &MultiMesh::_set_custom_data_array);
	ClassDB::bind_method(D_METHOD("_get_custom_data_array"), &MultiMesh::_get_custom_data_array);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "color_format", PROPERTY_HINT_ENUM, "None,Byte,Float"), "set_color_format", "get_color_format");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "transform_format", PROPERTY_HINT_ENUM, "2D,3D"), "set_transform_format", "get_transform_format");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "custom_data_format", PROPERTY_HINT_ENUM, "None,Byte,Float"), "set_custom_data_format", "get_custom_data_format");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "instance_count", PROPERTY_HINT_RANGE, "0,16384,1,or_greater"), "set_instance_count", "get_instance_count");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "visible_instance_count", PROPERTY_HINT_RANGE, "-1,16384,1,or_greater"), "set_visible_instance_count", "get_visible_instance_count");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "Mesh"), "set_mesh", "get_mesh");
	ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR3_ARRAY, "transform_array", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL), "_set_transform_array", "_get_transform_array");
	ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "transform_2d_array", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL), "_set_transform_2d_array", "_get_transform_2d_array");
	ADD_PROPERTY(PropertyInfo(Variant::POOL_COLOR_ARRAY, "color_array", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL), "_set_color_array", "_get_color_array");
	ADD_PROPERTY(PropertyInfo(Variant::POOL_COLOR_ARRAY, "custom_data_array", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL), "_set_custom_data_array", "_get_custom_data_array");

	ADD_GROUP("Physics Interpolation", "physics_interpolation");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "physics_interpolation_quality", PROPERTY_HINT_ENUM, "Fast,High"), "set_physics_interpolation_quality", "get_physics_interpolation_quality");

	BIND_ENUM_CONSTANT(TRANSFORM_2D);
	BIND_ENUM_CONSTANT(TRANSFORM_3D);

	BIND_ENUM_CONSTANT(COLOR_NONE);
	BIND_ENUM_CONSTANT(COLOR_8BIT);
	BIND_ENUM_CONSTANT(COLOR_FLOAT);

	BIND_ENUM_CONSTANT(CUSTOM_DATA_NONE);
	BIND_ENUM_CONSTANT(CUSTOM_DATA_8BIT);
	BIND_ENUM_CONSTANT(CUSTOM_DATA_FLOAT);

	BIND_ENUM_CONSTANT(INTERP_QUALITY_FAST);
	BIND_ENUM_CONSTANT(INTERP_QUALITY_HIGH);
}

MultiMesh::MultiMesh() {
	multimesh = RID_PRIME(RenderingServer::get_singleton()->multimesh_create());
	color_format = COLOR_NONE;
	custom_data_format = CUSTOM_DATA_NONE;
	transform_format = TRANSFORM_2D;
	visible_instance_count = -1;
	instance_count = 0;
	_physics_interpolation_quality = INTERP_QUALITY_FAST;
}

MultiMesh::~MultiMesh() {
	RenderingServer::get_singleton()->free(multimesh);
}
