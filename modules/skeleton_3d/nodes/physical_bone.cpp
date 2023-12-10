/*************************************************************************/
/*  physics_body.cpp                                                     */
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

#include "physical_bone.h"

#include "core/core_string_names.h"
#include "core/config/engine.h"
#include "core/containers/list.h"
#include "core/object/method_bind_ext.gen.inc"
#include "core/object/object.h"
#include "core/config/project_settings.h"
#include "core/containers/rid.h"
#include "scene/resources/physics_material.h"
#include "scene/main/scene_string_names.h"
#include "skeleton.h"

#ifdef TOOLS_ENABLED
#include "editor/plugins/spatial_editor_plugin.h"
#endif

bool PhysicalBone::JointData::_set(const StringName &p_name, const Variant &p_value, RID j) {
	return false;
}

bool PhysicalBone::JointData::_get(const StringName &p_name, Variant &r_ret) const {
	return false;
}

void PhysicalBone::JointData::_get_property_list(List<PropertyInfo> *p_list) const {
}

void PhysicalBone::apply_central_impulse(const Vector3 &p_impulse) {
	PhysicsServer::get_singleton()->body_apply_central_impulse(get_rid(), p_impulse);
}

void PhysicalBone::apply_impulse(const Vector3 &p_pos, const Vector3 &p_impulse) {
	PhysicsServer::get_singleton()->body_apply_impulse(get_rid(), p_pos, p_impulse);
}

void PhysicalBone::reset_physics_simulation_state() {
	if (simulate_physics) {
		_start_physics_simulation();
	} else {
		_stop_physics_simulation();
	}
}

void PhysicalBone::reset_to_rest_position() {
	if (parent_skeleton) {
		if (-1 == bone_id) {
			set_global_transform(parent_skeleton->get_global_transform() * body_offset);
		} else {
			set_global_transform(parent_skeleton->get_global_transform() * parent_skeleton->get_bone_global_pose(bone_id) * body_offset);
		}
	}
}

bool PhysicalBone::PinJointData::_set(const StringName &p_name, const Variant &p_value, RID j) {
	if (JointData::_set(p_name, p_value, j)) {
		return true;
	}

	if ("joint_constraints/bias" == p_name) {
		bias = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->pin_joint_set_param(j, PhysicsServer::PIN_JOINT_BIAS, bias);
		}

	} else if ("joint_constraints/damping" == p_name) {
		damping = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->pin_joint_set_param(j, PhysicsServer::PIN_JOINT_DAMPING, damping);
		}

	} else if ("joint_constraints/impulse_clamp" == p_name) {
		impulse_clamp = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->pin_joint_set_param(j, PhysicsServer::PIN_JOINT_IMPULSE_CLAMP, impulse_clamp);
		}

	} else {
		return false;
	}

	return true;
}

bool PhysicalBone::PinJointData::_get(const StringName &p_name, Variant &r_ret) const {
	if (JointData::_get(p_name, r_ret)) {
		return true;
	}

	if ("joint_constraints/bias" == p_name) {
		r_ret = bias;
	} else if ("joint_constraints/damping" == p_name) {
		r_ret = damping;
	} else if ("joint_constraints/impulse_clamp" == p_name) {
		r_ret = impulse_clamp;
	} else {
		return false;
	}

	return true;
}

void PhysicalBone::PinJointData::_get_property_list(List<PropertyInfo> *p_list) const {
	JointData::_get_property_list(p_list);

	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/bias", PROPERTY_HINT_RANGE, "0.01,0.99,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/damping", PROPERTY_HINT_RANGE, "0.01,8.0,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/impulse_clamp", PROPERTY_HINT_RANGE, "0.0,64.0,0.01"));
}

bool PhysicalBone::ConeJointData::_set(const StringName &p_name, const Variant &p_value, RID j) {
	if (JointData::_set(p_name, p_value, j)) {
		return true;
	}

	if ("joint_constraints/swing_span" == p_name) {
		swing_span = Math::deg2rad(real_t(p_value));
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->cone_twist_joint_set_param(j, PhysicsServer::CONE_TWIST_JOINT_SWING_SPAN, swing_span);
		}

	} else if ("joint_constraints/twist_span" == p_name) {
		twist_span = Math::deg2rad(real_t(p_value));
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->cone_twist_joint_set_param(j, PhysicsServer::CONE_TWIST_JOINT_TWIST_SPAN, twist_span);
		}

	} else if ("joint_constraints/bias" == p_name) {
		bias = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->cone_twist_joint_set_param(j, PhysicsServer::CONE_TWIST_JOINT_BIAS, bias);
		}

	} else if ("joint_constraints/softness" == p_name) {
		softness = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->cone_twist_joint_set_param(j, PhysicsServer::CONE_TWIST_JOINT_SOFTNESS, softness);
		}

	} else if ("joint_constraints/relaxation" == p_name) {
		relaxation = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->cone_twist_joint_set_param(j, PhysicsServer::CONE_TWIST_JOINT_RELAXATION, relaxation);
		}

	} else {
		return false;
	}

	return true;
}

bool PhysicalBone::ConeJointData::_get(const StringName &p_name, Variant &r_ret) const {
	if (JointData::_get(p_name, r_ret)) {
		return true;
	}

	if ("joint_constraints/swing_span" == p_name) {
		r_ret = Math::rad2deg(swing_span);
	} else if ("joint_constraints/twist_span" == p_name) {
		r_ret = Math::rad2deg(twist_span);
	} else if ("joint_constraints/bias" == p_name) {
		r_ret = bias;
	} else if ("joint_constraints/softness" == p_name) {
		r_ret = softness;
	} else if ("joint_constraints/relaxation" == p_name) {
		r_ret = relaxation;
	} else {
		return false;
	}

	return true;
}

void PhysicalBone::ConeJointData::_get_property_list(List<PropertyInfo> *p_list) const {
	JointData::_get_property_list(p_list);

	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/swing_span", PROPERTY_HINT_RANGE, "-180,180,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/twist_span", PROPERTY_HINT_RANGE, "-40000,40000,0.1,or_lesser,or_greater"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/bias", PROPERTY_HINT_RANGE, "0.01,16.0,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/softness", PROPERTY_HINT_RANGE, "0.01,16.0,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/relaxation", PROPERTY_HINT_RANGE, "0.01,16.0,0.01"));
}

