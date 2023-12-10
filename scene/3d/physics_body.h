#ifndef PHYSICS_BODY__H
#define PHYSICS_BODY__H
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

#include "core/containers/vset.h"
#include "core/object/reference.h"
#include "scene/3d/collision_object.h"
#include "core/servers/physics/physics_server.h"

class PhysicsMaterial;
class Skeleton;

class PhysicsBody : public CollisionObject {
	GDCLASS(PhysicsBody, CollisionObject);

	void _set_layers(uint32_t p_mask);
	uint32_t _get_layers() const;

protected:
	static void _bind_methods();
	void _notification(int p_what);
	PhysicsBody(PhysicsServer::BodyMode p_mode);
	~PhysicsBody();

public:
	virtual Vector3 get_linear_velocity() const;
	virtual Vector3 get_angular_velocity() const;
	virtual float get_inverse_mass() const;

	Array get_collision_exceptions();
	void add_collision_exception_with(Node *p_node); //must be physicsbody
	void remove_collision_exception_with(Node *p_node);
};

class StaticBody : public PhysicsBody {
	GDCLASS(StaticBody, PhysicsBody);

	Vector3 constant_linear_velocity;
	Vector3 constant_angular_velocity;

	Ref<PhysicsMaterial> physics_material_override;

protected:
	static void _bind_methods();

public:
#ifndef DISABLE_DEPRECATED
	void set_friction(real_t p_friction);
	real_t get_friction() const;

	void set_bounce(real_t p_bounce);
	real_t get_bounce() const;
#endif

	void set_physics_material_override(const Ref<PhysicsMaterial> &p_physics_material_override);
	Ref<PhysicsMaterial> get_physics_material_override() const;

	void set_constant_linear_velocity(const Vector3 &p_vel);
	void set_constant_angular_velocity(const Vector3 &p_vel);

	Vector3 get_constant_linear_velocity() const;
	Vector3 get_constant_angular_velocity() const;

	StaticBody();
	~StaticBody();

private:
	void _reload_physics_characteristics();
};

class RigidBody : public PhysicsBody {
	GDCLASS(RigidBody, PhysicsBody);

public:
	enum Mode {
		MODE_RIGID,
		MODE_STATIC,
		MODE_CHARACTER,
		MODE_KINEMATIC,
	};

protected:
	bool can_sleep;
	PhysicsDirectBodyState *state;
	Mode mode;

	real_t mass;
	Ref<PhysicsMaterial> physics_material_override;

	Vector3 linear_velocity;
	Vector3 angular_velocity;
	Basis inverse_inertia_tensor;
	real_t gravity_scale;
	real_t linear_damp;
	real_t angular_damp;

	bool sleeping;
	bool ccd;

	int max_contacts_reported;

	bool custom_integrator;

	struct ShapePair {
		int body_shape;
		int local_shape;
		bool tagged;
		bool operator<(const ShapePair &p_sp) const {
			if (body_shape == p_sp.body_shape) {
				return local_shape < p_sp.local_shape;
			} else {
				return body_shape < p_sp.body_shape;
			}
		}

		ShapePair() {}
		ShapePair(int p_bs, int p_ls) {
			body_shape = p_bs;
			local_shape = p_ls;
			tagged = false;
		}
	};
	struct RigidBody_RemoveAction {
		RID rid;
		ObjectID body_id;
		ShapePair pair;
	};
	struct BodyState {
		RID rid;
		bool in_tree;
		VSet<ShapePair> shapes;
	};

	struct ContactMonitor {
		bool locked;
		RBMap<ObjectID, BodyState> body_map;
	};

	ContactMonitor *contact_monitor;
	void _body_enter_tree(ObjectID p_id);
	void _body_exit_tree(ObjectID p_id);

	void _body_inout(int p_status, const RID &p_body, ObjectID p_instance, int p_body_shape, int p_local_shape);
	virtual void _direct_state_changed(Object *p_state);

	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_mode(Mode p_mode);
	Mode get_mode() const;

