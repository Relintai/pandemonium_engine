
#include "gsai_rigid_body_3d_agent.h"

RigidBody GSAIRigidBody3DAgent::get_ *body() {
	return *body;
}

void GSAIRigidBody3DAgent::set_ *body(const RigidBody &val) {
	*body = val;
}

Vector3 GSAIRigidBody3DAgent::get__last_position() {
	return _last_position;
}

void GSAIRigidBody3DAgent::set__last_position(const Vector3 &val) {
	_last_position = val;
}

Ref<WeakRef> GSAIRigidBody3DAgent::get__body_ref() {
	return _body_ref;
}

void GSAIRigidBody3DAgent::set__body_ref(const Ref<WeakRef> &val) {
	_body_ref = val;
}

// A specialized steering agent that updates itself every frame so the user does;
// not have to using a RigidBody;
// @category - Specialized agents;
// The RigidBody to keep track of;
// setget _set_body;
RigidBody *body;
Vector3 _last_position = ;
Ref<WeakRef> _body_ref;

void GSAIRigidBody3DAgent::_body_ready() {
	// warning-ignore:return_value_discarded;
	body.get_tree().connect("physics_frame", self, "_on_SceneTree_frame");
}

// Moves the agent's `body` by target `acceleration`.;
// @tags - virtual;

void GSAIRigidBody3DAgent::_apply_steering(const GSAITargetAcceleration &acceleration, const float _delta) {
	RigidBody *_body = _body_ref.get_ref();

	if (!_body) {
		return;
	}

	applied_steering = true;
	_body.apply_central_impulse(acceleration.linear);
	_body.apply_torque_impulse(Vector3.UP * acceleration.angular);

	if (calculate_velocities) {
		linear_velocity = _body.linear_velocity;
		angular_velocity = _body.angular_velocity.y;
	}
}

void GSAIRigidBody3DAgent::_set_body(const RigidBody &value) {
	bool had_body = false;

	if (body) {
		had_body = true;
	}

	body = value;
	_body_ref = weakref(value);
	_last_position = value.transform.origin;
	last_orientation = value.rotation.y;
	position = _last_position;
	orientation = last_orientation;

	if (!had_body) {
		if (!body.is_inside_tree()) {
			body.connect("ready", self, "_body_ready");
		}

		else {
			_body_ready();
		}
	}
}

void GSAIRigidBody3DAgent::_on_SceneTree_frame() {
	RigidBody *_body = _body_ref.get_ref();

	if (not _body) {
		return;
	}

	if (not _body.is_inside_tree() || _body.get_tree().paused) {
		return;
	}

	Vector3 current_position = _body.transform.origin;
	float current_orientation = _body.rotation.y;
	position = current_position;
	orientation = current_orientation;

	if (calculate_velocities) {
		if (applied_steering) {
			applied_steering = false;
		}

		else {
			linear_velocity = _body.linear_velocity;
			angular_velocity = _body.angular_velocity.y;
		}
	}
}
}

GSAIRigidBody3DAgent::GSAIRigidBody3DAgent() {
	*body;
	_last_position = ;
	_body_ref;
}

GSAIRigidBody3DAgent::~GSAIRigidBody3DAgent() {
}

static void GSAIRigidBody3DAgent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_*body"), &GSAIRigidBody3DAgent::get_ * body);
	ClassDB::bind_method(D_METHOD("set_*body", "value"), &GSAIRigidBody3DAgent::set_ * body);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*body", PROPERTY_HINT_RESOURCE_TYPE, "RigidBody"), "set_*body", "get_*body");

	ClassDB::bind_method(D_METHOD("get__last_position"), &GSAIRigidBody3DAgent::get__last_position);
	ClassDB::bind_method(D_METHOD("set__last_position", "value"), &GSAIRigidBody3DAgent::set__last_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "_last_position"), "set__last_position", "get__last_position");

	ClassDB::bind_method(D_METHOD("get__body_ref"), &GSAIRigidBody3DAgent::get__body_ref);
	ClassDB::bind_method(D_METHOD("set__body_ref", "value"), &GSAIRigidBody3DAgent::set__body_ref);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "_body_ref", PROPERTY_HINT_RESOURCE_TYPE, "Ref<WeakRef>"), "set__body_ref", "get__body_ref");

	ClassDB::bind_method(D_METHOD("_body_ready"), &GSAIRigidBody3DAgent::_body_ready);
	ClassDB::bind_method(D_METHOD("_apply_steering", "acceleration", "_delta"), &GSAIRigidBody3DAgent::_apply_steering);
	ClassDB::bind_method(D_METHOD("_set_body", "value"), &GSAIRigidBody3DAgent::_set_body);
	ClassDB::bind_method(D_METHOD("_on_SceneTree_frame"), &GSAIRigidBody3DAgent::_on_SceneTree_frame);
}
