
#include "gsai_rigid_body_3d_agent.h"

#include "scene/3d/physics_body.h"

#include "../gsai_target_acceleration.h"

RigidBody *GSAIRigidBody3DAgent::get_body() {
	return Object::cast_to<RigidBody>(ObjectDB::get_instance(_body_ref));
}

void GSAIRigidBody3DAgent::set_body(RigidBody *value) {
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

void GSAIRigidBody3DAgent::set_body_bind(Node *p_body) {
	set_body(Object::cast_to<RigidBody>(p_body));
}

void GSAIRigidBody3DAgent::_physics_process_connect() {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		if (!st->is_connected("physics_frame", this, "_on_SceneTree_physics_frame")) {
			st->connect("physics_frame", this, "_on_SceneTree_physics_frame");
		}
	}
}

void GSAIRigidBody3DAgent::_physics_process_disconnect() {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		if (st->is_connected("physics_frame", this, "_on_SceneTree_physics_frame")) {
			st->disconnect("physics_frame", this, "_on_SceneTree_physics_frame");
		}
	}
}

void GSAIRigidBody3DAgent::_apply_steering(Ref<GSAITargetAcceleration> acceleration, float delta) {
	RigidBody *body = get_body();

	if (!body || !body->is_inside_tree()) {
		return;
	}

	applied_steering = true;
	body->apply_central_impulse(acceleration->get_linear());
	body->apply_torque_impulse(Vector3(0, 1, 0) * acceleration->get_angular());

	if (calculate_velocities) {
		linear_velocity = body->get_linear_velocity();
		angular_velocity = body->get_angular_velocity().y;
	}
}

void GSAIRigidBody3DAgent::_on_SceneTree_frame() {
	RigidBody *body = get_body();

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
			linear_velocity = body->get_linear_velocity();
			angular_velocity = body->get_angular_velocity().y;
		}
	}
}

GSAIRigidBody3DAgent::GSAIRigidBody3DAgent() {
	_body_ref = 0;
}

GSAIRigidBody3DAgent::~GSAIRigidBody3DAgent() {
}

void GSAIRigidBody3DAgent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_body"), &GSAIRigidBody3DAgent::get_body);
	ClassDB::bind_method(D_METHOD("set_body", "value"), &GSAIRigidBody3DAgent::set_body_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "body", PROPERTY_HINT_RESOURCE_TYPE, "RigidBody"), "set_body", "get_body");

	ClassDB::bind_method(D_METHOD("_physics_process_connect"), &GSAIRigidBody3DAgent::_physics_process_connect);
	ClassDB::bind_method(D_METHOD("_physics_process_disconnect"), &GSAIRigidBody3DAgent::_physics_process_disconnect);

	ClassDB::bind_method(D_METHOD("_on_SceneTree_frame"), &GSAIRigidBody3DAgent::_on_SceneTree_frame);
}
