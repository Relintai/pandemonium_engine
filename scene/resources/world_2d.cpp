/*************************************************************************/
/*  world_2d.cpp                                                         */
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

#include "world_2d.h"

#include "core/config/project_settings.h"
#include "scene/2d/camera_2d.h"
#include "scene/2d/visibility_notifier_2d.h"
#include "scene/main/viewport.h"
#include "scene/main/world.h"
#include "servers/navigation_2d_server.h"
#include "servers/physics_2d_server.h"
#include "servers/rendering_server.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_VERTEX_LIGHTS_2D_ENABLED
#include "modules/vertex_lights_2d/vertex_lights_2d_server.h"
#endif

struct SpatialIndexer2D {
	struct CellRef {
		int ref;

		_FORCE_INLINE_ int inc() {
			ref++;
			return ref;
		}
		_FORCE_INLINE_ int dec() {
			ref--;
			return ref;
		}

		_FORCE_INLINE_ CellRef() {
			ref = 0;
		}
	};

	struct CellKey {
		union {
			struct {
				int32_t x;
				int32_t y;
			};
			uint64_t key;
		};

		bool operator==(const CellKey &p_key) const { return key == p_key.key; }
		_FORCE_INLINE_ bool operator<(const CellKey &p_key) const {
			return key < p_key.key;
		}
	};

	struct CellData {
		RBMap<VisibilityNotifier2D *, CellRef> notifiers;
	};

	RBMap<CellKey, CellData> cells;
	int cell_size;

	RBMap<VisibilityNotifier2D *, Rect2> notifiers;

	struct WorldData {
		RBMap<VisibilityNotifier2D *, uint64_t> notifiers;
		Rect2 rect;
	};

	RBMap<World *, WorldData> worlds;

	bool changed;

	uint64_t pass;

	void _notifier_update_cells(VisibilityNotifier2D *p_notifier, const Rect2 &p_rect, bool p_add) {
		Point2i begin = p_rect.position;
		begin /= cell_size;
		Point2i end = p_rect.position + p_rect.size;
		end /= cell_size;
		for (int i = begin.x; i <= end.x; i++) {
			for (int j = begin.y; j <= end.y; j++) {
				CellKey ck;
				ck.x = i;
				ck.y = j;
				RBMap<CellKey, CellData>::Element *E = cells.find(ck);

				if (p_add) {
					if (!E) {
						E = cells.insert(ck, CellData());
					}
					E->get().notifiers[p_notifier].inc();
				} else {
					ERR_CONTINUE(!E);
					if (E->get().notifiers[p_notifier].dec() == 0) {
						E->get().notifiers.erase(p_notifier);
						if (E->get().notifiers.empty()) {
							cells.erase(E);
						}
					}
				}
			}
		}
	}

	void _notifier_add(VisibilityNotifier2D *p_notifier, const Rect2 &p_rect) {
		ERR_FAIL_COND(notifiers.has(p_notifier));
		notifiers[p_notifier] = p_rect;
		_notifier_update_cells(p_notifier, p_rect, true);
		changed = true;
	}

	void _notifier_update(VisibilityNotifier2D *p_notifier, const Rect2 &p_rect) {
		RBMap<VisibilityNotifier2D *, Rect2>::Element *E = notifiers.find(p_notifier);
		ERR_FAIL_COND(!E);
		if (E->get() == p_rect) {
			return;
		}

		_notifier_update_cells(p_notifier, p_rect, true);
		_notifier_update_cells(p_notifier, E->get(), false);
		E->get() = p_rect;
		changed = true;
	}

	void _notifier_remove(VisibilityNotifier2D *p_notifier) {
		RBMap<VisibilityNotifier2D *, Rect2>::Element *E = notifiers.find(p_notifier);
		ERR_FAIL_COND(!E);
		_notifier_update_cells(p_notifier, E->get(), false);
		notifiers.erase(p_notifier);

		List<World *> removed;
		for (RBMap<World *, WorldData>::Element *F = worlds.front(); F; F = F->next()) {
			RBMap<VisibilityNotifier2D *, uint64_t>::Element *G = F->get().notifiers.find(p_notifier);

			if (G) {
				F->get().notifiers.erase(G);
				removed.push_back(F->key());
			}
		}

		while (!removed.empty()) {
			p_notifier->_exit_world(removed.front()->get());
			removed.pop_front();
		}

		changed = true;
	}

	void _add_world(World *p_world, const Rect2 &p_rect) {
		ERR_FAIL_COND(worlds.has(p_world));
		WorldData vd;
		vd.rect = p_rect;
		worlds[p_world] = vd;
		changed = true;
	}

	void _update_world(World *p_world, const Rect2 &p_rect) {
		RBMap<World *, WorldData>::Element *E = worlds.find(p_world);
		ERR_FAIL_COND(!E);
		if (E->get().rect == p_rect) {
			return;
		}
		E->get().rect = p_rect;
		changed = true;
	}

