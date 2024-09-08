
#include "procedural_tree_mesh.h"
#include "core/object/class_db.h"
#include "core/object/object.h"

#include "servers/rendering_server.h"

// General
int ProceduralTreeMesh::get_seed() const {
	return _seed;
}
void ProceduralTreeMesh::set_seed(const int p_value) {
	_seed = p_value;
}

int ProceduralTreeMesh::get_branch_segments() const {
	return _branch_segments;
}
void ProceduralTreeMesh::set_branch_segments(const int p_value) {
	_branch_segments = p_value;
}

int ProceduralTreeMesh::get_branch_levels() const {
	return _branch_levels;
}
void ProceduralTreeMesh::set_branch_levels(const int p_value) {
	_branch_levels = p_value;
}

int ProceduralTreeMesh::get_trunk_forks() const {
	return _trunk_forks;
}
void ProceduralTreeMesh::set_trunk_forks(const int p_value) {
	_trunk_forks = p_value;
}

float ProceduralTreeMesh::get_texture_v_multiplier() const {
	return _texture_v_multiplier;
}
void ProceduralTreeMesh::set_texture_v_multiplier(const float p_value) {
	_texture_v_multiplier = p_value;
}

float ProceduralTreeMesh::get_twig_scale() const {
	return _twig_scale;
}
void ProceduralTreeMesh::set_twig_scale(const float p_value) {
	_twig_scale = p_value;
}

// Branching
float ProceduralTreeMesh::get_initial_length() const {
	return _initial_length;
}
void ProceduralTreeMesh::set_initial_length(const float p_value) {
	_initial_length = p_value;
}

float ProceduralTreeMesh::get_length_falloff_rate() const {
	return _length_falloff_rate;
}
void ProceduralTreeMesh::set_length_falloff_rate(const float p_value) {
	_length_falloff_rate = p_value;
}

float ProceduralTreeMesh::get_length_falloff_power() const {
	return _length_falloff_power;
}
void ProceduralTreeMesh::set_length_falloff_power(const float p_value) {
	_length_falloff_power = p_value;
}

float ProceduralTreeMesh::get_max_clumping() const {
	return _max_clumping;
}
void ProceduralTreeMesh::set_max_clumping(const float p_value) {
	_max_clumping = p_value;
}

float ProceduralTreeMesh::get_min_clumping() const {
	return _min_clumping;
}
void ProceduralTreeMesh::set_min_clumping(const float p_value) {
	_min_clumping = p_value;
}

float ProceduralTreeMesh::get_symmetry() const {
	return _symmetry;
}
void ProceduralTreeMesh::set_symmetry(const float p_value) {
	_symmetry = p_value;
}

float ProceduralTreeMesh::get_droop() const {
	return _droop;
}
void ProceduralTreeMesh::set_droop(const float p_value) {
	_droop = p_value;
}

float ProceduralTreeMesh::get_growth() const {
	return _growth;
}
void ProceduralTreeMesh::set_growth(const float p_value) {
	_growth = p_value;
}

float ProceduralTreeMesh::get_sweep() const {
	return _sweep;
}
void ProceduralTreeMesh::set_sweep(const float p_value) {
	_sweep = p_value;
}

// Trunk
float ProceduralTreeMesh::get_trunk_radius() const {
	return _trunk_radius;
}
void ProceduralTreeMesh::set_trunk_radius(const float p_value) {
	_trunk_radius = p_value;
}

float ProceduralTreeMesh::get_radius_falloff() const {
	return _radius_falloff;
}
void ProceduralTreeMesh::set_radius_falloff(const float p_value) {
	_radius_falloff = p_value;
}

float ProceduralTreeMesh::get_climb_rate() const {
	return _climb_rate;
}
void ProceduralTreeMesh::set_climb_rate(const float p_value) {
	_climb_rate = p_value;
}

float ProceduralTreeMesh::get_kink() const {
	return _kink;
}
void ProceduralTreeMesh::set_kink(const float p_value) {
	_kink = p_value;
}

