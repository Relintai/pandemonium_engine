#ifndef WORLD_2D_H
#define WORLD_2D_H

/*************************************************************************/
/*  world_2d.h                                                           */
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

#include "core/config/project_settings.h"
#include "core/object/resource.h"
#include "servers/physics_2d_server.h"

class VisibilityNotifier2D;
class Viewport;
class World;
struct SpatialIndexer2D;

class World2D : public Resource {
	GDCLASS(World2D, Resource);

	RID canvas;
	RID space;
	RID navigation_map;
	RID vertex_lights_2d_map;

	SpatialIndexer2D *indexer;

protected:
	static void _bind_methods();
	friend class Viewport;
	friend class World;
	friend class VisibilityNotifier2D;

	void _register_world(World *p_world, const Rect2 &p_rect);
	void _update_world(World *p_world, const Rect2 &p_rect);
	void _remove_world(World *p_world);

	void _register_notifier(VisibilityNotifier2D *p_notifier, const Rect2 &p_rect);
	void _update_notifier(VisibilityNotifier2D *p_notifier, const Rect2 &p_rect);
	void _remove_notifier(VisibilityNotifier2D *p_notifier);

	void _update();

public:
	RID get_canvas();
	RID get_space();
	RID get_navigation_map() const;

	// TODO Maybe World should have a callback on creation?
	RID get_vertex_lights_2d_map();

	Physics2DDirectSpaceState *get_direct_space_state();

	void get_world_list(List<World *> *r_worlds);
	void get_viewport_list(List<Viewport *> *r_worlds);

	World2D();
	~World2D();
};

#endif // WORLD_2D_H