	void set_mass(real_t p_mass);
	real_t get_mass() const;

	virtual float get_inverse_mass() const { return 1.0 / mass; }

	void set_weight(real_t p_weight);
	real_t get_weight() const;

#ifndef DISABLE_DEPRECATED
	void set_friction(real_t p_friction);
	real_t get_friction() const;

	void set_bounce(real_t p_bounce);
	real_t get_bounce() const;
#endif

	void set_physics_material_override(const Ref<PhysicsMaterial> &p_physics_material_override);
	Ref<PhysicsMaterial> get_physics_material_override() const;

	void set_linear_velocity(const Vector3 &p_velocity);
	Vector3 get_linear_velocity() const;

	void set_axis_velocity(const Vector3 &p_axis);

	void set_angular_velocity(const Vector3 &p_velocity);
	Vector3 get_angular_velocity() const;

	Basis get_inverse_inertia_tensor();

	void set_gravity_scale(real_t p_gravity_scale);
	real_t get_gravity_scale() const;

	void set_linear_damp(real_t p_linear_damp);
	real_t get_linear_damp() const;

	void set_angular_damp(real_t p_angular_damp);
	real_t get_angular_damp() const;

	void set_use_custom_integrator(bool p_enable);
	bool is_using_custom_integrator();

	void set_sleeping(bool p_sleeping);
	bool is_sleeping() const;

	void set_can_sleep(bool p_active);
	bool is_able_to_sleep() const;

	void set_contact_monitor(bool p_enabled);
	bool is_contact_monitor_enabled() const;

	void set_max_contacts_reported(int p_amount);
	int get_max_contacts_reported() const;

	void set_use_continuous_collision_detection(bool p_enable);
	bool is_using_continuous_collision_detection() const;

	void set_axis_lock(PhysicsServer::BodyAxis p_axis, bool p_lock);
	bool get_axis_lock(PhysicsServer::BodyAxis p_axis) const;

	Array get_colliding_bodies() const;

	void add_central_force(const Vector3 &p_force);
	void add_force(const Vector3 &p_force, const Vector3 &p_pos);
	void add_torque(const Vector3 &p_torque);

	void apply_central_impulse(const Vector3 &p_impulse);
	void apply_impulse(const Vector3 &p_pos, const Vector3 &p_impulse);
	void apply_torque_impulse(const Vector3 &p_impulse);

	virtual String get_configuration_warning() const;

	RigidBody();
	~RigidBody();

private:
	void _reload_physics_characteristics();
};

VARIANT_ENUM_CAST(RigidBody::Mode);

class KinematicCollision;

class KinematicBody : public PhysicsBody {
	GDCLASS(KinematicBody, PhysicsBody);

public:
	enum MovingPlatformApplyVelocityOnLeave {
		PLATFORM_VEL_ON_LEAVE_ALWAYS,
		PLATFORM_VEL_ON_LEAVE_UPWARD_ONLY,
		PLATFORM_VEL_ON_LEAVE_NEVER,
	};
	struct Collision {
		Vector3 collision;
		Vector3 normal;
		Vector3 collider_vel;
		ObjectID collider;
		RID collider_rid;
		int collider_shape;
		Variant collider_metadata;
		Vector3 remainder;
		Vector3 travel;
		int local_shape;
		real_t collision_safe_fraction;

		real_t get_angle(const Vector3 &p_up_direction) const {
			return Math::acos(normal.dot(p_up_direction));
		}
	};

private:
	uint16_t locked_axis;

	float margin;

	Vector3 floor_normal;
	Vector3 floor_velocity;
	RID on_floor_body;
	bool on_floor;
	bool on_ceiling;
	bool on_wall;
	bool sync_to_physics = false;
	MovingPlatformApplyVelocityOnLeave moving_platform_apply_velocity_on_leave = PLATFORM_VEL_ON_LEAVE_ALWAYS;

	Vector<Collision> colliders;
	Vector<Ref<KinematicCollision>> slide_colliders;
	Ref<KinematicCollision> motion_cache;

