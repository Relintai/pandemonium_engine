#ifndef SKELETON_2D_H
#define SKELETON_2D_H

/*************************************************************************/
/*  skeleton_2d.h                                                        */
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

#include "../resources/skeleton_modification_2d.h"
#include "scene/main/node_2d.h"

class Skeleton2D;

class Bone2D : public Node2D {
	GDCLASS(Bone2D, Node2D);

	friend class Skeleton2D;
#ifdef TOOLS_ENABLED
	friend class AnimatedValuesBackup;
#endif

	Bone2D *parent_bone;
	Skeleton2D *skeleton;
	Transform2D rest;

	bool autocalculate_length_and_angle;
	float length;
	float bone_angle;

	int skeleton_index;

	void calculate_length_and_rotation();

#ifdef TOOLS_ENABLED
	RID editor_gizmo_rid;
	bool _editor_get_bone_shape(Vector<Vector2> *p_shape, Vector<Vector2> *p_outline_shape, Bone2D *p_other_bone);
	bool _editor_show_bone_gizmo = true;
#endif // TOOLS ENABLED

protected:
	void _notification(int p_what);
	static void _bind_methods();
	bool _set(const StringName &p_path, const Variant &p_value);
	bool _get(const StringName &p_path, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

public:
	Transform2D cache_transform;
	bool copy_transform_to_cache = true;

	void set_rest(const Transform2D &p_rest);
	Transform2D get_rest() const;
	void apply_rest();
	Transform2D get_skeleton_rest() const;

	String get_configuration_warning() const;

	void set_default_length(float p_length);
	float get_default_length() const;

	void set_autocalculate_length_and_angle(bool p_autocalculate);
	bool get_autocalculate_length_and_angle() const;

	void set_length(float p_length);
	float get_length() const;

	void set_bone_angle(float p_angle);
	float get_bone_angle() const;

	int get_index_in_skeleton() const;

#ifdef TOOLS_ENABLED
	void _editor_set_show_bone_gizmo(bool p_show_gizmo);
	bool _editor_get_show_bone_gizmo() const;
#endif // TOOLS_ENABLED

	Bone2D();
	~Bone2D();
};

class SkeletonModificationStack2D;

class Skeleton2D : public Node2D {
	GDCLASS(Skeleton2D, Node2D);

	friend class Bone2D;
#ifdef TOOLS_ENABLED
	friend class AnimatedValuesBackup;
#endif

	struct Bone {
		bool operator<(const Bone &p_bone) const {
			return p_bone.bone->is_greater_than(bone);
		}

		Bone2D *bone;
		int parent_index;
		Transform2D accum_transform;
		Transform2D rest_inverse;

		//Transform2D local_pose_cache;
		Transform2D local_pose_override;
		float local_pose_override_amount = 0;
		bool local_pose_override_persistent = false;
	};

	Vector<Bone> bones;

	bool bone_setup_dirty;
	void _make_bone_setup_dirty();
	void _update_bone_setup();

	bool transform_dirty;
	void _make_transform_dirty();
	void _update_transform();

	RID skeleton;

	Ref<SkeletonModificationStack2D> modification_stack;

	void _update_process_mode();
	void _ensure_update_interpolation_data();

	struct InterpolationData {
		Transform2D xform_curr;
		Transform2D xform_prev;
		uint32_t last_update_physics_tick = UINT32_MAX;
	} _interpolation_data;

protected:
	void _notification(int p_what);
	static void _bind_methods();
	bool _set(const StringName &p_path, const Variant &p_value);
	bool _get(const StringName &p_path, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	virtual void _physics_interpolated_changed();

public:
	int get_bone_count() const;
	Bone2D *get_bone(int p_idx);

	RID get_skeleton() const;

	void set_bone_local_pose_override(int p_bone_idx, Transform2D p_override, float p_amount, bool p_persistent = true);
	Transform2D get_bone_local_pose_override(int p_bone_idx);

	Ref<SkeletonModificationStack2D> get_modification_stack() const;
	void set_modification_stack(Ref<SkeletonModificationStack2D> p_stack);
	void execute_modifications(float p_delta, int p_execution_mode);

	Skeleton2D();
	~Skeleton2D();
};

#endif // SKELETON_2D_H
