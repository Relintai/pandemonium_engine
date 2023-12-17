/*************************************************************************/
/*  physical_bone_editor_plugin.cpp                                      */
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

#include "physical_bone_editor_plugin.h"

#include "editor/editor_settings.h"

#include "../nodes/physical_bone.h"
#include "../nodes/skeleton.h"
#include "../resources/skin.h"

PhysicalBoneSpatialGizmoPlugin::PhysicalBoneSpatialGizmoPlugin() {
	create_material("joint_material", EDITOR_DEF("editors/3d_gizmos/gizmo_colors/joint", Color(0.5, 0.8, 1)));
}

bool PhysicalBoneSpatialGizmoPlugin::has_gizmo(Spatial *p_spatial) {
	return Object::cast_to<PhysicalBone>(p_spatial) != nullptr;
}

String PhysicalBoneSpatialGizmoPlugin::get_gizmo_name() const {
	return "PhysicalBones";
}

int PhysicalBoneSpatialGizmoPlugin::get_priority() const {
	return -1;
}

void PhysicalBoneSpatialGizmoPlugin::redraw(EditorSpatialGizmo *p_gizmo) {
	p_gizmo->clear();

	PhysicalBone *physical_bone = Object::cast_to<PhysicalBone>(p_gizmo->get_spatial_node());

	if (!physical_bone) {
		return;
	}

	Skeleton *sk(physical_bone->find_skeleton_parent());
	if (!sk) {
		return;
	}

	PhysicalBone *pb(sk->get_physical_bone(physical_bone->get_bone_id()));
	if (!pb) {
		return;
	}

	PhysicalBone *pbp(sk->get_physical_bone_parent(physical_bone->get_bone_id()));
	if (!pbp) {
		return;
	}

	Vector<Vector3> points;

	switch (physical_bone->get_joint_type()) {
		case PhysicalBone::JOINT_TYPE_PIN: {
			JointSpatialGizmoPlugin::CreatePinJointGizmo(physical_bone->get_joint_offset(), points);
		} break;
		case PhysicalBone::JOINT_TYPE_CONE: {
			const PhysicalBone::ConeJointData *cjd(static_cast<const PhysicalBone::ConeJointData *>(physical_bone->get_joint_data()));
			JointSpatialGizmoPlugin::CreateConeTwistJointGizmo(
					physical_bone->get_joint_offset(),
					physical_bone->get_global_transform() * physical_bone->get_joint_offset(),
					pb->get_global_transform(),
					pbp->get_global_transform(),
					cjd->swing_span,
					cjd->twist_span,
					&points,
					&points);
		} break;
		case PhysicalBone::JOINT_TYPE_HINGE: {
			const PhysicalBone::HingeJointData *hjd(static_cast<const PhysicalBone::HingeJointData *>(physical_bone->get_joint_data()));
			JointSpatialGizmoPlugin::CreateHingeJointGizmo(
					physical_bone->get_joint_offset(),
					physical_bone->get_global_transform() * physical_bone->get_joint_offset(),
					pb->get_global_transform(),
					pbp->get_global_transform(),
					hjd->angular_limit_lower,
					hjd->angular_limit_upper,
					hjd->angular_limit_enabled,
					points,
					&points,
					&points);
		} break;
		case PhysicalBone::JOINT_TYPE_SLIDER: {
			const PhysicalBone::SliderJointData *sjd(static_cast<const PhysicalBone::SliderJointData *>(physical_bone->get_joint_data()));
			JointSpatialGizmoPlugin::CreateSliderJointGizmo(
					physical_bone->get_joint_offset(),
					physical_bone->get_global_transform() * physical_bone->get_joint_offset(),
					pb->get_global_transform(),
					pbp->get_global_transform(),
					sjd->angular_limit_lower,
					sjd->angular_limit_upper,
					sjd->linear_limit_lower,
					sjd->linear_limit_upper,
					points,
					&points,
					&points);
		} break;
		case PhysicalBone::JOINT_TYPE_6DOF: {
			const PhysicalBone::SixDOFJointData *sdofjd(static_cast<const PhysicalBone::SixDOFJointData *>(physical_bone->get_joint_data()));
			JointSpatialGizmoPlugin::CreateGeneric6DOFJointGizmo(
					physical_bone->get_joint_offset(),

					physical_bone->get_global_transform() * physical_bone->get_joint_offset(),
					pb->get_global_transform(),
					pbp->get_global_transform(),

					sdofjd->axis_data[0].angular_limit_lower,
					sdofjd->axis_data[0].angular_limit_upper,
					sdofjd->axis_data[0].linear_limit_lower,
					sdofjd->axis_data[0].linear_limit_upper,
					sdofjd->axis_data[0].angular_limit_enabled,
					sdofjd->axis_data[0].linear_limit_enabled,

					sdofjd->axis_data[1].angular_limit_lower,
					sdofjd->axis_data[1].angular_limit_upper,
					sdofjd->axis_data[1].linear_limit_lower,
					sdofjd->axis_data[1].linear_limit_upper,
					sdofjd->axis_data[1].angular_limit_enabled,
					sdofjd->axis_data[1].linear_limit_enabled,

					sdofjd->axis_data[2].angular_limit_lower,
					sdofjd->axis_data[2].angular_limit_upper,
					sdofjd->axis_data[2].linear_limit_lower,
					sdofjd->axis_data[2].linear_limit_upper,
					sdofjd->axis_data[2].angular_limit_enabled,
					sdofjd->axis_data[2].linear_limit_enabled,

					points,
					&points,
					&points);
		} break;
		default:
			return;
	}

	Ref<Material> material = get_material("joint_material", p_gizmo);

	p_gizmo->add_collision_segments(points);
	p_gizmo->add_lines(points, material);
}
