
#include "shape.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMShape::get_image() {
	return image;
}

void MMShape::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

int MMShape::get_shape_type() const {
	return shape_type;
}

void MMShape::set_shape_type(const int val) {
	shape_type = val;
	set_dirty(true);
}

int MMShape::get_sides() const {
	return sides;
}

void MMShape::set_sides(const int val) {
	sides = val;
	set_dirty(true);
}

Ref<MMNodeUniversalProperty> MMShape::get_radius() {
	return radius;
}

void MMShape::set_radius(const Ref<MMNodeUniversalProperty> &val) {
	radius = val;
}

Ref<MMNodeUniversalProperty> MMShape::get_edge() {
	return edge;
}

void MMShape::set_edge(const Ref<MMNodeUniversalProperty> &val) {
	edge = val;
}

void MMShape::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!radius.is_valid()) {
		radius.instance();
		radius->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		radius->set_default_value(0.34375);
	}

	radius->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	radius->set_slot_name("radius");
	radius->set_value_step(0.05);

	if (!edge.is_valid()) {
		edge.instance();
		edge->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		edge->set_default_value(0.2);
	}

	edge->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	edge->set_slot_name("edge");
	edge->set_value_step(0.05);

	register_input_property(radius);
	register_input_property(edge);
	register_output_property(image);
}

void MMShape::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("Circle");
	arr.push_back("Polygon");
	arr.push_back("Star");
	arr.push_back("Curved Star");
	arr.push_back("Rays");

	mm_graph_node->add_slot_enum("get_shape_type", "set_shape_type", "shape_type", arr);
	//, Vector2(1, 10));
	mm_graph_node->add_slot_int("get_sides", "set_sides", "sides");
	mm_graph_node->add_slot_float_universal(radius);
	mm_graph_node->add_slot_float_universal(edge);
}

void MMShape::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMShape::_get_value_for(const Vector2 &uv, const int pseed) {
	float c = 0;
	float rad = radius->get_value(uv);
	float edg = edge->get_value(uv);

	if (rad == 0) {
		rad = 0.0000001;
	}

	if (edg == 0) {
		edg = 0.0000001;
	}

	if (shape_type == SHAPE_TYPE_CIRCLE) {
		c = MMAlgos::shape_circle(uv, sides, rad, edg);
	}

	else if (shape_type == SHAPE_TYPE_POLYGON) {
		c = MMAlgos::shape_polygon(uv, sides, rad, edg);
	}

	else if (shape_type == SHAPE_TYPE_STAR) {
		c = MMAlgos::shape_star(uv, sides, rad, edg);
	}

	else if (shape_type == SHAPE_TYPE_CURVED_STAR) {
		c = MMAlgos::shape_curved_star(uv, sides, rad, edg);
	}

	else if (shape_type == SHAPE_TYPE_RAYS) {
		c = MMAlgos::shape_rays(uv, sides, rad, edg);
	}

	return Color(c, c, c, 1);
}

MMShape::MMShape() {
	shape_type = 0;
	sides = 6;
}

MMShape::~MMShape() {
}

void MMShape::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMShape::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMShape::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_shape_type"), &MMShape::get_shape_type);
	ClassDB::bind_method(D_METHOD("set_shape_type", "value"), &MMShape::set_shape_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shape_type"), "set_shape_type", "get_shape_type");

	ClassDB::bind_method(D_METHOD("get_sides"), &MMShape::get_sides);
	ClassDB::bind_method(D_METHOD("set_sides", "value"), &MMShape::set_sides);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sides"), "set_sides", "get_sides");

	ClassDB::bind_method(D_METHOD("get_radius"), &MMShape::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius", "value"), &MMShape::set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "radius", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_radius", "get_radius");

	ClassDB::bind_method(D_METHOD("get_edge"), &MMShape::get_edge);
	ClassDB::bind_method(D_METHOD("set_edge", "value"), &MMShape::set_edge);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "edge", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_edge", "get_edge");
}
