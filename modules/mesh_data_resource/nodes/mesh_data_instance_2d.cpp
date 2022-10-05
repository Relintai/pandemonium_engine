#include "mesh_data_instance_2d.h"

#include "scene/resources/texture.h"

#include "core/io/image.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_resource/packer_image_resource.h"
#endif

Ref<MeshDataResource> MeshDataInstance2D::get_mesh_data() {
	return _mesh;
}
void MeshDataInstance2D::set_mesh_data(const Ref<MeshDataResource> &mesh) {
	if (_mesh == mesh) {
		return;
	}

	if (_mesh.is_valid()) {
		_mesh->disconnect("changed", this, "refresh");
	}

	_mesh = mesh;

	refresh();

	if (_mesh.is_valid()) {
		_mesh->connect("changed", this, "refresh");
	}

	emit_signal("mesh_data_resource_changed");
}

Ref<Texture> MeshDataInstance2D::get_texture() {
	return _texture;
}
void MeshDataInstance2D::set_texture(const Ref<Texture> &texture) {
	if (_texture == texture) {
		return;
	}

	_texture = texture;

	emit_signal("texture_changed");
	_change_notify("texture");
	refresh();
}

Ref<Texture> MeshDataInstance2D::get_normal_map() {
	return _normal_map;
}
void MeshDataInstance2D::set_normal_map(const Ref<Texture> &texture) {
	if (_normal_map == texture) {
		return;
	}

	_normal_map = texture;

	refresh();
}

void MeshDataInstance2D::refresh() {
	if (!is_inside_tree()) {
		return;
	}

	RenderingServer::get_singleton()->mesh_clear(_mesh_rid);

	if (!_mesh.is_valid()) {
		return;
	}

	Array arr = _mesh->get_array();

	if (arr.size() != Mesh::ARRAY_MAX) {
		return;
	}

	PoolVector<Vector2> vertices = arr[Mesh::ARRAY_VERTEX];

	if (vertices.size() == 0) {
		return;
	}

	RenderingServer::get_singleton()->mesh_add_surface_from_arrays(_mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, arr);
}

#ifdef TOOLS_ENABLED
Rect2 MeshDataInstance2D::_edit_get_rect() const {
	if (_mesh.is_valid()) {
		AABB aabb = _mesh->get_aabb();
		return Rect2(aabb.position.x, aabb.position.y, aabb.size.x, aabb.size.y);
	}

	return Node2D::_edit_get_rect();
}

bool MeshDataInstance2D::_edit_use_rect() const {
	return _mesh.is_valid();
}
#endif

MeshDataInstance2D::MeshDataInstance2D() {
	_mesh_rid = RenderingServer::get_singleton()->mesh_create();
}
MeshDataInstance2D::~MeshDataInstance2D() {
	_mesh.unref();
	_texture.unref();

	if (_mesh_rid != RID()) {
		RS::get_singleton()->free(_mesh_rid);
		_mesh_rid = RID();
	}
}

void MeshDataInstance2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			refresh();
		} break;
		case NOTIFICATION_DRAW: {
			if (_mesh.is_valid()) {
				RID texture_rid = _texture.is_valid() ? _texture->get_rid() : RID();
				RID normal_map_rid = _normal_map.is_valid() ? _normal_map->get_rid() : RID();

				RenderingServer::get_singleton()->canvas_item_add_mesh(get_canvas_item(), _mesh_rid, Transform2D(), get_modulate(), texture_rid, normal_map_rid);
			}
		} break;
	}
}

void MeshDataInstance2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesh_data"), &MeshDataInstance2D::get_mesh_data);
	ClassDB::bind_method(D_METHOD("set_mesh_data", "value"), &MeshDataInstance2D::set_mesh_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh_data", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh_data", "get_mesh_data");

	ClassDB::bind_method(D_METHOD("get_texture"), &MeshDataInstance2D::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "value"), &MeshDataInstance2D::set_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");

	ClassDB::bind_method(D_METHOD("get_normal_map"), &MeshDataInstance2D::get_normal_map);
	ClassDB::bind_method(D_METHOD("set_normal_map", "value"), &MeshDataInstance2D::set_normal_map);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "normal_map", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_normal_map", "get_normal_map");

	ClassDB::bind_method(D_METHOD("refresh"), &MeshDataInstance2D::refresh);

	ADD_SIGNAL(MethodInfo("mesh_data_resource_changed"));
	ADD_SIGNAL(MethodInfo("texture_changed"));
}
