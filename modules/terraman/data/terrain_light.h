#ifndef TERRAIN_LIGHT_H
#define TERRAIN_LIGHT_H

/*************************************************************************/
/*  terrain_light.h                                                      */
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
#include "core/object/resource.h"

#include "modules/modules_enabled.gen.h"

class TerrainLight : public Resource {
	GDCLASS(TerrainLight, Resource);

public:
	enum OwnerType {
		OWNER_TYPE_NONE = 0,
#ifdef MODULE_PROPS_ENABLED
		OWNER_TYPE_PROP,
#endif
	};

	enum LightMode {
		LIGHT_MODE_ADD = 0,
		LIGHT_MODE_SUB,
		LIGHT_MODE_MIX,
		//LIGHT_MODE_MASK
	};

	OwnerType get_owner_type() const;
	void set_owner_type(const OwnerType p_value);

	bool get_has_owner_chunk() const;
	void set_has_owner_chunk(const bool p_value);

	Vector2i get_owner_chunk_position() const;
	void set_owner_chunk_position(const Vector2i &p_owner_chunk_position);

	Vector3i get_world_data_position() const;
	void set_world_data_position(const Vector3i &p_world_data_position);

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

	LightMode get_light_mode() const;
	void set_light_mode(const LightMode value);

	int get_item_cull_mask();
	void set_item_cull_mask(const int p_item_cull_mask);

	Dictionary to_dict();
	void from_dict(const Dictionary &p_data);

	TerrainLight();
	~TerrainLight();

private:
	static void _bind_methods();

private:
	OwnerType _owner_type;

	bool _has_owner_chunk;
	Vector2i _owner_chunk_position;

	Vector3i _world_data_position;

	real_t _range;
	real_t _attenuation;
	Color _color;
	real_t _energy;
	real_t _indirect_energy;
	bool _negative;
	real_t _specular;

	LightMode _light_mode;

	int _item_cull_mask;
};

VARIANT_ENUM_CAST(TerrainLight::OwnerType);
VARIANT_ENUM_CAST(TerrainLight::LightMode);

#endif
