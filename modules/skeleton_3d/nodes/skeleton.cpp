/*************************************************************************/
/*  skeleton.cpp                                                         */
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

#include "skeleton.h"

#include "core/object/message_queue.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"
#include "physical_bone.h"
#include "../resources/skeleton_modification_3d.h"
#include "../resources/skeleton_modification_stack_3d.h"
#include "../resources/skin.h"
#include "scene/resources/surface_tool.h"
#include "scene/scene_string_names.h"

void SkinReference::_skin_changed() {
	if (skeleton_node) {
		skeleton_node->_make_dirty();
	}
	skeleton_version = 0;
}

void SkinReference::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_skin_changed"), &SkinReference::_skin_changed);
	ClassDB::bind_method(D_METHOD("get_skeleton"), &SkinReference::get_skeleton);
	ClassDB::bind_method(D_METHOD("get_skin"), &SkinReference::get_skin);
}

RID SkinReference::get_skeleton() const {
	return skeleton;
}

Skeleton *SkinReference::get_skeleton_node() const {
	return skeleton_node;
}

Ref<Skin> SkinReference::get_skin() const {
	return skin;
}

SkinReference::~SkinReference() {
	if (skeleton_node) {
		skeleton_node->skin_bindings.erase(this);
	}

	RS::get_singleton()->free(skeleton);
}

bool Skeleton::_set(const StringName &p_path, const Variant &p_value) {
	String path = p_path;

#ifndef _3D_DISABLED
	if (path.begins_with("modification_stack")) {
		set_modification_stack(p_value);
		return true;
	}
#endif //_3D_DISABLED

	if (!path.begins_with("bones/")) {
		return false;
	}

	int which = path.get_slicec('/', 1).to_int();
	String what = path.get_slicec('/', 2);

	if (which == bones.size() && what == "name") {
		add_bone(p_value);
		return true;
	}

	ERR_FAIL_INDEX_V(which, bones.size(), false);

	if (what == "parent") {
		set_bone_parent(which, p_value);
	} else if (what == "rest") {
		set_bone_rest(which, p_value);

	} else if (what == "enabled") {
		set_bone_enabled(which, p_value);
	} else if (what == "position") {
		set_bone_pose_position(which, p_value);
	} else if (what == "rotation") {
		set_bone_pose_rotation(which, p_value);
	} else if (what == "scale") {
		set_bone_pose_scale(which, p_value);
	} else if (what == "pose") {
		set_bone_pose(which, p_value);
	} else {
		return false;
	}

	return true;
}

