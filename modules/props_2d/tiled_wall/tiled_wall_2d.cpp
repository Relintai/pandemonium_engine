#include "tiled_wall_2d.h"

#include "scene/resources/texture.h"

#include "core/io/image.h"

#if TEXTURE_PACKER_PRESENT
#include "../../texture_packer/texture_resource/packer_image_resource.h"
#endif

#include "../material_cache/prop_2d_material_cache.h"
#include "../prop_2d_mesher.h"
#include "../singleton/prop_2d_cache.h"

#include "core/core_string_names.h"
#include "tiled_wall_2d_data.h"

#include "../lights/prop_2d_light.h"
#include "core/math/geometry.h"

int TiledWall2D::get_width() const {
	return _width;
}
void TiledWall2D::set_width(const int value) {
	_width = value;

	clear_mesh();
	generate_mesh();
}

int TiledWall2D::get_heigth() const {
	return _height;
}
void TiledWall2D::set_heigth(const int value) {
	_height = value;

	clear_mesh();
	generate_mesh();
}

Transform2D TiledWall2D::get_mesh_transform() const {
	return _mesh_transform;
}
void TiledWall2D::set_mesh_transform(const Transform2D &value) {
	_mesh_transform = value;

	clear_mesh();
	generate_mesh();
}

Ref<TiledWall2DData> TiledWall2D::get_data() {
	return _data;
}
void TiledWall2D::set_data(const Ref<TiledWall2DData> &data) {
	if (_data.is_valid()) {
		_data->disconnect(CoreStringNames::get_singleton()->changed, this, "refresh");
	}

	_data = data;

	if (_data.is_valid()) {
		_data->connect(CoreStringNames::get_singleton()->changed, this, "refresh");
	}

	call_deferred("refresh");
}

Rect2 TiledWall2D::get_rect() const {
	return _rect;
}

PoolVector<Face3> TiledWall2D::get_faces(uint32_t p_usage_flags) const {
	PoolVector<Face3> faces;

	if (_mesh_array.size() != Mesh::ARRAY_MAX) {
		return faces;
	}

	PoolVector<Vector3> vertices = _mesh_array[Mesh::ARRAY_VERTEX];
	PoolVector<int> indices = _mesh_array[Mesh::ARRAY_INDEX];

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

	return faces;
}

#ifdef TOOLS_ENABLED
bool TiledWall2D::_edit_is_selected_on_click(const Point2 &p_point, double p_tolerance) const {
	if (_editor_selection_points.size() == 0) {
		return false;
	}

	return Geometry::is_point_in_polygon(p_point, _editor_selection_points);
}

bool TiledWall2D::_edit_use_rect() const {
	return true;
}

Rect2 TiledWall2D::_edit_get_rect() const {
	return get_rect();
}

#endif

void TiledWall2D::refresh() {
	if (!is_inside_tree()) {
		return;
	}

	clear_mesh();

	if (!_data.is_valid()) {
		return;
	}

	if (_mesh_rid == RID()) {
		_mesh_rid = RenderingServer::get_singleton()->mesh_create();
	}

	Ref<Prop2DMaterialCache> old_cache;

	old_cache = _cache;

	_cache = Prop2DCache::get_singleton()->tiled_wall_material_cache_get(_data);

	if (old_cache.is_valid() && old_cache != _cache) {
		Prop2DCache::get_singleton()->tiled_wall_material_cache_unref(old_cache);
	}

	if (!_cache->get_initialized()) {
		_cache->mutex_lock();

		//An anouther thread could have initialized it before wo got the mutex!
		if (!_cache->get_initialized()) {
			//can only be called from the main thread!
			_cache->initial_setup_default();

			_data->setup_cache(_cache);

			_cache->refresh_rects();
		}

		_cache->mutex_unlock();
	}

	Ref<Material> mat = _cache->material_get();
	if (mat != get_material()) {
		set_material(mat);
	}

	Ref<Texture> tex = _cache->texture_get_merged();

	if (tex.is_valid()) {
		_texture_rid = tex->get_rid();
	} else {
		_texture_rid = RID();
	}

	generate_mesh();
}