bool PhysicalBone::HingeJointData::_set(const StringName &p_name, const Variant &p_value, RID j) {
	if (JointData::_set(p_name, p_value, j)) {
		return true;
	}

	if ("joint_constraints/angular_limit_enabled" == p_name) {
		angular_limit_enabled = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->hinge_joint_set_flag(j, PhysicsServer::HINGE_JOINT_FLAG_USE_LIMIT, angular_limit_enabled);
		}

	} else if ("joint_constraints/angular_limit_upper" == p_name) {
		angular_limit_upper = Math::deg2rad(real_t(p_value));
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->hinge_joint_set_param(j, PhysicsServer::HINGE_JOINT_LIMIT_UPPER, angular_limit_upper);
		}

	} else if ("joint_constraints/angular_limit_lower" == p_name) {
		angular_limit_lower = Math::deg2rad(real_t(p_value));
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->hinge_joint_set_param(j, PhysicsServer::HINGE_JOINT_LIMIT_LOWER, angular_limit_lower);
		}

	} else if ("joint_constraints/angular_limit_bias" == p_name) {
		angular_limit_bias = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->hinge_joint_set_param(j, PhysicsServer::HINGE_JOINT_LIMIT_BIAS, angular_limit_bias);
		}

	} else if ("joint_constraints/angular_limit_softness" == p_name) {
		angular_limit_softness = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->hinge_joint_set_param(j, PhysicsServer::HINGE_JOINT_LIMIT_SOFTNESS, angular_limit_softness);
		}

	} else if ("joint_constraints/angular_limit_relaxation" == p_name) {
		angular_limit_relaxation = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->hinge_joint_set_param(j, PhysicsServer::HINGE_JOINT_LIMIT_RELAXATION, angular_limit_relaxation);
		}

	} else {
		return false;
	}

	return true;
}

bool PhysicalBone::HingeJointData::_get(const StringName &p_name, Variant &r_ret) const {
	if (JointData::_get(p_name, r_ret)) {
		return true;
	}

	if ("joint_constraints/angular_limit_enabled" == p_name) {
		r_ret = angular_limit_enabled;
	} else if ("joint_constraints/angular_limit_upper" == p_name) {
		r_ret = Math::rad2deg(angular_limit_upper);
	} else if ("joint_constraints/angular_limit_lower" == p_name) {
		r_ret = Math::rad2deg(angular_limit_lower);
	} else if ("joint_constraints/angular_limit_bias" == p_name) {
		r_ret = angular_limit_bias;
	} else if ("joint_constraints/angular_limit_softness" == p_name) {
		r_ret = angular_limit_softness;
	} else if ("joint_constraints/angular_limit_relaxation" == p_name) {
		r_ret = angular_limit_relaxation;
	} else {
		return false;
	}

	return true;
}

void PhysicalBone::HingeJointData::_get_property_list(List<PropertyInfo> *p_list) const {
	JointData::_get_property_list(p_list);

	p_list->push_back(PropertyInfo(Variant::BOOL, "joint_constraints/angular_limit_enabled"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/angular_limit_upper", PROPERTY_HINT_RANGE, "-180,180,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/angular_limit_lower", PROPERTY_HINT_RANGE, "-180,180,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/angular_limit_bias", PROPERTY_HINT_RANGE, "0.01,0.99,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/angular_limit_softness", PROPERTY_HINT_RANGE, "0.01,16,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/angular_limit_relaxation", PROPERTY_HINT_RANGE, "0.01,16,0.01"));
}

bool PhysicalBone::SliderJointData::_set(const StringName &p_name, const Variant &p_value, RID j) {
	if (JointData::_set(p_name, p_value, j)) {
		return true;
	}

	if ("joint_constraints/linear_limit_upper" == p_name) {
		linear_limit_upper = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->slider_joint_set_param(j, PhysicsServer::SLIDER_JOINT_LINEAR_LIMIT_UPPER, linear_limit_upper);
		}

	} else if ("joint_constraints/linear_limit_lower" == p_name) {
		linear_limit_lower = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->slider_joint_set_param(j, PhysicsServer::SLIDER_JOINT_LINEAR_LIMIT_LOWER, linear_limit_lower);
		}

	} else if ("joint_constraints/linear_limit_softness" == p_name) {
		linear_limit_softness = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->slider_joint_set_param(j, PhysicsServer::SLIDER_JOINT_LINEAR_LIMIT_SOFTNESS, linear_limit_softness);
		}

	} else if ("joint_constraints/linear_limit_restitution" == p_name) {
		linear_limit_restitution = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->slider_joint_set_param(j, PhysicsServer::SLIDER_JOINT_LINEAR_LIMIT_RESTITUTION, linear_limit_restitution);
		}

	} else if ("joint_constraints/linear_limit_damping" == p_name) {
		linear_limit_damping = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->slider_joint_set_param(j, PhysicsServer::SLIDER_JOINT_LINEAR_LIMIT_DAMPING, linear_limit_restitution);
		}

	} else if ("joint_constraints/angular_limit_upper" == p_name) {
		angular_limit_upper = Math::deg2rad(real_t(p_value));
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->slider_joint_set_param(j, PhysicsServer::SLIDER_JOINT_ANGULAR_LIMIT_UPPER, angular_limit_upper);
		}

	} else if ("joint_constraints/angular_limit_lower" == p_name) {
		angular_limit_lower = Math::deg2rad(real_t(p_value));
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->slider_joint_set_param(j, PhysicsServer::SLIDER_JOINT_ANGULAR_LIMIT_LOWER, angular_limit_lower);
		}

	} else if ("joint_constraints/angular_limit_softness" == p_name) {
		angular_limit_softness = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->slider_joint_set_param(j, PhysicsServer::SLIDER_JOINT_ANGULAR_LIMIT_SOFTNESS, angular_limit_softness);
		}

	} else if ("joint_constraints/angular_limit_restitution" == p_name) {
		angular_limit_restitution = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->slider_joint_set_param(j, PhysicsServer::SLIDER_JOINT_ANGULAR_LIMIT_SOFTNESS, angular_limit_softness);
		}

	} else if ("joint_constraints/angular_limit_damping" == p_name) {
		angular_limit_damping = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->slider_joint_set_param(j, PhysicsServer::SLIDER_JOINT_ANGULAR_LIMIT_DAMPING, angular_limit_damping);
		}

	} else {
		return false;
	}

	return true;
}

