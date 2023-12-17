#ifndef TERRAIN_MESHER_DEFAULT_H
#define TERRAIN_MESHER_DEFAULT_H


#include "core/math/color.h"

#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "../terrain_mesher.h"

class TerrainMesherDefault : public TerrainMesher {
	GDCLASS(TerrainMesherDefault, TerrainMesher);

public:
	int get_build_flags() const;
	void set_build_flags(const int flags);

	TerrainMesherDefault();
	~TerrainMesherDefault();

protected:
	virtual void _bake_colors(Ref<TerrainChunk> p_chunk);
	virtual void _bake_liquid_colors(Ref<TerrainChunk> p_chunk);

	static void _bind_methods();

private:
	int _build_flags;
};

#endif