float ProceduralTreeMesh::get_taper_rate() const {
	return _taper_rate;
}
void ProceduralTreeMesh::set_taper_rate(const float p_value) {
	_taper_rate = p_value;
}

float ProceduralTreeMesh::get_twists() const {
	return _twists;
}
void ProceduralTreeMesh::set_twists(const float p_value) {
	_twists = p_value;
}

float ProceduralTreeMesh::get_trunk_length() const {
	return _trunk_length;
}
void ProceduralTreeMesh::set_trunk_length(const float p_value) {
	_trunk_length = p_value;
}

void ProceduralTreeMesh::_update() const {
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

	RenderingServer::get_singleton()->mesh_surface_set_material(mesh, TREE_SURFACE_TWIG, _surfaces[TREE_SURFACE_TWIG].material.is_null() ? RID() : _surfaces[TREE_SURFACE_TWIG].material->get_rid());
	RenderingServer::get_singleton()->mesh_surface_set_material(mesh, TREE_SURFACE_TRUNK, _surfaces[TREE_SURFACE_TRUNK].material.is_null() ? RID() : _surfaces[TREE_SURFACE_TRUNK].material->get_rid());

	pending_request = false;

	clear_cache();

	const_cast<ProceduralTreeMesh *>(this)->emit_changed();
}

void ProceduralTreeMesh::_request_update() {
	if (pending_request) {
		return;
	}
	_update();
}

int ProceduralTreeMesh::get_surface_count() const {
	if (pending_request) {
		_update();
	}
	return TREE_SURFACE_COUNT;
}

int ProceduralTreeMesh::surface_get_array_len(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, TREE_SURFACE_COUNT, -1);
	if (pending_request) {
		_update();
	}

	return RenderingServer::get_singleton()->mesh_surface_get_array_len(mesh, p_idx);
}

int ProceduralTreeMesh::surface_get_array_index_len(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, TREE_SURFACE_COUNT, -1);
	if (pending_request) {
		_update();
	}

	return RenderingServer::get_singleton()->mesh_surface_get_array_index_len(mesh, p_idx);
}

Array ProceduralTreeMesh::surface_get_arrays(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, TREE_SURFACE_COUNT, Array());
	if (pending_request) {
		_update();
	}

	return RenderingServer::get_singleton()->mesh_surface_get_arrays(mesh, p_surface);
}

Array ProceduralTreeMesh::surface_get_blend_shape_arrays(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, TREE_SURFACE_COUNT, Array());
	if (pending_request) {
		_update();
	}

	return Array();
}

uint32_t ProceduralTreeMesh::surface_get_format(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, TREE_SURFACE_COUNT, 0);
	if (pending_request) {
		_update();
	}

	return RenderingServer::get_singleton()->mesh_surface_get_format(mesh, p_idx);
}

Mesh::PrimitiveType ProceduralTreeMesh::surface_get_primitive_type(int p_idx) const {
	return Mesh::PRIMITIVE_TRIANGLES;
}

void ProceduralTreeMesh::surface_set_material(int p_idx, const Ref<Material> &p_material) {
	ERR_FAIL_INDEX(p_idx, TREE_SURFACE_COUNT);

	switch (p_idx) {
		case TREE_SURFACE_TWIG:
			set_twig_material(p_material);
			break;
		case TREE_SURFACE_TRUNK:
			set_trunk_material(p_material);
			break;
		case TREE_SURFACE_COUNT:
		default:
			break;
	}
}

Ref<Material> ProceduralTreeMesh::surface_get_material(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, TREE_SURFACE_COUNT, nullptr);

	return _surfaces[p_idx].material;
}

int ProceduralTreeMesh::get_blend_shape_count() const {
	return 0;
}

StringName ProceduralTreeMesh::get_blend_shape_name(int p_index) const {
	return StringName();
}

void ProceduralTreeMesh::set_blend_shape_name(int p_index, const StringName &p_name) {
}

AABB ProceduralTreeMesh::get_aabb() const {
	if (pending_request) {
		_update();
	}

	return aabb;
}

RID ProceduralTreeMesh::get_rid() const {
	if (pending_request) {
		_update();
	}
	return mesh;
}