bool PhysicalBone::SliderJointData::_get(const StringName &p_name, Variant &r_ret) const {
	if (JointData::_get(p_name, r_ret)) {
		return true;
	}

	if ("joint_constraints/linear_limit_upper" == p_name) {
		r_ret = linear_limit_upper;
	} else if ("joint_constraints/linear_limit_lower" == p_name) {
		r_ret = linear_limit_lower;
	} else if ("joint_constraints/linear_limit_softness" == p_name) {
		r_ret = linear_limit_softness;
	} else if ("joint_constraints/linear_limit_restitution" == p_name) {
		r_ret = linear_limit_restitution;
	} else if ("joint_constraints/linear_limit_damping" == p_name) {
		r_ret = linear_limit_damping;
	} else if ("joint_constraints/angular_limit_upper" == p_name) {
		r_ret = Math::rad2deg(angular_limit_upper);
	} else if ("joint_constraints/angular_limit_lower" == p_name) {
		r_ret = Math::rad2deg(angular_limit_lower);
	} else if ("joint_constraints/angular_limit_softness" == p_name) {
		r_ret = angular_limit_softness;
	} else if ("joint_constraints/angular_limit_restitution" == p_name) {
		r_ret = angular_limit_restitution;
	} else if ("joint_constraints/angular_limit_damping" == p_name) {
		r_ret = angular_limit_damping;
	} else {
		return false;
	}

	return true;
}

void PhysicalBone::SliderJointData::_get_property_list(List<PropertyInfo> *p_list) const {
	JointData::_get_property_list(p_list);

	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/linear_limit_upper"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/linear_limit_lower"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/linear_limit_softness", PROPERTY_HINT_RANGE, "0.01,16.0,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/linear_limit_restitution", PROPERTY_HINT_RANGE, "0.01,16.0,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/linear_limit_damping", PROPERTY_HINT_RANGE, "0,16.0,0.01"));

	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/angular_limit_upper", PROPERTY_HINT_RANGE, "-180,180,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/angular_limit_lower", PROPERTY_HINT_RANGE, "-180,180,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/angular_limit_softness", PROPERTY_HINT_RANGE, "0.01,16.0,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/angular_limit_restitution", PROPERTY_HINT_RANGE, "0.01,16.0,0.01"));
	p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/angular_limit_damping", PROPERTY_HINT_RANGE, "0,16.0,0.01"));
}

bool PhysicalBone::SixDOFJointData::_set(const StringName &p_name, const Variant &p_value, RID j) {
	if (JointData::_set(p_name, p_value, j)) {
		return true;
	}

	String path = p_name;

	if (!path.begins_with("joint_constraints/")) {
		return false;
	}

	Vector3::Axis axis;
	{
		const String axis_s = path.get_slicec('/', 1);
		if ("x" == axis_s) {
			axis = Vector3::AXIS_X;
		} else if ("y" == axis_s) {
			axis = Vector3::AXIS_Y;
		} else if ("z" == axis_s) {
			axis = Vector3::AXIS_Z;
		} else {
			return false;
		}
	}

	String var_name = path.get_slicec('/', 2);

	if ("linear_limit_enabled" == var_name) {
		axis_data[axis].linear_limit_enabled = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_flag(j, axis, PhysicsServer::G6DOF_JOINT_FLAG_ENABLE_LINEAR_LIMIT, axis_data[axis].linear_limit_enabled);
		}

	} else if ("linear_limit_upper" == var_name) {
		axis_data[axis].linear_limit_upper = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_LINEAR_UPPER_LIMIT, axis_data[axis].linear_limit_upper);
		}

	} else if ("linear_limit_lower" == var_name) {
		axis_data[axis].linear_limit_lower = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_LINEAR_LOWER_LIMIT, axis_data[axis].linear_limit_lower);
		}

	} else if ("linear_limit_softness" == var_name) {
		axis_data[axis].linear_limit_softness = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_LINEAR_LIMIT_SOFTNESS, axis_data[axis].linear_limit_softness);
		}

	} else if ("linear_spring_enabled" == var_name) {
		axis_data[axis].linear_spring_enabled = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_flag(j, axis, PhysicsServer::G6DOF_JOINT_FLAG_ENABLE_LINEAR_SPRING, axis_data[axis].linear_spring_enabled);
		}

	} else if ("linear_spring_stiffness" == var_name) {
		axis_data[axis].linear_spring_stiffness = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_LINEAR_SPRING_STIFFNESS, axis_data[axis].linear_spring_stiffness);
		}

	} else if ("linear_spring_damping" == var_name) {
		axis_data[axis].linear_spring_damping = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_LINEAR_SPRING_DAMPING, axis_data[axis].linear_spring_damping);
		}

	} else if ("linear_equilibrium_point" == var_name) {
		axis_data[axis].linear_equilibrium_point = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_LINEAR_SPRING_EQUILIBRIUM_POINT, axis_data[axis].linear_equilibrium_point);
		}

	} else if ("linear_restitution" == var_name) {
		axis_data[axis].linear_restitution = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_LINEAR_RESTITUTION, axis_data[axis].linear_restitution);
		}

	} else if ("linear_damping" == var_name) {
		axis_data[axis].linear_damping = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_LINEAR_DAMPING, axis_data[axis].linear_damping);
		}

	} else if ("angular_limit_enabled" == var_name) {
		axis_data[axis].angular_limit_enabled = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_flag(j, axis, PhysicsServer::G6DOF_JOINT_FLAG_ENABLE_ANGULAR_LIMIT, axis_data[axis].angular_limit_enabled);
		}

	} else if ("angular_limit_upper" == var_name) {
		axis_data[axis].angular_limit_upper = Math::deg2rad(real_t(p_value));
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_ANGULAR_UPPER_LIMIT, axis_data[axis].angular_limit_upper);
		}

	} else if ("angular_limit_lower" == var_name) {
		axis_data[axis].angular_limit_lower = Math::deg2rad(real_t(p_value));
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_ANGULAR_LOWER_LIMIT, axis_data[axis].angular_limit_lower);
		}

	} else if ("angular_limit_softness" == var_name) {
		axis_data[axis].angular_limit_softness = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_ANGULAR_LIMIT_SOFTNESS, axis_data[axis].angular_limit_softness);
		}

	} else if ("angular_restitution" == var_name) {
		axis_data[axis].angular_restitution = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_ANGULAR_RESTITUTION, axis_data[axis].angular_restitution);
		}

	} else if ("angular_damping" == var_name) {
		axis_data[axis].angular_damping = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_ANGULAR_DAMPING, axis_data[axis].angular_damping);
		}

	} else if ("erp" == var_name) {
		axis_data[axis].erp = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_ANGULAR_ERP, axis_data[axis].erp);
		}

	} else if ("angular_spring_enabled" == var_name) {
		axis_data[axis].angular_spring_enabled = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_flag(j, axis, PhysicsServer::G6DOF_JOINT_FLAG_ENABLE_ANGULAR_SPRING, axis_data[axis].angular_spring_enabled);
		}

	} else if ("angular_spring_stiffness" == var_name) {
		axis_data[axis].angular_spring_stiffness = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_ANGULAR_SPRING_STIFFNESS, axis_data[axis].angular_spring_stiffness);
		}

	} else if ("angular_spring_damping" == var_name) {
		axis_data[axis].angular_spring_damping = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_ANGULAR_SPRING_DAMPING, axis_data[axis].angular_spring_damping);
		}

	} else if ("angular_equilibrium_point" == var_name) {
		axis_data[axis].angular_equilibrium_point = p_value;
		if (j.is_valid()) {
			PhysicsServer::get_singleton()->generic_6dof_joint_set_param(j, axis, PhysicsServer::G6DOF_JOINT_ANGULAR_SPRING_EQUILIBRIUM_POINT, axis_data[axis].angular_equilibrium_point);
		}

	} else {
		return false;
	}

	return true;
}

