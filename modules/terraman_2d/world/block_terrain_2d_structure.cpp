/*************************************************************************/
/*  block_terrain_2d_structure.cpp                                       */
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

#include "block_terrain_2d_structure.h"

int BlockTerrain2DStructure::get_channel_type() const {
	return _channel_type;
}
void BlockTerrain2DStructure::set_channel_type(const int value) {
	_channel_type = value;
}

int BlockTerrain2DStructure::get_voxel_type(int p_x, int p_y) const {
	DataEntry p;

	for (int i = 0; i < _data.size(); ++i) {
		p = _data[i];

		if (p.x == p_x && p.y == p_y) {
			return p.data_type;
		}
	}

	return 0;
}

void BlockTerrain2DStructure::set_voxel(int p_x, int p_y, int p_type, int p_isolevel) {
	DataEntry p;
	p.x = p_x;
	p.y = p_y;
	p.data_type = p_type;
	p.data_isolevel = p_isolevel;

	_data.push_back(p);
}

void BlockTerrain2DStructure::_write_to_chunk(Ref<Terrain2DChunk> chunk) {
	//Ref<Terrain2DChunk> c = Object::cast_to<Terrain2DChunk>(chunk);
}

void BlockTerrain2DStructure::clear() {
	_data.clear();
}

BlockTerrain2DStructure::BlockTerrain2DStructure() {
	_channel_type = 0;
	_channel_isolevel = 0;
}

BlockTerrain2DStructure::~BlockTerrain2DStructure() {
	_data.clear();
}

void BlockTerrain2DStructure::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_channel_type"), &BlockTerrain2DStructure::get_channel_type);
	ClassDB::bind_method(D_METHOD("set_channel_type", "value"), &BlockTerrain2DStructure::set_channel_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_type"), "set_channel_type", "get_channel_type");

	ClassDB::bind_method(D_METHOD("get_voxel_type", "x", "y"), &BlockTerrain2DStructure::get_voxel_type);

	ClassDB::bind_method(D_METHOD("set_voxel", "x", "y", "type", "isolevel"), &BlockTerrain2DStructure::set_voxel);

	ClassDB::bind_method(D_METHOD("_write_to_chunk", "chunk"), &BlockTerrain2DStructure::_write_to_chunk);
}
