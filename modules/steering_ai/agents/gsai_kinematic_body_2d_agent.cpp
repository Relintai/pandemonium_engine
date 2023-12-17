/*************************************************************************/
/*  gsai_kinematic_body_2d_agent.cpp                                     */
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

#include "gsai_kinematic_body_2d_agent.h"

#include "core/math/math_funcs.h"
#include "scene/2d/physics_body_2d.h"
#include "scene/main/scene_tree.h"

#include "../gsai_target_acceleration.h"
#include "../gsai_utils.h"

KinematicBody2D *GSAIKinematicBody2DAgent::get_body() {
	return Object::cast_to<KinematicBody2D>(ObjectDB::get_instance(_body_ref));
}

void GSAIKinematicBody2DAgent::set_body(KinematicBody2D *value) {
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

void GSAIKinematicBody2DAgent::set_body_bind(Node *p_body) {
	set_body(Object::cast_to<KinematicBody2D>(p_body));
}

int GSAIKinematicBody2DAgent::get_movement_type() const {
	return movement_type;
}

void GSAIKinematicBody2DAgent::set_movement_type(const int val) {
	movement_type = val;
}

void GSAIKinematicBody2DAgent::_apply_steering(Ref<GSAITargetAcceleration> acceleration, float delta) {
	applied_steering = true;

	KinematicBody2D *body = get_body();

	if (!body || !body->is_inside_tree()) {
		return;
	}

	SceneTree *st = SceneTree::get_singleton();

	if (st && st->is_paused()) {
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

void GSAIKinematicBody2DAgent::_apply_sliding_steering(KinematicBody2D *body, const Vector3 &accel, const float delta) {
	Vector2 velocity = GSAIUtils::to_vector2(linear_velocity + accel * delta).limit_length(linear_speed_max);

	if (apply_linear_drag) {
		velocity = velocity.linear_interpolate(Vector2(), linear_drag_percentage);
	}

	velocity = body->move_and_slide(velocity);

	if (calculate_velocities) {
		linear_velocity = GSAIUtils::to_vector3(velocity);
	}
}

void GSAIKinematicBody2DAgent::_apply_collide_steering(KinematicBody2D *body, const Vector3 &accel, const float delta) {
	Vector3 velocity = GSAIUtils::clampedv3(linear_velocity + accel * delta, linear_speed_max);

	if (apply_linear_drag) {
		velocity = velocity.linear_interpolate(Vector3(), linear_drag_percentage);
	}

	KinematicBody2D::Collision col;
	body->move_and_collide(GSAIUtils::to_vector2(velocity) * delta, true, col);

	if (calculate_velocities) {
		linear_velocity = velocity;
	}
}

void GSAIKinematicBody2DAgent::_apply_position_steering(KinematicBody2D *body, const Vector3 &accel, const float delta) {
	Vector3 velocity = GSAIUtils::clampedv3(linear_velocity + accel * delta, linear_speed_max);

	if (apply_linear_drag) {
		velocity = velocity.linear_interpolate(Vector3(), linear_drag_percentage);
	}

	Vector2 global_position = body->get_global_position();

	global_position += GSAIUtils::to_vector2(velocity) * delta;

	body->set_global_position(global_position);

	if (calculate_velocities) {
		linear_velocity = velocity;
	}
}

void GSAIKinematicBody2DAgent::_apply_orientation_steering(KinematicBody2D *body, const float angular_acceleration, const float delta) {
	float velocity = CLAMP(angular_velocity + angular_acceleration * delta, -angular_speed_max, angular_speed_max);

	if (apply_angular_drag) {
		velocity = Math::lerp(velocity, 0, angular_drag_percentage);
	}

	body->set_rotation(body->get_rotation() + velocity * delta);

	if (calculate_velocities) {
		angular_velocity = velocity;
	}
}

void GSAIKinematicBody2DAgent::_physics_process_connect() {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		if (!st->is_connected("physics_frame", this, "_on_SceneTree_physics_frame")) {
			st->connect("physics_frame", this, "_on_SceneTree_physics_frame");
		}
	}
}

void GSAIKinematicBody2DAgent::_physics_process_disconnect() {
	SceneTree *st = SceneTree::get_singleton();

	if (st) {
		if (st->is_connected("physics_frame", this, "_on_SceneTree_physics_frame")) {
			st->disconnect("physics_frame", this, "_on_SceneTree_physics_frame");
		}
	}
}

void GSAIKinematicBody2DAgent::_on_SceneTree_physics_frame() {
	KinematicBody2D *body = get_body();

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
		}

		else {
			linear_velocity = GSAIUtils::clampedv3(GSAIUtils::to_vector3(current_position - _last_position), linear_speed_max);

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

GSAIKinematicBody2DAgent::GSAIKinematicBody2DAgent() {
	movement_type = SLIDE;
	_body_ref = 0;
}

GSAIKinematicBody2DAgent::~GSAIKinematicBody2DAgent() {
}

void GSAIKinematicBody2DAgent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_body"), &GSAIKinematicBody2DAgent::get_body);
	ClassDB::bind_method(D_METHOD("set_body", "value"), &GSAIKinematicBody2DAgent::set_body_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "body", PROPERTY_HINT_RESOURCE_TYPE, "KinematicBody2D"), "set_body", "get_body");

	ClassDB::bind_method(D_METHOD("get_movement_type"), &GSAIKinematicBody2DAgent::get_movement_type);
	ClassDB::bind_method(D_METHOD("set_movement_type", "value"), &GSAIKinematicBody2DAgent::set_movement_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "movement_type"), "set_movement_type", "get_movement_type");

	ClassDB::bind_method(D_METHOD("_physics_process_connect"), &GSAIKinematicBody2DAgent::_physics_process_connect);
	ClassDB::bind_method(D_METHOD("_physics_process_disconnect"), &GSAIKinematicBody2DAgent::_physics_process_disconnect);

	ClassDB::bind_method(D_METHOD("_on_SceneTree_physics_frame"), &GSAIKinematicBody2DAgent::_on_SceneTree_physics_frame);

	BIND_ENUM_CONSTANT(SLIDE);
	BIND_ENUM_CONSTANT(COLLIDE);
	BIND_ENUM_CONSTANT(POSITION);
}
