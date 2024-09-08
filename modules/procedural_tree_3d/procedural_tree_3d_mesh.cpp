
#include "procedural_tree_3d_mesh.h"
#include "core/object/object.h"

#include "servers/rendering_server.h"

void ProceduralTree3DMesh::_update() const {
	Array arr;
	arr.resize(RS::ARRAY_MAX);

	PoolVector<Vector3> points = arr[RS::ARRAY_VERTEX];

	aabb = AABB();

	int pc = points.size();
	ERR_FAIL_COND(pc == 0);
	{
		PoolVector<Vector3>::Read r = points.read();
		for (int i = 0; i < pc; i++) {
			if (i == 0) {
				aabb.position = r[i];
			} else {
				aabb.expand_to(r[i]);
			}
		}
	}

	RenderingServer::get_singleton()->mesh_clear(mesh);

	// in with the new
	RenderingServer::get_singleton()->mesh_add_surface_from_arrays(mesh, RenderingServer::PRIMITIVE_TRIANGLES, arr);
	RenderingServer::get_singleton()->mesh_add_surface_from_arrays(mesh, RenderingServer::PRIMITIVE_TRIANGLES, arr);

	RenderingServer::get_singleton()->mesh_surface_set_material(mesh, TREE_SURFACE_TRUNK, _surfaces[TREE_SURFACE_TRUNK].material.is_null() ? RID() : _surfaces[TREE_SURFACE_TRUNK].material->get_rid());
	RenderingServer::get_singleton()->mesh_surface_set_material(mesh, TREE_SURFACE_TWIG, _surfaces[TREE_SURFACE_TWIG].material.is_null() ? RID() : _surfaces[TREE_SURFACE_TWIG].material->get_rid());

	pending_request = false;

	clear_cache();

	const_cast<ProceduralTree3DMesh *>(this)->emit_changed();
}

void ProceduralTree3DMesh::_request_update() {
	if (pending_request) {
		return;
	}
	_update();
}

int ProceduralTree3DMesh::get_surface_count() const {
	if (pending_request) {
		_update();
	}
	return TREE_SURFACE_COUNT;
}

int ProceduralTree3DMesh::surface_get_array_len(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, TREE_SURFACE_COUNT, -1);
	if (pending_request) {
		_update();
	}

	return RenderingServer::get_singleton()->mesh_surface_get_array_len(mesh, p_idx);
}

int ProceduralTree3DMesh::surface_get_array_index_len(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, TREE_SURFACE_COUNT, -1);
	if (pending_request) {
		_update();
	}

	return RenderingServer::get_singleton()->mesh_surface_get_array_index_len(mesh, p_idx);
}

Array ProceduralTree3DMesh::surface_get_arrays(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, TREE_SURFACE_COUNT, Array());
	if (pending_request) {
		_update();
	}

	return RenderingServer::get_singleton()->mesh_surface_get_arrays(mesh, p_surface);
}

Array ProceduralTree3DMesh::surface_get_blend_shape_arrays(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, TREE_SURFACE_COUNT, Array());
	if (pending_request) {
		_update();
	}

	return Array();
}

uint32_t ProceduralTree3DMesh::surface_get_format(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, TREE_SURFACE_COUNT, 0);
	if (pending_request) {
		_update();
	}

	return RenderingServer::get_singleton()->mesh_surface_get_format(mesh, p_idx);
}

Mesh::PrimitiveType ProceduralTree3DMesh::surface_get_primitive_type(int p_idx) const {
	return Mesh::PRIMITIVE_TRIANGLES;
}

void ProceduralTree3DMesh::surface_set_material(int p_idx, const Ref<Material> &p_material) {
	ERR_FAIL_INDEX(p_idx, TREE_SURFACE_COUNT);

	switch (p_idx) {
		case TREE_SURFACE_TRUNK:
			set_trunk_material(p_material);
			break;
		case TREE_SURFACE_TWIG:
			set_twig_material(p_material);
			break;
		case TREE_SURFACE_COUNT:
		default:
			break;
	}
}

Ref<Material> ProceduralTree3DMesh::surface_get_material(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, TREE_SURFACE_COUNT, nullptr);

	return _surfaces[p_idx].material;
}

int ProceduralTree3DMesh::get_blend_shape_count() const {
	return 0;
}

StringName ProceduralTree3DMesh::get_blend_shape_name(int p_index) const {
	return StringName();
}

