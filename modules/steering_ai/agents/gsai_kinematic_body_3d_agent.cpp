
#include "gsai_kinematic_body_3d_agent.h"

#include "scene/3d/physics_body.h"

#include "../gsai_target_acceleration.h"
#include "../gsai_utils.h"

KinematicBody *GSAIKinematicBody3DAgent::get_body() {
	return Object::cast_to<KinematicBody>(ObjectDB::get_instance(_body_ref));
}

void GSAIKinematicBody3DAgent::set_body(KinematicBody *value) {
	if (!value) {
		_body_ref = 0;
		_physics_process_disconnect();
		return;
	}

	_body_ref = value->get_instance_id();
	_last_position = value->get_transform().origin;
	last_orientation = value->get_rotation().y;
	position = _last_position;
	orientation = last_orientation;

	if (!value->is_inside_tree()) {
		value->connect("ready", this, "_physics_process_connect", varray(), CONNECT_ONESHOT);
	} else {
		_physics_process_connect();
	}
}

void GSAIKinematicBody3DAgent::set_body_bind(Node *p_body) {
	set_body(Object::cast_to<KinematicBody>(p_body));
}

int GSAIKinematicBody3DAgent::get_movement_type() const {
	return movement_type;
}

void GSAIKinematicBody3DAgent::set_movement_type(const int val) {
	movement_type = val;
}

void GSAIKinematicBody3DAgent::_physics_process_connect() {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		if (!st->is_connected("physics_frame", this, "_on_SceneTree_physics_frame")) {
			st->connect("physics_frame", this, "_on_SceneTree_physics_frame");
		}
	}
}

void GSAIKinematicBody3DAgent::_physics_process_disconnect() {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		if (st->is_connected("physics_frame", this, "_on_SceneTree_physics_frame")) {
			st->disconnect("physics_frame", this, "_on_SceneTree_physics_frame");
		}
	}
}

void GSAIKinematicBody3DAgent::_apply_steering(Ref<GSAITargetAcceleration> acceleration, float delta) {
	applied_steering = true;

	KinematicBody *body = get_body();

	if (!body) {
		return;
	}

	if (movement_type == COLLIDE) {
		_apply_collide_steering(body, acceleration->get_linear(), delta);
	} else if (movement_type == SLIDE) {
		_apply_sliding_steering(body, acceleration->get_linear(), delta);
	} else {
		_apply_position_steering(body, acceleration->get_linear(), delta);
	}

	_apply_orientation_steering(body, acceleration->get_angular(), delta);
}

void GSAIKinematicBody3DAgent::_apply_sliding_steering(KinematicBody *body, const Vector3 &accel, const float delta) {
	Vector3 velocity = GSAIUtils::clampedv3(linear_velocity + accel * delta, linear_speed_max);

	if (apply_linear_drag) {
		velocity = velocity.linear_interpolate(Vector3(), linear_drag_percentage);
	}

	velocity = body->move_and_slide(velocity);

	if (calculate_velocities) {
		linear_velocity = velocity;
	}
}

void GSAIKinematicBody3DAgent::_apply_collide_steering(KinematicBody *body, const Vector3 &accel, const float delta) {
	Vector3 velocity = GSAIUtils::clampedv3(linear_velocity + accel * delta, linear_speed_max);

	if (apply_linear_drag) {
		velocity = velocity.linear_interpolate(Vector3(), linear_drag_percentage);
	}

	KinematicBody::Collision coll;
	body->move_and_collide(velocity * delta, true, coll);

	if (calculate_velocities) {
		linear_velocity = velocity;
	}
}

void GSAIKinematicBody3DAgent::_apply_position_steering(KinematicBody *body, const Vector3 &accel, const float delta) {
	Vector3 velocity = GSAIUtils::clampedv3(linear_velocity + accel * delta, linear_speed_max);

	if (apply_linear_drag) {
		velocity = velocity.linear_interpolate(Vector3(), linear_drag_percentage);
	}

	body->get_global_transform().origin += velocity * delta;

	if (calculate_velocities) {
		linear_velocity = velocity;
	}
}

void GSAIKinematicBody3DAgent::_apply_orientation_steering(KinematicBody *body, const float angular_acceleration, const float delta) {
	float velocity = CLAMP(angular_velocity + angular_acceleration * delta, -angular_speed_max, angular_speed_max);

	if (apply_angular_drag) {
		velocity = Math::lerp(velocity, 0, angular_drag_percentage);
	}

	Vector3 rotation = body->get_rotation();

	rotation.y += velocity * delta;

	body->set_rotation(rotation);

	if (calculate_velocities) {
		angular_velocity = velocity;
	}
}

void GSAIKinematicBody3DAgent::_on_SceneTree_physics_frame() {
	KinematicBody *body = get_body();

	if (!body) {
		call_deferred("_physics_process_disconnect");
		return;
	}

	if (!body->is_inside_tree()) {
		return;
	}

	SceneTree *st = SceneTree::get_singleton();

	if (st && st->is_paused()) {
		return;
	}

	Vector3 current_position = body->get_transform().origin;
	float current_orientation = body->get_rotation().y;
	position = current_position;
	orientation = current_orientation;

	if (calculate_velocities) {
		if (applied_steering) {
			applied_steering = false;
		} else {
			linear_velocity = GSAIUtils::clampedv3(current_position - _last_position, linear_speed_max);

			if (apply_linear_drag) {
				linear_velocity = linear_velocity.linear_interpolate(Vector3(), linear_drag_percentage);
			}

			angular_velocity = CLAMP(last_orientation - current_orientation, -angular_speed_max, angular_speed_max);

			if (apply_angular_drag) {
				angular_velocity = Math::lerp(angular_velocity, 0, angular_drag_percentage);
			}

			_last_position = current_position;
			last_orientation = current_orientation;
		}
	}
}

GSAIKinematicBody3DAgent::GSAIKinematicBody3DAgent() {
	movement_type = 0;
	_body_ref = 0;
}

GSAIKinematicBody3DAgent::~GSAIKinematicBody3DAgent() {
}

void GSAIKinematicBody3DAgent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_body"), &GSAIKinematicBody3DAgent::get_body);
	ClassDB::bind_method(D_METHOD("set_body", "value"), &GSAIKinematicBody3DAgent::set_body_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "body", PROPERTY_HINT_RESOURCE_TYPE, "KinematicBody"), "set_body", "get_body");

	ClassDB::bind_method(D_METHOD("get_movement_type"), &GSAIKinematicBody3DAgent::get_movement_type);
	ClassDB::bind_method(D_METHOD("set_movement_type", "value"), &GSAIKinematicBody3DAgent::set_movement_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "movement_type"), "set_movement_type", "get_movement_type");

	ClassDB::bind_method(D_METHOD("_physics_process_connect"), &GSAIKinematicBody3DAgent::_physics_process_connect);
	ClassDB::bind_method(D_METHOD("_physics_process_disconnect"), &GSAIKinematicBody3DAgent::_physics_process_disconnect);

	ClassDB::bind_method(D_METHOD("_on_SceneTree_physics_frame"), &GSAIKinematicBody3DAgent::_on_SceneTree_physics_frame);
}
