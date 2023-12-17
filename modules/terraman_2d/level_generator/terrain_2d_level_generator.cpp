

#include "terrain_2d_level_generator.h"

#include "../world/terrain_2d_chunk.h"

void Terrain2DLevelGenerator::generate_chunk(Ref<Terrain2DChunk> chunk) {
	if (has_method("_generate_chunk")) {
		call("_generate_chunk", chunk);
	}
}

Terrain2DLevelGenerator::Terrain2DLevelGenerator() {
}

Terrain2DLevelGenerator::~Terrain2DLevelGenerator() {
}

void Terrain2DLevelGenerator::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_generate_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));

	ClassDB::bind_method(D_METHOD("generate_chunk", "chunk"), &Terrain2DLevelGenerator::generate_chunk);
}
