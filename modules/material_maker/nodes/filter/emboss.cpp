
#include "emboss.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMEmboss::get_image() {
	return image;
}

void MMEmboss::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMEmboss::get_input() {
	return input;
}

void MMEmboss::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

float MMEmboss::get_angle() const {
	return angle;
}

void MMEmboss::set_angle(const float val) {
	angle = val;
	set_dirty(true);
}

float MMEmboss::get_amount() const {
	return amount;
}

void MMEmboss::set_amount(const float val) {
	amount = val;
	set_dirty(true);
}

float MMEmboss::get_width() const {
	return width;
}

void MMEmboss::set_width(const float val) {
	width = val;
	set_dirty(true);
}

int MMEmboss::get_size() const {
	return size;
}

void MMEmboss::set_size(const int val) {
	size = val;
	set_dirty(true);
}

void MMEmboss::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		input->set_default_value(1);
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input1    ");

	register_input_property(input);
	register_output_property(image);
}

void MMEmboss::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_float("get_angle", "set_angle", "Angle", 0.1);
	mm_graph_node->add_slot_float("get_amount", "set_amount", "Amount", 0.1);
	mm_graph_node->add_slot_float("get_width", "set_width", "Width", 1);
}

void MMEmboss::_render(const Ref<MMMaterial> &material) {
	size = MAX(material->image_size.x, material->image_size.y);
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMEmboss::_get_value_for(const Vector2 &uv, const int pseed) {
	float f = 0;
	f = emboss(uv, size, angle, amount, width);
	return Color(f, f, f, 1);
}

//float $(name)_fct(vec2 uv) {;
//	float pixels = max(1.0, $width);
//	float e = 1.0/$size;
//	float rv = 0.0;
//;
//	for (float dx = -pixels; dx <= pixels; dx += 1.0) {;
//		for (float dy = -pixels; dy <= pixels; dy += 1.0) {;
//			if (abs(dx) > 0.5 || abs(dy) > 0.5) {;
//				rv += $in(uv+e*vec2(dx, dy))*cos(atan(dy, dx)-$angle*3.14159265359/180.0)/length(vec2(dx, dy));
//			};
//		};
//	};
//;
//	return $amount*rv/pixels+0.5;
//};

float MMEmboss::emboss(const Vector2 &uv, const float psize, const float pangle, const float pamount, const float pwidth) {
	float pixels = MAX(1.0, pwidth);
	float e = 1.0 / psize;
	float rv = 0.0;
	float dx = -pixels;
	float dy = -pixels;
	//for (float dx = -pixels; dx <= pixels; dx += 1.0) {;

	while (dx <= pixels) {
		//for (float dy = -pixels; dy <= pixels; dy += 1.0) {;

		while (dy <= pixels) {
			if ((abs(dx) > 0.5 || abs(dy) > 0.5)) {
				rv += float(input->get_value(uv + e * Vector2(dx, dy))) * Math::cos(Math::atan2(dy, dx) - pangle * 3.14159265359 / 180.0) / Vector2(dx, dy).length();
			}

			dx += 1;
			dy += 1;
		}
	}

	return pamount * rv / pixels + 0.5;
}

MMEmboss::MMEmboss() {
	angle = 0;
	amount = 5;
	width = 1;
	size = 0;
}

MMEmboss::~MMEmboss() {
}

void MMEmboss::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMEmboss::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMEmboss::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMEmboss::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMEmboss::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_angle"), &MMEmboss::get_angle);
	ClassDB::bind_method(D_METHOD("set_angle", "value"), &MMEmboss::set_angle);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "angle"), "set_angle", "get_angle");

	ClassDB::bind_method(D_METHOD("get_amount"), &MMEmboss::get_amount);
	ClassDB::bind_method(D_METHOD("set_amount", "value"), &MMEmboss::set_amount);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "amount"), "set_amount", "get_amount");

	ClassDB::bind_method(D_METHOD("get_width"), &MMEmboss::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "value"), &MMEmboss::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");

	ClassDB::bind_method(D_METHOD("get_size"), &MMEmboss::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &MMEmboss::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");

	ClassDB::bind_method(D_METHOD("emboss", "uv", "psize", "pangle", "pamount", "pwidth"), &MMEmboss::emboss);
}
