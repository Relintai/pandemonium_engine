#ifndef NAVIGATION_LINK_3D_H
#define NAVIGATION_LINK_3D_H

/*************************************************************************/
/*  navigation_link_3d.h                                                 */
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

#include "scene/main/spatial.h"

class NavigationLink3D : public Spatial {
	GDCLASS(NavigationLink3D, Spatial);

	bool enabled = true;
	RID link;
	bool bidirectional = true;
	uint32_t navigation_layers = 1;
	Vector3 end_position;
	Vector3 start_position;
	real_t enter_cost = 0.0;
	real_t travel_cost = 1.0;

	Transform current_global_transform;

#ifdef DEBUG_ENABLED
	RID debug_instance;
	Ref<ArrayMesh> debug_mesh;

	void _update_debug_mesh();
#endif // DEBUG_ENABLED

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	NavigationLink3D();
	~NavigationLink3D();

	void set_enabled(bool p_enabled);
	bool is_enabled() const { return enabled; }

	void set_bidirectional(bool p_bidirectional);
	bool is_bidirectional() const { return bidirectional; }

	void set_navigation_layers(uint32_t p_navigation_layers);
	uint32_t get_navigation_layers() const { return navigation_layers; }

	void set_navigation_layer_value(int p_layer_number, bool p_value);
	bool get_navigation_layer_value(int p_layer_number) const;

	void set_start_position(Vector3 p_position);
	Vector3 get_start_position() const { return start_position; }

	void set_end_position(Vector3 p_position);
	Vector3 get_end_position() const { return end_position; }

	void set_global_start_position(Vector3 p_position);
	Vector3 get_global_start_position() const;

	void set_global_end_position(Vector3 p_position);
	Vector3 get_global_end_position() const;

	void set_enter_cost(real_t p_enter_cost);
	real_t get_enter_cost() const { return enter_cost; }

	void set_travel_cost(real_t p_travel_cost);
	real_t get_travel_cost() const { return travel_cost; }

	String get_configuration_warning() const;
};

#endif // NAVIGATION_LINK_3D_H
