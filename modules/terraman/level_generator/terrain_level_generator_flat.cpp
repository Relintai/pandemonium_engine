/*
Copyright (c) 2019-2023 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
