

#include "prop_2d_data.h"

#include "prop_2d_data_entry.h"
#include "prop_2d_data_light.h"
#include "prop_2d_data_prop.h"

#include "servers/physics_server.h"

#include "modules/modules_enabled.gen.h"

int Prop2DData::get_id() const {
	return _id;
}
void Prop2DData::set_id(const int value) {
	_id = value;
}

Ref<Prop2DDataEntry> Prop2DData::get_prop(const int index) const {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<Prop2DDataEntry>());

	return _props.get(index);
}
void Prop2DData::set_prop(const int index, const Ref<Prop2DDataEntry> prop) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.set(index, prop);
}
void Prop2DData::add_prop(const Ref<Prop2DDataEntry> prop) {
	_props.push_back(prop);
}
void Prop2DData::remove_prop(const int index) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.remove(index);
}

int Prop2DData::get_prop_count() const {
	return _props.size();
}

Vector<Variant> Prop2DData::get_props() {
	Vector<Variant> r;
	for (int i = 0; i < _props.size(); i++) {
		r.push_back(_props[i].get_ref_ptr());
	}
	return r;
}
void Prop2DData::set_props(const Vector<Variant> &props) {
	_props.clear();
	for (int i = 0; i < props.size(); i++) {
		Ref<Prop2DDataEntry> prop = Ref<Prop2DDataEntry>(props[i]);

		_props.push_back(prop);
	}
}

#ifdef MODULE_TEXTURE_PACKER_ENABLED
void Prop2DData::add_textures_into(Ref<TexturePacker> texture_packer) {
	ERR_FAIL_COND(!texture_packer.is_valid());

	for (int i = 0; i < _props.size(); ++i) {
		Ref<Prop2DDataEntry> entry = _props.get(i);

		if (entry.is_valid()) {
			entry->add_textures_into(texture_packer);
		}
	}
}
#endif

bool Prop2DData::get_is_room() const {
	return _is_room;
}
void Prop2DData::set_is_room(const bool value) {
	_is_room = value;
}

PoolVector3Array Prop2DData::get_room_bounds() {
	return _room_bounds;
}
void Prop2DData::set_room_bounds(const PoolVector3Array &bounds) {
	_room_bounds = bounds;
}

void Prop2DData::copy_from(const Ref<Prop2DData> &prop_data) {
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

Prop2DData::Prop2DData() {
	_id = 0;
	_snap_to_mesh = false;
	_is_room = false;
	_snap_axis = Vector3(0, -1, 0);
}
Prop2DData::~Prop2DData() {
	_props.clear();
}

void Prop2DData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop", "index"), &Prop2DData::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "index", "spell"), &Prop2DData::set_prop);
	ClassDB::bind_method(D_METHOD("add_prop", "prop"), &Prop2DData::add_prop);
	ClassDB::bind_method(D_METHOD("remove_prop", "index"), &Prop2DData::remove_prop);

	ClassDB::bind_method(D_METHOD("get_prop_count"), &Prop2DData::get_prop_count);

	ClassDB::bind_method(D_METHOD("get_props"), &Prop2DData::get_props);
	ClassDB::bind_method(D_METHOD("set_props", "props"), &Prop2DData::set_props);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "props", PROPERTY_HINT_NONE, "23/20:Prop2DDataEntry", PROPERTY_USAGE_DEFAULT, "Prop2DDataEntry"), "set_props", "get_props");

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	ClassDB::bind_method(D_METHOD("add_textures_into", "texture_packer"), &Prop2DData::add_textures_into);
#endif

	ClassDB::bind_method(D_METHOD("get_is_room"), &Prop2DData::get_is_room);
	ClassDB::bind_method(D_METHOD("set_is_room", "value"), &Prop2DData::set_is_room);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_room"), "set_is_room", "get_is_room");

	ClassDB::bind_method(D_METHOD("get_room_bounds"), &Prop2DData::get_room_bounds);
	ClassDB::bind_method(D_METHOD("set_room_bounds", "value"), &Prop2DData::set_room_bounds);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR3_ARRAY, "room_bounds"), "set_room_bounds", "get_room_bounds");

	ClassDB::bind_method(D_METHOD("copy_from", "prop_data"), &Prop2DData::copy_from);
}
