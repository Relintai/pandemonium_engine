/*************************************************************************/
/*  terrain_2d_level_generator_flat.cpp                                  */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "terrain_2d_level_generator_flat.h"

#include "../world/terrain_2d_chunk.h"

int Terrain2DLevelGeneratorFlat::get_floor_position() const {
	return _floor_position;
}
void Terrain2DLevelGeneratorFlat::set_floor_position(const int floor_height) {
	_floor_position = floor_height;
}

Dictionary Terrain2DLevelGeneratorFlat::get_channel_map() {
	return _channel_map;
}
void Terrain2DLevelGeneratorFlat::set_channel_map(const Dictionary &map) {
	_channel_map = map;
}

void Terrain2DLevelGeneratorFlat::_generate_chunk(Ref<Terrain2DChunk> chunk) {
	const Variant *key = NULL;
	while ((key = _channel_map.next(key))) {
		int k = *key;
		int value = _channel_map[*key];

		chunk->channel_fill(value, k);
	}
}

Terrain2DLevelGeneratorFlat::Terrain2DLevelGeneratorFlat() {
	_floor_position = 0;
}

Terrain2DLevelGeneratorFlat::~Terrain2DLevelGeneratorFlat() {
	_channel_map.clear();
}

void Terrain2DLevelGeneratorFlat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_floor_position"), &Terrain2DLevelGeneratorFlat::get_floor_position);
	ClassDB::bind_method(D_METHOD("set_floor_position", "value"), &Terrain2DLevelGeneratorFlat::set_floor_position);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "floor_position"), "set_floor_position", "get_floor_position");

	ClassDB::bind_method(D_METHOD("get_channel_map"), &Terrain2DLevelGeneratorFlat::get_channel_map);
	ClassDB::bind_method(D_METHOD("set_channel_map", "value"), &Terrain2DLevelGeneratorFlat::set_channel_map);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "channel_map"), "set_channel_map", "get_channel_map");

	ClassDB::bind_method(D_METHOD("_generate_chunk", "chunk"), &Terrain2DLevelGeneratorFlat::_generate_chunk);
}
