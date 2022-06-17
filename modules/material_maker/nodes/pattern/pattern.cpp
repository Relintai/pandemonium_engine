
#include "pattern.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMPattern::get_image() {
	return image;
}

void MMPattern::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

int MMPattern::get_combiner_type() const {
	return combiner_type;
}

void MMPattern::set_combiner_type(const int val) {
	combiner_type = val;
	set_dirty(true);
}

int MMPattern::get_combiner_axis_type_x() const {
	return combiner_axis_type_x;
}

void MMPattern::set_combiner_axis_type_x(const int val) {
	combiner_axis_type_x = val;
	set_dirty(true);
}

int MMPattern::get_combiner_axis_type_y() const {
	return combiner_axis_type_y;
}

void MMPattern::set_combiner_axis_type_y(const int val) {
	combiner_axis_type_y = val;
	set_dirty(true);
}

Vector2 MMPattern::get_repeat() {
	return repeat;
}

void MMPattern::set_repeat(const Vector2 &val) {
	repeat = val;
	set_dirty(true);
}

void MMPattern::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	register_output_property(image);
}

void MMPattern::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);

	Array arr1;
	arr1.push_back("Multiply");
	arr1.push_back("Add");
	arr1.push_back("Max");
	arr1.push_back("Min");
	arr1.push_back("Xor");
	arr1.push_back("Pow");

	mm_graph_node->add_slot_enum("get_combiner_type", "set_combiner_type", "Combiner Type", arr1);

	Array arr2;
	arr2.push_back("Sine");
	arr2.push_back("Triangle");
	arr2.push_back("Square");
	arr2.push_back("Sawtooth");
	arr2.push_back("Constant");
	arr2.push_back("Bounce");

	mm_graph_node->add_slot_enum("get_combiner_axis_type_x", "set_combiner_axis_type_x", "Combiner Axis type", arr2);
	mm_graph_node->add_slot_enum("get_combiner_axis_type_y", "set_combiner_axis_type_y", "", arr2);
	//, Vector2(0, 32));
	mm_graph_node->add_slot_vector2("get_repeat", "set_repeat", "Repeat", 1);
}

void MMPattern::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMPattern::_get_value_for(const Vector2 &uv, const int pseed) {
	float f = MMAlgos::pattern(uv, repeat.x, repeat.y, combiner_type, combiner_axis_type_x, combiner_axis_type_y);
	return Color(f, f, f, 1);
}

MMPattern::MMPattern() {
	combiner_type = 0;
	combiner_axis_type_x = 0;
	combiner_axis_type_y = 0;
	repeat = Vector2(4, 4);
}

MMPattern::~MMPattern() {
}

void MMPattern::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMPattern::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMPattern::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_combiner_type"), &MMPattern::get_combiner_type);
	ClassDB::bind_method(D_METHOD("set_combiner_type", "value"), &MMPattern::set_combiner_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "combiner_type"), "set_combiner_type", "get_combiner_type");

	ClassDB::bind_method(D_METHOD("get_combiner_axis_type_x"), &MMPattern::get_combiner_axis_type_x);
	ClassDB::bind_method(D_METHOD("set_combiner_axis_type_x", "value"), &MMPattern::set_combiner_axis_type_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "combiner_axis_type_x"), "set_combiner_axis_type_x", "get_combiner_axis_type_x");

	ClassDB::bind_method(D_METHOD("get_combiner_axis_type_y"), &MMPattern::get_combiner_axis_type_y);
	ClassDB::bind_method(D_METHOD("set_combiner_axis_type_y", "value"), &MMPattern::set_combiner_axis_type_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "combiner_axis_type_y"), "set_combiner_axis_type_y", "get_combiner_axis_type_y");

	ClassDB::bind_method(D_METHOD("get_repeat"), &MMPattern::get_repeat);
	ClassDB::bind_method(D_METHOD("set_repeat", "value"), &MMPattern::set_repeat);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "repeat"), "set_repeat", "get_repeat");
}
