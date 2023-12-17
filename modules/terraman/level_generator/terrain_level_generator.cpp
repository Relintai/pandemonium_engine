

#include "terrain_level_generator.h"

#include "../world/terrain_chunk.h"

void TerrainLevelGenerator::generate_chunk(Ref<TerrainChunk> chunk) {
	if (has_method("_generate_chunk")) {
		call("_generate_chunk", chunk);
	}
}

TerrainLevelGenerator::TerrainLevelGenerator() {
}

TerrainLevelGenerator::~TerrainLevelGenerator() {
}

void TerrainLevelGenerator::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "TerrainChunk")));

	ClassDB::bind_method(D_METHOD("generate_chunk", "chunk"), &TerrainLevelGenerator::generate_chunk);
}
