#ifndef SKELETON_H
#define SKELETON_H
/*************************************************************************/
/*  skeleton.h                                                           */
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

#include "core/object/reference.h"
#include "core/containers/rid.h"
#include "scene/3d/spatial.h"

#ifndef _3D_DISABLED
typedef int BoneId;

class PhysicalBone;
#endif // _3D_DISABLED

class SkeletonModificationStack3D;
class Skeleton;
class Skin;

class SkinReference : public Reference {
	GDCLASS(SkinReference, Reference)
	friend class Skeleton;

	Skeleton *skeleton_node = nullptr;
	RID skeleton;
	Ref<Skin> skin;
	uint32_t bind_count = 0;
	uint64_t skeleton_version = 0;
	Vector<uint32_t> skin_bone_indices;
	uint32_t *skin_bone_indices_ptrs = nullptr;
	void _skin_changed();

protected:
	static void _bind_methods();

public:
	RID get_skeleton() const;
	Skeleton *get_skeleton_node() const;
	Ref<Skin> get_skin() const;
	~SkinReference();
};

class Skeleton : public Spatial {
	GDCLASS(Skeleton, Spatial);

private:
	friend class SkinReference;

	RBSet<SkinReference *> skin_bindings;

	void _skin_changed();

	struct Bone {
		String name;

		bool enabled;
		int parent;

		Transform rest;
		Transform global_rest;

		_FORCE_INLINE_ void update_pose_cache() {
			if (pose_cache_dirty) {
				pose_cache.basis.set_quaternion_scale(pose_rotation, pose_scale);
				pose_cache.origin = pose_position;
				pose_cache_dirty = false;
			}
		}
		bool pose_cache_dirty;
		Transform pose_cache;
		Vector3 pose_position;
		Quaternion pose_rotation;
		Vector3 pose_scale;

		Transform pose_global;
		Transform pose_global_no_override;

		float global_pose_override_amount;
		bool global_pose_override_reset;
		Transform global_pose_override;

#ifndef _3D_DISABLED
		PhysicalBone *physical_bone;
		PhysicalBone *cache_parent_physical_bone;
#endif // _3D_DISABLED

		real_t local_pose_override_amount;
		bool local_pose_override_reset;
		Transform local_pose_override;

		Vector<int> child_bones;

		// The forward direction vector and rest bone forward axis are cached because they do not change
		// 99% of the time, but recalculating them can be expensive on models with many bones.
		Vector3 rest_bone_forward_vector;
		int rest_bone_forward_axis;

		Bone() {
			parent = -1;
			enabled = true;
			pose_cache_dirty = true;
			pose_scale = Vector3(1, 1, 1);
			global_pose_override_amount = 0;
			global_pose_override_reset = false;
#ifndef _3D_DISABLED
			physical_bone = nullptr;
			cache_parent_physical_bone = nullptr;
#endif // _3D_DISABLED
			local_pose_override_amount = 0;
			local_pose_override_reset = false;
			child_bones = Vector<int>();

			rest_bone_forward_vector = Vector3(0, 0, 0);
			rest_bone_forward_axis = -1;
		}
	};

	bool animate_physical_bones;
	Vector<Bone> bones;
	bool process_order_dirty;

	Vector<int> parentless_bones;

	void _make_dirty();
	bool dirty;
	bool rest_dirty;
	int updating;

	bool show_rest_only;

	uint64_t version;

	void _update_process_order();

protected:
	bool _get(const StringName &p_path, Variant &r_ret) const;
	bool _set(const StringName &p_path, const Variant &p_value);
	void _get_property_list(List<PropertyInfo> *p_list) const;
	//void _validate_property(PropertyInfo &property) const;

	void _notification(int p_what);

	static void _bind_methods();

#ifndef _3D_DISABLED
	Ref<SkeletonModificationStack3D> modification_stack;
#endif // _3D_DISABLED

public:
	enum Bone_Forward_Axis {
		BONE_AXIS_X_FORWARD = 0,
		BONE_AXIS_Y_FORWARD = 1,
		BONE_AXIS_Z_FORWARD = 2,
		BONE_AXIS_NEGATIVE_X_FORWARD = 3,
		BONE_AXIS_NEGATIVE_Y_FORWARD = 4,
		BONE_AXIS_NEGATIVE_Z_FORWARD = 5,
	};

	enum {

		NOTIFICATION_UPDATE_SKELETON = 50
	};

	// skeleton creation api
	void add_bone(const String &p_name);
	int find_bone(const String &p_name) const;
	String get_bone_name(int p_bone) const;
	void set_bone_name(int p_bone, const String &p_name);