void ProceduralTreeMesh::set_twig_material(const Ref<Material> &p_material) {
	_surfaces[TREE_SURFACE_TWIG].material = p_material;
	if (!pending_request) {
		// just apply it, else it'll happen when _update is called.
		RenderingServer::get_singleton()->mesh_surface_set_material(mesh, TREE_SURFACE_TWIG, _surfaces[TREE_SURFACE_TWIG].material.is_null() ? RID() : _surfaces[TREE_SURFACE_TWIG].material->get_rid());
		_change_notify();
		emit_changed();
	};
}

Ref<Material> ProceduralTreeMesh::get_twig_material() const {
	return _surfaces[TREE_SURFACE_TWIG].material;
}

void ProceduralTreeMesh::set_trunk_material(const Ref<Material> &p_material) {
	_surfaces[TREE_SURFACE_TRUNK].material = p_material;
	if (!pending_request) {
		// just apply it, else it'll happen when _update is called.
		RenderingServer::get_singleton()->mesh_surface_set_material(mesh, TREE_SURFACE_TRUNK, _surfaces[TREE_SURFACE_TRUNK].material.is_null() ? RID() : _surfaces[TREE_SURFACE_TRUNK].material->get_rid());
		_change_notify();
		emit_changed();
	};
}

Ref<Material> ProceduralTreeMesh::get_trunk_material() const {
	return _surfaces[TREE_SURFACE_TRUNK].material;
}

Array ProceduralTreeMesh::get_mesh_arrays() const {
	Array arr;

	for (int i = 0; i < TREE_SURFACE_COUNT; ++i) {
		arr.push_back(surface_get_arrays(i));
	}

	return arr;
}

void ProceduralTreeMesh::set_custom_aabb(const AABB &p_custom) {
	custom_aabb = p_custom;
	RS::get_singleton()->mesh_set_custom_aabb(mesh, custom_aabb);
	emit_changed();
}

AABB ProceduralTreeMesh::get_custom_aabb() const {
	return custom_aabb;
}

ProceduralTreeMesh::ProceduralTreeMesh() {
	// defaults
	mesh = RID_PRIME(RenderingServer::get_singleton()->mesh_create());

	// make sure we do an update after we've finished constructing our object
	pending_request = true;

	// General
	_seed = 262;
	_branch_segments = 6;
	_branch_levels = 5;
	_twig_scale = 0.39;
	_trunk_forks = 5;
	_texture_v_multiplier = 0.36;

	// Branching
	_initial_length = 0.49;
	_length_falloff_rate = 0.85;
	_length_falloff_power = 0.99;
	_max_clumping = 0.454;
	_min_clumping = 0.404;
	_symmetry = 2.45;
	_droop = -0.1;
	_growth = 0.235;
	_sweep = 0.01;

	// Trunk
	_trunk_radius = 0.139;
	_radius_falloff = 0.73;
	_climb_rate = 0.371;
	_kink = 0.093;
	_taper_rate = 0.947;
	_twists = 3.02;
	_trunk_length = 2.4;
}

ProceduralTreeMesh::~ProceduralTreeMesh() {
	RenderingServer::get_singleton()->free(mesh);
}