bool PhysicalBone::SixDOFJointData::_get(const StringName &p_name, Variant &r_ret) const {
	if (JointData::_get(p_name, r_ret)) {
		return true;
	}

	String path = p_name;

	if (!path.begins_with("joint_constraints/")) {
		return false;
	}

	int axis;
	{
		const String axis_s = path.get_slicec('/', 1);
		if ("x" == axis_s) {
			axis = 0;
		} else if ("y" == axis_s) {
			axis = 1;
		} else if ("z" == axis_s) {
			axis = 2;
		} else {
			return false;
		}
	}

	String var_name = path.get_slicec('/', 2);

	if ("linear_limit_enabled" == var_name) {
		r_ret = axis_data[axis].linear_limit_enabled;
	} else if ("linear_limit_upper" == var_name) {
		r_ret = axis_data[axis].linear_limit_upper;
	} else if ("linear_limit_lower" == var_name) {
		r_ret = axis_data[axis].linear_limit_lower;
	} else if ("linear_limit_softness" == var_name) {
		r_ret = axis_data[axis].linear_limit_softness;
	} else if ("linear_spring_enabled" == var_name) {
		r_ret = axis_data[axis].linear_spring_enabled;
	} else if ("linear_spring_stiffness" == var_name) {
		r_ret = axis_data[axis].linear_spring_stiffness;
	} else if ("linear_spring_damping" == var_name) {
		r_ret = axis_data[axis].linear_spring_damping;
	} else if ("linear_equilibrium_point" == var_name) {
		r_ret = axis_data[axis].linear_equilibrium_point;
	} else if ("linear_restitution" == var_name) {
		r_ret = axis_data[axis].linear_restitution;
	} else if ("linear_damping" == var_name) {
		r_ret = axis_data[axis].linear_damping;
	} else if ("angular_limit_enabled" == var_name) {
		r_ret = axis_data[axis].angular_limit_enabled;
	} else if ("angular_limit_upper" == var_name) {
		r_ret = Math::rad2deg(axis_data[axis].angular_limit_upper);
	} else if ("angular_limit_lower" == var_name) {
		r_ret = Math::rad2deg(axis_data[axis].angular_limit_lower);
	} else if ("angular_limit_softness" == var_name) {
		r_ret = axis_data[axis].angular_limit_softness;
	} else if ("angular_restitution" == var_name) {
		r_ret = axis_data[axis].angular_restitution;
	} else if ("angular_damping" == var_name) {
		r_ret = axis_data[axis].angular_damping;
	} else if ("erp" == var_name) {
		r_ret = axis_data[axis].erp;
	} else if ("angular_spring_enabled" == var_name) {
		r_ret = axis_data[axis].angular_spring_enabled;
	} else if ("angular_spring_stiffness" == var_name) {
		r_ret = axis_data[axis].angular_spring_stiffness;
	} else if ("angular_spring_damping" == var_name) {
		r_ret = axis_data[axis].angular_spring_damping;
	} else if ("angular_equilibrium_point" == var_name) {
		r_ret = axis_data[axis].angular_equilibrium_point;
	} else {
		return false;
	}

	return true;
}

