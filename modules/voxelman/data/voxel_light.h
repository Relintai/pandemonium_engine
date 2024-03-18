#ifndef VOXEL_LIGHT_H
#define VOXEL_LIGHT_H

/*************************************************************************/
/*  voxel_light.h                                                        */
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

#include "core/containers/vector.h"
#include "core/math/color.h"
#include "core/object/reference.h"

class VoxelLight : public Reference {
	GDCLASS(VoxelLight, Reference);

public:
	int get_world_position_x() const;
	int get_world_position_y() const;
	int get_world_position_z() const;
	Vector3 get_world_position();
	void set_world_position(const int x, const int y, const int z);

	Vector3 get_position();
	void set_position(const Vector3 &pos);

	real_t get_range() const;
	void set_range(const real_t value);

	real_t get_attenuation() const;
	void set_attenuation(const real_t value);

	Color get_color() const;
	void set_color(const Color value);

	real_t get_energy() const;
	void set_energy(const real_t value);

	real_t get_indirect_energy() const;
	void set_indirect_energy(const real_t value);

	bool get_negative() const;
	void set_negative(const bool value);

	real_t get_specular() const;
	void set_specular(const real_t value);

	VoxelLight();
	~VoxelLight();

private:
#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
#endif

	static void _bind_methods();

private:
	int _chunk_position_x;
	int _chunk_position_y;
	int _chunk_position_z;

	int _world_position_x;
	int _world_position_y;
	int _world_position_z;

	real_t _range;
	real_t _attenuation;
	Color _color;
	real_t _energy;
	real_t _indirect_energy;
	bool _negative;
	real_t _specular;
};

#endif
