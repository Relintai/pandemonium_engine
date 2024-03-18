#ifndef PROP_DATA_LIGHT_H
#define PROP_DATA_LIGHT_H

/*************************************************************************/
/*  prop_data_light.h                                                    */
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

#include "core/math/color.h"

#include "prop_data_entry.h"

class PropDataLight : public PropDataEntry {
	GDCLASS(PropDataLight, PropDataEntry);

public:
	real_t get_light_range() const;
	void set_light_range(const real_t value);

	real_t get_light_attenuation() const;
	void set_light_attenuation(const real_t value);

	Color get_light_color() const;
	void set_light_color(const Color value);

	real_t get_light_energy() const;
	void set_light_energy(const real_t value);

	real_t get_light_indirect_energy() const;
	void set_light_indirect_energy(const real_t value);

	bool get_light_negative() const;
	void set_light_negative(const bool value);

	real_t get_light_specular() const;
	void set_light_specular(const real_t value);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	Node *_processor_get_node_for(const Transform &transform);

	PropDataLight();
	~PropDataLight();

protected:
#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
#endif

	static void _bind_methods();

private:
	real_t _light_range;
	real_t _light_attenuation;
	Color _light_color;
	real_t _light_energy;
	real_t _light_indirect_energy;
	bool _light_negative;
	real_t _light_specular;
};

#endif