void PhysicalBone::SixDOFJointData::_get_property_list(List<PropertyInfo> *p_list) const {
	const StringName axis_names[] = { "x", "y", "z" };
	for (int i = 0; i < 3; ++i) {
		p_list->push_back(PropertyInfo(Variant::BOOL, "joint_constraints/" + axis_names[i] + "/linear_limit_enabled"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/linear_limit_upper"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/linear_limit_lower"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/linear_limit_softness", PROPERTY_HINT_RANGE, "0.01,16,0.01"));
		p_list->push_back(PropertyInfo(Variant::BOOL, "joint_constraints/" + axis_names[i] + "/linear_spring_enabled"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/linear_spring_stiffness"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/linear_spring_damping"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/linear_equilibrium_point"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/linear_restitution", PROPERTY_HINT_RANGE, "0.01,16,0.01"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/linear_damping", PROPERTY_HINT_RANGE, "0.01,16,0.01"));
		p_list->push_back(PropertyInfo(Variant::BOOL, "joint_constraints/" + axis_names[i] + "/angular_limit_enabled"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/angular_limit_upper", PROPERTY_HINT_RANGE, "-180,180,0.01"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/angular_limit_lower", PROPERTY_HINT_RANGE, "-180,180,0.01"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/angular_limit_softness", PROPERTY_HINT_RANGE, "0.01,16,0.01"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/angular_restitution", PROPERTY_HINT_RANGE, "0.01,16,0.01"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/angular_damping", PROPERTY_HINT_RANGE, "0.01,16,0.01"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/erp"));
		p_list->push_back(PropertyInfo(Variant::BOOL, "joint_constraints/" + axis_names[i] + "/angular_spring_enabled"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/angular_spring_stiffness"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/angular_spring_damping"));
		p_list->push_back(PropertyInfo(Variant::REAL, "joint_constraints/" + axis_names[i] + "/angular_equilibrium_point"));
	}
}

bool PhysicalBone::_set(const StringName &p_name, const Variant &p_value) {
	if (p_name == "bone_name") {
		set_bone_name(p_value);
		return true;
	}

	if (joint_data) {
		if (joint_data->_set(p_name, p_value, joint)) {
#ifdef TOOLS_ENABLED
			update_gizmos();
#endif
			return true;
		}
	}

	return false;
}

bool PhysicalBone::_get(const StringName &p_name, Variant &r_ret) const {
	if (p_name == "bone_name") {
		r_ret = get_bone_name();
		return true;
	}

	if (joint_data) {
		return joint_data->_get(p_name, r_ret);
	}

	return false;
}

void PhysicalBone::_get_property_list(List<PropertyInfo> *p_list) const {
	Skeleton *parent = find_skeleton_parent(get_parent());

	if (parent) {
		String names;
		for (int i = 0; i < parent->get_bone_count(); i++) {
			if (i > 0) {
				names += ",";
			}
			names += parent->get_bone_name(i);
		}

		p_list->push_back(PropertyInfo(Variant::STRING_NAME, "bone_name", PROPERTY_HINT_ENUM, names));
	} else {
		p_list->push_back(PropertyInfo(Variant::STRING_NAME, "bone_name"));
	}

	if (joint_data) {
		joint_data->_get_property_list(p_list);
	}
}

void PhysicalBone::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
			parent_skeleton = find_skeleton_parent(get_parent());
			update_bone_id();
			reset_to_rest_position();
			reset_physics_simulation_state();
			if (!joint.is_valid() && joint_data) {
				_reload_joint();
			}
			break;
		case NOTIFICATION_EXIT_TREE:
			if (parent_skeleton) {
				if (-1 != bone_id) {
					parent_skeleton->unbind_physical_bone_from_bone(bone_id);
					bone_id = -1;
				}
			}
			parent_skeleton = nullptr;
			if (joint.is_valid()) {
				PhysicsServer::get_singleton()->free(joint);
				joint = RID();
			}
			break;
		case NOTIFICATION_TRANSFORM_CHANGED:
			if (Engine::get_singleton()->is_editor_hint()) {
				update_offset();
			}
			break;
	}
}

void PhysicalBone::_direct_state_changed(Object *p_state) {
	if (!simulate_physics || !_internal_simulate_physics) {
		return;
	}

	/// Update bone transform

	PhysicsDirectBodyState *state = Object::cast_to<PhysicsDirectBodyState>(p_state);
	ERR_FAIL_COND_MSG(!state, "Method '_direct_state_changed' must receive a valid PhysicsDirectBodyState object as argument");

	Transform global_transform(state->get_transform());

	set_ignore_transform_notification(true);
	set_global_transform(global_transform);
	set_ignore_transform_notification(false);
	_on_transform_changed();

	// Update skeleton
	if (parent_skeleton) {
		if (-1 != bone_id) {
			parent_skeleton->set_bone_global_pose_override(bone_id, parent_skeleton->get_global_transform().affine_inverse() * (global_transform * body_offset_inverse), 1.0, true);
		}
	}
}

void PhysicalBone::_bind_methods() {
	ClassDB::bind_method(D_METHOD("apply_central_impulse", "impulse"), &PhysicalBone::apply_central_impulse);
	ClassDB::bind_method(D_METHOD("apply_impulse", "position", "impulse"), &PhysicalBone::apply_impulse);

	ClassDB::bind_method(D_METHOD("_direct_state_changed"), &PhysicalBone::_direct_state_changed);

	ClassDB::bind_method(D_METHOD("set_joint_type", "joint_type"), &PhysicalBone::set_joint_type);
	ClassDB::bind_method(D_METHOD("get_joint_type"), &PhysicalBone::get_joint_type);

	ClassDB::bind_method(D_METHOD("set_joint_offset", "offset"), &PhysicalBone::set_joint_offset);
	ClassDB::bind_method(D_METHOD("get_joint_offset"), &PhysicalBone::get_joint_offset);

	ClassDB::bind_method(D_METHOD("set_body_offset", "offset"), &PhysicalBone::set_body_offset);
	ClassDB::bind_method(D_METHOD("get_body_offset"), &PhysicalBone::get_body_offset);

	ClassDB::bind_method(D_METHOD("get_simulate_physics"), &PhysicalBone::get_simulate_physics);

	ClassDB::bind_method(D_METHOD("is_simulating_physics"), &PhysicalBone::is_simulating_physics);

	ClassDB::bind_method(D_METHOD("get_bone_id"), &PhysicalBone::get_bone_id);

	ClassDB::bind_method(D_METHOD("set_mass", "mass"), &PhysicalBone::set_mass);
	ClassDB::bind_method(D_METHOD("get_mass"), &PhysicalBone::get_mass);

	ClassDB::bind_method(D_METHOD("set_weight", "weight"), &PhysicalBone::set_weight);
	ClassDB::bind_method(D_METHOD("get_weight"), &PhysicalBone::get_weight);

	ClassDB::bind_method(D_METHOD("set_friction", "friction"), &PhysicalBone::set_friction);
	ClassDB::bind_method(D_METHOD("get_friction"), &PhysicalBone::get_friction);

	ClassDB::bind_method(D_METHOD("set_bounce", "bounce"), &PhysicalBone::set_bounce);
	ClassDB::bind_method(D_METHOD("get_bounce"), &PhysicalBone::get_bounce);

	ClassDB::bind_method(D_METHOD("set_gravity_scale", "gravity_scale"), &PhysicalBone::set_gravity_scale);
	ClassDB::bind_method(D_METHOD("get_gravity_scale"), &PhysicalBone::get_gravity_scale);

	ADD_GROUP("Joint", "joint_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "joint_type", PROPERTY_HINT_ENUM, "None,PinJoint,ConeJoint,HingeJoint,SliderJoint,6DOFJoint"), "set_joint_type", "get_joint_type");
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "joint_offset"), "set_joint_offset", "get_joint_offset");

	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "body_offset"), "set_body_offset", "get_body_offset");

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "mass", PROPERTY_HINT_EXP_RANGE, "0.01,65535,0.01"), "set_mass", "get_mass");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "weight", PROPERTY_HINT_EXP_RANGE, "0.01,65535,0.01"), "set_weight", "get_weight");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "friction", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_friction", "get_friction");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "bounce", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_bounce", "get_bounce");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "gravity_scale", PROPERTY_HINT_RANGE, "-10,10,0.01"), "set_gravity_scale", "get_gravity_scale");

	BIND_ENUM_CONSTANT(JOINT_TYPE_NONE);
	BIND_ENUM_CONSTANT(JOINT_TYPE_PIN);
	BIND_ENUM_CONSTANT(JOINT_TYPE_CONE);
	BIND_ENUM_CONSTANT(JOINT_TYPE_HINGE);
	BIND_ENUM_CONSTANT(JOINT_TYPE_SLIDER);
	BIND_ENUM_CONSTANT(JOINT_TYPE_6DOF);
}

