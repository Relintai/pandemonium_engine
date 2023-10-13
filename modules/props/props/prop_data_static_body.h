#ifndef PROP_DATA_STATIC_BODY_H
#define PROP_DATA_STATIC_BODY_H

/*
Copyright (c) 2023-present Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "prop_data_collision_object.h"

class PhysicsMaterial;

class PropDataStaticBody : public PropDataCollisionObject {
	GDCLASS(PropDataStaticBody, PropDataCollisionObject);

public:
	Ref<PhysicsMaterial> get_physics_material_override() const;
	void set_physics_material_override(const Ref<PhysicsMaterial> &p_material);

	Vector3 get_constant_linear_velocity() const;
	void set_constant_linear_velocity(const Vector3 &p_value);

	Vector3 get_constant_angular_velocity() const;
	void set_constant_angular_velocity(const Vector3 &p_value);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	Node *_processor_get_node_for(const Transform &transform);

	PropDataStaticBody();
	~PropDataStaticBody();

protected:
	static void _bind_methods();

	Ref<PhysicsMaterial> _physics_material_override;
	Vector3 _constant_linear_velocity;
	Vector3 _constant_angular_velocity;
};

#endif
