

#include "terrain_2d_chunk_isometric.h"

#include "../../defines.h"

Terrain2DChunkIsometric::Terrain2DChunkIsometric() {
}

Terrain2DChunkIsometric::~Terrain2DChunkIsometric() {
}

void Terrain2DChunkIsometric::_setup_channels() {
	channel_set_count(MAX_DEFAULT_CHANNELS);
}

void Terrain2DChunkIsometric::_bind_methods() {
	ADD_PROPERTYI(PropertyInfo(Variant::POOL_BYTE_ARRAY, "data_channel"), "channel_set_compressed", "channel_get_compressed", 0);
}
