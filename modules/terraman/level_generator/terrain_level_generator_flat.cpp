

#include "terrain_level_generator_flat.h"

#include "../world/terrain_chunk.h"

int TerrainLevelGeneratorFlat::get_floor_position() const {
	return _floor_position;
}
void TerrainLevelGeneratorFlat::set_floor_position(const int floor_height) {
	_floor_position = floor_height;
}

Dictionary TerrainLevelGeneratorFlat::get_channel_map() {
	return _channel_map;
}
void TerrainLevelGeneratorFlat::set_channel_map(const Dictionary &map) {
	_channel_map = map;
}

void TerrainLevelGeneratorFlat::_generate_chunk(Ref<TerrainChunk> chunk) {
	const Variant *key = NULL;
	while ((key = _channel_map.next(key))) {
		int k = *key;
		int value = _channel_map[*key];

		chunk->channel_fill(value, k);
	}
}

TerrainLevelGeneratorFlat::TerrainLevelGeneratorFlat() {
	_floor_position = 0;
}

TerrainLevelGeneratorFlat::~TerrainLevelGeneratorFlat() {
	_channel_map.clear();
}

void TerrainLevelGeneratorFlat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_floor_position"), &TerrainLevelGeneratorFlat::get_floor_position);
	ClassDB::bind_method(D_METHOD("set_floor_position", "value"), &TerrainLevelGeneratorFlat::set_floor_position);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "floor_position"), "set_floor_position", "get_floor_position");

	ClassDB::bind_method(D_METHOD("get_channel_map"), &TerrainLevelGeneratorFlat::get_channel_map);
	ClassDB::bind_method(D_METHOD("set_channel_map", "value"), &TerrainLevelGeneratorFlat::set_channel_map);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "channel_map"), "set_channel_map", "get_channel_map");

	ClassDB::bind_method(D_METHOD("_generate_chunk", "chunk"), &TerrainLevelGeneratorFlat::_generate_chunk);
}
