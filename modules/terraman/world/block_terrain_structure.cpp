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

#include "block_terrain_structure.h"

int BlockTerrainStructure::get_channel_type() const {
	return _channel_type;
}
void BlockTerrainStructure::set_channel_type(const int value) {
	_channel_type = value;
}

int BlockTerrainStructure::get_channel_isolevel() const {
	return _channel_isolevel;
}
void BlockTerrainStructure::set_channel_isolevel(const int value) {
	_channel_isolevel = value;
}

int BlockTerrainStructure::get_voxel_type(int p_x, int p_y, int p_z) const {
	DataEntry p;

	for (int i = 0; i < _data.size(); ++i) {
		p = _data[i];

		if (p.x == p_x && p.y == p_y && p.z == p_z) {
			return p.data_type;
		}
	}

	return 0;
}
int BlockTerrainStructure::get_voxel_isolevel(int p_x, int p_y, int p_z) const {
	DataEntry p;

	for (int i = 0; i < _data.size(); ++i) {
		p = _data[i];

		if (p.x == p_x && p.y == p_y && p.z == p_z) {
			return p.data_isolevel;
		}
	}

	return 0;
}

void BlockTerrainStructure::set_voxel(int p_x, int p_y, int p_z, int p_type, int p_isolevel) {
	DataEntry p;
	p.x = p_x;
	p.y = p_y;
	p.z = p_z;
	p.data_type = p_type;
	p.data_isolevel = p_isolevel;

	_data.push_back(p);
}

void BlockTerrainStructure::_write_to_chunk(Ref<TerrainChunk> chunk) {
	//Ref<TerrainChunk> c = Object::cast_to<TerrainChunk>(chunk);
}

void BlockTerrainStructure::clear() {
	_data.clear();
}

BlockTerrainStructure::BlockTerrainStructure() {
	_channel_type = 0;
	_channel_isolevel = 0;
}

BlockTerrainStructure::~BlockTerrainStructure() {
	_data.clear();
}

void BlockTerrainStructure::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_channel_type"), &BlockTerrainStructure::get_channel_type);
	ClassDB::bind_method(D_METHOD("set_channel_type", "value"), &BlockTerrainStructure::set_channel_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_type"), "set_channel_type", "get_channel_type");

	ClassDB::bind_method(D_METHOD("get_channel_isolevel"), &BlockTerrainStructure::get_channel_isolevel);
	ClassDB::bind_method(D_METHOD("set_channel_isolevel", "value"), &BlockTerrainStructure::set_channel_isolevel);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_isolevel"), "set_channel_isolevel", "get_channel_isolevel");

	ClassDB::bind_method(D_METHOD("get_voxel_type", "x", "y", "z"), &BlockTerrainStructure::get_voxel_type);
	ClassDB::bind_method(D_METHOD("get_voxel_isolevel", "x", "y", "z"), &BlockTerrainStructure::get_voxel_isolevel);

	ClassDB::bind_method(D_METHOD("set_voxel", "x", "y", "z", "type", "isolevel"), &BlockTerrainStructure::set_voxel);

	ClassDB::bind_method(D_METHOD("_write_to_chunk", "chunk"), &BlockTerrainStructure::_write_to_chunk);
}
