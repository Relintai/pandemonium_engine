#ifndef TERRAIN_2D_MESHER_DEFAULT_H
#define TERRAIN_2D_MESHER_DEFAULT_H


#include "core/math/color.h"

#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "../terrain_2d_mesher.h"

class Terrain2DMesherDefault : public Terrain2DMesher {
	GDCLASS(Terrain2DMesherDefault, Terrain2DMesher);

public:
	int get_build_flags() const;
	void set_build_flags(const int flags);

	void create_tile_colliders(Ref<Terrain2DChunk> chunk);

	Terrain2DMesherDefault();
	~Terrain2DMesherDefault();

protected:
	virtual void _bake_colors(Ref<Terrain2DChunk> p_chunk);
	virtual void _bake_liquid_colors(Ref<Terrain2DChunk> p_chunk);

	static void _bind_methods();

private:
	int _build_flags;
};

#endif