void TiledWall2D::generate_mesh() {
	if (!_data.is_valid()) {
		update();
		return;
	}

	if (!_cache.is_valid()) {
		update();
		return;
	}

	_mesher->add_tiled_wall_simple(_width, _height, _mesh_transform, _data, _cache);
	_mesher->bake_colors();

	_rect = _mesher->calculate_rect();

	_mesh_array = _mesher->build_mesh();

	if (_mesh_array.size() != Mesh::ARRAY_MAX) {
		update();
		return;
	}

	PoolVector<Vector2> vertices = _mesh_array[Mesh::ARRAY_VERTEX];

#ifdef TOOLS_ENABLED
	Vector<Vector2> editor_point_vertices;
	editor_point_vertices.resize(vertices.size());

	for (int i = 0; i < vertices.size(); ++i) {
		editor_point_vertices.set(i, vertices[i]);
	}

	_editor_selection_points = Geometry::convex_hull_2d(editor_point_vertices);
#endif

	if (vertices.size() == 0) {
		update();
		return;
	}

	RenderingServer::get_singleton()->mesh_add_surface_from_arrays(_mesh_rid, RenderingServer::PRIMITIVE_TRIANGLES, _mesh_array);

	_aabb.size = Vector3(_width, _height, 0);

	update();
}

void TiledWall2D::clear_mesh() {
	_mesher->reset();
	_aabb = AABB();
	_mesh_array.clear();

	if (_mesh_rid != RID()) {
		if (RS::get_singleton()->mesh_get_surface_count(_mesh_rid) > 0)
			RS::get_singleton()->mesh_remove_surface(_mesh_rid, 0);
	}
}

void TiledWall2D::free_mesh() {
	if (_mesh_rid != RID()) {
		RS::get_singleton()->free(_mesh_rid);
		_mesh_rid = RID();
	}
}

void TiledWall2D::draw() {
	if (_mesh_rid == RID()) {
		return;
	}

	RenderingServer::get_singleton()->canvas_item_add_mesh(get_canvas_item(), _mesh_rid, get_transform(), Color(1, 1, 1, 1), _texture_rid, RID());
}

TiledWall2D::TiledWall2D() {
	_width = 1;
	_height = 1;

	_mesher.instance();
	//_mesher->set_build_flags(Prop2DMesher::BUILD_FLAG_USE_LIGHTING | Prop2DMesher::BUILD_FLAG_USE_AO | Prop2DMesher::BUILD_FLAG_USE_RAO | Prop2DMesher::BUILD_FLAG_BAKE_LIGHTS);
}
TiledWall2D::~TiledWall2D() {
	_data.unref();
	_cache.unref();
	_mesher.unref();

	free_mesh();
}

void TiledWall2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			refresh();
			break;
		}
		case NOTIFICATION_EXIT_TREE: {
			break;
		}
		case NOTIFICATION_TRANSFORM_CHANGED: {
			break;
		}
		case NOTIFICATION_DRAW: {
			draw();
		}
	}
}

void TiledWall2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_width"), &TiledWall2D::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "value"), &TiledWall2D::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "width"), "set_width", "get_width");

	ClassDB::bind_method(D_METHOD("get_heigth"), &TiledWall2D::get_heigth);
	ClassDB::bind_method(D_METHOD("set_heigth", "value"), &TiledWall2D::set_heigth);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "heigth"), "set_heigth", "get_heigth");

	ClassDB::bind_method(D_METHOD("get_data"), &TiledWall2D::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "value"), &TiledWall2D::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "TiledWall2DData"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_mesh_transform"), &TiledWall2D::get_mesh_transform);
	ClassDB::bind_method(D_METHOD("set_mesh_transform", "value"), &TiledWall2D::set_mesh_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform"), "set_mesh_transform", "get_mesh_transform");

	//ADD_GROUP("Collision", "collision_");
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_layer", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_layer", "get_collision_layer");
	//ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_mask", "get_collision_mask");

	ClassDB::bind_method(D_METHOD("refresh"), &TiledWall2D::refresh);
	ClassDB::bind_method(D_METHOD("generate_mesh"), &TiledWall2D::generate_mesh);
	ClassDB::bind_method(D_METHOD("clear_mesh"), &TiledWall2D::clear_mesh);
	ClassDB::bind_method(D_METHOD("free_mesh"), &TiledWall2D::free_mesh);
}
