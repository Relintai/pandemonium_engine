
#include "gsai_rigid_body_2d_agent.h"

#include "scene/2d/physics_body_2d.h"

#include "../gsai_target_acceleration.h"
#include "../gsai_utils.h"

RigidBody2D *GSAIRigidBody2DAgent::get_body() {
	return Object::cast_to<RigidBody2D>(ObjectDB::get_instance(_body_ref));
}

void GSAIRigidBody2DAgent::set_body(RigidBody2D *value) {
	if (!value) {
		_body_ref = 0;
		_physics_process_disconnect();
		return;
	}

	_body_ref = value->get_instance_id();
	_last_position = value->get_global_position();
	last_orientation = value->get_rotation();
	position = GSAIUtils::to_vector3(_last_position);
	orientation = last_orientation;

	if (!value->is_inside_tree()) {
		value->connect("ready", this, "_physics_process_connect", varray(), CONNECT_ONESHOT);
	} else {
		_physics_process_connect();
	}
}

void GSAIRigidBody2DAgent::set_body_bind(Node *p_body) {
	set_body(Object::cast_to<RigidBody2D>(p_body));
}

void GSAIRigidBody2DAgent::_physics_process_connect() {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		if (!st->is_connected("physics_frame", this, "_on_SceneTree_physics_frame")) {
			st->connect("physics_frame", this, "_on_SceneTree_physics_frame");
		}
	}
}

void GSAIRigidBody2DAgent::_physics_process_disconnect() {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		if (st->is_connected("physics_frame", this, "_on_SceneTree_physics_frame")) {
			st->disconnect("physics_frame", this, "_on_SceneTree_physics_frame");
		}
	}
}

void GSAIRigidBody2DAgent::_apply_steering(Ref<GSAITargetAcceleration> acceleration, float delta) {
	RigidBody2D *body = get_body();

	if (!body) {
		return;
	}

	applied_steering = true;
	body->apply_central_impulse(GSAIUtils::to_vector2(acceleration->get_linear()));
	body->apply_torque_impulse(acceleration->get_angular());

	if (calculate_velocities) {
		linear_velocity = GSAIUtils::to_vector3(body->get_linear_velocity());
		angular_velocity = body->get_angular_velocity();
	}
}

void GSAIRigidBody2DAgent::_on_SceneTree_frame() {
	RigidBody2D *body = get_body();

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

	Vector2 current_position = body->get_global_position();
	float current_orientation = body->get_rotation();
	position = GSAIUtils::to_vector3(current_position);
	orientation = current_orientation;

	if (calculate_velocities) {
		if (applied_steering) {
			applied_steering = false;
		} else {
			linear_velocity = GSAIUtils::to_vector3(body->get_linear_velocity());
			angular_velocity = body->get_angular_velocity();
		}
	}
}

GSAIRigidBody2DAgent::GSAIRigidBody2DAgent() {
	_body_ref = 0;
}

GSAIRigidBody2DAgent::~GSAIRigidBody2DAgent() {
}

void GSAIRigidBody2DAgent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_body"), &GSAIRigidBody2DAgent::get_body);
	ClassDB::bind_method(D_METHOD("set_body", "value"), &GSAIRigidBody2DAgent::set_body_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "body", PROPERTY_HINT_RESOURCE_TYPE, "RigidBody2D"), "set_body", "get_body");

	ClassDB::bind_method(D_METHOD("_physics_process_connect"), &GSAIRigidBody2DAgent::_physics_process_connect);
	ClassDB::bind_method(D_METHOD("_physics_process_disconnect"), &GSAIRigidBody2DAgent::_physics_process_disconnect);

	ClassDB::bind_method(D_METHOD("_on_SceneTree_frame"), &GSAIRigidBody2DAgent::_on_SceneTree_frame);
}