void ProceduralTree3DMesh::set_blend_shape_name(int p_index, const StringName &p_name) {
}

AABB ProceduralTree3DMesh::get_aabb() const {
	if (pending_request) {
		_update();
	}

	return aabb;
}

RID ProceduralTree3DMesh::get_rid() const {
	if (pending_request) {
		_update();
	}
	return mesh;
}

void ProceduralTree3DMesh::set_trunk_material(const Ref<Material> &p_material) {
	_surfaces[TREE_SURFACE_TRUNK].material = p_material;
	if (!pending_request) {
		// just apply it, else it'll happen when _update is called.
		RenderingServer::get_singleton()->mesh_surface_set_material(mesh, TREE_SURFACE_TRUNK, _surfaces[TREE_SURFACE_TRUNK].material.is_null() ? RID() : _surfaces[TREE_SURFACE_TRUNK].material->get_rid());
		_change_notify();
		emit_changed();
	};
}

Ref<Material> ProceduralTree3DMesh::get_trunk_material() const {
	return _surfaces[TREE_SURFACE_TRUNK].material;
}


void ProceduralTree3DMesh::set_twig_material(const Ref<Material> &p_material) {
	_surfaces[TREE_SURFACE_TWIG].material = p_material;
	if (!pending_request) {
		// just apply it, else it'll happen when _update is called.
		RenderingServer::get_singleton()->mesh_surface_set_material(mesh, TREE_SURFACE_TWIG, _surfaces[TREE_SURFACE_TWIG].material.is_null() ? RID() : _surfaces[TREE_SURFACE_TWIG].material->get_rid());
		_change_notify();
		emit_changed();
	};
}

Ref<Material> ProceduralTree3DMesh::get_twig_material() const {
	return _surfaces[TREE_SURFACE_TWIG].material;
}


Array ProceduralTree3DMesh::get_mesh_arrays() const {
	Array arr;

	for (int i = 0; i < TREE_SURFACE_COUNT; ++i) {
		arr.push_back(surface_get_arrays(i));
	}

	return arr;
}

void ProceduralTree3DMesh::set_custom_aabb(const AABB &p_custom) {
	custom_aabb = p_custom;
	RS::get_singleton()->mesh_set_custom_aabb(mesh, custom_aabb);
	emit_changed();
}

AABB ProceduralTree3DMesh::get_custom_aabb() const {
	return custom_aabb;
}

ProceduralTree3DMesh::ProceduralTree3DMesh() {
	// defaults
	mesh = RID_PRIME(RenderingServer::get_singleton()->mesh_create());

	// make sure we do an update after we've finished constructing our object
	pending_request = true;
}

ProceduralTree3DMesh::~ProceduralTree3DMesh() {
	RenderingServer::get_singleton()->free(mesh);
}

void ProceduralTree3DMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_update"), &ProceduralTree3DMesh::_update);

	ClassDB::bind_method(D_METHOD("set_trunk_material", "material"), &ProceduralTree3DMesh::set_trunk_material);
	ClassDB::bind_method(D_METHOD("get_trunk_material"), &ProceduralTree3DMesh::get_trunk_material);

	ClassDB::bind_method(D_METHOD("set_twig_material", "material"), &ProceduralTree3DMesh::set_twig_material);
	ClassDB::bind_method(D_METHOD("get_twig_material"), &ProceduralTree3DMesh::get_twig_material);

	ClassDB::bind_method(D_METHOD("get_mesh_arrays"), &ProceduralTree3DMesh::get_mesh_arrays);

	ClassDB::bind_method(D_METHOD("set_custom_aabb", "aabb"), &ProceduralTree3DMesh::set_custom_aabb);
	ClassDB::bind_method(D_METHOD("get_custom_aabb"), &ProceduralTree3DMesh::get_custom_aabb);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "trunk_material", PROPERTY_HINT_RESOURCE_TYPE, "SpatialMaterial,ShaderMaterial"), "set_trunk_material", "get_trunk_material");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "twig_material", PROPERTY_HINT_RESOURCE_TYPE, "SpatialMaterial,ShaderMaterial"), "set_twig_material", "get_twig_material");

	ADD_PROPERTY(PropertyInfo(Variant::AABB, "custom_aabb", PROPERTY_HINT_NONE, ""), "set_custom_aabb", "get_custom_aabb");
}
