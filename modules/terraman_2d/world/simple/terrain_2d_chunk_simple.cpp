

#include "terrain_2d_chunk_simple.h"

#include "../../defines.h"

Terrain2DChunkSimple::Terrain2DChunkSimple() {
}

Terrain2DChunkSimple::~Terrain2DChunkSimple() {
}

void Terrain2DChunkSimple::_setup_channels() {
	channel_set_count(MAX_DEFAULT_CHANNELS);
}

void Terrain2DChunkSimple::_bind_methods() {
	ADD_PROPERTYI(PropertyInfo(Variant::POOL_BYTE_ARRAY, "data_channel"), "channel_set_compressed", "channel_get_compressed", 0);
}
