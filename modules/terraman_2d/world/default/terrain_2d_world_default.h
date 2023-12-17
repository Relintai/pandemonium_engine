#ifndef TERRAIN_2D_WORLD_DEFAULT_H
#define TERRAIN_2D_WORLD_DEFAULT_H


#include "../terrain_2d_world.h"

class Terrain2DWorldDefault : public Terrain2DWorld {
	GDCLASS(Terrain2DWorldDefault, Terrain2DWorld);

public:
	int get_build_flags() const;
	void set_build_flags(const int flags);

	PoolColorArray get_vertex_colors(const Transform2D &transform, const PoolVector2Array &vertices, const float base_light_value = 0.45, const float ao_strength = 0.2);

	Terrain2DWorldDefault();
	~Terrain2DWorldDefault();

protected:
	void _notification(int p_what);
	Ref<Terrain2DChunk> _create_chunk(int x, int y, Ref<Terrain2DChunk> p_chunk);
	virtual void _chunk_added(Ref<Terrain2DChunk> chunk);
	int _get_channel_index_info(const ChannelTypeInfo channel_type);

	//virtual void _notification(int p_what);

	static void _bind_methods();

private:
	int _build_flags;
};

#endif
