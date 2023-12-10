#ifndef PHYSICAL_BONE_H
#define PHYSICAL_BONE_H

/*************************************************************************/
/*  physics_body.h                                                       */
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
#include "core/containers/vset.h"
#include "scene/3d/collision_object.h"
#include "core/servers/physics/physics_server.h"
#include "scene/3d/physics_body.h"

class PhysicsMaterial;
class Skeleton;

class PhysicalBone : public PhysicsBody {
	GDCLASS(PhysicalBone, PhysicsBody);

public:
	enum JointType {
		JOINT_TYPE_NONE,
		JOINT_TYPE_PIN,
		JOINT_TYPE_CONE,
		JOINT_TYPE_HINGE,
		JOINT_TYPE_SLIDER,
		JOINT_TYPE_6DOF
	};

	struct JointData {
		virtual JointType get_joint_type() { return JOINT_TYPE_NONE; }

		/// "j" is used to set the parameter inside the PhysicsServer
		virtual bool _set(const StringName &p_name, const Variant &p_value, RID j);
		virtual bool _get(const StringName &p_name, Variant &r_ret) const;
		virtual void _get_property_list(List<PropertyInfo> *p_list) const;

		virtual ~JointData() {}
	};

	struct PinJointData : public JointData {
		virtual JointType get_joint_type() { return JOINT_TYPE_PIN; }

		virtual bool _set(const StringName &p_name, const Variant &p_value, RID j);
		virtual bool _get(const StringName &p_name, Variant &r_ret) const;
		virtual void _get_property_list(List<PropertyInfo> *p_list) const;

		real_t bias;
		real_t damping;
		real_t impulse_clamp;

		PinJointData() :
				bias(0.3),
				damping(1.),
				impulse_clamp(0) {}
	};

	struct ConeJointData : public JointData {
		virtual JointType get_joint_type() { return JOINT_TYPE_CONE; }

		virtual bool _set(const StringName &p_name, const Variant &p_value, RID j);
		virtual bool _get(const StringName &p_name, Variant &r_ret) const;
		virtual void _get_property_list(List<PropertyInfo> *p_list) const;

		real_t swing_span;
		real_t twist_span;
		real_t bias;
		real_t softness;
		real_t relaxation;

		ConeJointData() :
				swing_span(Math_PI * 0.25),
				twist_span(Math_PI),
				bias(0.3),
				softness(0.8),
				relaxation(1.) {}
	};

	struct HingeJointData : public JointData {
		virtual JointType get_joint_type() { return JOINT_TYPE_HINGE; }

		virtual bool _set(const StringName &p_name, const Variant &p_value, RID j);
		virtual bool _get(const StringName &p_name, Variant &r_ret) const;
		virtual void _get_property_list(List<PropertyInfo> *p_list) const;

		bool angular_limit_enabled;
		real_t angular_limit_upper;
		real_t angular_limit_lower;
		real_t angular_limit_bias;
		real_t angular_limit_softness;
		real_t angular_limit_relaxation;

		HingeJointData() :
				angular_limit_enabled(false),
				angular_limit_upper(Math_PI * 0.5),
				angular_limit_lower(-Math_PI * 0.5),
				angular_limit_bias(0.3),
				angular_limit_softness(0.9),
				angular_limit_relaxation(1.) {}
	};

	struct SliderJointData : public JointData {
		virtual JointType get_joint_type() { return JOINT_TYPE_SLIDER; }

		virtual bool _set(const StringName &p_name, const Variant &p_value, RID j);
		virtual bool _get(const StringName &p_name, Variant &r_ret) const;
		virtual void _get_property_list(List<PropertyInfo> *p_list) const;

		real_t linear_limit_upper;
		real_t linear_limit_lower;
		real_t linear_limit_softness;
		real_t linear_limit_restitution;
		real_t linear_limit_damping;
		real_t angular_limit_upper;
		real_t angular_limit_lower;
		real_t angular_limit_softness;
		real_t angular_limit_restitution;
		real_t angular_limit_damping;

		SliderJointData() :
				linear_limit_upper(1.),
				linear_limit_lower(-1.),
				linear_limit_softness(1.),
				linear_limit_restitution(0.7),
				linear_limit_damping(1.),
				angular_limit_upper(0),
				angular_limit_lower(0),
				angular_limit_softness(1.),
				angular_limit_restitution(0.7),
				angular_limit_damping(1.) {}
	};

