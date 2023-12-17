#ifndef TERRAIN_TERRAIN_JOB_STEP_H
#define TERRAIN_TERRAIN_JOB_STEP_H


#include "core/object/reference.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_UTILS_ENABLED
#include "../../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

class TerrainMesherJobStep : public Reference {
	GDCLASS(TerrainMesherJobStep, Reference);

public:
	//todo add:
	//type generate lighting,
	//type skip (this would leave the mesh empty)
	//type previous mesh (this would set the previous mesh's rid to the current lod level)
	enum TerrainMesherJobStepType {
		TYPE_NORMAL = 0,
		TYPE_NORMAL_LOD,
		TYPE_DROP_UV2,
		TYPE_MERGE_VERTS,
		TYPE_BAKE_TEXTURE,
		TYPE_SIMPLIFY_MESH,
		TYPE_OTHER,
	};

	static const String BINDING_STRING_TERRAIN_TERRAIN_JOB_STEP_TYPE;

	TerrainMesherJobStepType get_job_type() const;
	void set_job_type(const TerrainMesherJobStepType value);

	int get_lod_index() const;
	void set_lod_index(const int value);

#ifdef MODULE_MESH_UTILS_ENABLED
	Ref<FastQuadraticMeshSimplifier> get_fqms();
	void set_fqms(const Ref<FastQuadraticMeshSimplifier> &val);

	float get_simplification_step_ratio() const;
	void set_simplification_step_ratio(const float value);

	int get_simplification_steps() const;
	void set_simplification_steps(const int value);

	float get_simplification_agressiveness() const;
	void set_simplification_agressiveness(const float value);
#endif

	TerrainMesherJobStep();
	~TerrainMesherJobStep();

protected:
	static void _bind_methods();

	TerrainMesherJobStepType _job_type;
	int _lod_index;

#ifdef MODULE_MESH_UTILS_ENABLED
	Ref<FastQuadraticMeshSimplifier> _fqms;
	float _simplification_step_ratio;
	int _simplification_steps;
	float _simplification_agressiveness;
#endif
};

VARIANT_ENUM_CAST(TerrainMesherJobStep::TerrainMesherJobStepType);

#endif
