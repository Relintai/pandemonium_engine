
#include "procedural_tree_mesh.h"
#include "core/containers/pool_vector.h"
#include "core/object/class_db.h"
#include "core/object/object.h"

#include "servers/rendering_server.h"

#include "proctree/proctree.h"

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
float ProceduralTreeMesh::branching_get_initial_length() const {
	return _branching_initial_length;
}
void ProceduralTreeMesh::branching_set_initial_length(const float p_value) {
	_branching_initial_length = p_value;
}

float ProceduralTreeMesh::branching_get_length_falloff_rate() const {
	return _branching_length_falloff_rate;
}
void ProceduralTreeMesh::branching_set_length_falloff_rate(const float p_value) {
	_branching_length_falloff_rate = p_value;
}

float ProceduralTreeMesh::branching_get_length_falloff_power() const {
	return _branching_length_falloff_power;
}
void ProceduralTreeMesh::branching_set_length_falloff_power(const float p_value) {
	_branching_length_falloff_power = p_value;
}

float ProceduralTreeMesh::branching_get_max_clumping() const {
	return _branching_max_clumping;
}
void ProceduralTreeMesh::branching_set_max_clumping(const float p_value) {
	_branching_max_clumping = p_value;
}

float ProceduralTreeMesh::branching_get_min_clumping() const {
	return _branching_min_clumping;
}
void ProceduralTreeMesh::branching_set_min_clumping(const float p_value) {
	_branching_min_clumping = p_value;
}

float ProceduralTreeMesh::branching_get_symmetry() const {
	return _branching_symmetry;
}
void ProceduralTreeMesh::branching_set_symmetry(const float p_value) {
	_branching_symmetry = p_value;
}

float ProceduralTreeMesh::branching_get_droop() const {
	return _branching_droop;
}
void ProceduralTreeMesh::branching_set_droop(const float p_value) {
	_branching_droop = p_value;
}

float ProceduralTreeMesh::branching_get_growth() const {
	return _branching_growth;
}
void ProceduralTreeMesh::branching_set_growth(const float p_value) {
	_branching_growth = p_value;
}

float ProceduralTreeMesh::branching_get_sweep() const {
	return _branching_sweep;
}
void ProceduralTreeMesh::branching_set_sweep(const float p_value) {
	_branching_sweep = p_value;
}

// Trunk
float ProceduralTreeMesh::trunk_get_radius() const {
	return _trunk_radius;
}
void ProceduralTreeMesh::trunk_set_radius(const float p_value) {
	_trunk_radius = p_value;
}

float ProceduralTreeMesh::trunk_get_radius_falloff() const {
	return _trunk_radius_falloff;
}
void ProceduralTreeMesh::trunk_set_radius_falloff(const float p_value) {
	_trunk_radius_falloff = p_value;
}

float ProceduralTreeMesh::trunk_get_climb_rate() const {
	return _trunk_climb_rate;
}
void ProceduralTreeMesh::trunk_set_climb_rate(const float p_value) {
	_trunk_climb_rate = p_value;
}

float ProceduralTreeMesh::trunk_get_kink() const {
	return _trunk_kink;
}
void ProceduralTreeMesh::trunk_set_kink(const float p_value) {
	_trunk_kink = p_value;
}

float ProceduralTreeMesh::trunk_get_taper_rate() const {
	return _trunk_taper_rate;
}
void ProceduralTreeMesh::trunk_set_taper_rate(const float p_value) {
	_trunk_taper_rate = p_value;
}

float ProceduralTreeMesh::trunk_get_twists() const {
	return _trunk_twists;
}
void ProceduralTreeMesh::trunk_set_twists(const float p_value) {
	_trunk_twists = p_value;
}

float ProceduralTreeMesh::trunk_get_length() const {
	return _trunk_length;
}
void ProceduralTreeMesh::trunk_set_length(const float p_value) {
	_trunk_length = p_value;
}