bool Skeleton::_get(const StringName &p_path, Variant &r_ret) const {
	String path = p_path;

#ifndef _3D_DISABLED
	if (path.begins_with("modification_stack")) {
		r_ret = modification_stack;
		return true;
	}
#endif //_3D_DISABLED

	if (!path.begins_with("bones/")) {
		return false;
	}

	int which = path.get_slicec('/', 1).to_int();
	String what = path.get_slicec('/', 2);

	ERR_FAIL_INDEX_V(which, bones.size(), false);

	if (what == "name") {
		r_ret = get_bone_name(which);
	} else if (what == "parent") {
		r_ret = get_bone_parent(which);
	} else if (what == "rest") {
		r_ret = get_bone_rest(which);
	} else if (what == "enabled") {
		r_ret = is_bone_enabled(which);
	} else if (what == "position") {
		r_ret = get_bone_pose_position(which);
	} else if (what == "rotation") {
		r_ret = get_bone_pose_rotation(which);
	} else if (what == "scale") {
		r_ret = get_bone_pose_scale(which);
	} else if (what == "pose") {
		r_ret = get_bone_pose(which);
	} else {
		return false;
	}

	return true;
}
void Skeleton::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < bones.size(); i++) {
		String prep = "bones/" + itos(i) + "/";
		p_list->push_back(PropertyInfo(Variant::STRING, prep + "name"));
		p_list->push_back(PropertyInfo(Variant::INT, prep + "parent", PROPERTY_HINT_RANGE, "-1," + itos(bones.size() - 1) + ",1"));
		p_list->push_back(PropertyInfo(Variant::TRANSFORM, prep + "rest"));
		p_list->push_back(PropertyInfo(Variant::BOOL, prep + "enabled"));
		p_list->push_back(PropertyInfo(Variant::VECTOR3, prep + "position", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
		p_list->push_back(PropertyInfo(Variant::QUATERNION, prep + "rotation", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
		p_list->push_back(PropertyInfo(Variant::VECTOR3, prep + "scale", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
	}

#ifndef _3D_DISABLED
	p_list->push_back(PropertyInfo(Variant::OBJECT, "modification_stack", PROPERTY_HINT_RESOURCE_TYPE, "SkeletonModificationStack3D", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE));
#endif //_3D_DISABLED
}

/*
void Skeleton::_validate_property(PropertyInfo &property) const {
	Vector<String> spr = property.name.split("/");
	if (spr.size() == 3 && spr[0] == "bones") {
		if (spr[2] == "rest") {
			property.usage |= PROPERTY_USAGE_KEYING_INCREMENTS;
		}
		if (is_show_rest_only()) {
			if (spr[2] == "enabled") {
				property.usage |= PROPERTY_USAGE_READ_ONLY;
			}
			if (spr[2] == "position") {
				property.usage |= PROPERTY_USAGE_READ_ONLY;
			}
			if (spr[2] == "rotation") {
				property.usage |= PROPERTY_USAGE_READ_ONLY;
			}
			if (spr[2] == "scale") {
				property.usage |= PROPERTY_USAGE_READ_ONLY;
			}
		} else if (!is_bone_enabled(spr[1].to_int())) {
			if (spr[2] == "position") {
				property.usage |= PROPERTY_USAGE_READ_ONLY;
			}
			if (spr[2] == "rotation") {
				property.usage |= PROPERTY_USAGE_READ_ONLY;
			}
			if (spr[2] == "scale") {
				property.usage |= PROPERTY_USAGE_READ_ONLY;
			}
		}
	}
}
*/

void Skeleton::_update_process_order() {
	if (!process_order_dirty) {
		return;
	}

	++updating;

	Bone *bonesptr = bones.ptrw();
	int len = bones.size();

	parentless_bones.clear();

	for (int i = 0; i < len; i++) {
		bonesptr[i].child_bones.clear();
	}

	for (int i = 0; i < len; i++) {
		if (bonesptr[i].parent >= len) {
			//validate this just in case
			ERR_PRINT("Bone " + itos(i) + " has invalid parent: " + itos(bonesptr[i].parent));
			bonesptr[i].parent = -1;
		}

		if (bonesptr[i].parent != -1) {
			int parent_bone_idx = bonesptr[i].parent;

			// Check to see if this node is already added to the parent:
			if (bonesptr[parent_bone_idx].child_bones.find(i) < 0) {
				// Add the child node
				bonesptr[parent_bone_idx].child_bones.push_back(i);
			} else {
				ERR_PRINT("Skeleton parenthood graph is cyclic");
			}
		} else {
			parentless_bones.push_back(i);
		}
	}

	process_order_dirty = false;

	--updating;

	emit_signal("bones_updated");
}

void Skeleton::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_UPDATE_SKELETON: {
			++updating;
			// Update bone transforms
			force_update_all_bone_transforms();

			RenderingServer *vs = RenderingServer::get_singleton();

			Bone *bonesptr = bones.ptrw();

			int len = bones.size();
			dirty = false;

			//update skins
			for (RBSet<SkinReference *>::Element *E = skin_bindings.front(); E; E = E->next()) {
				const Skin *skin = E->get()->skin.operator->();
				RID skeleton = E->get()->skeleton;
				uint32_t bind_count = skin->get_bind_count();

				if (E->get()->bind_count != bind_count) {
					RS::get_singleton()->skeleton_allocate(skeleton, bind_count);
					E->get()->bind_count = bind_count;
					E->get()->skin_bone_indices.resize(bind_count);
					E->get()->skin_bone_indices_ptrs = E->get()->skin_bone_indices.ptrw();
				}

				if (E->get()->skeleton_version != version) {
					for (uint32_t i = 0; i < bind_count; i++) {
						StringName bind_name = skin->get_bind_name(i);

						if (bind_name != StringName()) {
							//bind name used, use this
							bool found = false;
							for (int j = 0; j < len; j++) {
								if (bonesptr[j].name == bind_name) {
									E->get()->skin_bone_indices_ptrs[i] = j;
									found = true;
									break;
								}
							}

							if (!found) {
								ERR_PRINT("Skin bind #" + itos(i) + " contains named bind '" + String(bind_name) + "' but Skeleton has no bone by that name.");
								E->get()->skin_bone_indices_ptrs[i] = 0;
							}
						} else if (skin->get_bind_bone(i) >= 0) {
							int bind_index = skin->get_bind_bone(i);
							if (bind_index >= len) {
								ERR_PRINT("Skin bind #" + itos(i) + " contains bone index bind: " + itos(bind_index) + " , which is greater than the skeleton bone count: " + itos(len) + ".");
								E->get()->skin_bone_indices_ptrs[i] = 0;
							} else {
								E->get()->skin_bone_indices_ptrs[i] = bind_index;
							}
						} else {
							ERR_PRINT("Skin bind #" + itos(i) + " does not contain a name nor a bone index.");
							E->get()->skin_bone_indices_ptrs[i] = 0;
						}
					}

					E->get()->skeleton_version = version;
				}

				for (uint32_t i = 0; i < bind_count; i++) {
					uint32_t bone_index = E->get()->skin_bone_indices_ptrs[i];
					ERR_CONTINUE(bone_index >= (uint32_t)len);
					vs->skeleton_bone_set_transform(skeleton, i, bonesptr[bone_index].pose_global * skin->get_bind_pose(i));
				}
			}

			--updating;

			//TODO Not sure if this is useful or not in runtime
			//#ifdef TOOLS_ENABLED
			emit_signal("pose_updated");
			//#endif // TOOLS_ENABLED
		} break;

#ifndef _3D_DISABLED
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS: {
			// This is active only if the skeleton animates the physical bones
			// and the state of the bone is not active.
			if (Engine::get_singleton()->is_editor_hint()) {
				if (animate_physical_bones) {
					for (int i = 0; i < bones.size(); i += 1) {
						if (bones[i].physical_bone) {
							if (bones[i].physical_bone->is_simulating_physics() == false) {
								bones[i].physical_bone->reset_to_rest_position();
							}
						}
					}
				}
			}

			if (modification_stack.is_valid()) {
				execute_modifications(get_physics_process_delta_time(), SkeletonModificationStack3D::EXECUTION_MODE::execution_mode_physics_process);
			}
		} break;
#endif

#ifndef _3D_DISABLED
		case NOTIFICATION_INTERNAL_PROCESS: {
			if (modification_stack.is_valid()) {
				execute_modifications(get_process_delta_time(), SkeletonModificationStack3D::EXECUTION_MODE::execution_mode_process);
			}
		} break;
#endif // _3D_DISABLED

#ifndef _3D_DISABLED

		case NOTIFICATION_READY: {
			set_physics_process_internal(true);
			set_process_internal(true);

			if (modification_stack.is_valid()) {
				set_modification_stack(modification_stack);
			}
		} break;
#endif
	}
}

void Skeleton::clear_bones_global_pose_override() {
	for (int i = 0; i < bones.size(); i += 1) {
		bones.write[i].global_pose_override_amount = 0;
		bones.write[i].global_pose_override_reset = true;
	}
	_make_dirty();
}

void Skeleton::set_bone_global_pose_override(int p_bone, const Transform &p_pose, float p_amount, bool p_persistent) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);

	bones.write[p_bone].global_pose_override_amount = p_amount;
	bones.write[p_bone].global_pose_override = p_pose;
	bones.write[p_bone].global_pose_override_reset = !p_persistent;
	_make_dirty();
}

Transform Skeleton::get_bone_global_pose_override(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Transform());

	return bones[p_bone].global_pose_override;
}

Transform Skeleton::get_bone_global_pose(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Transform());

	if (dirty && updating == 0) {
		const_cast<Skeleton *>(this)->notification(NOTIFICATION_UPDATE_SKELETON);
	}

	return bones[p_bone].pose_global;
}

Transform Skeleton::get_bone_global_pose_no_override(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Transform());

	if (dirty && updating == 0) {
		const_cast<Skeleton *>(this)->notification(NOTIFICATION_UPDATE_SKELETON);
	}

	return bones[p_bone].pose_global_no_override;
}

