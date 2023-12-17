

#include "terrain_chunk_blocky.h"

#include "../../defines.h"

TerrainChunkBlocky::TerrainChunkBlocky() {
}

TerrainChunkBlocky::~TerrainChunkBlocky() {
}

void TerrainChunkBlocky::_setup_channels() {
	channel_set_count(MAX_DEFAULT_CHANNELS);
}

void TerrainChunkBlocky::_bind_methods() {
	ADD_PROPERTYI(PropertyInfo(Variant::POOL_BYTE_ARRAY, "data_channel"), "channel_set_compressed", "channel_get_compressed", 0);
	ADD_PROPERTYI(PropertyInfo(Variant::POOL_BYTE_ARRAY, "isolevel_channel"), "channel_set_compressed", "channel_get_compressed", 1);

	//ClassDB::bind_method(D_METHOD("get_channel_compressed", "channel_index"), &TerrainChunk::get_channel_compressed);
	//ClassDB::bind_method(D_METHOD("set_channel_compressed", "channel_index", "array"), &TerrainChunk::set_channel_compressed);
}
