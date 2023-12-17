#ifndef TERRAIN_WORLD_DEFAULT_H
#define TERRAIN_WORLD_DEFAULT_H


#include "../terrain_world.h"

class TerrainWorldDefault : public TerrainWorld {
	GDCLASS(TerrainWorldDefault, TerrainWorld);

public:
	int get_build_flags() const;
	void set_build_flags(const int flags);

	float get_lod_update_interval() const;
	void set_lod_update_interval(const float value);

	int get_chunk_lod_first_falloff() const;
	void set_chunk_lod_first_falloff(const int value);

	int get_chunk_lod_falloff() const;
	void set_chunk_lod_falloff(const int value);

	int get_num_lods() const;
	void set_num_lods(const int value);

	void update_lods();

	PoolColorArray get_vertex_colors(const Transform &transform, const PoolVector3Array &vertices, const float base_light_value = 0.45, const float ao_strength = 0.2);

	TerrainWorldDefault();
	~TerrainWorldDefault();

protected:
	void _notification(int p_what);
	void _update_lods();
	Ref<TerrainChunk> _create_chunk(int x, int z, Ref<TerrainChunk> p_chunk);
	virtual void _chunk_added(Ref<TerrainChunk> chunk);
	int _get_channel_index_info(const ChannelTypeInfo channel_type);

	//virtual void _notification(int p_what);

	static void _bind_methods();

private:
	int _build_flags;
	float _lod_update_timer;
	float _lod_update_interval;
	int _chunk_lod_first_falloff;
	int _chunk_lod_falloff;
	int _num_lods;
};

#endif