void Skeleton::clear_bones_local_pose_override() {
	for (int i = 0; i < bones.size(); i += 1) {
		bones.write[i].local_pose_override_amount = 0;
	}
	_make_dirty();
}

void Skeleton::set_bone_local_pose_override(int p_bone, const Transform &p_pose, real_t p_amount, bool p_persistent) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);
	bones.write[p_bone].local_pose_override_amount = p_amount;
	bones.write[p_bone].local_pose_override = p_pose;
	bones.write[p_bone].local_pose_override_reset = !p_persistent;
	_make_dirty();
}

Transform Skeleton::get_bone_local_pose_override(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Transform());
	return bones[p_bone].local_pose_override;
}

void Skeleton::update_bone_rest_forward_vector(int p_bone, bool p_force_update) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);

	if (bones[p_bone].rest_bone_forward_vector.length_squared() > 0 && p_force_update == false) {
		update_bone_rest_forward_axis(p_bone, p_force_update);
	}

	// If it is a child/leaf bone...
	if (get_bone_parent(p_bone) > 0) {
		bones.write[p_bone].rest_bone_forward_vector = bones[p_bone].rest.origin.normalized();
	} else {
		// If it has children...
		Vector<int> child_bones = get_bone_children(p_bone);
		if (child_bones.size() > 0) {
			Vector3 combined_child_dir = Vector3(0, 0, 0);
			for (int i = 0; i < child_bones.size(); i++) {
				combined_child_dir += bones[child_bones[i]].rest.origin.normalized();
			}
			combined_child_dir = combined_child_dir / child_bones.size();
			bones.write[p_bone].rest_bone_forward_vector = combined_child_dir.normalized();
		} else {
			WARN_PRINT_ONCE("Cannot calculate forward direction for bone " + itos(p_bone));
			WARN_PRINT_ONCE("Assuming direction of (0, 1, 0) for bone");
			bones.write[p_bone].rest_bone_forward_vector = Vector3(0, 1, 0);
		}
	}
	update_bone_rest_forward_axis(p_bone, p_force_update);
}

void Skeleton::update_bone_rest_forward_axis(int p_bone, bool p_force_update) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);
	if (bones[p_bone].rest_bone_forward_axis > -1 && p_force_update == false) {
		return;
	}

	Vector3 forward_axis_absolute = bones[p_bone].rest_bone_forward_vector.abs();
	if (forward_axis_absolute.x > forward_axis_absolute.y && forward_axis_absolute.x > forward_axis_absolute.z) {
		if (bones[p_bone].rest_bone_forward_vector.x > 0) {
			bones.write[p_bone].rest_bone_forward_axis = BONE_AXIS_X_FORWARD;
		} else {
			bones.write[p_bone].rest_bone_forward_axis = BONE_AXIS_NEGATIVE_X_FORWARD;
		}
	} else if (forward_axis_absolute.y > forward_axis_absolute.x && forward_axis_absolute.y > forward_axis_absolute.z) {
		if (bones[p_bone].rest_bone_forward_vector.y > 0) {
			bones.write[p_bone].rest_bone_forward_axis = BONE_AXIS_Y_FORWARD;
		} else {
			bones.write[p_bone].rest_bone_forward_axis = BONE_AXIS_NEGATIVE_Y_FORWARD;
		}
	} else {
		if (bones[p_bone].rest_bone_forward_vector.z > 0) {
			bones.write[p_bone].rest_bone_forward_axis = BONE_AXIS_Z_FORWARD;
		} else {
			bones.write[p_bone].rest_bone_forward_axis = BONE_AXIS_NEGATIVE_Z_FORWARD;
		}
	}
}

Vector3 Skeleton::get_bone_axis_forward_vector(int p_bone) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Vector3(0, 0, 0));
	return bones[p_bone].rest_bone_forward_vector;
}

int Skeleton::get_bone_axis_forward_enum(int p_bone) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, -1);
	return bones[p_bone].rest_bone_forward_axis;
}

// skeleton creation api
void Skeleton::add_bone(const String &p_name) {
	ERR_FAIL_COND(p_name == "" || p_name.find(":") != -1 || p_name.find("/") != -1);

	for (int i = 0; i < bones.size(); i++) {
		ERR_FAIL_COND(bones[i].name == p_name);
	}

	Bone b;
	b.name = p_name;
	bones.push_back(b);
	process_order_dirty = true;
	version++;
	rest_dirty = true;
	_make_dirty();
	update_gizmos();
}
int Skeleton::find_bone(const String &p_name) const {
	for (int i = 0; i < bones.size(); i++) {
		if (bones[i].name == p_name) {
			return i;
		}
	}

	return -1;
}
String Skeleton::get_bone_name(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, "");

	return bones[p_bone].name;
}

void Skeleton::set_bone_name(int p_bone, const String &p_name) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);

	for (int i = 0; i < bone_size; i++) {
		if (i != p_bone) {
			ERR_FAIL_COND(bones[i].name == p_name);
		}
	}

	bones.write[p_bone].name = p_name;
}

bool Skeleton::is_bone_parent_of(int p_bone, int p_parent_bone_id) const {
	int parent_of_bone = get_bone_parent(p_bone);

	if (-1 == parent_of_bone) {
		return false;
	}

	if (parent_of_bone == p_parent_bone_id) {
		return true;
	}

	return is_bone_parent_of(parent_of_bone, p_parent_bone_id);
}

int Skeleton::get_bone_count() const {
	return bones.size();
}

void Skeleton::set_bone_parent(int p_bone, int p_parent) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);
	ERR_FAIL_COND(p_parent != -1 && (p_parent < 0));
	ERR_FAIL_COND(p_bone == p_parent);

	bones.write[p_bone].parent = p_parent;
	process_order_dirty = true;
	rest_dirty = true;

	_make_dirty();
}

void Skeleton::unparent_bone_and_rest(int p_bone) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);

	_update_process_order();

	int parent = bones[p_bone].parent;
	while (parent >= 0) {
		bones.write[p_bone].rest = bones[parent].rest * bones[p_bone].rest;
		parent = bones[parent].parent;
	}

	bones.write[p_bone].parent = -1;
	process_order_dirty = true;
	rest_dirty = true;

	_make_dirty();
}

int Skeleton::get_bone_parent(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, -1);

	return bones[p_bone].parent;
}