void ProceduralTreeMesh::_update() const {

	Proctree::Tree tree;

	// Grneral
	tree.mProperties.mSeed = _seed;
	tree.mProperties.mSegments = _branch_segments;
	tree.mProperties.mLevels = _branch_levels;
	tree.mProperties.mTreeSteps = _trunk_forks;
	tree.mProperties.mVMultiplier = _texture_v_multiplier;
	tree.mProperties.mTwigScale = _twig_scale;

	// Branching
	tree.mProperties.mInitialBranchLength = _branching_initial_length;
	tree.mProperties.mLengthFalloffFactor = _branching_length_falloff_rate;
	tree.mProperties.mLengthFalloffPower = _branching_length_falloff_power;
	tree.mProperties.mClumpMax = _branching_max_clumping;
	tree.mProperties.mClumpMin = _branching_min_clumping;
	tree.mProperties.mBranchFactor = _branching_symmetry;
	tree.mProperties.mDropAmount = _branching_droop;
	tree.mProperties.mGrowAmount = _branching_growth;
	tree.mProperties.mSweepAmount = _branching_sweep;

	// Trunk
	tree.mProperties.mMaxRadius = _trunk_radius;
	tree.mProperties.mRadiusFalloffRate = _trunk_radius_falloff;
	tree.mProperties.mClimbRate = _trunk_climb_rate;
	tree.mProperties.mTrunkKink = _trunk_kink;
	tree.mProperties.mTaperRate = _trunk_taper_rate;
	tree.mProperties.mTwistRate = _trunk_twists;
	tree.mProperties.mTrunkLength = _trunk_length;

	tree.generate();


	RenderingServer::get_singleton()->mesh_clear(mesh);

	aabb = AABB();

	{
		int vert_count = tree.mTwigVertCount;

		PoolVector<Vector2> uvs;
		PoolVector<Vector3> normals;
		PoolVector<Vector3> verts;

		uvs.resize(vert_count);
		normals.resize(vert_count);
		verts.resize(vert_count);

		{
			PoolVector<Vector2>::Write uvw = uvs.write();
			PoolVector<Vector3>::Write nw = normals.write();
			PoolVector<Vector3>::Write vw = verts.write();

			for (int i = 0; i < vert_count; ++i) {
				Proctree::fvec2 tuv = tree.mTwigUV[i];
				Proctree::fvec3 tnormal = tree.mTwigNormal[i];
				Proctree::fvec3 tvert = tree.mTwigVert[i];

				uvw[i] = Vector2(tuv.u, tuv.v);
				nw[i] = Vector3(tnormal.x, tnormal.y, tnormal.z);
				vw[i] = Vector3(tvert.x, tvert.y, tvert.z);
			}
		}

	  PoolVector<int> indices;

		int face_count = tree.mTwigFaceCount;

		indices.resize(face_count * 3);

		{
			PoolVector<int>::Write iw = indices.write();

			for (int i = 0; i < face_count; ++i) {
				Proctree::ivec3 tface = tree.mTwigFace[i];

				int ind = i * 3;

				iw[ind] = tface.x;
				iw[ind + 1] = tface.y;
				iw[ind + 2] = tface.z;
			}
		}

		{
			PoolVector<Vector3>::Read r = verts.read();
			for (int i = 0; i < vert_count; i++) {
				if (i == 0) {
					aabb.position = r[i];
				} else {
					aabb.expand_to(r[i]);
				}
			}
		}

		Array arr;
		arr.resize(RS::ARRAY_MAX);
		arr[RS::ARRAY_VERTEX] = verts;
		arr[RS::ARRAY_TEX_UV] = uvs;
		arr[RS::ARRAY_NORMAL] = normals;
		arr[RS::ARRAY_INDEX] = indices;

		RenderingServer::get_singleton()->mesh_add_surface_from_arrays(mesh, RenderingServer::PRIMITIVE_TRIANGLES, arr);
		RenderingServer::get_singleton()->mesh_surface_set_material(mesh, TREE_SURFACE_TWIG, _surfaces[TREE_SURFACE_TWIG].material.is_null() ? RID() : _surfaces[TREE_SURFACE_TWIG].material->get_rid());
	}

	{
		int vert_count = tree.mVertCount;

		PoolVector<Vector2> uvs;
		PoolVector<Vector3> normals;
		PoolVector<Vector3> verts;

		uvs.resize(vert_count);
		normals.resize(vert_count);
		verts.resize(vert_count);

		{
			PoolVector<Vector2>::Write uvw = uvs.write();
			PoolVector<Vector3>::Write nw = normals.write();
			PoolVector<Vector3>::Write vw = verts.write();

			for (int i = 0; i < vert_count; ++i) {
				Proctree::fvec2 tuv = tree.mUV[i];
				Proctree::fvec3 tnormal = tree.mNormal[i];
				Proctree::fvec3 tvert = tree.mVert[i];

				uvw[i] = Vector2(tuv.u, tuv.v);
				nw[i] = Vector3(tnormal.x, tnormal.y, tnormal.z);
				vw[i] = Vector3(tvert.x, tvert.y, tvert.z);
			}
		}

	  PoolVector<int> indices;

		int face_count = tree.mFaceCount;

		indices.resize(face_count * 3);

		{
			PoolVector<int>::Write iw = indices.write();

			for (int i = 0; i < face_count; ++i) {
				Proctree::ivec3 tface = tree.mFace[i];

				int ind = i * 3;

				iw[ind] = tface.x;
				iw[ind + 1] = tface.y;
				iw[ind + 2] = tface.z;
			}
		}

		{
			PoolVector<Vector3>::Read r = verts.read();
			for (int i = 0; i < vert_count; i++) {
				//if (i == 0) {
				//	aabb.position = r[i];
				//} else {
					aabb.expand_to(r[i]);
				//}
			}
		}

		Array arr;
		arr.resize(RS::ARRAY_MAX);
		arr[RS::ARRAY_VERTEX] = verts;
		arr[RS::ARRAY_TEX_UV] = uvs;
		arr[RS::ARRAY_NORMAL] = normals;
		arr[RS::ARRAY_INDEX] = indices;
		RenderingServer::get_singleton()->mesh_add_surface_from_arrays(mesh, RenderingServer::PRIMITIVE_TRIANGLES, arr);
		RenderingServer::get_singleton()->mesh_surface_set_material(mesh, TREE_SURFACE_TRUNK, _surfaces[TREE_SURFACE_TRUNK].material.is_null() ? RID() : _surfaces[TREE_SURFACE_TRUNK].material->get_rid());
	}

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
	_branching_initial_length = 0.49;
	_branching_length_falloff_rate = 0.85;
	_branching_length_falloff_power = 0.99;
	_branching_max_clumping = 0.454;
	_branching_min_clumping = 0.404;
	_branching_symmetry = 2.45;
	_branching_droop = -0.1;
	_branching_growth = 0.235;
	_branching_sweep = 0.01;

	// Trunk
	_trunk_radius = 0.139;
	_trunk_radius_falloff = 0.73;
	_trunk_climb_rate = 0.371;
	_trunk_kink = 0.093;
	_trunk_taper_rate = 0.947;
	_trunk_twists = 3.02;
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
	ADD_PROPERTY(PropertyInfo(Variant::INT, "branch_segments", PROPERTY_HINT_RANGE, "2,32,2"), "set_branch_segments", "get_branch_segments");

	ClassDB::bind_method(D_METHOD("get_branch_levels"), &ProceduralTreeMesh::get_branch_levels);
	ClassDB::bind_method(D_METHOD("set_branch_levels", "value"), &ProceduralTreeMesh::set_branch_levels);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "branch_levels", PROPERTY_HINT_RANGE, "1,10,1"), "set_branch_levels", "get_branch_levels");

	ClassDB::bind_method(D_METHOD("get_trunk_forks"), &ProceduralTreeMesh::get_trunk_forks);
	ClassDB::bind_method(D_METHOD("set_trunk_forks", "value"), &ProceduralTreeMesh::set_trunk_forks);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "trunk_forks", PROPERTY_HINT_RANGE, "0,32,1"), "set_trunk_forks", "get_trunk_forks");

	ClassDB::bind_method(D_METHOD("get_texture_v_multiplier"), &ProceduralTreeMesh::get_texture_v_multiplier);
	ClassDB::bind_method(D_METHOD("set_texture_v_multiplier", "value"), &ProceduralTreeMesh::set_texture_v_multiplier);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "texture_v_multiplier", PROPERTY_HINT_RANGE, "0.01,10,0.01"), "set_texture_v_multiplier", "get_texture_v_multiplier");

	ClassDB::bind_method(D_METHOD("get_twig_scale"), &ProceduralTreeMesh::get_twig_scale);
	ClassDB::bind_method(D_METHOD("set_twig_scale", "value"), &ProceduralTreeMesh::set_twig_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "twig_scale", PROPERTY_HINT_RANGE, "0.01,2,0.01"), "set_twig_scale", "get_twig_scale");

  ADD_GROUP("Branching", "branching");
	ClassDB::bind_method(D_METHOD("branching_get_initial_length"), &ProceduralTreeMesh::branching_get_initial_length);
	ClassDB::bind_method(D_METHOD("branching_set_initial_length", "value"), &ProceduralTreeMesh::branching_set_initial_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "branching_initial_length", PROPERTY_HINT_RANGE, "0.01,5,0.001"), "branching_set_initial_length", "branching_get_initial_length");

	ClassDB::bind_method(D_METHOD("branching_get_length_falloff_rate"), &ProceduralTreeMesh::branching_get_length_falloff_rate);
	ClassDB::bind_method(D_METHOD("branching_set_length_falloff_rate", "value"), &ProceduralTreeMesh::branching_set_length_falloff_rate);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "branching_length_falloff_rate", PROPERTY_HINT_RANGE, "0.01,1.5,0.001"), "branching_set_length_falloff_rate", "branching_get_length_falloff_rate");

	ClassDB::bind_method(D_METHOD("branching_get_length_falloff_power"), &ProceduralTreeMesh::branching_get_length_falloff_power);
	ClassDB::bind_method(D_METHOD("branching_set_length_falloff_power", "value"), &ProceduralTreeMesh::branching_set_length_falloff_power);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "branching_length_falloff_power", PROPERTY_HINT_RANGE, "-2,2,0.001"), "branching_set_length_falloff_power", "branching_get_length_falloff_power");

	ClassDB::bind_method(D_METHOD("branching_get_max_clumping"), &ProceduralTreeMesh::branching_get_max_clumping);
	ClassDB::bind_method(D_METHOD("branching_set_max_clumping", "value"), &ProceduralTreeMesh::branching_set_max_clumping);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "branching_max_clumping", PROPERTY_HINT_RANGE, "0.01,10,0.001"), "branching_set_max_clumping", "branching_get_max_clumping");

	ClassDB::bind_method(D_METHOD("branching_get_min_clumping"), &ProceduralTreeMesh::branching_get_min_clumping);
	ClassDB::bind_method(D_METHOD("branching_set_min_clumping", "value"), &ProceduralTreeMesh::branching_set_min_clumping);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "branching_min_clumping", PROPERTY_HINT_RANGE, "0.01,10,0.001"), "branching_set_min_clumping", "branching_get_min_clumping");

	ClassDB::bind_method(D_METHOD("branching_get_symmetry"), &ProceduralTreeMesh::branching_get_symmetry);
	ClassDB::bind_method(D_METHOD("branching_set_symmetry", "value"), &ProceduralTreeMesh::branching_set_symmetry);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "branching_symmetry", PROPERTY_HINT_RANGE, "2,4,0.001"), "branching_set_symmetry", "branching_get_symmetry");

	ClassDB::bind_method(D_METHOD("branching_get_droop"), &ProceduralTreeMesh::branching_get_droop);
	ClassDB::bind_method(D_METHOD("branching_set_droop", "value"), &ProceduralTreeMesh::branching_set_droop);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "branching_droop", PROPERTY_HINT_RANGE, "-2,2,0.001"), "branching_set_droop", "branching_get_droop");

	ClassDB::bind_method(D_METHOD("branching_get_growth"), &ProceduralTreeMesh::branching_get_growth);
	ClassDB::bind_method(D_METHOD("branching_set_growth", "value"), &ProceduralTreeMesh::branching_set_growth);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "branching_growth", PROPERTY_HINT_RANGE, "-4,4,0.001"), "branching_set_growth", "branching_get_growth");

	ClassDB::bind_method(D_METHOD("branching_get_sweep"), &ProceduralTreeMesh::branching_get_sweep);
	ClassDB::bind_method(D_METHOD("branching_set_sweep", "value"), &ProceduralTreeMesh::branching_set_sweep);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "branching_sweep", PROPERTY_HINT_RANGE, "-1,1,0.001"), "branching_set_sweep", "branching_get_sweep");

  ADD_GROUP("Trunk", "trunk");
	ClassDB::bind_method(D_METHOD("trunk_get_radius"), &ProceduralTreeMesh::trunk_get_radius);
	ClassDB::bind_method(D_METHOD("trunk_set_radius", "value"), &ProceduralTreeMesh::trunk_set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "trunk_radius", PROPERTY_HINT_RANGE, "0.01,0.5,0.001"), "trunk_set_radius", "trunk_get_radius");

	ClassDB::bind_method(D_METHOD("trunk_get_radius_falloff"), &ProceduralTreeMesh::trunk_get_radius_falloff);
	ClassDB::bind_method(D_METHOD("trunk_set_radius_falloff", "value"), &ProceduralTreeMesh::trunk_set_radius_falloff);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "trunk_radius_falloff", PROPERTY_HINT_RANGE, "0.1,1,0.001"), "trunk_set_radius_falloff", "trunk_get_radius_falloff");

	ClassDB::bind_method(D_METHOD("trunk_get_climb_rate"), &ProceduralTreeMesh::trunk_get_climb_rate);
	ClassDB::bind_method(D_METHOD("trunk_set_climb_rate", "value"), &ProceduralTreeMesh::trunk_set_climb_rate);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "trunk_climb_rate", PROPERTY_HINT_RANGE, "0.01,1,0.001"), "trunk_set_climb_rate", "trunk_get_climb_rate");

	ClassDB::bind_method(D_METHOD("trunk_get_kink"), &ProceduralTreeMesh::trunk_get_kink);
	ClassDB::bind_method(D_METHOD("trunk_set_kink", "value"), &ProceduralTreeMesh::trunk_set_kink);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "trunk_kink", PROPERTY_HINT_RANGE, "-2,2,0.001"), "trunk_set_kink", "trunk_get_kink");

	ClassDB::bind_method(D_METHOD("trunk_get_taper_rate"), &ProceduralTreeMesh::trunk_get_taper_rate);
	ClassDB::bind_method(D_METHOD("trunk_set_taper_rate", "value"), &ProceduralTreeMesh::trunk_set_taper_rate);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "trunk_taper_rate", PROPERTY_HINT_RANGE, "0.5,2,0.001"), "trunk_set_taper_rate", "trunk_get_taper_rate");

	ClassDB::bind_method(D_METHOD("trunk_get_twists"), &ProceduralTreeMesh::trunk_get_twists);
	ClassDB::bind_method(D_METHOD("trunk_set_twists", "value"), &ProceduralTreeMesh::trunk_set_twists);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "trunk_twists", PROPERTY_HINT_RANGE, "0.01,10,0.001"), "trunk_set_twists", "trunk_get_twists");

	ClassDB::bind_method(D_METHOD("trunk_get_length"), &ProceduralTreeMesh::trunk_get_length);
	ClassDB::bind_method(D_METHOD("trunk_set_length", "value"), &ProceduralTreeMesh::trunk_set_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "trunk_length", PROPERTY_HINT_RANGE, "0.01,5,0.001"), "trunk_set_length", "trunk_get_length");

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
