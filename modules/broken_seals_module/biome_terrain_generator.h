#ifndef BIOME_TERRAIN_GENERATOR_H
#define BIOME_TERRAIN_GENERATOR_H

/*************************************************************************/
/*  biome_terrain_generator.h                                            */
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



#include "core/object/reference.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_VOXELMAN_ENABLED
class VoxelChunk;
#endif

class BiomeTerrainGenerator : public Reference {
	GDCLASS(BiomeTerrainGenerator, Reference);

public:
	int get_current_seed();
	void set_current_seed(int value);

#ifdef MODULE_VOXELMAN_ENABLED
	void generate_simple_terrarin(Ref<VoxelChunk> chunk, bool spawn_mobs);
#endif

	BiomeTerrainGenerator();
	~BiomeTerrainGenerator();

protected:
	static void _bind_methods();

private:
	int _current_seed;
};

#endif
