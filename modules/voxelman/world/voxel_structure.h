#ifndef VOXEL_STRUCTURE_H
#define VOXEL_STRUCTURE_H

/*************************************************************************/
/*  voxel_structure.h                                                    */
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

#include "core/containers/hash_map.h"
#include "core/object/resource.h"

#include "../defines.h"

#include "core/containers/pool_vector.h"
#include "core/math/aabb.h"
#include "voxel_chunk.h"

class VoxelStructure : public Resource {
	GDCLASS(VoxelStructure, Resource);

public:
	bool get_use_aabb() const;
	void set_use_aabb(const bool value);

	AABB get_chunk_aabb() const;
	void set_chunk_aabb(const AABB &value);

	int get_position_x() const;
	void set_position_x(const int value);

	int get_position_y() const;
	void set_position_y(const int value);

	int get_position_z() const;
	void set_position_z(const int value);

	void set_position(const int x, const int y, const int z);

	void write_to_chunk(Ref<VoxelChunk> chunk);

	VoxelStructure();
	~VoxelStructure();

protected:
	static void _bind_methods();

private:
	bool _use_aabb;
	AABB _chunk_aabb;

	int _position_x;
	int _position_y;
	int _position_z;
};

#endif