Vector<int> Skeleton::get_bone_children(int p_bone) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Vector<int>());
	return bones[p_bone].child_bones;
}

void Skeleton::set_bone_children(int p_bone, Vector<int> p_children) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);
	bones.write[p_bone].child_bones = p_children;

	process_order_dirty = true;
	rest_dirty = true;

	_make_dirty();
}

void Skeleton::add_bone_child(int p_bone, int p_child) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);
	bones.write[p_bone].child_bones.push_back(p_child);

	process_order_dirty = true;
	rest_dirty = true;

	_make_dirty();
}

void Skeleton::remove_bone_child(int p_bone, int p_child) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);

	int child_idx = bones[p_bone].child_bones.find(p_child);
	if (child_idx >= 0) {
		bones.write[p_bone].child_bones.remove(child_idx);
	} else {
		WARN_PRINT("Cannot remove child bone: Child bone not found.");
	}

	process_order_dirty = true;
	rest_dirty = true;

	_make_dirty();
}

Vector<int> Skeleton::get_parentless_bones() {
	return parentless_bones;
}

void Skeleton::set_bone_rest(int p_bone, const Transform &p_rest) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);

	bones.write[p_bone].rest = p_rest;
	rest_dirty = true;
	_make_dirty();
}
Transform Skeleton::get_bone_rest(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Transform());

	return bones[p_bone].rest;
}

Transform Skeleton::get_bone_global_rest(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Transform());

	if (rest_dirty && updating == 0) {
		const_cast<Skeleton *>(this)->notification(NOTIFICATION_UPDATE_SKELETON);
	}

	return bones[p_bone].global_rest;
}

void Skeleton::set_bone_enabled(int p_bone, bool p_enabled) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);

	bones.write[p_bone].enabled = p_enabled;
	emit_signal(SceneStringNames::get_singleton()->bone_enabled_changed, p_bone);
	_make_dirty();
}

bool Skeleton::is_bone_enabled(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, false);
	return bones[p_bone].enabled;
}

void Skeleton::set_show_rest_only(bool p_enabled) {
	show_rest_only = p_enabled;
	emit_signal(SceneStringNames::get_singleton()->show_rest_only_changed);
	_make_dirty();
}

bool Skeleton::is_show_rest_only() const {
	return show_rest_only;
}

void Skeleton::clear_bones() {
	bones.clear();
	process_order_dirty = true;
	version++;
	_make_dirty();
}

// posing api
void Skeleton::set_bone_pose(int p_bone, const Transform &p_pose) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);

	bones.write[p_bone].pose_position = p_pose.origin;
	bones.write[p_bone].pose_rotation = p_pose.basis.get_rotation_quaternion();
	bones.write[p_bone].pose_scale = p_pose.basis.get_scale();
	bones.write[p_bone].pose_cache_dirty = true;

	if (is_inside_tree()) {
		_make_dirty();
	}
}

void Skeleton::set_bone_pose_position(int p_bone, const Vector3 &p_position) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);

	bones.write[p_bone].pose_position = p_position;
	bones.write[p_bone].pose_cache_dirty = true;

	if (is_inside_tree()) {
		_make_dirty();
	}
}
void Skeleton::set_bone_pose_rotation(int p_bone, const Quaternion &p_rotation) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);

	bones.write[p_bone].pose_rotation = p_rotation;
	bones.write[p_bone].pose_cache_dirty = true;

	if (is_inside_tree()) {
		_make_dirty();
	}
}
void Skeleton::set_bone_pose_scale(int p_bone, const Vector3 &p_scale) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);

	bones.write[p_bone].pose_scale = p_scale;
	bones.write[p_bone].pose_cache_dirty = true;

	if (is_inside_tree()) {
		_make_dirty();
	}
}

Transform Skeleton::get_bone_pose(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Transform());
	((Skeleton *)this)->bones.write[p_bone].update_pose_cache();
	return bones[p_bone].pose_cache;
}

Vector3 Skeleton::get_bone_pose_position(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Vector3());
	return bones[p_bone].pose_position;
}

Quaternion Skeleton::get_bone_pose_rotation(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Quaternion());
	return bones[p_bone].pose_rotation;
}

Vector3 Skeleton::get_bone_pose_scale(int p_bone) const {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, Vector3());
	return bones[p_bone].pose_scale;
}

void Skeleton::_make_dirty() {
	if (dirty) {
		return;
	}

	MessageQueue::get_singleton()->push_notification(this, NOTIFICATION_UPDATE_SKELETON);
	dirty = true;
}

void Skeleton::localize_rests() {
	_update_process_order();

	Vector<int> bones_to_process = get_parentless_bones();
	while (bones_to_process.size() > 0) {
		int current_bone_idx = bones_to_process[0];
		bones_to_process.erase(current_bone_idx);

		if (bones[current_bone_idx].parent >= 0) {
			set_bone_rest(current_bone_idx, bones[bones[current_bone_idx].parent].rest.affine_inverse() * bones[current_bone_idx].rest);
		}

		// Add the bone's children to the list of bones to be processed
		int child_bone_size = bones[current_bone_idx].child_bones.size();
		for (int i = 0; i < child_bone_size; i++) {
			bones_to_process.push_back(bones[current_bone_idx].child_bones[i]);
		}
	}
}

#ifndef _3D_DISABLED

void Skeleton::set_animate_physical_bones(bool p_animate) {
	animate_physical_bones = p_animate;

	if (Engine::get_singleton()->is_editor_hint() == false) {
		bool sim = false;
		for (int i = 0; i < bones.size(); i += 1) {
			if (bones[i].physical_bone) {
				bones[i].physical_bone->reset_physics_simulation_state();
				if (bones[i].physical_bone->is_simulating_physics()) {
					sim = true;
				}
			}
		}
		set_physics_process_internal(sim == false && p_animate);
	}
}

bool Skeleton::get_animate_physical_bones() const {
	return animate_physical_bones;
}

void Skeleton::bind_physical_bone_to_bone(int p_bone, PhysicalBone *p_physical_bone) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);
	ERR_FAIL_COND(bones[p_bone].physical_bone);
	ERR_FAIL_COND(!p_physical_bone);
	bones.write[p_bone].physical_bone = p_physical_bone;

	_rebuild_physical_bones_cache();
}

