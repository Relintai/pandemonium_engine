/*************************************************************************/
/*  world_environment_3d.cpp                                             */
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

#include "world_environment_3d.h"
#include "scene/main/spatial.h"
#include "scene/main/viewport.h"

#include "scene/resources/environment_3d.h"
#include "scene/resources/world_3d.h"

void WorldEnvironment3D::_notification(int p_what) {
	if (p_what == Spatial::NOTIFICATION_ENTER_WORLD || p_what == Spatial::NOTIFICATION_ENTER_TREE) {
		if (environment.is_valid()) {
			if (get_viewport()->find_world_3d()->get_environment().is_valid()) {
				WARN_PRINT("World already has an environment (Another WorldEnvironment3D?), overriding.");
			}
			get_viewport()->find_world_3d()->set_environment(environment);
			add_to_group("_world_environment_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
		}

	} else if (p_what == Spatial::NOTIFICATION_EXIT_WORLD || p_what == Spatial::NOTIFICATION_EXIT_TREE) {
		if (environment.is_valid() && get_viewport()->find_world_3d()->get_environment() == environment) {
			get_viewport()->find_world_3d()->set_environment(Ref<Environment3D>());
			remove_from_group("_world_environment_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
		}
	}
}

void WorldEnvironment3D::set_environment(const Ref<Environment3D> &p_environment) {
	if (is_inside_tree() && environment.is_valid() && get_viewport()->find_world_3d()->get_environment() == environment) {
		get_viewport()->find_world_3d()->set_environment(Ref<Environment3D>());
		remove_from_group("_world_environment_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
		//clean up
	}

	environment = p_environment;
	if (is_inside_tree() && environment.is_valid()) {
		if (get_viewport()->find_world_3d()->get_environment().is_valid()) {
			WARN_PRINT("World already has an environment (Another WorldEnvironment3D?), overriding.");
		}
		get_viewport()->find_world_3d()->set_environment(environment);
		add_to_group("_world_environment_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()));
	}

	update_configuration_warning();
}

Ref<Environment3D> WorldEnvironment3D::get_environment() const {
	return environment;
}

String WorldEnvironment3D::get_configuration_warning() const {
	String warning = Node::get_configuration_warning();
	if (!environment.is_valid()) {
		if (warning != String()) {
			warning += "\n\n";
		}
		warning += TTR("WorldEnvironment3D requires its \"Environment3D\" property to contain an Environment3D to have a visible effect.");
		return warning;
	}

	if (/*!is_visible_in_tree() ||*/ !is_inside_tree()) {
		return String();
	}

	List<Node *> nodes;
	get_tree()->get_nodes_in_group("_world_environment_" + itos(get_viewport()->find_world_3d()->get_scenario().get_id()), &nodes);

	if (nodes.size() > 1) {
		if (warning != String()) {
			warning += "\n\n";
		}
		warning += TTR("Only one WorldEnvironment3D is allowed per scene (or set of instanced scenes).");
	}

	// Commenting this warning for now, I think it makes no sense. If anyone can figure out what its supposed to do, feedback welcome. Else it should be deprecated.
	//if (environment.is_valid() && get_viewport() && !get_viewport()->get_camera() && environment->get_background() != Environment3D::BG_CANVAS) {
	//	return TTR("This WorldEnvironment3D is ignored. Either add a Camera (for 3D scenes) or set this environment's Background Mode to Canvas (for 2D scenes).");
	//}

	return warning;
}

void WorldEnvironment3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_environment", "env"), &WorldEnvironment3D::set_environment);
	ClassDB::bind_method(D_METHOD("get_environment"), &WorldEnvironment3D::get_environment);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "environment", PROPERTY_HINT_RESOURCE_TYPE, "Environment3D"), "set_environment", "get_environment");
}

WorldEnvironment3D::WorldEnvironment3D() {
}
