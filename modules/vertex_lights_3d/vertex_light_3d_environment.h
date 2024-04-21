#ifndef VERTEX_LIGHT_3D_ENVIRONMENT_H
#define VERTEX_LIGHT_3D_ENVIRONMENT_H

/*************************************************************************/
/*  vertex_light_3d_environment.h                                        */
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

#include "core/containers/hash_map.h"
#include "core/containers/vector.h"
#include "core/math/color.h"
#include "core/math/vector2i.h"

class VertexLight3DEnvironment : public Spatial {
	GDCLASS(VertexLight3DEnvironment, Spatial);

public:
	bool get_is_enabled();
	void set_enabled(const bool p_enabled);

	Color get_color();
	void set_color(const Color &p_color);

	VertexLight3DEnvironment();
	~VertexLight3DEnvironment();

protected:
	void _notification(int p_what);

	void _update_light_visibility();

	static void _bind_methods();

	bool _enabled;
	Color _color;
};

#endif