void Skeleton::unbind_physical_bone_from_bone(int p_bone) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone, bone_size);
	bones.write[p_bone].physical_bone = nullptr;

	_rebuild_physical_bones_cache();
}

PhysicalBone *Skeleton::get_physical_bone(int p_bone) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, nullptr);

	return bones[p_bone].physical_bone;
}

PhysicalBone *Skeleton::get_physical_bone_parent(int p_bone) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, nullptr);

	if (bones[p_bone].cache_parent_physical_bone) {
		return bones[p_bone].cache_parent_physical_bone;
	}

	return _get_physical_bone_parent(p_bone);
}

PhysicalBone *Skeleton::_get_physical_bone_parent(int p_bone) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone, bone_size, nullptr);

	const int parent_bone = bones[p_bone].parent;
	if (0 > parent_bone) {
		return nullptr;
	}

	PhysicalBone *pb = bones[parent_bone].physical_bone;
	if (pb) {
		return pb;
	} else {
		return get_physical_bone_parent(parent_bone);
	}
}

void Skeleton::_rebuild_physical_bones_cache() {
	const int b_size = bones.size();
	for (int i = 0; i < b_size; ++i) {
		PhysicalBone *parent_pb = _get_physical_bone_parent(i);
		if (parent_pb != bones[i].cache_parent_physical_bone) {
			bones.write[i].cache_parent_physical_bone = parent_pb;
			if (bones[i].physical_bone) {
				bones[i].physical_bone->_on_bone_parent_changed();
			}
		}
	}
}

void _pb_stop_simulation(Node *p_node) {
	for (int i = p_node->get_child_count() - 1; 0 <= i; --i) {
		_pb_stop_simulation(p_node->get_child(i));
	}

	PhysicalBone *pb = Object::cast_to<PhysicalBone>(p_node);
	if (pb) {
		pb->set_simulate_physics(false);
	}
}

void Skeleton::physical_bones_stop_simulation() {
	_pb_stop_simulation(this);
}

void _pb_start_simulation(const Skeleton *p_skeleton, Node *p_node, const Vector<int> &p_sim_bones) {
	for (int i = p_node->get_child_count() - 1; 0 <= i; --i) {
		_pb_start_simulation(p_skeleton, p_node->get_child(i), p_sim_bones);
	}

	PhysicalBone *pb = Object::cast_to<PhysicalBone>(p_node);
	if (pb) {
		for (int i = p_sim_bones.size() - 1; 0 <= i; --i) {
			if (p_sim_bones[i] == pb->get_bone_id() || p_skeleton->is_bone_parent_of(pb->get_bone_id(), p_sim_bones[i])) {
				pb->set_simulate_physics(true);
				break;
			}
		}
	}
}

void Skeleton::physical_bones_start_simulation_on(const Array &p_bones) {
	set_physics_process_internal(false);

	Vector<int> sim_bones;
	if (p_bones.size() <= 0) {
		sim_bones.push_back(0); // if no bones is specified, activate ragdoll on full body
	} else {
		sim_bones.resize(p_bones.size());
		int c = 0;
		for (int i = sim_bones.size() - 1; 0 <= i; --i) {
			Variant::Type type = p_bones.get(i).get_type();
			if (Variant::STRING == type || Variant::STRING_NAME == type) {
				int bone_id = find_bone(p_bones.get(i));
				if (bone_id != -1) {
					sim_bones.write[c++] = bone_id;
				}
			}
		}
		sim_bones.resize(c);
	}

	_pb_start_simulation(this, this, sim_bones);
}

void _physical_bones_add_remove_collision_exception(bool p_add, Node *p_node, RID p_exception) {
	for (int i = p_node->get_child_count() - 1; 0 <= i; --i) {
		_physical_bones_add_remove_collision_exception(p_add, p_node->get_child(i), p_exception);
	}

	CollisionObject *co = Object::cast_to<CollisionObject>(p_node);
	if (co) {
		if (p_add) {
			PhysicsServer::get_singleton()->body_add_collision_exception(co->get_rid(), p_exception);
		} else {
			PhysicsServer::get_singleton()->body_remove_collision_exception(co->get_rid(), p_exception);
		}
	}
}

void Skeleton::physical_bones_add_collision_exception(RID p_exception) {
	_physical_bones_add_remove_collision_exception(true, this, p_exception);
}

void Skeleton::physical_bones_remove_collision_exception(RID p_exception) {
	_physical_bones_add_remove_collision_exception(false, this, p_exception);
}

#endif // _3D_DISABLED

void Skeleton::_skin_changed() {
	_make_dirty();
}

Ref<Skin> Skeleton::create_skin_from_rest_transforms() {
	Ref<Skin> skin;

	skin.instance();
	skin->set_bind_count(bones.size());
	_update_process_order(); // Just in case.

	// Pose changed, rebuild cache of inverses.
	const Bone *bonesptr = bones.ptr();
	int len = bones.size();

	// Calculate global rests and invert them.
	LocalVector<int> bones_to_process;
	bones_to_process = get_parentless_bones();
	while (bones_to_process.size() > 0) {
		int current_bone_idx = bones_to_process[0];
		const Bone &b = bonesptr[current_bone_idx];
		bones_to_process.erase(current_bone_idx);
		LocalVector<int> child_bones_vector;
		child_bones_vector = get_bone_children(current_bone_idx);
		int child_bones_size = child_bones_vector.size();

		if (b.parent < 0) {
			skin->set_bind_pose(current_bone_idx, b.rest);
		}

		for (int i = 0; i < child_bones_size; i++) {
			int child_bone_idx = child_bones_vector[i];
			const Bone &cb = bonesptr[child_bone_idx];
			skin->set_bind_pose(child_bone_idx, skin->get_bind_pose(current_bone_idx) * cb.rest);
			// Add the bone's children to the list of bones to be processed.
			bones_to_process.push_back(child_bones_vector[i]);
		}
	}

	for (int i = 0; i < len; i++) {
		// The inverse is what is actually required.
		skin->set_bind_bone(i, i);
		skin->set_bind_pose(i, skin->get_bind_pose(i).affine_inverse());
	}

	return skin;
}

