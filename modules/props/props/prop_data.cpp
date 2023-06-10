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

#include "prop_data.h"

#include "prop_data_entry.h"
#include "prop_data_light.h"
#include "prop_data_prop.h"

#include "servers/physics_server.h"

#include "modules/modules_enabled.gen.h"

int PropData::get_id() const {
	return _id;
}
void PropData::set_id(const int value) {
	_id = value;
}

bool PropData::get_snap_to_mesh() const {
	return _snap_to_mesh;
}
void PropData::set_snap_to_mesh(const bool value) {
	_snap_to_mesh = value;
}

Vector3 PropData::get_snap_axis() const {
	return _snap_axis;
}
void PropData::set_snap_axis(const Vector3 &value) {
	_snap_axis = value;
}

Ref<PropDataEntry> PropData::get_prop(const int index) const {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<PropDataEntry>());

	return _props.get(index);
}
void PropData::set_prop(const int index, const Ref<PropDataEntry> prop) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.set(index, prop);
}
void PropData::add_prop(const Ref<PropDataEntry> prop) {
	_props.push_back(prop);
}
void PropData::remove_prop(const int index) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.remove(index);
}

int PropData::get_prop_count() const {
	return _props.size();
}

Vector<Variant> PropData::get_props() {
	Vector<Variant> r;
	for (int i = 0; i < _props.size(); i++) {
		r.push_back(_props[i].get_ref_ptr());
	}
	return r;
}
void PropData::set_props(const Vector<Variant> &props) {
	_props.clear();
	for (int i = 0; i < props.size(); i++) {
		Ref<PropDataEntry> prop = Ref<PropDataEntry>(props[i]);

		_props.push_back(prop);
	}
}

#ifdef MODULE_TEXTURE_PACKER_ENABLED
void PropData::add_textures_into(Ref<TexturePacker> texture_packer) {
	ERR_FAIL_COND(!texture_packer.is_valid());

	for (int i = 0; i < _props.size(); ++i) {
		Ref<PropDataEntry> entry = _props.get(i);

		if (entry.is_valid()) {
			entry->add_textures_into(texture_packer);
		}
	}
}
#endif

bool PropData::get_is_room() const {
	return _is_room;
}
void PropData::set_is_room(const bool value) {
	_is_room = value;
}

PoolVector3Array PropData::get_room_bounds() {
	return _room_bounds;
}
void PropData::set_room_bounds(const PoolVector3Array &bounds) {
	_room_bounds = bounds;
}

void PropData::copy_from(const Ref<PropData> &prop_data) {
	_id = prop_data->_id;
	_snap_to_mesh = prop_data->_snap_to_mesh;
	_snap_axis = prop_data->_snap_axis;

	_props.clear();

	for (int i = 0; i < prop_data->_props.size(); ++i) {
		_props.push_back(prop_data->_props[i]);
	}

	_is_room = prop_data->_is_room;
	_room_bounds = prop_data->_room_bounds;

	emit_changed();
}

PropData::PropData() {
	_id = 0;
	_snap_to_mesh = false;
	_is_room = false;
	_snap_axis = Vector3(0, -1, 0);
}
PropData::~PropData() {
	_props.clear();
}

void PropData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropData::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropData::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &PropData::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &PropData::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

	ClassDB::bind_method(D_METHOD("get_prop", "index"), &PropData::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "index", "spell"), &PropData::set_prop);
	ClassDB::bind_method(D_METHOD("add_prop", "prop"), &PropData::add_prop);
	ClassDB::bind_method(D_METHOD("remove_prop", "index"), &PropData::remove_prop);

	ClassDB::bind_method(D_METHOD("get_prop_count"), &PropData::get_prop_count);

	ClassDB::bind_method(D_METHOD("get_props"), &PropData::get_props);
	ClassDB::bind_method(D_METHOD("set_props", "props"), &PropData::set_props);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "props", PROPERTY_HINT_NONE, "23/20:PropDataEntry", PROPERTY_USAGE_DEFAULT, "PropDataEntry"), "set_props", "get_props");

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	ClassDB::bind_method(D_METHOD("add_textures_into", "texture_packer"), &PropData::add_textures_into);
#endif

	ClassDB::bind_method(D_METHOD("get_is_room"), &PropData::get_is_room);
	ClassDB::bind_method(D_METHOD("set_is_room", "value"), &PropData::set_is_room);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_room"), "set_is_room", "get_is_room");

	ClassDB::bind_method(D_METHOD("get_room_bounds"), &PropData::get_room_bounds);
	ClassDB::bind_method(D_METHOD("set_room_bounds", "value"), &PropData::set_room_bounds);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR3_ARRAY, "room_bounds"), "set_room_bounds", "get_room_bounds");

	ClassDB::bind_method(D_METHOD("copy_from", "prop_data"), &PropData::copy_from);
}