	void _remove_world(World *p_world) {
		ERR_FAIL_COND(!worlds.has(p_world));
		List<VisibilityNotifier2D *> removed;
		for (RBMap<VisibilityNotifier2D *, uint64_t>::Element *E = worlds[p_world].notifiers.front(); E; E = E->next()) {
			removed.push_back(E->key());
		}

		while (!removed.empty()) {
			removed.front()->get()->_exit_world(p_world);
			removed.pop_front();
		}

		worlds.erase(p_world);
	}

	void _update() {
		if (!changed) {
			return;
		}

		for (RBMap<World *, WorldData>::Element *E = worlds.front(); E; E = E->next()) {
			Point2i begin = E->get().rect.position;
			begin /= cell_size;
			Point2i end = E->get().rect.position + E->get().rect.size;
			end /= cell_size;
			pass++;
			List<VisibilityNotifier2D *> added;
			List<VisibilityNotifier2D *> removed;

			uint64_t visible_cells = (uint64_t)(end.x - begin.x) * (uint64_t)(end.y - begin.y);

			if (visible_cells > 10000) {
				//well you zoomed out a lot, it's your problem. To avoid freezing in the for loops below, we'll manually check cell by cell

				for (RBMap<CellKey, CellData>::Element *F = cells.front(); F; F = F->next()) {
					const CellKey &ck = F->key();

					if (ck.x < begin.x || ck.x > end.x) {
						continue;
					}
					if (ck.y < begin.y || ck.y > end.y) {
						continue;
					}

					//notifiers in cell
					for (RBMap<VisibilityNotifier2D *, CellRef>::Element *G = F->get().notifiers.front(); G; G = G->next()) {
						RBMap<VisibilityNotifier2D *, uint64_t>::Element *H = E->get().notifiers.find(G->key());
						if (!H) {
							H = E->get().notifiers.insert(G->key(), pass);
							added.push_back(G->key());
						} else {
							H->get() = pass;
						}
					}
				}

			} else {
				//check cells in grid fashion
				for (int i = begin.x; i <= end.x; i++) {
					for (int j = begin.y; j <= end.y; j++) {
						CellKey ck;
						ck.x = i;
						ck.y = j;

						RBMap<CellKey, CellData>::Element *F = cells.find(ck);
						if (!F) {
							continue;
						}

						//notifiers in cell
						for (RBMap<VisibilityNotifier2D *, CellRef>::Element *G = F->get().notifiers.front(); G; G = G->next()) {
							RBMap<VisibilityNotifier2D *, uint64_t>::Element *H = E->get().notifiers.find(G->key());
							if (!H) {
								H = E->get().notifiers.insert(G->key(), pass);
								added.push_back(G->key());
							} else {
								H->get() = pass;
							}
						}
					}
				}
			}

			for (RBMap<VisibilityNotifier2D *, uint64_t>::Element *F = E->get().notifiers.front(); F; F = F->next()) {
				if (F->get() != pass) {
					removed.push_back(F->key());
				}
			}

			while (!added.empty()) {
				added.front()->get()->_enter_world(E->key());
				added.pop_front();
			}

			while (!removed.empty()) {
				E->get().notifiers.erase(removed.front()->get());
				removed.front()->get()->_exit_world(E->key());
				removed.pop_front();
			}
		}

		changed = false;
	}

	SpatialIndexer2D() {
		pass = 0;
		changed = false;
		cell_size = GLOBAL_DEF("world/2d/cell_size", 100);
	}
};

void World2D::_register_world(World *p_world, const Rect2 &p_rect) {
	indexer->_add_world(p_world, p_rect);
}

void World2D::_update_world(World *p_world, const Rect2 &p_rect) {
	indexer->_update_world(p_world, p_rect);
}
void World2D::_remove_world(World *p_world) {
	indexer->_remove_world(p_world);
}

void World2D::_register_notifier(VisibilityNotifier2D *p_notifier, const Rect2 &p_rect) {
	indexer->_notifier_add(p_notifier, p_rect);
}
void World2D::_update_notifier(VisibilityNotifier2D *p_notifier, const Rect2 &p_rect) {
	indexer->_notifier_update(p_notifier, p_rect);
}
void World2D::_remove_notifier(VisibilityNotifier2D *p_notifier) {
	indexer->_notifier_remove(p_notifier);
}

void World2D::_update() {
	indexer->_update();
}

RID World2D::get_canvas() {
	return canvas;
}

RID World2D::get_space() {
	return space;
}

RID World2D::get_navigation_map() const {
	return navigation_map;
}

RID World2D::get_vertex_lights_2d_map() {
	return vertex_lights_2d_map;
}

void World2D::get_world_list(List<World *> *r_worlds) {
	for (RBMap<World *, SpatialIndexer2D::WorldData>::Element *E = indexer->worlds.front(); E; E = E->next()) {
		r_worlds->push_back(E->key());
	}
}