	bool is_bone_parent_of(int p_bone_id, int p_parent_bone_id) const;

	void set_bone_parent(int p_bone, int p_parent);
	int get_bone_parent(int p_bone) const;

	void unparent_bone_and_rest(int p_bone);

	Vector<int> get_bone_children(int p_bone);
	void set_bone_children(int p_bone, Vector<int> p_children);
	void add_bone_child(int p_bone, int p_child);
	void remove_bone_child(int p_bone, int p_child);
	Vector<int> get_parentless_bones();

	int get_bone_count() const;

	void set_bone_rest(int p_bone, const Transform &p_rest);
	Transform get_bone_rest(int p_bone) const;
	Transform get_bone_global_rest(int p_bone) const;
	Transform get_bone_global_pose(int p_bone) const;
	Transform get_bone_global_pose_no_override(int p_bone) const;

	void set_bone_enabled(int p_bone, bool p_enabled);
	bool is_bone_enabled(int p_bone) const;

	void set_show_rest_only(bool p_enabled);
	bool is_show_rest_only() const;

	void clear_bones();

	// posing api
	void set_bone_pose(int p_bone, const Transform &p_pose);
	void set_bone_pose_position(int p_bone, const Vector3 &p_position);
	void set_bone_pose_rotation(int p_bone, const Quaternion &p_rotation);
	void set_bone_pose_scale(int p_bone, const Vector3 &p_scale);

	Transform get_bone_pose(int p_bone) const;
	Vector3 get_bone_pose_position(int p_bone) const;
	Quaternion get_bone_pose_rotation(int p_bone) const;
	Vector3 get_bone_pose_scale(int p_bone) const;

	void clear_bones_global_pose_override();
	Transform get_bone_global_pose_override(int p_bone) const;
	void set_bone_global_pose_override(int p_bone, const Transform &p_pose, real_t p_amount, bool p_persistent = false);

	void clear_bones_local_pose_override();
	Transform get_bone_local_pose_override(int p_bone) const;
	void set_bone_local_pose_override(int p_bone, const Transform &p_pose, real_t p_amount, bool p_persistent = false);

	void localize_rests(); // used for loaders and tools

	Ref<Skin> create_skin_from_rest_transforms();

	Ref<SkinReference> register_skin(const Ref<Skin> &p_skin);

	void init_pose();

	void force_update_all_dirty_bones();
	void force_update_all_bone_transforms();
	void force_update_bone_children_transforms(int bone_idx);

	void update_bone_rest_forward_vector(int p_bone, bool p_force_update = false);
	void update_bone_rest_forward_axis(int p_bone, bool p_force_update = false);
	Vector3 get_bone_axis_forward_vector(int p_bone);
	int get_bone_axis_forward_enum(int p_bone);

	// Helper functions
	Transform global_pose_to_world_transform(Transform p_global_pose);
	Transform world_transform_to_global_pose(Transform p_transform);
	Transform global_pose_to_local_pose(int p_bone_idx, Transform p_global_pose);
	Transform local_pose_to_global_pose(int p_bone_idx, Transform p_local_pose);

	Basis global_pose_z_forward_to_bone_forward(int p_bone_idx, Basis p_basis);

	// Modifications
#ifndef _3D_DISABLED
	Ref<SkeletonModificationStack3D> get_modification_stack();
	void set_modification_stack(Ref<SkeletonModificationStack3D> p_stack);
	void execute_modifications(real_t p_delta, int p_execution_mode);
#endif // _3D_DISABLED

	// Physical bone API
#ifndef _3D_DISABLED

	void set_animate_physical_bones(bool p_animate);
	bool get_animate_physical_bones() const;

	void bind_physical_bone_to_bone(int p_bone, PhysicalBone *p_physical_bone);
	void unbind_physical_bone_from_bone(int p_bone);

	PhysicalBone *get_physical_bone(int p_bone);
	PhysicalBone *get_physical_bone_parent(int p_bone);

private:
	/// This is a slow API os it's cached
	PhysicalBone *_get_physical_bone_parent(int p_bone);
	void _rebuild_physical_bones_cache();

public:
	void physical_bones_stop_simulation();
	void physical_bones_start_simulation_on(const Array &p_bones);
	void physical_bones_add_collision_exception(RID p_exception);
	void physical_bones_remove_collision_exception(RID p_exception);
#endif // _3D_DISABLED

public:
	Skeleton();
	~Skeleton();

public:
	void set_selected_bone(int p_bone);
	int get_selected_bone() const;

	int selected_bone = -1;

	void remove_bone(const int p_bone_idx);
};

#endif