void ProceduralTreeMesh::_bind_methods() {
  ADD_GROUP("General", "");
	ClassDB::bind_method(D_METHOD("get_seed"), &ProceduralTreeMesh::get_seed);
	ClassDB::bind_method(D_METHOD("set_seed", "value"), &ProceduralTreeMesh::set_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "seed"), "set_seed", "get_seed");

	ClassDB::bind_method(D_METHOD("get_branch_segments"), &ProceduralTreeMesh::get_branch_segments);
	ClassDB::bind_method(D_METHOD("set_branch_segments", "value"), &ProceduralTreeMesh::set_branch_segments);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "branch_segments"), "set_branch_segments", "get_branch_segments");

	ClassDB::bind_method(D_METHOD("get_branch_levels"), &ProceduralTreeMesh::get_branch_levels);
	ClassDB::bind_method(D_METHOD("set_branch_levels", "value"), &ProceduralTreeMesh::set_branch_levels);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "branch_levels"), "set_branch_levels", "get_branch_levels");

	ClassDB::bind_method(D_METHOD("get_trunk_forks"), &ProceduralTreeMesh::get_trunk_forks);
	ClassDB::bind_method(D_METHOD("set_trunk_forks", "value"), &ProceduralTreeMesh::set_trunk_forks);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "trunk_forks"), "set_trunk_forks", "get_trunk_forks");

	ClassDB::bind_method(D_METHOD("get_texture_v_multiplier"), &ProceduralTreeMesh::get_texture_v_multiplier);
	ClassDB::bind_method(D_METHOD("set_texture_v_multiplier", "value"), &ProceduralTreeMesh::set_texture_v_multiplier);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "texture_v_multiplier"), "set_texture_v_multiplier", "get_texture_v_multiplier");

	ClassDB::bind_method(D_METHOD("get_twig_scale"), &ProceduralTreeMesh::get_twig_scale);
	ClassDB::bind_method(D_METHOD("set_twig_scale", "value"), &ProceduralTreeMesh::set_twig_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "twig_scale"), "set_twig_scale", "get_twig_scale");

  ADD_GROUP("Branching", "");
	ClassDB::bind_method(D_METHOD("get_initial_length"), &ProceduralTreeMesh::get_initial_length);
	ClassDB::bind_method(D_METHOD("set_initial_length", "value"), &ProceduralTreeMesh::set_initial_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "initial_length"), "set_initial_length", "get_initial_length");

	ClassDB::bind_method(D_METHOD("get_length_falloff_rate"), &ProceduralTreeMesh::get_length_falloff_rate);
	ClassDB::bind_method(D_METHOD("set_length_falloff_rate", "value"), &ProceduralTreeMesh::set_length_falloff_rate);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "length_falloff_rate"), "set_length_falloff_rate", "get_length_falloff_rate");

	ClassDB::bind_method(D_METHOD("get_length_falloff_power"), &ProceduralTreeMesh::get_length_falloff_power);
	ClassDB::bind_method(D_METHOD("set_length_falloff_power", "value"), &ProceduralTreeMesh::set_length_falloff_power);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "length_falloff_power"), "set_length_falloff_power", "get_length_falloff_power");

	ClassDB::bind_method(D_METHOD("get_max_clumping"), &ProceduralTreeMesh::get_max_clumping);
	ClassDB::bind_method(D_METHOD("set_max_clumping", "value"), &ProceduralTreeMesh::set_max_clumping);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_clumping"), "set_max_clumping", "get_max_clumping");

	ClassDB::bind_method(D_METHOD("get_min_clumping"), &ProceduralTreeMesh::get_min_clumping);
	ClassDB::bind_method(D_METHOD("set_min_clumping", "value"), &ProceduralTreeMesh::set_min_clumping);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "min_clumping"), "set_min_clumping", "get_min_clumping");

	ClassDB::bind_method(D_METHOD("get_symmetry"), &ProceduralTreeMesh::get_symmetry);
	ClassDB::bind_method(D_METHOD("set_symmetry", "value"), &ProceduralTreeMesh::set_symmetry);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "symmetry"), "set_symmetry", "get_symmetry");

	ClassDB::bind_method(D_METHOD("get_droop"), &ProceduralTreeMesh::get_droop);
	ClassDB::bind_method(D_METHOD("set_droop", "value"), &ProceduralTreeMesh::set_droop);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "droop"), "set_droop", "get_droop");

	ClassDB::bind_method(D_METHOD("get_growth"), &ProceduralTreeMesh::get_growth);
	ClassDB::bind_method(D_METHOD("set_growth", "value"), &ProceduralTreeMesh::set_growth);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "growth"), "set_growth", "get_growth");

	ClassDB::bind_method(D_METHOD("get_sweep"), &ProceduralTreeMesh::get_sweep);
	ClassDB::bind_method(D_METHOD("set_sweep", "value"), &ProceduralTreeMesh::set_sweep);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "sweep"), "set_sweep", "get_sweep");

  ADD_GROUP("Trunk", "");
	ClassDB::bind_method(D_METHOD("get_trunk_radius"), &ProceduralTreeMesh::get_trunk_radius);
	ClassDB::bind_method(D_METHOD("set_trunk_radius", "value"), &ProceduralTreeMesh::set_trunk_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "trunk_radius"), "set_trunk_radius", "get_trunk_radius");

	ClassDB::bind_method(D_METHOD("get_radius_falloff"), &ProceduralTreeMesh::get_radius_falloff);
	ClassDB::bind_method(D_METHOD("set_radius_falloff", "value"), &ProceduralTreeMesh::set_radius_falloff);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius_falloff"), "set_radius_falloff", "get_radius_falloff");

	ClassDB::bind_method(D_METHOD("get_climb_rate"), &ProceduralTreeMesh::get_climb_rate);
	ClassDB::bind_method(D_METHOD("set_climb_rate", "value"), &ProceduralTreeMesh::set_climb_rate);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "climb_rate"), "set_climb_rate", "get_climb_rate");

	ClassDB::bind_method(D_METHOD("get_kink"), &ProceduralTreeMesh::get_kink);
	ClassDB::bind_method(D_METHOD("set_kink", "value"), &ProceduralTreeMesh::set_kink);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "kink"), "set_kink", "get_kink");

	ClassDB::bind_method(D_METHOD("get_taper_rate"), &ProceduralTreeMesh::get_taper_rate);
	ClassDB::bind_method(D_METHOD("set_taper_rate", "value"), &ProceduralTreeMesh::set_taper_rate);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "taper_rate"), "set_taper_rate", "get_taper_rate");

	ClassDB::bind_method(D_METHOD("get_twists"), &ProceduralTreeMesh::get_twists);
	ClassDB::bind_method(D_METHOD("set_twists", "value"), &ProceduralTreeMesh::set_twists);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "twists"), "set_twists", "get_twists");

	ClassDB::bind_method(D_METHOD("get_trunk_length"), &ProceduralTreeMesh::get_trunk_length);
	ClassDB::bind_method(D_METHOD("set_trunk_length", "value"), &ProceduralTreeMesh::set_trunk_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "trunk_length"), "set_trunk_length", "get_trunk_length");

  ADD_GROUP("Mesh", "");

	ClassDB::bind_method(D_METHOD("_update"), &ProceduralTreeMesh::_update);

	ClassDB::bind_method(D_METHOD("set_twig_material", "material"), &ProceduralTreeMesh::set_twig_material);
	ClassDB::bind_method(D_METHOD("get_twig_material"), &ProceduralTreeMesh::get_twig_material);

	ClassDB::bind_method(D_METHOD("set_trunk_material", "material"), &ProceduralTreeMesh::set_trunk_material);
	ClassDB::bind_method(D_METHOD("get_trunk_material"), &ProceduralTreeMesh::get_trunk_material);

	ClassDB::bind_method(D_METHOD("get_mesh_arrays"), &ProceduralTreeMesh::get_mesh_arrays);

	ClassDB::bind_method(D_METHOD("set_custom_aabb", "aabb"), &ProceduralTreeMesh::set_custom_aabb);
	ClassDB::bind_method(D_METHOD("get_custom_aabb"), &ProceduralTreeMesh::get_custom_aabb);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "twig_material", PROPERTY_HINT_RESOURCE_TYPE, "SpatialMaterial,ShaderMaterial"), "set_twig_material", "get_twig_material");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "trunk_material", PROPERTY_HINT_RESOURCE_TYPE, "SpatialMaterial,ShaderMaterial"), "set_trunk_material", "get_trunk_material");

	ADD_PROPERTY(PropertyInfo(Variant::AABB, "custom_aabb", PROPERTY_HINT_NONE, ""), "set_custom_aabb", "get_custom_aabb");

	BIND_ENUM_CONSTANT(TREE_SURFACE_TRUNK);
	BIND_ENUM_CONSTANT(TREE_SURFACE_TWIG);
	BIND_ENUM_CONSTANT(TREE_SURFACE_COUNT);
}
