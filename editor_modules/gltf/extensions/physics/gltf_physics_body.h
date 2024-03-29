#ifndef GLTF_PHYSICS_BODY_H
#define GLTF_PHYSICS_BODY_H

/*************************************************************************/
/*  gltf_physics_body.h                                                  */
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

#include "scene/3d/physics_body.h"

// GLTFPhysicsBody is an intermediary between OMI_physics_body and Godot's physics body nodes.
// https://github.com/omigroup/gltf-extensions/tree/main/extensions/2.0/OMI_physics_body

class GLTFPhysicsBody : public Resource {
	GDCLASS(GLTFPhysicsBody, Resource)

protected:
	static void _bind_methods();

private:
	String body_type = "static";
	real_t mass = 1.0;
	Vector3 linear_velocity = Vector3();
	Vector3 angular_velocity = Vector3();

public:
	String get_body_type() const;
	void set_body_type(String p_body_type);

	real_t get_mass() const;
	void set_mass(real_t p_mass);

	Vector3 get_linear_velocity() const;
	void set_linear_velocity(Vector3 p_linear_velocity);

	Vector3 get_angular_velocity() const;
	void set_angular_velocity(Vector3 p_angular_velocity);

	static Ref<GLTFPhysicsBody> from_node(const CollisionObject *p_body_node);
	CollisionObject *to_node() const;

	static Ref<GLTFPhysicsBody> from_dictionary(const Dictionary p_dictionary);
	Dictionary to_dictionary() const;
};

#endif // GLTF_PHYSICS_BODY_H
