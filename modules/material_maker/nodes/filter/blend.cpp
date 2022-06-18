
#include "blend.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMBlend::get_image() {
	return image;
}

void MMBlend::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMBlend::get_input1() {
	return input1;
}

void MMBlend::set_input1(const Ref<MMNodeUniversalProperty> &val) {
	input1 = val;
}

Ref<MMNodeUniversalProperty> MMBlend::get_input2() {
	return input2;
}

void MMBlend::set_input2(const Ref<MMNodeUniversalProperty> &val) {
	input2 = val;
}

int MMBlend::get_blend_type() const {
	return blend_type;
}

void MMBlend::set_blend_type(const int val) {
	blend_type = val;
	set_dirty(true);
}

Ref<MMNodeUniversalProperty> MMBlend::get_opacity() {
	return opacity;
}

void MMBlend::set_opacity(const Ref<MMNodeUniversalProperty> &val) {
	opacity = val;
}

void MMBlend::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!input1.is_valid()) {
		input1.instance();
		input1->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input1->set_default_value(Color(1, 1, 1, 1));
	}

	input1->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input1->set_slot_name(">>>    Input1    ");

	if (!input2.is_valid()) {
		input2.instance();
		input2->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input2->set_default_value(Color(1, 1, 1, 1));
	}

	input2->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input2->set_slot_name(">>>    Input2    ");

	if (!opacity.is_valid()) {
		opacity.instance();
		opacity->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		opacity->set_default_value(0.5);
		opacity->set_value_range(Vector2(0, 1));
		opacity->set_value_step(0.01);
	}

	opacity->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	opacity->set_slot_name("opacity");

	register_input_property(input1);
	register_input_property(input2);
	register_output_property(image);
	register_input_property(opacity);
}

void MMBlend::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("Normal");
	arr.push_back("Dissolve");
	arr.push_back("Multiply");
	arr.push_back("Screen");
	arr.push_back("Overlay");
	arr.push_back("Hard Light");
	arr.push_back("Soft Light");
	arr.push_back("Burn");
	arr.push_back("Dodge");
	arr.push_back("Lighten");
	arr.push_back("Darken");
	arr.push_back("Difference");

	mm_graph_node->add_slot_enum("get_blend_type", "set_blend_type", "blend_type", arr);
	mm_graph_node->add_slot_label_universal(input1);
	mm_graph_node->add_slot_label_universal(input2);
	mm_graph_node->add_slot_float_universal(opacity);
}

void MMBlend::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMBlend::_get_value_for(const Vector2 &uv, const int pseed) {
	Vector3 b = Vector3();
	//vec4 $(name_uv)_s1 = $s1($uv);
	Color s1 = input1->get_value(uv);
	//vec4 $(name_uv)_s2 = $s2($uv);
	Color s2 = input2->get_value(uv);
	//float $(name_uv)_a = $amount*$a($uv);
	float a = opacity->get_value(uv);
	//vec4(blend_$blend_type($uv, $(name_uv)_s1.rgb, $(name_uv)_s2.rgb, $(name_uv)_a*$(name_uv)_s1.a), min(1.0, $(name_uv)_s2.a+$(name_uv)_a*$(name_uv)_s1.a));
	//"Normal,Dissolve,Multiply,Screen,Overlay,Hard Light,Soft Light,Burn,Dodge,Lighten,Darken,Difference";

	if (blend_type == BLEND_TYPE_NORMAL) {
		b = MMAlgos::blend_normal(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	} else if (blend_type == BLEND_TYPE_DISSOLVE) {
		b = MMAlgos::blend_dissolve(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	} else if (blend_type == BLEND_TYPE_MULTIPLY) {
		b = MMAlgos::blend_multiply(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	} else if (blend_type == BLEND_TYPE_SCREEN) {
		b = MMAlgos::blend_screen(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	} else if (blend_type == BLEND_TYPE_OVERLAY) {
		b = MMAlgos::blend_overlay(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	} else if (blend_type == BLEND_TYPE_HARD_LIGHT) {
		b = MMAlgos::blend_hard_light(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	} else if (blend_type == BLEND_TYPE_SOFT_LIGHT) {
		b = MMAlgos::blend_soft_light(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	} else if (blend_type == BLEND_TYPE_BURN) {
		b = MMAlgos::blend_burn(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	} else if (blend_type == BLEND_TYPE_DODGE) {
		b = MMAlgos::blend_dodge(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	} else if (blend_type == BLEND_TYPE_LIGHTEN) {
		b = MMAlgos::blend_lighten(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	} else if (blend_type == BLEND_TYPE_DARKEN) {
		b = MMAlgos::blend_darken(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	} else if (blend_type == BLEND_TYPE_DIFFRENCE) {
		b = MMAlgos::blend_difference(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
	}

	return Color(b.x, b.y, b.z, MIN(1, s2.a + a * s1.a));
}

MMBlend::MMBlend() {
	blend_type = 0;
}

MMBlend::~MMBlend() {
}

void MMBlend::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMBlend::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMBlend::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input1"), &MMBlend::get_input1);
	ClassDB::bind_method(D_METHOD("set_input1", "value"), &MMBlend::set_input1);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input1", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input1", "get_input1");

	ClassDB::bind_method(D_METHOD("get_input2"), &MMBlend::get_input2);
	ClassDB::bind_method(D_METHOD("set_input2", "value"), &MMBlend::set_input2);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input2", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input2", "get_input2");

	ClassDB::bind_method(D_METHOD("get_blend_type"), &MMBlend::get_blend_type);
	ClassDB::bind_method(D_METHOD("set_blend_type", "value"), &MMBlend::set_blend_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "blend_type"), "set_blend_type", "get_blend_type");

	ClassDB::bind_method(D_METHOD("get_opacity"), &MMBlend::get_opacity);
	ClassDB::bind_method(D_METHOD("set_opacity", "value"), &MMBlend::set_opacity);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "opacity", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_opacity", "get_opacity");
}