Skeleton *PhysicalBone::find_skeleton_parent(Node *p_parent) {
	if (!p_parent) {
		return nullptr;
	}
	Skeleton *s = Object::cast_to<Skeleton>(p_parent);
	return s ? s : find_skeleton_parent(p_parent->get_parent());
}

void PhysicalBone::_fix_joint_offset() {
	// Clamp joint origin to bone origin
	if (parent_skeleton) {
		joint_offset.origin = body_offset.affine_inverse().origin;
	}
}

void PhysicalBone::_reload_joint() {
	if (joint.is_valid()) {
		PhysicsServer::get_singleton()->free(joint);
		joint = RID();
	}

	if (!parent_skeleton) {
		return;
	}

	PhysicalBone *body_a = parent_skeleton->get_physical_bone_parent(bone_id);
	if (!body_a) {
		return;
	}

	Transform joint_transf = get_global_transform() * joint_offset;
	Transform local_a = body_a->get_global_transform().affine_inverse() * joint_transf;
	local_a.orthonormalize();

	switch (get_joint_type()) {
		case JOINT_TYPE_PIN: {
			joint = RID_PRIME(PhysicsServer::get_singleton()->joint_create_pin(body_a->get_rid(), local_a.origin, get_rid(), joint_offset.origin));
			const PinJointData *pjd(static_cast<const PinJointData *>(joint_data));
			PhysicsServer::get_singleton()->pin_joint_set_param(joint, PhysicsServer::PIN_JOINT_BIAS, pjd->bias);
			PhysicsServer::get_singleton()->pin_joint_set_param(joint, PhysicsServer::PIN_JOINT_DAMPING, pjd->damping);
			PhysicsServer::get_singleton()->pin_joint_set_param(joint, PhysicsServer::PIN_JOINT_IMPULSE_CLAMP, pjd->impulse_clamp);

		} break;
		case JOINT_TYPE_CONE: {
			joint = RID_PRIME(PhysicsServer::get_singleton()->joint_create_cone_twist(body_a->get_rid(), local_a, get_rid(), joint_offset));
			const ConeJointData *cjd(static_cast<const ConeJointData *>(joint_data));
			PhysicsServer::get_singleton()->cone_twist_joint_set_param(joint, PhysicsServer::CONE_TWIST_JOINT_SWING_SPAN, cjd->swing_span);
			PhysicsServer::get_singleton()->cone_twist_joint_set_param(joint, PhysicsServer::CONE_TWIST_JOINT_TWIST_SPAN, cjd->twist_span);
			PhysicsServer::get_singleton()->cone_twist_joint_set_param(joint, PhysicsServer::CONE_TWIST_JOINT_BIAS, cjd->bias);
			PhysicsServer::get_singleton()->cone_twist_joint_set_param(joint, PhysicsServer::CONE_TWIST_JOINT_SOFTNESS, cjd->softness);
			PhysicsServer::get_singleton()->cone_twist_joint_set_param(joint, PhysicsServer::CONE_TWIST_JOINT_RELAXATION, cjd->relaxation);

		} break;
		case JOINT_TYPE_HINGE: {
			joint = RID_PRIME(PhysicsServer::get_singleton()->joint_create_hinge(body_a->get_rid(), local_a, get_rid(), joint_offset));
			const HingeJointData *hjd(static_cast<const HingeJointData *>(joint_data));
			PhysicsServer::get_singleton()->hinge_joint_set_flag(joint, PhysicsServer::HINGE_JOINT_FLAG_USE_LIMIT, hjd->angular_limit_enabled);
			PhysicsServer::get_singleton()->hinge_joint_set_param(joint, PhysicsServer::HINGE_JOINT_LIMIT_UPPER, hjd->angular_limit_upper);
			PhysicsServer::get_singleton()->hinge_joint_set_param(joint, PhysicsServer::HINGE_JOINT_LIMIT_LOWER, hjd->angular_limit_lower);
			PhysicsServer::get_singleton()->hinge_joint_set_param(joint, PhysicsServer::HINGE_JOINT_LIMIT_BIAS, hjd->angular_limit_bias);
			PhysicsServer::get_singleton()->hinge_joint_set_param(joint, PhysicsServer::HINGE_JOINT_LIMIT_SOFTNESS, hjd->angular_limit_softness);
			PhysicsServer::get_singleton()->hinge_joint_set_param(joint, PhysicsServer::HINGE_JOINT_LIMIT_RELAXATION, hjd->angular_limit_relaxation);

		} break;
		case JOINT_TYPE_SLIDER: {
			joint = RID_PRIME(PhysicsServer::get_singleton()->joint_create_slider(body_a->get_rid(), local_a, get_rid(), joint_offset));
			const SliderJointData *sjd(static_cast<const SliderJointData *>(joint_data));
			PhysicsServer::get_singleton()->slider_joint_set_param(joint, PhysicsServer::SLIDER_JOINT_LINEAR_LIMIT_UPPER, sjd->linear_limit_upper);
			PhysicsServer::get_singleton()->slider_joint_set_param(joint, PhysicsServer::SLIDER_JOINT_LINEAR_LIMIT_LOWER, sjd->linear_limit_lower);
			PhysicsServer::get_singleton()->slider_joint_set_param(joint, PhysicsServer::SLIDER_JOINT_LINEAR_LIMIT_SOFTNESS, sjd->linear_limit_softness);
			PhysicsServer::get_singleton()->slider_joint_set_param(joint, PhysicsServer::SLIDER_JOINT_LINEAR_LIMIT_RESTITUTION, sjd->linear_limit_restitution);
			PhysicsServer::get_singleton()->slider_joint_set_param(joint, PhysicsServer::SLIDER_JOINT_LINEAR_LIMIT_DAMPING, sjd->linear_limit_restitution);
			PhysicsServer::get_singleton()->slider_joint_set_param(joint, PhysicsServer::SLIDER_JOINT_ANGULAR_LIMIT_UPPER, sjd->angular_limit_upper);
			PhysicsServer::get_singleton()->slider_joint_set_param(joint, PhysicsServer::SLIDER_JOINT_ANGULAR_LIMIT_LOWER, sjd->angular_limit_lower);
			PhysicsServer::get_singleton()->slider_joint_set_param(joint, PhysicsServer::SLIDER_JOINT_ANGULAR_LIMIT_SOFTNESS, sjd->angular_limit_softness);
			PhysicsServer::get_singleton()->slider_joint_set_param(joint, PhysicsServer::SLIDER_JOINT_ANGULAR_LIMIT_SOFTNESS, sjd->angular_limit_softness);
			PhysicsServer::get_singleton()->slider_joint_set_param(joint, PhysicsServer::SLIDER_JOINT_ANGULAR_LIMIT_DAMPING, sjd->angular_limit_damping);

		} break;
		case JOINT_TYPE_6DOF: {
			joint = RID_PRIME(PhysicsServer::get_singleton()->joint_create_generic_6dof(body_a->get_rid(), local_a, get_rid(), joint_offset));
			const SixDOFJointData *g6dofjd(static_cast<const SixDOFJointData *>(joint_data));
			for (int axis = 0; axis < 3; ++axis) {
				PhysicsServer::get_singleton()->generic_6dof_joint_set_flag(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_FLAG_ENABLE_LINEAR_LIMIT, g6dofjd->axis_data[axis].linear_limit_enabled);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_LINEAR_UPPER_LIMIT, g6dofjd->axis_data[axis].linear_limit_upper);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_LINEAR_LOWER_LIMIT, g6dofjd->axis_data[axis].linear_limit_lower);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_LINEAR_LIMIT_SOFTNESS, g6dofjd->axis_data[axis].linear_limit_softness);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_flag(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_FLAG_ENABLE_LINEAR_SPRING, g6dofjd->axis_data[axis].linear_spring_enabled);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_LINEAR_SPRING_STIFFNESS, g6dofjd->axis_data[axis].linear_spring_stiffness);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_LINEAR_SPRING_DAMPING, g6dofjd->axis_data[axis].linear_spring_damping);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_LINEAR_SPRING_EQUILIBRIUM_POINT, g6dofjd->axis_data[axis].linear_equilibrium_point);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_LINEAR_RESTITUTION, g6dofjd->axis_data[axis].linear_restitution);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_LINEAR_DAMPING, g6dofjd->axis_data[axis].linear_damping);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_flag(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_FLAG_ENABLE_ANGULAR_LIMIT, g6dofjd->axis_data[axis].angular_limit_enabled);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_ANGULAR_UPPER_LIMIT, g6dofjd->axis_data[axis].angular_limit_upper);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_ANGULAR_LOWER_LIMIT, g6dofjd->axis_data[axis].angular_limit_lower);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_ANGULAR_LIMIT_SOFTNESS, g6dofjd->axis_data[axis].angular_limit_softness);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_ANGULAR_RESTITUTION, g6dofjd->axis_data[axis].angular_restitution);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_ANGULAR_DAMPING, g6dofjd->axis_data[axis].angular_damping);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_ANGULAR_ERP, g6dofjd->axis_data[axis].erp);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_flag(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_FLAG_ENABLE_ANGULAR_SPRING, g6dofjd->axis_data[axis].angular_spring_enabled);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_ANGULAR_SPRING_STIFFNESS, g6dofjd->axis_data[axis].angular_spring_stiffness);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_ANGULAR_SPRING_DAMPING, g6dofjd->axis_data[axis].angular_spring_damping);
				PhysicsServer::get_singleton()->generic_6dof_joint_set_param(joint, static_cast<Vector3::Axis>(axis), PhysicsServer::G6DOF_JOINT_ANGULAR_SPRING_EQUILIBRIUM_POINT, g6dofjd->axis_data[axis].angular_equilibrium_point);
			}

		} break;
		case JOINT_TYPE_NONE: {
		} break;
	}
}

