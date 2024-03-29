#ifndef SKELETON_MODIFICATION_2D_LOOKAT_H
#define SKELETON_MODIFICATION_2D_LOOKAT_H

/*************************************************************************/
/*  skeleton_modification_2d_lookat.h                                    */
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

#include "skeleton_modification_2d.h"

///////////////////////////////////////
// SkeletonModification2DLookAt
///////////////////////////////////////

class SkeletonModificationStack2D;
class Node2D;

class SkeletonModification2DLookAt : public SkeletonModification2D {
	GDCLASS(SkeletonModification2DLookAt, SkeletonModification2D);

private:
	int bone_idx;
	NodePath bone2d_node;
	ObjectID bone2d_node_cache;

	NodePath target_node;
	ObjectID target_node_cache;
	Node2D *target_node_reference;

	float additional_rotation;
	bool enable_constraint;
	float constraint_angle_min;
	float constraint_angle_max;
	bool constraint_angle_invert;
	bool constraint_in_localspace;

	void update_bone2d_cache();
	void update_target_cache();

protected:
	static void _bind_methods();
	bool _set(const StringName &p_path, const Variant &p_value);
	bool _get(const StringName &p_path, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

public:
	void _execute(float p_delta);
	void _setup_modification(Ref<SkeletonModificationStack2D> p_stack);
	void _draw_editor_gizmo();

	void set_bone2d_node(const NodePath &p_target_node);
	NodePath get_bone2d_node() const;
	void set_bone_index(int p_idx);
	int get_bone_index() const;

	void set_target_node(const NodePath &p_target_node);
	NodePath get_target_node() const;

	void set_additional_rotation(float p_rotation);
	float get_additional_rotation() const;

	void set_enable_constraint(bool p_constraint);
	bool get_enable_constraint() const;
	void set_constraint_angle_min(float p_angle_min);
	float get_constraint_angle_min() const;
	void set_constraint_angle_max(float p_angle_max);
	float get_constraint_angle_max() const;
	void set_constraint_angle_invert(bool p_invert);
	bool get_constraint_angle_invert() const;
	void set_constraint_in_localspace(bool p_constraint_in_localspace);
	bool get_constraint_in_localspace() const;

	SkeletonModification2DLookAt();
	~SkeletonModification2DLookAt();
};

#endif // SKELETON_MODIFICATION_2D_LOOKAT_H