Ref<SkinReference> Skeleton::register_skin(const Ref<Skin> &p_skin) {
	ERR_FAIL_COND_V(p_skin.is_null(), Ref<SkinReference>());

	for (RBSet<SkinReference *>::Element *E = skin_bindings.front(); E; E = E->next()) {
		if (E->get()->skin == p_skin) {
			return Ref<SkinReference>(E->get());
		}
	}

	Ref<SkinReference> skin_ref;
	skin_ref.instance();

	skin_ref->skeleton_node = this;
	skin_ref->bind_count = 0;
	skin_ref->skeleton = RID_PRIME(RenderingServer::get_singleton()->skeleton_create());
	skin_ref->skeleton_node = this;
	skin_ref->skin = p_skin;

	skin_bindings.insert(skin_ref.operator->());

	skin_ref->skin->connect("changed", skin_ref.operator->(), "_skin_changed");

	_make_dirty(); // Skin needs to be updated, so update skeleton.

	return skin_ref;
}

void Skeleton::force_update_all_dirty_bones() {
	if (dirty && updating == 0) {
		const_cast<Skeleton *>(this)->notification(NOTIFICATION_UPDATE_SKELETON);
	}
}

void Skeleton::force_update_all_bone_transforms() {
	++updating;
	_update_process_order();

	for (int i = 0; i < parentless_bones.size(); i++) {
		force_update_bone_children_transforms(parentless_bones[i]);
	}
	--updating;
}

void Skeleton::force_update_bone_children_transforms(int p_bone_idx) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX(p_bone_idx, bone_size);

	Bone *bonesptr = bones.ptrw();
	List<int> bones_to_process = List<int>();
	bones_to_process.push_back(p_bone_idx);

	while (bones_to_process.size() > 0) {
		int current_bone_idx = bones_to_process[0];
		bones_to_process.erase(current_bone_idx);

		Bone &b = bonesptr[current_bone_idx];
		bool bone_enabled = b.enabled && !show_rest_only;

		if (bone_enabled) {
			b.update_pose_cache();
			Transform pose = b.pose_cache;

			if (b.parent >= 0) {
				b.pose_global = bonesptr[b.parent].pose_global * pose;
				b.pose_global_no_override = b.pose_global;
			} else {
				b.pose_global = pose;
				b.pose_global_no_override = b.pose_global;
			}
		} else {
			if (b.parent >= 0) {
				b.pose_global = bonesptr[b.parent].pose_global * b.rest;
				b.pose_global_no_override = b.pose_global;
			} else {
				b.pose_global = b.rest;
				b.pose_global_no_override = b.pose_global;
			}
		}

		if (rest_dirty) {
			b.global_rest = b.parent >= 0 ? bonesptr[b.parent].global_rest * b.rest : b.rest;
		}

		if (b.local_pose_override_amount >= CMP_EPSILON) {
			Transform override_local_pose;
			if (b.parent >= 0) {
				override_local_pose = bonesptr[b.parent].pose_global * b.local_pose_override;
			} else {
				override_local_pose = b.local_pose_override;
			}
			b.pose_global = b.pose_global.interpolate_with(override_local_pose, b.local_pose_override_amount);
		}

		if (b.global_pose_override_amount >= CMP_EPSILON) {
			b.pose_global = b.pose_global.interpolate_with(b.global_pose_override, b.global_pose_override_amount);
		}

		if (b.local_pose_override_reset) {
			b.local_pose_override_amount = 0.0;
		}
		if (b.global_pose_override_reset) {
			b.global_pose_override_amount = 0.0;
		}

		// Add the bone's children to the list of bones to be processed
		int child_bone_size = b.child_bones.size();
		for (int i = 0; i < child_bone_size; i++) {
			bones_to_process.push_back(b.child_bones[i]);
		}

		emit_signal(SceneStringNames::get_singleton()->bone_pose_changed, current_bone_idx);
	}

	rest_dirty = false;
}

Transform Skeleton::global_pose_to_world_transform(Transform p_global_pose) {
	return get_global_transform() * p_global_pose;
}

Transform Skeleton::world_transform_to_global_pose(Transform p_world_transform) {
	return get_global_transform().affine_inverse() * p_world_transform;
}

Transform Skeleton::global_pose_to_local_pose(int p_bone_idx, Transform p_global_pose) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone_idx, bone_size, Transform());

	if (bones[p_bone_idx].parent >= 0) {
		int parent_bone_idx = bones[p_bone_idx].parent;
		Transform conversion_transform = get_bone_global_pose(parent_bone_idx).affine_inverse();
		return conversion_transform * p_global_pose;
	} else {
		return p_global_pose;
	}
}

Transform Skeleton::local_pose_to_global_pose(int p_bone_idx, Transform p_local_pose) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone_idx, bone_size, Transform());
	if (bones[p_bone_idx].parent >= 0) {
		int parent_bone_idx = bones[p_bone_idx].parent;
		return bones[parent_bone_idx].pose_global * p_local_pose;
	} else {
		return p_local_pose;
	}
}

Basis Skeleton::global_pose_z_forward_to_bone_forward(int p_bone_idx, Basis p_basis) {
	const int bone_size = bones.size();
	ERR_FAIL_INDEX_V(p_bone_idx, bone_size, Basis());
	Basis return_basis = p_basis;

	if (bones[p_bone_idx].rest_bone_forward_axis < 0) {
		update_bone_rest_forward_vector(p_bone_idx, true);
	}

	if (bones[p_bone_idx].rest_bone_forward_axis == BONE_AXIS_X_FORWARD) {
		return_basis.rotate_local(Vector3(0, 1, 0), (Math_PI / 2.0));
	} else if (bones[p_bone_idx].rest_bone_forward_axis == BONE_AXIS_NEGATIVE_X_FORWARD) {
		return_basis.rotate_local(Vector3(0, 1, 0), -(Math_PI / 2.0));
	} else if (bones[p_bone_idx].rest_bone_forward_axis == BONE_AXIS_Y_FORWARD) {
		return_basis.rotate_local(Vector3(1, 0, 0), -(Math_PI / 2.0));
	} else if (bones[p_bone_idx].rest_bone_forward_axis == BONE_AXIS_NEGATIVE_Y_FORWARD) {
		return_basis.rotate_local(Vector3(1, 0, 0), (Math_PI / 2.0));
	} else if (bones[p_bone_idx].rest_bone_forward_axis == BONE_AXIS_Z_FORWARD) {
		// Do nothing!
	} else if (bones[p_bone_idx].rest_bone_forward_axis == BONE_AXIS_NEGATIVE_Z_FORWARD) {
		return_basis.rotate_local(Vector3(0, 0, 1), Math_PI);
	}

	return return_basis;
}