void PhysicalBone::_on_bone_parent_changed() {
	_reload_joint();
}

void PhysicalBone::_set_gizmo_move_joint(bool p_move_joint) {
#ifdef TOOLS_ENABLED
	gizmo_move_joint = p_move_joint;
	SpatialEditor::get_singleton()->update_transform_gizmo();
#endif
}

#ifdef TOOLS_ENABLED
Transform PhysicalBone::get_global_gizmo_transform() const {
	return gizmo_move_joint ? get_global_transform() * joint_offset : get_global_transform();
}

Transform PhysicalBone::get_local_gizmo_transform() const {
	return gizmo_move_joint ? get_transform() * joint_offset : get_transform();
}
#endif

const PhysicalBone::JointData *PhysicalBone::get_joint_data() const {
	return joint_data;
}

Skeleton *PhysicalBone::find_skeleton_parent() {
	return find_skeleton_parent(this);
}

void PhysicalBone::set_joint_type(JointType p_joint_type) {
	if (p_joint_type == get_joint_type()) {
		return;
	}

	if (joint_data) {
		memdelete(joint_data);
	}
	joint_data = nullptr;
	switch (p_joint_type) {
		case JOINT_TYPE_PIN:
			joint_data = memnew(PinJointData);
			break;
		case JOINT_TYPE_CONE:
			joint_data = memnew(ConeJointData);
			break;
		case JOINT_TYPE_HINGE:
			joint_data = memnew(HingeJointData);
			break;
		case JOINT_TYPE_SLIDER:
			joint_data = memnew(SliderJointData);
			break;
		case JOINT_TYPE_6DOF:
			joint_data = memnew(SixDOFJointData);
			break;
		case JOINT_TYPE_NONE:
			break;
	}

	_reload_joint();

#ifdef TOOLS_ENABLED
	_change_notify();
	update_gizmos();
#endif
}

PhysicalBone::JointType PhysicalBone::get_joint_type() const {
	return joint_data ? joint_data->get_joint_type() : JOINT_TYPE_NONE;
}

void PhysicalBone::set_joint_offset(const Transform &p_offset) {
	joint_offset = p_offset;

	_fix_joint_offset();

	set_ignore_transform_notification(true);
	reset_to_rest_position();
	set_ignore_transform_notification(false);

#ifdef TOOLS_ENABLED
	update_gizmos();
#endif
}

const Transform &PhysicalBone::get_body_offset() const {
	return body_offset;
}

