#ifndef PROCEDURAL_TREE_3D_MESH_H
#define PROCEDURAL_TREE_3D_MESH_H

#include "scene/resources/mesh/mesh.h"

class ProceduralTreeMesh : public Mesh {
	GDCLASS(ProceduralTreeMesh, Mesh);

public:
	// General
	int get_seed() const;
	void set_seed(const int p_value);

	int get_branch_segments() const;
	void set_branch_segments(const int p_value);

	int get_branch_levels() const;
	void set_branch_levels(const int p_value);

	int get_trunk_forks() const;
	void set_trunk_forks(const int p_value);

	float get_texture_v_multiplier() const;
	void set_texture_v_multiplier(const float p_value);

	float get_twig_scale() const;
	void set_twig_scale(const float p_value);

	// Branching
	float get_initial_length() const;
	void set_initial_length(const float p_value);

	float get_length_falloff_rate() const;
	void set_length_falloff_rate(const float p_value);

	float get_length_falloff_power() const;
	void set_length_falloff_power(const float p_value);

	float get_max_clumping() const;
	void set_max_clumping(const float p_value);

	float get_min_clumping() const;
	void set_min_clumping(const float p_value);

	float get_symmetry() const;
	void set_symmetry(const float p_value);

	float get_droop() const;
	void set_droop(const float p_value);

	float get_growth() const;
	void set_growth(const float p_value);

	float get_sweep() const;
	void set_sweep(const float p_value);

	// Trunk
	float get_trunk_radius() const;
	void set_trunk_radius(const float p_value);

	float get_radius_falloff() const;
	void set_radius_falloff(const float p_value);

	float get_climb_rate() const;
	void set_climb_rate(const float p_value);

	float get_kink() const;
	void set_kink(const float p_value);

	float get_taper_rate() const;
	void set_taper_rate(const float p_value);

	float get_twists() const;
	void set_twists(const float p_value);

	float get_trunk_length() const;
	void set_trunk_length(const float p_value);

public:
	enum TreeSurfaces {
		TREE_SURFACE_TWIG = 0,
		TREE_SURFACE_TRUNK = 1,
		TREE_SURFACE_COUNT,
	};

	virtual int get_surface_count() const;
	virtual int surface_get_array_len(int p_idx) const;
	virtual int surface_get_array_index_len(int p_idx) const;
	virtual Array surface_get_arrays(int p_surface) const;
	virtual Array surface_get_blend_shape_arrays(int p_surface) const;
	virtual uint32_t surface_get_format(int p_idx) const;
	virtual Mesh::PrimitiveType surface_get_primitive_type(int p_idx) const;
	virtual void surface_set_material(int p_idx, const Ref<Material> &p_material);
	virtual Ref<Material> surface_get_material(int p_idx) const;
	virtual int get_blend_shape_count() const;
	virtual StringName get_blend_shape_name(int p_index) const;
	virtual void set_blend_shape_name(int p_index, const StringName &p_name);
	virtual AABB get_aabb() const;
	virtual RID get_rid() const;

	void set_twig_material(const Ref<Material> &p_material);
	Ref<Material> get_twig_material() const;

	void set_trunk_material(const Ref<Material> &p_material);
	Ref<Material> get_trunk_material() const;

	Array get_mesh_arrays() const;

	void set_custom_aabb(const AABB &p_custom);
	AABB get_custom_aabb() const;

	ProceduralTreeMesh();
	~ProceduralTreeMesh();

protected:
	static void _bind_methods();

	void _request_update();

private:
	// General
	int _seed; //mProperties.mSeed
	int _branch_segments; //mProperties.mSegments
	int _branch_levels; //mProperties.mLevels
	int _trunk_forks; //mProperties.mTreeSteps
	float _texture_v_multiplier; //mProperties.mVMultiplier
	float _twig_scale; //mProperties.mTwigScale

	// Branching
	float _initial_length; //mProperties.mInitialBranchLength
	float _length_falloff_rate; //mProperties.mLengthFalloffFactor
	float _length_falloff_power; //mProperties.mLengthFalloffPower
	float _max_clumping; //mProperties.mClumpMax
	float _min_clumping; //mProperties.mClumpMin
	float _symmetry; //mProperties.mBranchFactor
	float _droop; //mProperties.mDropAmount
	float _growth; //mProperties.mGrowAmount
	float _sweep; //mProperties.mSweepAmount

	// Trunk
	float _trunk_radius; //mProperties.mMaxRadius
	float _radius_falloff; //mProperties.mRadiusFalloffRate
	float _climb_rate; //mProperties.mClimbRate
	float _kink; //mProperties.mTrunkKink
	float _taper_rate; //mProperties.mTaperRate
	float _twists; //mProperties.mTwistRate
	float _trunk_length; //mProperties.mTrunkLength

private:
	RID mesh;

	struct TreeSurface {
		Ref<Material> material;
	};

	TreeSurface _surfaces[TREE_SURFACE_COUNT];

	mutable AABB aabb;
	AABB custom_aabb;

	mutable bool pending_request;
	void _update() const;
};

VARIANT_ENUM_CAST(ProceduralTreeMesh::TreeSurfaces);

#endif
