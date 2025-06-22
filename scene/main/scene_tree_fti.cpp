/**************************************************************************/
/*  scene_tree_fti.cpp                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef _3D_DISABLED

#include "scene_tree_fti.h"
#include "core/config/engine.h"
#include "core/error/error_macros.h"
#include "core/math/transform_interpolator.h"
#include "core/os/os.h"
#include "scene/main/spatial.h"
#include "scene/3d/visual_instance.h"
#include "core/config/project_settings.h"

#ifdef GODOT_SCENE_TREE_FTI_VERIFY
#include "scene_tree_fti_tests.h"
#endif

#ifdef DEV_ENABLED

// Uncomment this to enable some slow extra DEV_ENABLED
// checks to ensure there aren't more than one object added to the lists.
// #define GODOT_SCENE_TREE_FTI_EXTRA_CHECKS

// Uncomment this to regularly print the tree that is being interpolated.
// #define GODOT_SCENE_TREE_FTI_PRINT_TREE

#endif

void SceneTreeFTI::_reset_spatial_flags(Spatial &r_spatial) {
	r_spatial.data.fti_on_tick_xform_list = false;
	r_spatial.data.fti_on_tick_property_list = false;
	r_spatial.data.fti_on_frame_xform_list = false;
	r_spatial.data.fti_on_frame_property_list = false;
	r_spatial.data.fti_global_xform_interp_set = false;
	r_spatial.data.fti_frame_xform_force_update = false;
	r_spatial.data.fti_processed = false;
}

void SceneTreeFTI::_reset_flags(Node *p_node) {
	Spatial *s = Object::cast_to<Spatial>(p_node);

	if (s) {
		_reset_spatial_flags(*s);

		// In most cases the later  NOTIFICATION_RESET_PHYSICS_INTERPOLATION
		// will reset this, but this should help cover hidden nodes.
		s->data.local_transform_prev = s->get_transform();
	}

	for (int n = 0; n < p_node->get_child_count(); n++) {
		_reset_flags(p_node->get_child(n));
	}
}

void SceneTreeFTI::set_enabled(Node *p_root, bool p_enabled) {
	if (data.enabled == p_enabled) {
		return;
	}

	data.tick_xform_list[0].clear();
	data.tick_xform_list[1].clear();

	// Spatial flags must be reset.
	if (p_root) {
		_reset_flags(p_root);
	}

	data.enabled = p_enabled;
}

void SceneTreeFTI::tick_update() {
	if (!data.enabled) {
		return;
	}

	_update_request_resets();

	uint32_t curr_mirror = data.mirror;
	uint32_t prev_mirror = curr_mirror ? 0 : 1;

	LocalVector<Spatial *> &curr = data.tick_xform_list[curr_mirror];
	LocalVector<Spatial *> &prev = data.tick_xform_list[prev_mirror];

	// First detect on the previous list but not on this tick list.
	for (uint32_t n = 0; n < prev.size(); n++) {
		Spatial *s = prev[n];
		if (!s->data.fti_on_tick_xform_list) {
			// Needs a reset so jittering will stop.
			s->fti_pump_xform();

			// Optimization - detect whether we have rested at identity xform.
			s->data.fti_is_identity_xform = s->data.local_transform == Transform();

			// This may not get updated so set it to the same as global xform.
			// TODO: double check this is the best value.
			s->data.global_transform_interpolated = s->get_global_transform();

			// Remove from interpolation list.
			if (s->data.fti_on_frame_xform_list) {
				_spatial_remove_from_frame_list(*s, false);
			}

			// Ensure that the spatial gets at least ONE further
			// update in the resting position in the next frame update.
			if (!s->data.fti_frame_xform_force_update) {
				_spatial_add_to_frame_list(*s, true);
			}
		}
	}

	LocalVector<Spatial *> &curr_prop = data.tick_property_list[curr_mirror];
	LocalVector<Spatial *> &prev_prop = data.tick_property_list[prev_mirror];

	// Detect on the previous property list but not on this tick list.
	for (uint32_t n = 0; n < prev_prop.size(); n++) {
		Spatial *s = prev_prop[n];

		if (!s->data.fti_on_tick_property_list) {
			// Needs a reset so jittering will stop.
			s->fti_pump_xform();

			// Ensure the servers are up to date with the final resting value.
			s->fti_update_servers_property();

			// Remove from interpolation list.
			if (s->data.fti_on_frame_property_list) {
				s->data.fti_on_frame_property_list = false;
				data.frame_property_list.erase_unordered(s);

#ifdef GODOT_SCENE_TREE_FTI_EXTRA_CHECKS
				DEV_CHECK_ONCE(data.frame_property_list.find(s) == -1);
#endif
			}
		}
	}

	// Pump all on the property list that are NOT on the tick list.
	for (uint32_t n = 0; n < curr_prop.size(); n++) {
		Spatial *s = curr_prop[n];

		// Reset, needs to be marked each tick.
		s->data.fti_on_tick_property_list = false;
		s->fti_pump_property();
	}

	// Now pump all on the current list.
	for (uint32_t n = 0; n < curr.size(); n++) {
		Spatial *s = curr[n];

		// Reset, needs to be marked each tick.
		s->data.fti_on_tick_xform_list = false;

		// Pump.
		s->fti_pump_xform();
	}

	// Clear previous list and flip.
	prev.clear();
	prev_prop.clear();

	data.mirror = prev_mirror;
}

void SceneTreeFTI::_update_request_resets() {
	// For instance when first adding to the tree, when the previous transform is
	// unset, to prevent streaking from the origin.
	for (uint32_t n = 0; n < data.request_reset_list.size(); n++) {
		Spatial *s = data.request_reset_list[n];
		if (s->_is_physics_interpolation_reset_requested()) {
			if (s->is_visible_in_tree() && !s->_is_using_identity_transform()) {
				s->notification(Spatial::NOTIFICATION_RESET_PHYSICS_INTERPOLATION);
			}

			s->_set_physics_interpolation_reset_requested(false);
		}
	}

	data.request_reset_list.clear();
}

void SceneTreeFTI::spatial_request_reset(Spatial *p_spatial) {
	DEV_ASSERT(p_spatial);
	DEV_CHECK_ONCE(data.enabled);

	if (!p_spatial->_is_physics_interpolation_reset_requested()) {
		p_spatial->_set_physics_interpolation_reset_requested(true);
#ifdef GODOT_SCENE_TREE_FTI_EXTRA_CHECKS
		DEV_CHECK_ONCE(data.request_reset_list.find(p_spatial) == -1);
#endif
		data.request_reset_list.push_back(p_spatial);
	}
}

void SceneTreeFTI::_spatial_notify_set_property(Spatial &r_spatial) {
	if (!r_spatial.is_physics_interpolated()) {
		return;
	}

	DEV_CHECK_ONCE(data.enabled);

	// Note that a Spatial can be on BOTH the transform list and the property list.
	if (!r_spatial.data.fti_on_tick_property_list) {
		r_spatial.data.fti_on_tick_property_list = true;

		// Should only appear once in the property list.
#ifdef GODOT_SCENE_TREE_FTI_EXTRA_CHECKS
		DEV_CHECK_ONCE(data.tick_property_list[data.mirror].find(&r_spatial) == -1);
#endif
		data.tick_property_list[data.mirror].push_back(&r_spatial);
	}

	if (!r_spatial.data.fti_on_frame_property_list) {
		r_spatial.data.fti_on_frame_property_list = true;

		// Should only appear once in the property frame list.
#ifdef GODOT_SCENE_TREE_FTI_EXTRA_CHECKS
		DEV_CHECK_ONCE(data.frame_property_list.find(&r_spatial) == -1);
#endif
		data.frame_property_list.push_back(&r_spatial);
	}
}

void SceneTreeFTI::_create_depth_lists() {
	uint32_t first_list = data.frame_start ? 0 : 1;

	for (uint32_t l = first_list; l < 2; l++) {
		LocalVector<Spatial *> &source_list = l == 0 ? data.frame_xform_list : data.frame_xform_list_forced;

#ifdef DEBUG_ENABLED
		bool log_nodes_moved_on_frame = (data.traversal_mode == TM_DEBUG) && !data.frame_start && data.periodic_debug_log;
		if (log_nodes_moved_on_frame) {
			if (source_list.size()) {
				print_line(String("\n") + itos(source_list.size()) + " nodes moved during frame:");
			} else {
				print_line("0 nodes moved during frame.");
			}
		}
#endif

		for (uint32_t n = 0; n < source_list.size(); n++) {
			Spatial *s = source_list[n];
			s->data.fti_processed = false;

			int32_t depth = s->_get_scene_tree_depth();

			// This shouldn't happen, but wouldn't be terrible if it did.
			DEV_ASSERT(depth >= 0);
			depth = MIN(depth, (int32_t)data.scene_tree_depth_limit);

			LocalVector<Spatial *> &dest_list = data.dirty_spatial_depth_lists[depth];
#ifdef GODOT_SCENE_TREE_FTI_EXTRA_CHECKS
			// Shouldn't really happen, but duplicates don't really matter that much.
			if (dest_list.find(s) != -1) {
				ERR_FAIL_COND(dest_list.find(s) != -1);
			}
#endif

#ifdef DEBUG_ENABLED
			if (log_nodes_moved_on_frame) {
				print_line("\t" + s->get_name());
			}
#endif

			// Prevent being added to the dest_list twice when on
			// the frame_xform_list AND the frame_xform_list_forced.
			if ((l == 0) && s->data.fti_frame_xform_force_update) {
#ifdef GODOT_SCENE_TREE_FTI_EXTRA_CHECKS
				DEV_ASSERT(data.frame_xform_list_forced.find(s) != -1);
#endif
				continue;
			}

			dest_list.push_back(s);
		}
	}
}

void SceneTreeFTI::_clear_depth_lists() {
	for (uint32_t d = 0; d < data.scene_tree_depth_limit; d++) {
		data.dirty_spatial_depth_lists[d].clear();
	}
}

void SceneTreeFTI::_spatial_add_to_frame_list(Spatial &r_spatial, bool p_forced) {
	if (p_forced) {
		DEV_ASSERT(!r_spatial.data.fti_frame_xform_force_update);
#ifdef GODOT_SCENE_TREE_FTI_EXTRA_CHECKS
		int64_t found = data.frame_xform_list_forced.find(&r_spatial);
		if (found != -1) {
			ERR_FAIL_COND(found != -1);
		}
#endif
		data.frame_xform_list_forced.push_back(&r_spatial);
		r_spatial.data.fti_frame_xform_force_update = true;
	} else {
		DEV_ASSERT(!r_spatial.data.fti_on_frame_xform_list);
#ifdef GODOT_SCENE_TREE_FTI_EXTRA_CHECKS
		int64_t found = data.frame_xform_list.find(&r_spatial);
		if (found != -1) {
			ERR_FAIL_COND(found != -1);
		}
#endif
		data.frame_xform_list.push_back(&r_spatial);
		r_spatial.data.fti_on_frame_xform_list = true;
	}
}

void SceneTreeFTI::_spatial_remove_from_frame_list(Spatial &r_spatial, bool p_forced) {
	if (p_forced) {
		DEV_ASSERT(r_spatial.data.fti_frame_xform_force_update);
		data.frame_xform_list_forced.erase_unordered(&r_spatial);
		r_spatial.data.fti_frame_xform_force_update = false;
	} else {
		DEV_ASSERT(r_spatial.data.fti_on_frame_xform_list);
		data.frame_xform_list.erase_unordered(&r_spatial);
		r_spatial.data.fti_on_frame_xform_list = false;
	}
}

void SceneTreeFTI::_spatial_notify_set_xform(Spatial &r_spatial) {
	DEV_CHECK_ONCE(data.enabled);

	if (!r_spatial.is_physics_interpolated()) {
		// Force an update of non-interpolated to servers
		// on the next traversal.
		if (!r_spatial.data.fti_frame_xform_force_update) {
			_spatial_add_to_frame_list(r_spatial, true);
		}

		// ToDo: Double check this is a win,
		// non-interpolated nodes we always check for identity,
		// *just in case*.
		r_spatial.data.fti_is_identity_xform = r_spatial.get_transform() == Transform();
		return;
	}

	r_spatial.data.fti_is_identity_xform = false;

	if (!r_spatial.data.fti_on_tick_xform_list) {
		r_spatial.data.fti_on_tick_xform_list = true;

		// Should only appear once in the xform list.
#ifdef GODOT_SCENE_TREE_FTI_EXTRA_CHECKS
		DEV_CHECK_ONCE(data.tick_xform_list[data.mirror].find(&r_spatial) == -1);
#endif
		data.tick_xform_list[data.mirror].push_back(&r_spatial);

		// The following flag could have been previously set
		// (for removal from the tick list).
		// We no longer need this guarantee,
		// however there is probably no downside to leaving it set
		// as it will be cleared on the next frame anyway.
		// This line is left for reference.
		// r_spatial.data.fti_frame_xform_force_update = false;
	}

	if (!r_spatial.data.fti_on_frame_xform_list) {
		_spatial_add_to_frame_list(r_spatial, false);
	}

	// If we are in the second half of a frame, always add to the force update list,
	// because we ignore the tick update list during the second update.
	if (data.in_frame) {
		if (!r_spatial.data.fti_frame_xform_force_update) {
			_spatial_add_to_frame_list(r_spatial, true);
		}
	}
}

void SceneTreeFTI::spatial_notify_delete(Spatial *p_spatial) {
	if (!data.enabled) {
		return;
	}

	ERR_FAIL_NULL(p_spatial);

	// Remove from frame lists.
	if (p_spatial->data.fti_on_frame_xform_list) {
		_spatial_remove_from_frame_list(*p_spatial, false);
	}
	if (p_spatial->data.fti_frame_xform_force_update) {
		_spatial_remove_from_frame_list(*p_spatial, true);
	}

	// Ensure this is kept in sync with the lists, in case a node
	// is removed and readded to the scene tree multiple times
	// on the same frame / tick.
	p_spatial->_set_physics_interpolation_reset_requested(false);

	// Keep flags consistent for the same as a new node,
	// because this node may re-enter the scene tree.
	_reset_spatial_flags(*p_spatial);

	// This can potentially be optimized for large scenes with large churn,
	// as it will be doing a linear search through the lists.
	data.tick_xform_list[0].erase_unordered(p_spatial);
	data.tick_xform_list[1].erase_unordered(p_spatial);

	data.tick_property_list[0].erase_unordered(p_spatial);
	data.tick_property_list[1].erase_unordered(p_spatial);

	data.frame_property_list.erase_unordered(p_spatial);
	data.request_reset_list.erase_unordered(p_spatial);

#ifdef GODOT_SCENE_TREE_FTI_EXTRA_CHECKS
	// There should only be one occurrence on the lists.
	// Check this in DEV_ENABLED builds.
	DEV_CHECK_ONCE(data.tick_xform_list[0].find(p_spatial) == -1);
	DEV_CHECK_ONCE(data.tick_xform_list[1].find(p_spatial) == -1);

	DEV_CHECK_ONCE(data.tick_property_list[0].find(p_spatial) == -1);
	DEV_CHECK_ONCE(data.tick_property_list[1].find(p_spatial) == -1);

	DEV_CHECK_ONCE(data.frame_property_list.find(p_spatial) == -1);
	DEV_CHECK_ONCE(data.request_reset_list.find(p_spatial) == -1);

	DEV_CHECK_ONCE(data.frame_xform_list.find(p_spatial) == -1);
	DEV_CHECK_ONCE(data.frame_xform_list_forced.find(p_spatial) == -1);
#endif
}

void SceneTreeFTI::_update_dirty_spatials(Node *p_node, uint32_t p_current_half_frame, float p_interpolation_fraction, bool p_active, const Transform *p_parent_global_xform, int p_depth) {
	Spatial *s = Object::cast_to<Spatial>(p_node);

#ifdef DEBUG_ENABLED
	data.debug_node_count++;
#endif

	// Don't recurse into hidden branches.
	if (s && !s->is_visible()) {
		// NOTE : If we change from recursing entire tree, we should do an is_visible_in_tree()
		// check for the first of the branch.
		return;
	}

	// Not a Spatial.
	// Could be e.g. a viewport or something
	// so we should still recurse to children.
	if (!s) {
		for (int n = 0; n < p_node->get_child_count(); n++) {
			_update_dirty_spatials(p_node->get_child(n), p_current_half_frame, p_interpolation_fraction, p_active, nullptr, p_depth + 1);
		}
		return;
	}

	// We are going to be using data.global_transform, so
	// we need to ensure data.global_transform is not dirty!
	if (s->data.dirty & Spatial::DIRTY_GLOBAL) {
		_ALLOW_DISCARD_ s->get_global_transform();
	}

	// Start the active interpolation chain from here onwards
	// as we recurse further into the SceneTree.
	// Once we hit an active (interpolated) node, we have to fully
	// process all ancestors because their xform will also change.
	// Anything not moving (inactive) higher in the tree need not be processed.
	if (!p_active) {
		if (data.frame_start) {
			// On the frame start, activate whenever we hit something that requests interpolation.
			if (s->data.fti_on_frame_xform_list || s->data.fti_frame_xform_force_update) {
				p_active = true;
			}
		} else {
			// On the frame end, we want to re-interpolate *anything* that has moved
			// since the frame start.
			if (s->data.dirty & Spatial::DIRTY_GLOBAL_INTERPOLATED) {
				p_active = true;

#if 0
				if (data.periodic_debug_log) {
					print_line("activating on : " + s->get_name());
				}
#endif
			}
		}
	}

	// ToDo : Check global_xform_interp is up to date for nodes
	// that are not traversed by the depth lists.
	if (data.frame_start) {
		// Mark on the Spatial whether we have set global_transform_interp.
		// This can later be used when calling `get_global_transform_interpolated()`
		// to know which xform to return.
		s->data.fti_global_xform_interp_set = p_active;
	}

	if (p_active) {
#ifdef GODOT_SCENE_TREE_FTI_PRINT_TREE
		bool dirty = s->data.dirty & Spatial::DIRTY_GLOBAL_INTERPOLATED;

		if (data.periodic_debug_log && !data.use_optimized_traversal_method && !data.frame_start) {
			String sz;
			for (int n = 0; n < p_depth; n++) {
				sz += "\t";
			}
			print_line(sz + p_node->get_name() + (dirty ? " DIRTY" : "") + (s->get_transform() == Transform() ? "\t[IDENTITY]" : ""));
		}
#endif

		// First calculate our local xform.
		// This will either use interpolation, or just use the current local if not interpolated.
		Transform local_interp;
		if (s->is_physics_interpolated()) {
			// There may be no need to interpolate if the spatial has not been moved recently
			// and is therefore not on the tick list...
			if (s->data.fti_on_tick_xform_list) {
				// Make sure to call `get_transform()` rather than using local_transform directly, because
				// local_transform may be dirty and need updating from rotation / scale.
				TransformInterpolator::interpolate_transform(s->data.local_transform_prev, s->get_transform(), local_interp, p_interpolation_fraction);
			} else {
				local_interp = s->get_transform();
			}
		} else {
			local_interp = s->get_transform();
		}

		// Concatenate parent xform.
		if (!s->is_set_as_toplevel()) {
			if (p_parent_global_xform) {
				s->data.global_transform_interpolated = s->data.fti_is_identity_xform ? *p_parent_global_xform : ((*p_parent_global_xform) * local_interp);
			} else {
				const Spatial *parent = s->get_parent_spatial();

				if (parent) {
					const Transform &parent_glob = parent->data.fti_global_xform_interp_set ? parent->data.global_transform_interpolated : parent->get_global_transform();
					s->data.global_transform_interpolated = s->data.fti_is_identity_xform ? parent_glob : parent_glob * local_interp;
				} else {
					s->data.global_transform_interpolated = local_interp;
				}
			}
		} else {
			s->data.global_transform_interpolated = local_interp;
		}

		// Watch for this, disable_scale can cause incredibly confusing bugs
		// and must be checked for when calculating global xforms.
		if (s->data.disable_scale) {
			s->data.global_transform_interpolated.basis.orthonormalize();
		}

		// Upload to VisualServer the interpolated global xform.
		s->fti_update_servers_xform();

		// Ensure branches are only processed once on each traversal.
		s->data.fti_processed = true;

#ifdef DEBUG_ENABLED
		data.debug_nodes_processed++;
#endif
	} // if active.

	// Remove the dirty interp flag from EVERYTHING as we go.
	s->data.dirty &= ~Spatial::DIRTY_GLOBAL_INTERPOLATED;

	// Recurse to children.
	for (int n = 0; n < p_node->get_child_count(); n++) {
		_update_dirty_spatials(p_node->get_child(n), p_current_half_frame, p_interpolation_fraction, p_active, s->data.fti_global_xform_interp_set ? &s->data.global_transform_interpolated : &s->data.global_transform, p_depth + 1);
	}
}

void SceneTreeFTI::frame_update(Node *p_root, bool p_frame_start) {
	if (!data.enabled || !p_root) {
		return;
	}

	data.frame_start = p_frame_start;
	data.in_frame = true;

	_update_request_resets();

	float interpolation_fraction = Engine::get_singleton()->get_physics_interpolation_fraction();
	uint32_t frame = Engine::get_singleton()->get_frames_drawn();

	uint64_t before = 0;
#ifdef DEBUG_ENABLED
	if (data.traversal_mode == TM_DEBUG) {
		before = OS::get_singleton()->get_ticks_usec();

		if (p_frame_start && ((frame % ((60 * 15) - 3)) == 0)) {
			data.periodic_debug_log = true;
		}
	}

#ifdef GODOT_SCENE_TREE_FTI_PRINT_TREE
	if (data.periodic_debug_log) {
		print_line(String("\nScene: ") + (data.frame_start ? "start" : "end") + "\n");
	}
#endif
#endif

	data.debug_node_count = 0;
	data.debug_nodes_processed = 0;

	uint32_t half_frame = p_frame_start ? (frame * 2) : ((frame * 2) + 1);

	bool print_debug_stats = false;
	switch (data.traversal_mode) {
		case TM_LEGACY: {
			data.use_optimized_traversal_method = false;
		} break;
		case TM_DEBUG: {
			// Switch on alternate frames between the two methods.
			data.use_optimized_traversal_method = (frame % 2) == 1;

			// Odd number ensures we debug stats for both methods.
			print_debug_stats = (frame % ((60 * 8) - 1)) == 0;
		} break;
		default: {
			data.use_optimized_traversal_method = true;
		} break;
	}

#ifdef GODOT_SCENE_TREE_FTI_VERIFY
	_tests->frame_update(p_root, half_frame, interpolation_fraction);
#else

	uint32_t skipped = 0;

	if (!data.use_optimized_traversal_method) {
		// Reference approach.
		// Traverse the entire scene tree.
		// Slow, but robust.
		_update_dirty_spatials(p_root, half_frame, interpolation_fraction, false);
	} else {
		// Optimized approach.
		// Traverse from depth lists.
		// Be sure to check against the reference
		// implementation when making changes.
		_create_depth_lists();

		for (uint32_t d = 0; d < data.scene_tree_depth_limit; d++) {
			const LocalVector<Spatial *> &list = data.dirty_spatial_depth_lists[d];

#if 0
			if (list.size() > 0) {
				print_line("depth " + itos(d) + ", contains " + itos(list.size()));
			}
#endif

			for (uint32_t n = 0; n < list.size(); n++) {
				// Already processed this frame?
				Spatial *s = list[n];

				if (s->data.fti_processed) {
#ifdef DEBUG_ENABLED
					skipped++;
#endif
					continue;
				}

				if (!s->is_visible_in_tree()) {
#ifdef DEBUG_ENABLED
					skipped++;
#endif
					continue;
				}

				_update_dirty_spatials(s, half_frame, interpolation_fraction, true);
			}
		}

		_clear_depth_lists();
	}

	if (print_debug_stats) {
		uint64_t after = OS::get_singleton()->get_ticks_usec();
		print_line(String(data.use_optimized_traversal_method ? "FTI optimized" : "FTI reference") + " nodes traversed : " + itos(data.debug_node_count) + (skipped == 0 ? "" : ", skipped " + itos(skipped)) + ", processed : " + itos(data.debug_nodes_processed) + ", took " + itos(after - before) + " usec " + (data.frame_start ? "(start)" : "(end)"));
	}

#endif //  not GODOT_SCENE_TREE_FTI_VERIFY

	// In theory we could clear the `force_update` flags from the nodes in the traversal.
	// The problem is that hidden nodes are not recursed into, therefore the flags would
	// never get cleared and could get out of sync with the forced list.
	// So instead we are clearing them here manually.
	// This is not ideal in terms of cache coherence so perhaps another method can be
	// explored in future.
	uint32_t forced_list_size = data.frame_xform_list_forced.size();
	for (uint32_t n = 0; n < forced_list_size; n++) {
		Spatial *s = data.frame_xform_list_forced[n];
		s->data.fti_frame_xform_force_update = false;
	}
	data.frame_xform_list_forced.clear();

	if (!p_frame_start && data.periodic_debug_log) {
		data.periodic_debug_log = false;
	}

	// Update the properties once off at the end of the frame.
	// No need for two passes for properties.
	if (!p_frame_start) {
		for (uint32_t n = 0; n < data.frame_property_list.size(); n++) {
			Spatial *s = data.frame_property_list[n];
			s->fti_update_servers_property();
		}
	}

	// Marks the end of the frame.
	// Enables us to recognise when change notifications
	// come in _during_ a frame (they get treated differently).
	if (!data.frame_start) {
		data.in_frame = false;
	}
}

SceneTreeFTI::SceneTreeFTI() {
#ifdef GODOT_SCENE_TREE_FTI_VERIFY
	_tests = memnew(SceneTreeFTITests(*this));
#endif

	Variant traversal_mode_string = GLOBAL_DEF("physics/3d/physics_interpolation/scene_traversal", "DEFAULT");
	ProjectSettings::get_singleton()->set_custom_property_info("physics/3d/physics_interpolation/scene_traversal", PropertyInfo(Variant::STRING, "physics/3d/physics_interpolation/scene_traversal", PROPERTY_HINT_ENUM, "DEFAULT,Legacy,Debug"));

	data.traversal_mode = TM_DEFAULT;

	if (traversal_mode_string == "Legacy") {
		data.traversal_mode = TM_LEGACY;
	} else if (traversal_mode_string == "Debug") {
		// Don't allow debug mode in final exports,
		// it will almost certainly be a mistake.
#ifdef DEBUG_ENABLED
		data.traversal_mode = TM_DEBUG;
#else
		data.traversal_mode = TM_DEFAULT;
#endif
	}

	switch (data.traversal_mode) {
		default: {
			print_verbose("SceneTreeFTI: traversal method DEFAULT");
		} break;
		case TM_LEGACY: {
			print_verbose("SceneTreeFTI: traversal method Legacy");
		} break;
		case TM_DEBUG: {
			print_verbose("SceneTreeFTI: traversal method Debug");
		} break;
	}

#ifdef GODOT_SCENE_TREE_FTI_EXTRA_CHECKS
	print_line("SceneTreeFTI : GODOT_SCENE_TREE_FTI_EXTRA_CHECKS defined");
#endif
#ifdef GODOT_SCENE_TREE_FTI_PRINT_TREE
	print_line("SceneTreeFTI : GODOT_SCENE_TREE_FTI_PRINT_TREE defined");
#endif
}

SceneTreeFTI::~SceneTreeFTI() {
#ifdef GODOT_SCENE_TREE_FTI_VERIFY
	if (_tests) {
		memfree(_tests);
		_tests = nullptr;
	}
#endif
}

#endif // ndef _3D_DISABLED
