

#include "prop_data_mesh_data.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_PROPS_ENABLED

#include "../nodes/mesh_data_instance.h"
#include "scene/resources/material/material.h"
#include "scene/resources/material/spatial_material.h"

Ref<MeshDataResource> PropDataMeshData::get_mesh() const {
	return _mesh;
}
void PropDataMeshData::set_mesh(const Ref<MeshDataResource> mesh) {
	_mesh = mesh;
}

Ref<Texture> PropDataMeshData::get_texture() const {
	return _texture;
}
void PropDataMeshData::set_texture(const Ref<Texture> texture) {
	_texture = texture;
}

bool PropDataMeshData::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void PropDataMeshData::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 PropDataMeshData::get_snap_axis() {
	return _snap_axis;
}
void PropDataMeshData::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

#ifdef MODULE_TEXTURE_PACKER_ENABLED
void PropDataMeshData::_add_textures_into(Ref<TexturePacker> texture_packer) {
	if (get_texture().is_valid()) {
		texture_packer->add_texture(get_texture());
	}
}
#endif

bool PropDataMeshData::_processor_handles(Node *node) {
	MeshDataInstance *i = Object::cast_to<MeshDataInstance>(node);

	return i;
}

void PropDataMeshData::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	MeshDataInstance *i = Object::cast_to<MeshDataInstance>(node);

	ERR_FAIL_COND(!i);

	Ref<PropDataMeshData> m;
	m.instance();
	m->set_mesh(i->get_mesh_data());
	m->set_texture(i->get_texture());
	m->set_transform(transform * i->get_transform());
	prop_data->add_prop(m);
}

Node *PropDataMeshData::_processor_get_node_for(const Transform &transform) {
	MeshDataInstance *i = memnew(MeshDataInstance);

	Ref<SpatialMaterial> m;
	m.instance();

	i->set_material(m);
	i->set_texture(get_texture());
	i->set_mesh_data(get_mesh());
	i->set_transform(get_transform());

	return i;
}

PropDataMeshData::PropDataMeshData() {
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, 1, 0);
}
PropDataMeshData::~PropDataMeshData() {
	if (_mesh.is_valid())
		_mesh.unref();
}

void PropDataMeshData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh"), &PropDataMeshData::get_mesh);
	ClassDB::bind_method(D_METHOD("set_mesh", "value"), &PropDataMeshData::set_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh", "get_mesh");

	ClassDB::bind_method(D_METHOD("get_texture"), &PropDataMeshData::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "value"), &PropDataMeshData::set_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropDataMeshData::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropDataMeshData::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &PropDataMeshData::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &PropDataMeshData::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	ClassDB::bind_method(D_METHOD("_add_textures_into", "texture_packer"), &PropDataMeshData::_add_textures_into);
#endif
}

#endif