	struct SixDOFJointData : public JointData {
		struct SixDOFAxisData {
			bool linear_limit_enabled;
			real_t linear_limit_upper;
			real_t linear_limit_lower;
			real_t linear_limit_softness;
			real_t linear_restitution;
			real_t linear_damping;
			bool linear_spring_enabled;
			real_t linear_spring_stiffness;
			real_t linear_spring_damping;
			real_t linear_equilibrium_point;
			bool angular_limit_enabled;
			real_t angular_limit_upper;
			real_t angular_limit_lower;
			real_t angular_limit_softness;
			real_t angular_restitution;
			real_t angular_damping;
			real_t erp;
			bool angular_spring_enabled;
			real_t angular_spring_stiffness;
			real_t angular_spring_damping;
			real_t angular_equilibrium_point;

			SixDOFAxisData() :
					linear_limit_enabled(true),
					linear_limit_upper(0),
					linear_limit_lower(0),
					linear_limit_softness(0.7),
					linear_restitution(0.5),
					linear_damping(1.),
					linear_spring_enabled(false),
					linear_spring_stiffness(0),
					linear_spring_damping(0),
					linear_equilibrium_point(0),
					angular_limit_enabled(true),
					angular_limit_upper(0),
					angular_limit_lower(0),
					angular_limit_softness(0.5),
					angular_restitution(0),
					angular_damping(1.),
					erp(0.5),
					angular_spring_enabled(false),
					angular_spring_stiffness(0),
					angular_spring_damping(0.),
					angular_equilibrium_point(0) {}
		};

		virtual JointType get_joint_type() { return JOINT_TYPE_6DOF; }

		virtual bool _set(const StringName &p_name, const Variant &p_value, RID j);
		virtual bool _get(const StringName &p_name, Variant &r_ret) const;
		virtual void _get_property_list(List<PropertyInfo> *p_list) const;

		SixDOFAxisData axis_data[3];

		SixDOFJointData() {}
	};

private:
#ifdef TOOLS_ENABLED
	// if false gizmo move body
	bool gizmo_move_joint;
#endif

	JointData *joint_data;
	Transform joint_offset;
	RID joint;

	Skeleton *parent_skeleton;
	Transform body_offset;
	Transform body_offset_inverse;
	bool simulate_physics;
	bool _internal_simulate_physics;
	int bone_id;

	String bone_name;
	real_t bounce;
	real_t mass;
	real_t friction;
	real_t gravity_scale;

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	void _notification(int p_what);
	void _direct_state_changed(Object *p_state);

	static void _bind_methods();

private:
	static Skeleton *find_skeleton_parent(Node *p_parent);

	void _fix_joint_offset();
	void _reload_joint();

public:
	void _on_bone_parent_changed();
	void _set_gizmo_move_joint(bool p_move_joint);

public:
#ifdef TOOLS_ENABLED
	virtual Transform get_global_gizmo_transform() const;
	virtual Transform get_local_gizmo_transform() const;
#endif

	const JointData *get_joint_data() const;
	Skeleton *find_skeleton_parent();

	int get_bone_id() const { return bone_id; }

	void set_joint_type(JointType p_joint_type);
	JointType get_joint_type() const;

	void set_joint_offset(const Transform &p_offset);
	const Transform &get_joint_offset() const;

	void set_body_offset(const Transform &p_offset);
	const Transform &get_body_offset() const;

	void set_simulate_physics(bool p_simulate);
	bool get_simulate_physics();
	bool is_simulating_physics();

	void set_bone_name(const String &p_name);
	const String &get_bone_name() const;

	void set_mass(real_t p_mass);
	real_t get_mass() const;

	void set_weight(real_t p_weight);
	real_t get_weight() const;

	void set_friction(real_t p_friction);
	real_t get_friction() const;

	void set_bounce(real_t p_bounce);
	real_t get_bounce() const;

	void set_gravity_scale(real_t p_gravity_scale);
	real_t get_gravity_scale() const;

	void apply_central_impulse(const Vector3 &p_impulse);
	void apply_impulse(const Vector3 &p_pos, const Vector3 &p_impulse);

	void reset_physics_simulation_state();
	void reset_to_rest_position();

	PhysicalBone();
	~PhysicalBone();

private:
	void update_bone_id();
	void update_offset();

	void _start_physics_simulation();
	void _stop_physics_simulation();
};

VARIANT_ENUM_CAST(PhysicalBone::JointType);

#endif // PHYSICS_BODY__H
