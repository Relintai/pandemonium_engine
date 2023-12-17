#ifndef PROP_2D_DATA_LIGHT_H
#define PROP_2D_DATA_LIGHT_H

/*************************************************************************/
/*  prop_2d_data_light.h                                                 */
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

#include "prop_2d_data_entry.h"

class Prop2DDataLight : public Prop2DDataEntry {
	GDCLASS(Prop2DDataLight, Prop2DDataEntry);

public:
	Color get_light_color() const;
	void set_light_color(const Color value);

	int get_light_size_x() const;
	void set_light_size_x(const int value);

	int get_light_size_y() const;
	void set_light_size_y(const int value);

	float get_texture_scale() const;
	void set_texture_scale(const float value);

	float get_energy() const;
	void set_energy(const float value);

	Ref<Texture> get_texture() const;
	void set_texture(const Ref<Texture> value);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry = Ref<Prop2DDataEntry>());
	Node *_processor_get_node_for(const Transform2D &transform, Node *node = nullptr);

	Prop2DDataLight();
	~Prop2DDataLight();

protected:
	static void _bind_methods();

private:
	Color _light_color;
	int _light_size_x;
	int _light_size_y;
	float _texture_scale;
	float _energy;
	Ref<Texture> _texture;
};

#endif
