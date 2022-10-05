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

#include "prop_2d_data_mesh_data.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_PROPS_2D_ENABLED

#include "../nodes/mesh_data_instance.h"
#include "scene/resources/material.h"

Ref<MeshDataResource> Prop2DDataMeshData::get_mesh() const {
	return _mesh;
}
void Prop2DDataMeshData::set_mesh(const Ref<MeshDataResource> mesh) {
	_mesh = mesh;
}

Ref<Texture> Prop2DDataMeshData::get_texture() const {
	return _texture;
}
void Prop2DDataMeshData::set_texture(const Ref<Texture> texture) {
	_texture = texture;
}

bool Prop2DDataMeshData::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void Prop2DDataMeshData::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 Prop2DDataMeshData::get_snap_axis() {
	return _snap_axis;
}
void Prop2DDataMeshData::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

#ifdef MODULE_TEXTURE_PACKER_ENABLED
void Prop2DDataMeshData::_add_textures_into(Ref<TexturePacker> texture_packer) {
	if (get_texture().is_valid()) {
		texture_packer->add_texture(get_texture());
	}
}
#endif

bool Prop2DDataMeshData::_processor_handles(Node *node) {
	MeshDataInstance *i = Object::cast_to<MeshDataInstance>(node);

	return i;
}

void Prop2DDataMeshData::_processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry) {
	MeshDataInstance *i = Object::cast_to<MeshDataInstance>(node);

	ERR_FAIL_COND(!i);

	Ref<Prop2DDataMeshData> m;

	if (entry.is_valid()) {
		m = entry;
	} else {
		m.instance();
	}

	m->set_mesh(i->get_mesh_data());
	m->set_texture(i->get_texture());

	Prop2DDataEntry::_processor_process(prop_data, node, transform, m);
}

Node *Prop2DDataMeshData::_processor_get_node_for(const Transform2D &transform, Node *node) {
	MeshDataInstance *i = nullptr;

	if (!node) {
		i = memnew(MeshDataInstance);
	} else {
		i = Object::cast_to<MeshDataInstance>(node);
	}

	i->set_texture(get_texture());
	i->set_mesh_data(get_mesh());

	return Prop2DDataEntry::_processor_get_node_for(transform, i);
}

Prop2DDataMeshData::Prop2DDataMeshData() {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, 1, 0);
}
Prop2DDataMeshData::~Prop2DDataMeshData() {
	if (_mesh.is_valid())
		_mesh.unref();
}

void Prop2DDataMeshData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh"), &Prop2DDataMeshData::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "value"), &Prop2DDataMeshData::set_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh", "get_mesh");

	ClassDB::bind_method(D_METHOD("get_texture"), &Prop2DDataMeshData::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "value"), &Prop2DDataMeshData::set_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &Prop2DDataMeshData::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &Prop2DDataMeshData::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &Prop2DDataMeshData::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &Prop2DDataMeshData::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	ClassDB::bind_method(D_METHOD("_add_textures_into", "texture_packer"), &Prop2DDataMeshData::_add_textures_into);
#endif
}

#endif
