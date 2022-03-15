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

#include "prop_data_prop.h"

#include "../prop_instance.h"
#include "prop_data.h"

Ref<PropData> PropDataProp::get_prop() const {
	return _prop;
}
void PropDataProp::set_prop(const Ref<PropData> value) {
	_prop = value;
}

bool PropDataProp::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void PropDataProp::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 PropDataProp::get_snap_axis() {
	return _snap_axis;
}
void PropDataProp::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

#if TEXTURE_PACKER_PRESENT
void PropDataProp::_add_textures_into(Ref<TexturePacker> texture_packer) {
	if (get_prop().is_valid()) {
		get_prop()->add_textures_into(texture_packer);
	}
}
#endif

bool PropDataProp::_processor_handles(Node *node) {
	PropInstance *i = Object::cast_to<PropInstance>(node);

	return i;
}

void PropDataProp::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	PropInstance *i = Object::cast_to<PropInstance>(node);

	ERR_FAIL_COND(!i);

	Ref<PropDataProp> l;
	l.instance();
	l->set_prop(i->get_prop_data());
	l->set_transform(transform * i->get_transform());
	prop_data->add_prop(l);
}

Node *PropDataProp::_processor_get_node_for(const Transform &transform) {
	PropInstance *i = memnew(PropInstance);

	i->set_prop_data(get_prop());
	i->set_transform(get_transform());

	return i;
}

PropDataProp::PropDataProp() {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, 1, 0);
}
PropDataProp::~PropDataProp() {
	if (_prop.is_valid())
		_prop.unref();
}

void PropDataProp::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop"), &PropDataProp::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "value"), &PropDataProp::set_prop);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "PropData"), "set_prop", "get_prop");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropDataProp::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropDataProp::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &PropDataProp::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &PropDataProp::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

#if TEXTURE_PACKER_PRESENT
	ClassDB::bind_method(D_METHOD("_add_textures_into", "texture_packer"), &PropDataProp::_add_textures_into);
#endif
}