	Ref<KinematicCollision> _move(const Vector3 &p_motion, bool p_infinite_inertia = true, bool p_exclude_raycast_shapes = true, bool p_test_only = false);
	Ref<KinematicCollision> _get_slide_collision(int p_bounce);
	Ref<KinematicCollision> _get_last_slide_collision();

	Transform last_valid_transform;
	void _direct_state_changed(Object *p_state);

	Vector3 _move_and_slide_internal(const Vector3 &p_linear_velocity, const Vector3 &p_snap, const Vector3 &p_up_direction = Vector3(0, 0, 0), bool p_stop_on_slope = false, int p_max_slides = 4, float p_floor_max_angle = Math::deg2rad((float)45), bool p_infinite_inertia = true);
	void _set_collision_direction(const Collision &p_collision, const Vector3 &p_up_direction, float p_floor_max_angle);
	void set_moving_platform_apply_velocity_on_leave(MovingPlatformApplyVelocityOnLeave p_on_leave_velocity);
	MovingPlatformApplyVelocityOnLeave get_moving_platform_apply_velocity_on_leave() const;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	bool move_and_collide(const Vector3 &p_motion, bool p_infinite_inertia, Collision &r_collision, bool p_exclude_raycast_shapes = true, bool p_test_only = false, bool p_cancel_sliding = true, const RBSet<RID> &p_exclude = RBSet<RID>());
	bool test_move(const Transform &p_from, const Vector3 &p_motion, bool p_infinite_inertia);

	bool separate_raycast_shapes(bool p_infinite_inertia, Collision &r_collision);

	void set_axis_lock(PhysicsServer::BodyAxis p_axis, bool p_lock);
	bool get_axis_lock(PhysicsServer::BodyAxis p_axis) const;

	void set_safe_margin(float p_margin);
	float get_safe_margin() const;

	Vector3 move_and_slide(const Vector3 &p_linear_velocity, const Vector3 &p_up_direction = Vector3(0, 0, 0), bool p_stop_on_slope = false, int p_max_slides = 4, float p_floor_max_angle = Math::deg2rad((float)45), bool p_infinite_inertia = true);
	Vector3 move_and_slide_with_snap(const Vector3 &p_linear_velocity, const Vector3 &p_snap, const Vector3 &p_up_direction = Vector3(0, 0, 0), bool p_stop_on_slope = false, int p_max_slides = 4, float p_floor_max_angle = Math::deg2rad((float)45), bool p_infinite_inertia = true);
	bool is_on_floor() const;
	bool is_on_wall() const;
	bool is_on_ceiling() const;
	Vector3 get_floor_normal() const;
	real_t get_floor_angle(const Vector3 &p_up_direction = Vector3(0.0, 1.0, 0.0)) const;
	Vector3 get_floor_velocity() const;

	int get_slide_count() const;
	Collision get_slide_collision(int p_bounce) const;

	void set_sync_to_physics(bool p_enable);
	bool is_sync_to_physics_enabled() const;

	KinematicBody();
	~KinematicBody();
};

VARIANT_ENUM_CAST(KinematicBody::MovingPlatformApplyVelocityOnLeave);

class KinematicCollision : public Reference {
	GDCLASS(KinematicCollision, Reference);

	KinematicBody *owner;
	friend class KinematicBody;
	KinematicBody::Collision collision;

protected:
	static void _bind_methods();

public:
	Vector3 get_position() const;
	Vector3 get_normal() const;
	Vector3 get_travel() const;
	Vector3 get_remainder() const;
	real_t get_angle(const Vector3 &p_up_direction = Vector3(0.0, 1.0, 0.0)) const;
	Object *get_local_shape() const;
	Object *get_collider() const;
	ObjectID get_collider_id() const;
	RID get_collider_rid() const;
	Object *get_collider_shape() const;
	int get_collider_shape_index() const;
	Vector3 get_collider_velocity() const;
	Variant get_collider_metadata() const;

	KinematicCollision();
	~KinematicCollision();
};

#endif // PHYSICS_BODY__H
