#ifndef TERRAIN_MESHER_BLOCKY_H
#define TERRAIN_MESHER_BLOCKY_H


#include "core/math/color.h"

#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "../../world/default/terrain_chunk_default.h"
#include "../default/terrain_mesher_default.h"

class TerrainMesherBlocky : public TerrainMesherDefault {
	GDCLASS(TerrainMesherBlocky, TerrainMesherDefault);

public:
	bool get_always_add_colors() const;
	void set_always_add_colors(const bool value);

	void _add_chunk(Ref<TerrainChunk> p_chunk);

	void add_chunk_normal(Ref<TerrainChunkDefault> chunk);

	void add_chunk_lod(Ref<TerrainChunkDefault> chunk);
	void create_margin_zmin(Ref<TerrainChunkDefault> chunk);
	void create_margin_zmax(Ref<TerrainChunkDefault> chunk);
	void create_margin_xmin(Ref<TerrainChunkDefault> chunk);
	void create_margin_xmax(Ref<TerrainChunkDefault> chunk);
	void create_margin_corners(Ref<TerrainChunkDefault> chunk);
	void create_face(Ref<TerrainChunkDefault> chunk, int dataxmin, int dataxmax, int datazmin, int datazmax);

	TerrainMesherBlocky();
	~TerrainMesherBlocky();

protected:
	static void _bind_methods();

private:
	bool _always_add_colors;
};

#endif
