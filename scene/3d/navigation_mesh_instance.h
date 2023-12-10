#ifndef NAVIGATION_MESH_INSTANCE_H
#define NAVIGATION_MESH_INSTANCE_H
/*************************************************************************/
/*  navigation_mesh_instance.h                                           */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "core/os/thread.h"
#include "scene/main/spatial.h"

class Mesh;
class Navigation;
class NavigationMesh;

class NavigationMeshInstance : public Spatial {
	GDCLASS(NavigationMeshInstance, Spatial);

	bool enabled;
	bool use_edge_connections;

	RID region;
	RID map_override;
	Ref<NavigationMesh> navigation_mesh;

	Transform current_global_transform;

	real_t enter_cost;
	real_t travel_cost;

	uint32_t navigation_layers;

	Navigation *navigation;

	bool baking_started;

#ifdef DEBUG_ENABLED
	RID debug_instance;
	RID debug_edge_connections_instance;
	Ref<ArrayMesh> debug_mesh;
	Ref<ArrayMesh> debug_edge_connections_mesh;

private:
	void _update_debug_mesh();
	void _update_debug_edge_connections_mesh();
	void _navigation_map_changed(RID p_map);
#endif // DEBUG_ENABLED

protected:
	void _notification(int p_what);
	static void _bind_methods();
	void _changed_callback(Object *p_changed, const char *p_prop);

#ifndef DISABLE_DEPRECATED
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
#endif // DISABLE_DEPRECATED

public:
	void set_enabled(bool p_enabled);
	bool is_enabled() const;

	void set_use_edge_connections(bool p_enabled);
	bool get_use_edge_connections() const;

	void set_navigation_layers(uint32_t p_navigation_layers);
	uint32_t get_navigation_layers() const;

	void set_navigation_layer_value(int p_layer_number, bool p_value);
	bool get_navigation_layer_value(int p_layer_number) const;

	void set_enter_cost(real_t p_enter_cost);
	real_t get_enter_cost() const;

	void set_travel_cost(real_t p_travel_cost);
	real_t get_travel_cost() const;

	void set_navigation_mesh(const Ref<NavigationMesh> &p_navigation_mesh);
	Ref<NavigationMesh> get_navigation_mesh() const;

	void set_navigation_map(RID p_navigation_map);
	RID get_navigation_map() const;

	RID get_region_rid() const;

	/// Bakes the navigation mesh; once done, automatically
	/// sets the new navigation mesh and emits a signal
	void bake_navigation_mesh(bool p_on_thread = true);
	void _bake_finished(Ref<NavigationMesh> p_navigation_mesh);
	void _navigation_mesh_changed();

	String get_configuration_warning() const;

	NavigationMeshInstance();
	~NavigationMeshInstance();

private:
	void _region_enter_navigation_map();
	void _region_exit_navigation_map();
	void _region_update_transform();
};

#endif // NAVIGATION_MESH_INSTANCE_H
