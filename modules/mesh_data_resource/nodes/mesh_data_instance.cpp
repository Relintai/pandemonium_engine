#include "mesh_data_instance.h"

#include "scene/resources/texture.h"
#include "scene/resources/material/spatial_material.h"

#include "core/io/image.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_resource/packer_image_resource.h"
#endif

#include "scene/3d/mesh_instance.h"

bool MeshDataInstance::get_snap_to_mesh() const {
	return _snap_to_mesh;
}
void MeshDataInstance::set_snap_to_mesh(const bool value) {
	_snap_to_mesh = value;
}

Vector3 MeshDataInstance::get_snap_axis() const {
	return _snap_axis;
}
void MeshDataInstance::set_snap_axis(const Vector3 &value) {
	_snap_axis = value;
}

Ref<MeshDataResource> MeshDataInstance::get_mesh_data() {
	return _mesh;
}
void MeshDataInstance::set_mesh_data(const Ref<MeshDataResource> &mesh) {
	if (_mesh.is_valid()) {
		_mesh->disconnect("changed", this, "refresh");
	}

	_mesh = mesh;

	refresh();

	if (_mesh.is_valid()) {
		_mesh->connect("changed", this, "refresh");
	}

	emit_signal("mesh_data_resource_changed", _mesh);
}

Ref<Texture> MeshDataInstance::get_texture() {
	return _texture;
}
void MeshDataInstance::set_texture(const Ref<Texture> &texture) {
	_texture = texture;

	setup_material_texture();
	refresh();
}

Ref<Material> MeshDataInstance::get_material() {
	return _material;
}
void MeshDataInstance::set_material(const Ref<Material> &mat) {
	_material = mat;

	setup_material_texture();
	refresh();
}

AABB MeshDataInstance::get_aabb() const {
	if (!_mesh.is_valid()) {
		return AABB();
	}

	return _mesh->get_aabb();
}

PoolVector<Face3> MeshDataInstance::get_faces(uint32_t p_usage_flags) const {
	PoolVector<Face3> faces;

	if (_mesh.is_valid()) {
		Array arrs = _mesh->get_array_const();

		if (arrs.size() != Mesh::ARRAY_MAX) {
			return faces;
		}

		PoolVector<Vector3> vertices = arrs[Mesh::ARRAY_VERTEX];
		PoolVector<int> indices = arrs[Mesh::ARRAY_INDEX];

		int ts = indices.size() / 3;
		faces.resize(ts);

		PoolVector<Face3>::Write w = faces.write();
		PoolVector<Vector3>::Read rv = vertices.read();
		PoolVector<int>::Read ri = indices.read();

		for (int i = 0; i < ts; i++) {
			int im3 = (i * 3);

			for (int j = 0; j < 3; j++) {
				w[i].vertex[j] = rv[indices[im3 + j]];
			}
		}

		w.release();
	}

	return faces;
}

void MeshDataInstance::refresh() {
	if (!is_inside_tree()) {
		return;
	}

	if (_mesh_rid == RID()) {
		_mesh_rid = RenderingServer::get_singleton()->mesh_create();

		RS::get_singleton()->instance_set_base(get_instance(), _mesh_rid);
	}

	RenderingServer::get_singleton()->mesh_clear(_mesh_rid);

	if (!_mesh.is_valid()) {
		return;
	}

	Array arr = _mesh->get_array();

	if (arr.size() != Mesh::ARRAY_MAX) {
		return;
	}

	PoolVector<Vector3> vertices = arr[Mesh::ARRAY_VERTEX];

	if (vertices.size() == 0) {
		return;
	}

	RenderingServer::get_singleton()->mesh_add_surface_from_arrays(_mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, arr);

	if (_material.is_valid()) {
		RenderingServer::get_singleton()->mesh_surface_set_material(_mesh_rid, 0, _material->get_rid());
	}
}

void MeshDataInstance::setup_material_texture() {
	if (!is_inside_tree()) {
		return;
	}

	if (!_texture.is_valid()) {
		if (_material.is_valid()) {
			Ref<SpatialMaterial> sm = _material;

			if (!sm.is_valid()) {
				return;
			}

			sm->set_texture(SpatialMaterial::TEXTURE_ALBEDO, _texture);
		}

		return;
	} else {
		Ref<SpatialMaterial> sm = _material;

		if (!sm.is_valid()) {
			return;
		}

#ifdef MODULE_TEXTURE_PACKER_ENABLED
		Ref<PackerImageResource> r = _texture;

		if (r.is_valid()) {
			Ref<Image> i = r->get_data();

			Ref<ImageTexture> tex;
			tex.instance();
			tex->create_from_image(i, 0);

			if (sm.is_valid()) {
				sm->set_texture(SpatialMaterial::TEXTURE_ALBEDO, tex);
			}

			return;
		}
#endif

		sm->set_texture(SpatialMaterial::TEXTURE_ALBEDO, _texture);
	}
}

void MeshDataInstance::free_meshes() {
	if (_mesh_rid != RID()) {
		RS::get_singleton()->free(_mesh_rid);
		_mesh_rid = RID();
	}
}

MeshDataInstance::MeshDataInstance() {
	_dirty = false;
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, -1, 0);

	set_portal_mode(PORTAL_MODE_GLOBAL);

	//set_notify_transform(true);
}
MeshDataInstance::~MeshDataInstance() {
	_mesh.unref();
	_texture.unref();
}

void MeshDataInstance::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			setup_material_texture();
			refresh();

			break;
		}
		case NOTIFICATION_EXIT_TREE: {
			free_meshes();
			break;
		}
			/*
		case NOTIFICATION_TRANSFORM_CHANGED: {
			RenderingServer *vs = RenderingServer::get_singleton();

			vs->instance_set_transform(get_instance(), get_global_transform());

			break;
		}*/
	}
}

void MeshDataInstance::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &MeshDataInstance::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &MeshDataInstance::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &MeshDataInstance::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &MeshDataInstance::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

	ClassDB::bind_method(D_METHOD("get_mesh_data"), &MeshDataInstance::get_mesh_data);
	ClassDB::bind_method(D_METHOD("set_mesh_data", "value"), &MeshDataInstance::set_mesh_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh_data", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource"), "set_mesh_data", "get_mesh_data");

	ClassDB::bind_method(D_METHOD("get_texture"), &MeshDataInstance::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "value"), &MeshDataInstance::set_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");

	ClassDB::bind_method(D_METHOD("get_material"), &MeshDataInstance::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &MeshDataInstance::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("refresh"), &MeshDataInstance::refresh);

	ADD_SIGNAL(MethodInfo("mesh_data_resource_changed", PropertyInfo(Variant::OBJECT, "mdr", PROPERTY_HINT_RESOURCE_TYPE, "MeshDataResource")));
}