void PhysicalBone::set_body_offset(const Transform &p_offset) {
	body_offset = p_offset;
	body_offset_inverse = body_offset.affine_inverse();

	_fix_joint_offset();

	set_ignore_transform_notification(true);
	reset_to_rest_position();
	set_ignore_transform_notification(false);

#ifdef TOOLS_ENABLED
	update_gizmos();
#endif
}

const Transform &PhysicalBone::get_joint_offset() const {
	return joint_offset;
}

void PhysicalBone::set_simulate_physics(bool p_simulate) {
	if (simulate_physics == p_simulate) {
		return;
	}

	simulate_physics = p_simulate;
	reset_physics_simulation_state();
}

bool PhysicalBone::get_simulate_physics() {
	return simulate_physics;
}

bool PhysicalBone::is_simulating_physics() {
	return _internal_simulate_physics;
}

void PhysicalBone::set_bone_name(const String &p_name) {
	bone_name = p_name;
	bone_id = -1;

	update_bone_id();
	reset_to_rest_position();
}

const String &PhysicalBone::get_bone_name() const {
	return bone_name;
}

void PhysicalBone::set_mass(real_t p_mass) {
	ERR_FAIL_COND(p_mass <= 0);
	mass = p_mass;
	PhysicsServer::get_singleton()->body_set_param(get_rid(), PhysicsServer::BODY_PARAM_MASS, mass);
}

real_t PhysicalBone::get_mass() const {
	return mass;
}

void PhysicalBone::set_weight(real_t p_weight) {
	set_mass(p_weight / real_t(GLOBAL_DEF("physics/3d/default_gravity", 9.8)));
}

real_t PhysicalBone::get_weight() const {
	return mass * real_t(GLOBAL_DEF("physics/3d/default_gravity", 9.8));
}

void PhysicalBone::set_friction(real_t p_friction) {
	ERR_FAIL_COND(p_friction < 0 || p_friction > 1);

	friction = p_friction;
	PhysicsServer::get_singleton()->body_set_param(get_rid(), PhysicsServer::BODY_PARAM_FRICTION, friction);
}

real_t PhysicalBone::get_friction() const {
	return friction;
}

void PhysicalBone::set_bounce(real_t p_bounce) {
	ERR_FAIL_COND(p_bounce < 0 || p_bounce > 1);

	bounce = p_bounce;
	PhysicsServer::get_singleton()->body_set_param(get_rid(), PhysicsServer::BODY_PARAM_BOUNCE, bounce);
}

real_t PhysicalBone::get_bounce() const {
	return bounce;
}

void PhysicalBone::set_gravity_scale(real_t p_gravity_scale) {
	gravity_scale = p_gravity_scale;
	PhysicsServer::get_singleton()->body_set_param(get_rid(), PhysicsServer::BODY_PARAM_GRAVITY_SCALE, gravity_scale);
}

real_t PhysicalBone::get_gravity_scale() const {
	return gravity_scale;
}

PhysicalBone::PhysicalBone() :
		PhysicsBody(PhysicsServer::BODY_MODE_STATIC),
#ifdef TOOLS_ENABLED
		gizmo_move_joint(false),
#endif
		joint_data(nullptr),
		parent_skeleton(nullptr),
		simulate_physics(false),
		_internal_simulate_physics(false),
		bone_id(-1),
		bone_name(""),
		bounce(0),
		mass(1),
		friction(1),
		gravity_scale(1) {

	reset_physics_simulation_state();
}

PhysicalBone::~PhysicalBone() {
	if (joint_data) {
		memdelete(joint_data);
	}
}

void PhysicalBone::update_bone_id() {
	if (!parent_skeleton) {
		return;
	}

	const int new_bone_id = parent_skeleton->find_bone(bone_name);

	if (new_bone_id != bone_id) {
		if (-1 != bone_id) {
			// Assert the unbind from old node
			parent_skeleton->unbind_physical_bone_from_bone(bone_id);
		}

		bone_id = new_bone_id;

		parent_skeleton->bind_physical_bone_to_bone(bone_id, this);

		_fix_joint_offset();
		reset_physics_simulation_state();
	}
}

void PhysicalBone::update_offset() {
#ifdef TOOLS_ENABLED
	if (parent_skeleton) {
		Transform bone_transform(parent_skeleton->get_global_transform());
		if (-1 != bone_id) {
			bone_transform *= parent_skeleton->get_bone_global_pose(bone_id);
		}

		if (gizmo_move_joint) {
			bone_transform *= body_offset;
			set_joint_offset(bone_transform.affine_inverse() * get_global_transform());
		} else {
			set_body_offset(bone_transform.affine_inverse() * get_global_transform());
		}
	}
#endif
}

void PhysicalBone::_start_physics_simulation() {
	if (_internal_simulate_physics || !parent_skeleton) {
		return;
	}
	reset_to_rest_position();
	PhysicsServer::get_singleton()->body_set_mode(get_rid(), PhysicsServer::BODY_MODE_RIGID);
	PhysicsServer::get_singleton()->body_set_collision_layer(get_rid(), get_collision_layer());
	PhysicsServer::get_singleton()->body_set_collision_mask(get_rid(), get_collision_mask());
	PhysicsServer::get_singleton()->body_set_force_integration_callback(get_rid(), this, "_direct_state_changed");
	set_as_toplevel(true);
	_internal_simulate_physics = true;
}

void PhysicalBone::_stop_physics_simulation() {
	if (!parent_skeleton) {
		return;
	}

	if (parent_skeleton->get_animate_physical_bones()) {
		PhysicsServer::get_singleton()->body_set_mode(get_rid(), PhysicsServer::BODY_MODE_KINEMATIC);
		PhysicsServer::get_singleton()->body_set_collision_layer(get_rid(), get_collision_layer());
		PhysicsServer::get_singleton()->body_set_collision_mask(get_rid(), get_collision_mask());
	} else {
		PhysicsServer::get_singleton()->body_set_mode(get_rid(), PhysicsServer::BODY_MODE_STATIC);
		PhysicsServer::get_singleton()->body_set_collision_layer(get_rid(), 0);
		PhysicsServer::get_singleton()->body_set_collision_mask(get_rid(), 0);
	}
	if (_internal_simulate_physics) {
		PhysicsServer::get_singleton()->body_set_force_integration_callback(get_rid(), NULL, "");
		parent_skeleton->set_bone_global_pose_override(bone_id, Transform(), 0.0, false);
		set_as_toplevel(false);
		_internal_simulate_physics = false;
	}
}