// Modifications

#ifndef _3D_DISABLED

void Skeleton::set_modification_stack(Ref<SkeletonModificationStack3D> p_stack) {
	if (modification_stack.is_valid()) {
		modification_stack->is_setup = false;
		modification_stack->set_skeleton(nullptr);
	}

	modification_stack = p_stack;
	if (modification_stack.is_valid()) {
		modification_stack->set_skeleton(this);
		modification_stack->setup();
	}
}
Ref<SkeletonModificationStack3D> Skeleton::get_modification_stack() {
	return modification_stack;
}

void Skeleton::execute_modifications(real_t p_delta, int p_execution_mode) {
	if (!modification_stack.is_valid()) {
		return;
	}

	// Needed to avoid the issue where the stack looses reference to the skeleton when the scene is saved.
	if (modification_stack->skeleton != this) {
		modification_stack->set_skeleton(this);
	}

	modification_stack->execute(p_delta, p_execution_mode);
}

#endif // _3D_DISABLED

void Skeleton::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_bone", "name"), &Skeleton::add_bone);
	ClassDB::bind_method(D_METHOD("find_bone", "name"), &Skeleton::find_bone);
	ClassDB::bind_method(D_METHOD("get_bone_name", "bone_idx"), &Skeleton::get_bone_name);
	ClassDB::bind_method(D_METHOD("set_bone_name", "bone_idx", "name"), &Skeleton::set_bone_name);

	ClassDB::bind_method(D_METHOD("get_bone_parent", "bone_idx"), &Skeleton::get_bone_parent);
	ClassDB::bind_method(D_METHOD("set_bone_parent", "bone_idx", "parent_idx"), &Skeleton::set_bone_parent);

	ClassDB::bind_method(D_METHOD("get_bone_count"), &Skeleton::get_bone_count);

	ClassDB::bind_method(D_METHOD("unparent_bone_and_rest", "bone_idx"), &Skeleton::unparent_bone_and_rest);

	ClassDB::bind_method(D_METHOD("get_bone_children", "bone_idx"), &Skeleton::get_bone_children);
	ClassDB::bind_method(D_METHOD("set_bone_children", "bone_idx", "bone_children"), &Skeleton::set_bone_children);
	ClassDB::bind_method(D_METHOD("add_bone_child", "bone_idx", "child_bone_idx"), &Skeleton::add_bone_child);
	ClassDB::bind_method(D_METHOD("remove_bone_child", "bone_idx", "child_bone_idx"), &Skeleton::remove_bone_child);

	ClassDB::bind_method(D_METHOD("get_parentless_bones"), &Skeleton::get_parentless_bones);

	ClassDB::bind_method(D_METHOD("get_bone_rest", "bone_idx"), &Skeleton::get_bone_rest);
	ClassDB::bind_method(D_METHOD("set_bone_rest", "bone_idx", "rest"), &Skeleton::set_bone_rest);
	ClassDB::bind_method(D_METHOD("get_bone_global_rest", "bone_idx"), &Skeleton::get_bone_global_rest);

	ClassDB::bind_method(D_METHOD("create_skin_from_rest_transforms"), &Skeleton::create_skin_from_rest_transforms);

	ClassDB::bind_method(D_METHOD("register_skin", "skin"), &Skeleton::register_skin);

	ClassDB::bind_method(D_METHOD("localize_rests"), &Skeleton::localize_rests);

	ClassDB::bind_method(D_METHOD("clear_bones"), &Skeleton::clear_bones);

	ClassDB::bind_method(D_METHOD("set_bone_pose", "bone_idx", "pose"), &Skeleton::set_bone_pose);
	ClassDB::bind_method(D_METHOD("set_bone_pose_position", "bone_idx", "position"), &Skeleton::set_bone_pose_position);
	ClassDB::bind_method(D_METHOD("set_bone_pose_rotation", "bone_idx", "rotation"), &Skeleton::set_bone_pose_rotation);
	ClassDB::bind_method(D_METHOD("set_bone_pose_scale", "bone_idx", "scale"), &Skeleton::set_bone_pose_scale);

	ClassDB::bind_method(D_METHOD("get_bone_pose", "bone_idx"), &Skeleton::get_bone_pose);
	ClassDB::bind_method(D_METHOD("get_bone_pose_position", "bone_idx"), &Skeleton::get_bone_pose_position);
	ClassDB::bind_method(D_METHOD("get_bone_pose_rotation", "bone_idx"), &Skeleton::get_bone_pose_rotation);
	ClassDB::bind_method(D_METHOD("get_bone_pose_scale", "bone_idx"), &Skeleton::get_bone_pose_scale);

	ClassDB::bind_method(D_METHOD("is_bone_enabled", "bone_idx"), &Skeleton::is_bone_enabled);
	ClassDB::bind_method(D_METHOD("set_bone_enabled", "bone_idx", "enabled"), &Skeleton::set_bone_enabled, true);

	ClassDB::bind_method(D_METHOD("clear_bones_global_pose_override"), &Skeleton::clear_bones_global_pose_override);
	ClassDB::bind_method(D_METHOD("set_bone_global_pose_override", "bone_idx", "pose", "amount", "persistent"), &Skeleton::set_bone_global_pose_override, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("get_bone_global_pose_override", "bone_idx"), &Skeleton::get_bone_global_pose_override);
	ClassDB::bind_method(D_METHOD("get_bone_global_pose", "bone_idx"), &Skeleton::get_bone_global_pose);
	ClassDB::bind_method(D_METHOD("get_bone_global_pose_no_override", "bone_idx"), &Skeleton::get_bone_global_pose_no_override);

	ClassDB::bind_method(D_METHOD("clear_bones_local_pose_override"), &Skeleton::clear_bones_local_pose_override);
	ClassDB::bind_method(D_METHOD("set_bone_local_pose_override", "bone_idx", "pose", "amount", "persistent"), &Skeleton::set_bone_local_pose_override, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("get_bone_local_pose_override", "bone_idx"), &Skeleton::get_bone_local_pose_override);

	ClassDB::bind_method(D_METHOD("force_update_all_bone_transforms"), &Skeleton::force_update_all_bone_transforms);
	ClassDB::bind_method(D_METHOD("force_update_bone_child_transform", "bone_idx"), &Skeleton::force_update_bone_children_transforms);

	// Helper functions
	ClassDB::bind_method(D_METHOD("global_pose_to_world_transform", "global_pose"), &Skeleton::global_pose_to_world_transform);
	ClassDB::bind_method(D_METHOD("world_transform_to_global_pose", "world_transform"), &Skeleton::world_transform_to_global_pose);
	ClassDB::bind_method(D_METHOD("global_pose_to_local_pose", "bone_idx", "global_pose"), &Skeleton::global_pose_to_local_pose);
	ClassDB::bind_method(D_METHOD("local_pose_to_global_pose", "bone_idx", "local_pose"), &Skeleton::local_pose_to_global_pose);
	ClassDB::bind_method(D_METHOD("global_pose_z_forward_to_bone_forward", "bone_idx", "basis"), &Skeleton::global_pose_z_forward_to_bone_forward);

	ClassDB::bind_method(D_METHOD("set_show_rest_only"), &Skeleton::set_show_rest_only);
	ClassDB::bind_method(D_METHOD("is_show_rest_only"), &Skeleton::is_show_rest_only);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "show_rest_only"), "set_show_rest_only", "is_show_rest_only");

