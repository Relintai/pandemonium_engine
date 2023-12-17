#ifndef TERRAIN_JOB_H
#define TERRAIN_JOB_H


#include "scene/resources/texture.h"

#include "core/os/thread_pool_job.h"

#include "../../defines.h"

class TerrainChunk;

class TerrainJob : public ThreadPoolJob {
	GDCLASS(TerrainJob, ThreadPoolJob);

public:
	static const String BINDING_STRING_ACTIVE_BUILD_PHASE_TYPE;

	enum ActiveBuildPhaseType {
		BUILD_PHASE_TYPE_NORMAL = 0,
		BUILD_PHASE_TYPE_PROCESS,
		BUILD_PHASE_TYPE_PHYSICS_PROCESS,
	};

public:
	ActiveBuildPhaseType get_build_phase_type();
	void set_build_phase_type(TerrainJob::ActiveBuildPhaseType build_phase_type);

	void set_chunk(const Ref<TerrainChunk> &chunk);

	int get_phase();
	void set_phase(const int phase);
	void next_phase();

	bool get_build_done();
	void set_build_done(const bool val);

	void next_job();

	void reset();
	virtual void _reset();

	void _execute();

	void execute_phase();
	virtual void _execute_phase();

	void process(const float delta);
	void physics_process(const float delta);

	void generate_ao();
	void generate_random_ao(int seed, int octaves = 4, int period = 30, float persistence = 0.3, float scale_factor = 0.6);
	Array merge_mesh_array(Array arr) const;
	Array bake_mesh_array_uv(Array arr, Ref<Texture> tex, float mul_color = 0.7) const;

	void chunk_exit_tree();

	TerrainJob();
	~TerrainJob();

protected:
	static void _bind_methods();

	ActiveBuildPhaseType _build_phase_type;
	bool _build_done;
	int _phase;
	bool _in_tree;
	Ref<TerrainChunk> _chunk;
};

VARIANT_ENUM_CAST(TerrainJob::ActiveBuildPhaseType);

#endif