void World2D::get_viewport_list(List<Viewport *> *r_viewports) {
	for (RBMap<World *, SpatialIndexer2D::WorldData>::Element *E = indexer->worlds.front(); E; E = E->next()) {
		Viewport *w = Object::cast_to<Viewport>(E->key());

		if (w) {
			r_viewports->push_back(w);
		}
	}
}

void World2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_canvas"), &World2D::get_canvas);
	ClassDB::bind_method(D_METHOD("get_space"), &World2D::get_space);
	ClassDB::bind_method(D_METHOD("get_navigation_map"), &World2D::get_navigation_map);
	ClassDB::bind_method(D_METHOD("get_vertex_lights_2d_map"), &World2D::get_vertex_lights_2d_map);

	ClassDB::bind_method(D_METHOD("get_direct_space_state"), &World2D::get_direct_space_state);

	ADD_PROPERTY(PropertyInfo(Variant::RID, "canvas", PROPERTY_HINT_NONE, "", 0), "", "get_canvas");
	ADD_PROPERTY(PropertyInfo(Variant::RID, "space", PROPERTY_HINT_NONE, "", 0), "", "get_space");
	ADD_PROPERTY(PropertyInfo(Variant::RID, "navigation_map", PROPERTY_HINT_NONE, "", 0), "", "get_navigation_map");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "direct_space_state", PROPERTY_HINT_RESOURCE_TYPE, "Physics2DDirectSpaceState", 0), "", "get_direct_space_state");
	ADD_PROPERTY(PropertyInfo(Variant::RID, "vertex_lights_2d_map", PROPERTY_HINT_NONE, "", 0), "", "get_vertex_lights_2d_map");
}

Physics2DDirectSpaceState *World2D::get_direct_space_state() {
	return Physics2DServer::get_singleton()->space_get_direct_state(space);
}

World2D::World2D() {
	canvas = RID_PRIME(RenderingServer::get_singleton()->canvas_create());
	space = RID_PRIME(Physics2DServer::get_singleton()->space_create());

#ifdef MODULE_VERTEX_LIGHTS_2D_ENABLED
	vertex_lights_2d_map = RID_PRIME(VertexLights2DServer::get_singleton()->map_create());
#endif

	//set space2D to be more friendly with pixels than meters, by adjusting some constants
	Physics2DServer::get_singleton()->space_set_active(space, true);
	Physics2DServer::get_singleton()->area_set_param(space, Physics2DServer::AREA_PARAM_GRAVITY, GLOBAL_DEF("physics/2d/default_gravity", 98));
	Physics2DServer::get_singleton()->area_set_param(space, Physics2DServer::AREA_PARAM_GRAVITY_VECTOR, GLOBAL_DEF("physics/2d/default_gravity_vector", Vector2(0, 1)));
	Physics2DServer::get_singleton()->area_set_param(space, Physics2DServer::AREA_PARAM_LINEAR_DAMP, GLOBAL_DEF("physics/2d/default_linear_damp", 0.1));
	ProjectSettings::get_singleton()->set_custom_property_info("physics/2d/default_linear_damp", PropertyInfo(Variant::REAL, "physics/2d/default_linear_damp", PROPERTY_HINT_RANGE, "-1,100,0.001,or_greater"));
	Physics2DServer::get_singleton()->area_set_param(space, Physics2DServer::AREA_PARAM_ANGULAR_DAMP, GLOBAL_DEF("physics/2d/default_angular_damp", 1.0));
	ProjectSettings::get_singleton()->set_custom_property_info("physics/2d/default_angular_damp", PropertyInfo(Variant::REAL, "physics/2d/default_angular_damp", PROPERTY_HINT_RANGE, "-1,100,0.001,or_greater"));

	// Create default navigation map
	navigation_map = Navigation2DServer::get_singleton()->map_create();
	Navigation2DServer::get_singleton()->map_set_active(navigation_map, true);
	Navigation2DServer::get_singleton()->map_set_cell_size(navigation_map, GLOBAL_DEF("navigation/2d/default_cell_size", 1.0));
	Navigation2DServer::get_singleton()->map_set_edge_connection_margin(navigation_map, GLOBAL_DEF("navigation/2d/default_edge_connection_margin", 1.0));
	Navigation2DServer::get_singleton()->map_set_link_connection_radius(navigation_map, GLOBAL_DEF("navigation/2d/default_link_connection_radius", 4.0));
	Navigation2DServer::get_singleton()->map_set_use_edge_connections(navigation_map, GLOBAL_DEF("navigation/2d/use_edge_connections", true));

	indexer = memnew(SpatialIndexer2D);
}

World2D::~World2D() {
	RenderingServer::get_singleton()->free(canvas);
	Physics2DServer::get_singleton()->free(space);
	Navigation2DServer::get_singleton()->free(navigation_map);

#ifdef MODULE_VERTEX_LIGHTS_2D_ENABLED
	VertexLights2DServer::get_singleton()->free(vertex_lights_2d_map);
#endif

	memdelete(indexer);
}