#ifndef _3D_DISABLED

	ClassDB::bind_method(D_METHOD("get_animate_physical_bones"), &Skeleton::get_animate_physical_bones);
	ClassDB::bind_method(D_METHOD("set_animate_physical_bones"), &Skeleton::set_animate_physical_bones);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "animate_physical_bones"), "set_animate_physical_bones", "get_animate_physical_bones");

	ClassDB::bind_method(D_METHOD("physical_bones_stop_simulation"), &Skeleton::physical_bones_stop_simulation);
	ClassDB::bind_method(D_METHOD("physical_bones_start_simulation", "bones"), &Skeleton::physical_bones_start_simulation_on, DEFVAL(Array()));
	ClassDB::bind_method(D_METHOD("physical_bones_add_collision_exception", "exception"), &Skeleton::physical_bones_add_collision_exception);
	ClassDB::bind_method(D_METHOD("physical_bones_remove_collision_exception", "exception"), &Skeleton::physical_bones_remove_collision_exception);

#endif // _3D_DISABLED

	// Modifications
	ClassDB::bind_method(D_METHOD("set_modification_stack", "modification_stack"), &Skeleton::set_modification_stack);
	ClassDB::bind_method(D_METHOD("get_modification_stack"), &Skeleton::get_modification_stack);
	ClassDB::bind_method(D_METHOD("execute_modifications", "delta", "execution_mode"), &Skeleton::execute_modifications);

	//TODO Not sure if this is useful or not in runtime
	//#ifdef TOOLS_ENABLED
	ADD_SIGNAL(MethodInfo("pose_updated"));
	//#endif // TOOLS_ENABLED

	ADD_SIGNAL(MethodInfo("bone_pose_changed", PropertyInfo(Variant::INT, "bone_idx")));
	ADD_SIGNAL(MethodInfo("bones_updated"));
	ADD_SIGNAL(MethodInfo("bone_enabled_changed", PropertyInfo(Variant::INT, "bone_idx")));
	ADD_SIGNAL(MethodInfo("show_rest_only_changed"));

	BIND_CONSTANT(NOTIFICATION_UPDATE_SKELETON);

	ClassDB::bind_method(D_METHOD("remove_bone", "bone_idx"), &Skeleton::remove_bone);
}

Skeleton::Skeleton() {
	animate_physical_bones = true;
	dirty = false;
	version = 1;
	process_order_dirty = true;
	show_rest_only = false;
	rest_dirty = false;
	updating = 0;
}

Skeleton::~Skeleton() {
	//some skins may remain bound
	for (RBSet<SkinReference *>::Element *E = skin_bindings.front(); E; E = E->next()) {
		E->get()->skeleton_node = nullptr;
	}
}

void Skeleton::set_selected_bone(int p_bone) {
	selected_bone = p_bone;
	update_gizmos();
	return;
}

int Skeleton::get_selected_bone() const {
	return selected_bone;
}

void Skeleton::remove_bone(const int p_bone_idx) {
	ERR_FAIL_INDEX(p_bone_idx, bones.size());

	for (int i = 0; i < bones.size(); ++i) {
		ERR_FAIL_COND_MSG(bones[i].parent == p_bone_idx, "Cannot remove bone if it has children.");
	}

#ifndef _3D_DISABLED
	const Bone &bonet = bones[p_bone_idx];

	ERR_FAIL_COND_MSG(bonet.physical_bone || bonet.cache_parent_physical_bone, "Cannot remove bone if it has a physical bone attached, please remove physical bones first.");
#endif // _3D_DISABLED

	bones.remove(p_bone_idx);

	for (int i = 0; i < bones.size(); ++i) {
		int parent = bones[i].parent;

		if (parent > p_bone_idx) {
			bones.write[i].parent = parent - 1;
		}
	}

	process_order_dirty = true;
	version++;

	_update_process_order();

	// pose changed, rebuild cache of inverses
	const Bone *bonesptr = bones.ptr();
	int len = bones.size();

	for (RBSet<SkinReference *>::Element *E = skin_bindings.front(); E; E = E->next()) {
		Ref<SkinReference> sr = E->get();
		Ref<Skin> skin = sr->skin;

		skin->set_bind_count(bones.size());

		// calculate global rests and invert them
		Vector<int> bones_to_process = get_parentless_bones();
		while (bones_to_process.size() > 0) {
			int current_bone_idx = bones_to_process[0];
			bones_to_process.erase(current_bone_idx);
			const Bone &b = bonesptr[current_bone_idx];

			// Note: the code below may not work by default. May need to track an integer for the bone pose index order
			// in the while loop, instead of using current_bone_idx.

			if (b.parent >= 0) {
				skin->set_bind_pose(current_bone_idx, skin->get_bind_pose(b.parent) * b.rest);
			} else {
				skin->set_bind_pose(current_bone_idx, b.rest);
			}
		}

		for (int i = 0; i < len; i++) {
			//the inverse is what is actually required
			skin->set_bind_bone(i, i);
			skin->set_bind_pose(i, skin->get_bind_pose(i).affine_inverse());
		}
	}

	property_list_changed_notify();
	_make_dirty();
	update_gizmos();
}
