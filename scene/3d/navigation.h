#ifndef NAVIGATION_H
#define NAVIGATION_H

/*************************************************************************/
/*  navigation.h                                                         */
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

class Navigation : public Spatial {
	GDCLASS(Navigation, Spatial);

	RID map;

	Vector3 up;
	real_t cell_size;
	real_t cell_height;
	real_t edge_connection_margin;

	uint32_t navigation_layers;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	RID get_rid() const {
		return map;
	}

	void set_up_vector(const Vector3 &p_up);
	Vector3 get_up_vector() const;

	void set_cell_size(float p_cell_size);
	float get_cell_size() const {
		return cell_size;
	}

	void set_cell_height(float p_cell_height);
	float get_cell_height() const {
		return cell_height;
	}

	void set_navigation_layers(uint32_t p_navigation_layers);
	uint32_t get_navigation_layers() const;

	void set_edge_connection_margin(float p_edge_connection_margin);
	float get_edge_connection_margin() const {
		return edge_connection_margin;
	}

	Vector<Vector3> get_simple_path(const Vector3 &p_start, const Vector3 &p_end, bool p_optimize = true) const;
	Vector3 get_closest_point_to_segment(const Vector3 &p_from, const Vector3 &p_to, bool p_use_collision = false) const;
	Vector3 get_closest_point(const Vector3 &p_point) const;
	Vector3 get_closest_point_normal(const Vector3 &p_point) const;
	RID get_closest_point_owner(const Vector3 &p_point) const;

	Navigation();
	~Navigation();
};

#endif // NAVIGATION_H
