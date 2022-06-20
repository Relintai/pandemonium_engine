
#include "sdf3d_shape_capsule.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dShapeCapsule::get_output() {
	return output;
}

void MMSdf3dShapeCapsule::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

int MMSdf3dShapeCapsule::get_axis() const {
	return axis;
}

void MMSdf3dShapeCapsule::set_axis(const int val) {
	axis = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdf3dShapeCapsule::get_length() const {
	return length;
}

void MMSdf3dShapeCapsule::set_length(const float val) {
	length = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdf3dShapeCapsule::get_radius() const {
	return radius;
}

void MMSdf3dShapeCapsule::set_radius(const float val) {
	radius = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdf3dShapeCapsule::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_VECTOR2);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);

	register_output_property(output);
}

void MMSdf3dShapeCapsule::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);

	Array arr;
	arr.push_back("X");
	arr.push_back("Y");
	arr.push_back("Z");

	mm_graph_node->add_slot_enum("get_axis", "set_axis", "Axis", arr);
	mm_graph_node->add_slot_float("get_length", "set_length", "Length", 0.01);
	mm_graph_node->add_slot_float("get_radius", "set_radius", "Radius", 0.01);
	mm_graph_node->add_slot_curve();
}

Vector2 MMSdf3dShapeCapsule::_get_property_value_sdf3d(const Vector3 &uv3) {
	if (axis == 0) {
		return sdf3d_capsule_x(uv3, radius, length);
	} else if (axis == 1) {
		return sdf3d_capsule_y(uv3, radius, length);
	} else if (axis == 2) {
		return sdf3d_capsule_z(uv3, radius, length);
	}

	return Vector2();
}

//vec3 $(name_uv)_p = $uv;
//$(name_uv)_p.$axis -= clamp($(name_uv)_p.$axis, -$l, $l);
//return length($(name_uv)_p) - $r * $profile(clamp(0.5+0.5*($uv).$axis/$l, 0.0, 1.0));

Vector2 MMSdf3dShapeCapsule::sdf3d_capsule_y(const Vector3 &p, const float r, const float l) {
	Vector3 v = p;
	v.y -= CLAMP(v.y, -l, l);
	float cx = CLAMP(0.5 + 0.5 * p.y / l, 0.0, 1.0);
	float cp = MMAlgos::curve(cx, points);
	float f = v.length() - r * cp;
	return Vector2(f, 0.0);
}

Vector2 MMSdf3dShapeCapsule::sdf3d_capsule_x(const Vector3 &p, const float r, const float l) {
	Vector3 v = p;
	v.x -= CLAMP(v.x, -l, l);
	float cx = CLAMP(0.5 + 0.5 * p.x / l, 0.0, 1.0);
	float cp = MMAlgos::curve(cx, points);
	float f = v.length() - r * cp;
	return Vector2(f, 0.0);
}

Vector2 MMSdf3dShapeCapsule::sdf3d_capsule_z(const Vector3 &p, const float r, const float l) {
	Vector3 v = p;
	v.z -= CLAMP(v.z, -l, l);
	float cx = CLAMP(0.5 + 0.5 * p.z / l, 0.0, 1.0);
	float cp = MMAlgos::curve(cx, points);
	float f = v.length() - r * cp;
	return Vector2(f, 0.0);
}

void MMSdf3dShapeCapsule::_curve_changed() {
	emit_changed();
	output->do_emit_changed();
}

MMSdf3dShapeCapsule::MMSdf3dShapeCapsule() {
	axis = 1;
	length = 0.3;
	radius = 0.2;
}

MMSdf3dShapeCapsule::~MMSdf3dShapeCapsule() {
}

void MMSdf3dShapeCapsule::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		if (points.size() == 0) {
			init_points_11();
		}
	}
}

void MMSdf3dShapeCapsule::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdf3dShapeCapsule::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdf3dShapeCapsule::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_axis"), &MMSdf3dShapeCapsule::get_axis);
	ClassDB::bind_method(D_METHOD("set_axis", "value"), &MMSdf3dShapeCapsule::set_axis);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "axis"), "set_axis", "get_axis");

	ClassDB::bind_method(D_METHOD("get_length"), &MMSdf3dShapeCapsule::get_length);
	ClassDB::bind_method(D_METHOD("set_length", "value"), &MMSdf3dShapeCapsule::set_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "length"), "set_length", "get_length");

	ClassDB::bind_method(D_METHOD("get_radius"), &MMSdf3dShapeCapsule::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius", "value"), &MMSdf3dShapeCapsule::set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");

	ClassDB::bind_method(D_METHOD("sdf3d_capsule_y", "p", "r", "l"), &MMSdf3dShapeCapsule::sdf3d_capsule_y);
	ClassDB::bind_method(D_METHOD("sdf3d_capsule_x", "p", "r", "l"), &MMSdf3dShapeCapsule::sdf3d_capsule_x);
	ClassDB::bind_method(D_METHOD("sdf3d_capsule_z", "p", "r", "l"), &MMSdf3dShapeCapsule::sdf3d_capsule_z);
}
