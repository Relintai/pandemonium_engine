
#include "transform.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"

Ref<MMNodeUniversalProperty> MMTransform::get_image() {
	return image;
}

void MMTransform::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMTransform::get_input() {
	return input;
}

void MMTransform::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMTransform::get_translate_x() {
	return translate_x;
}

void MMTransform::set_translate_x(const Ref<MMNodeUniversalProperty> &val) {
	translate_x = val;
}

Ref<MMNodeUniversalProperty> MMTransform::get_translate_y() {
	return translate_y;
}

void MMTransform::set_translate_y(const Ref<MMNodeUniversalProperty> &val) {
	translate_y = val;
}

Ref<MMNodeUniversalProperty> MMTransform::get_rotate() {
	return rotate;
}

void MMTransform::set_rotate(const Ref<MMNodeUniversalProperty> &val) {
	rotate = val;
}

Ref<MMNodeUniversalProperty> MMTransform::get_scale_x() {
	return scale_x;
}

void MMTransform::set_scale_x(const Ref<MMNodeUniversalProperty> &val) {
	scale_x = val;
}

Ref<MMNodeUniversalProperty> MMTransform::get_scale_y() {
	return scale_y;
}

void MMTransform::set_scale_y(const Ref<MMNodeUniversalProperty> &val) {
	scale_y = val;
}

int MMTransform::get_mode() const {
	return mode;
}

void MMTransform::set_mode(const int val) {
	mode = val;
	set_dirty(true);
}

void MMTransform::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color(0, 0, 0, 1));
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input    ");

	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	//image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	//image.force_override = true;

	if (!translate_x.is_valid()) {
		translate_x.instance();
		translate_x->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		translate_x->set_default_value(0);
		translate_x->set_value_step(0.01);
	}

	translate_x->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	translate_x->set_slot_name("Translate X");

	if (!translate_y.is_valid()) {
		translate_y.instance();
		translate_y->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		translate_y->set_default_value(0);
		translate_y->set_value_step(0.01);
	}

	translate_y->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	translate_y->set_slot_name("Translate Y");

	if (!rotate.is_valid()) {
		rotate.instance();
		rotate->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		rotate->set_default_value(0);
		rotate->set_value_step(0.01);
	}

	rotate->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	rotate->set_slot_name("Rotate");

	if (!scale_x.is_valid()) {
		scale_x.instance();
		scale_x->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		scale_x->set_default_value(1);
		scale_x->set_value_step(0.01);
	}

	scale_x->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	scale_x->set_slot_name("Scale X");

	if (!scale_y.is_valid()) {
		scale_y.instance();
		scale_y->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		scale_y->set_default_value(1);
		scale_y->set_value_step(0.01);
	}

	scale_y->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	scale_y->set_slot_name("Scale Y");

	register_input_property(input);
	register_output_property(image);
	register_input_property(translate_x);
	register_input_property(translate_y);
	register_input_property(rotate);
	register_input_property(scale_x);
	register_input_property(scale_y);
}

void MMTransform::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_float_universal(translate_x);
	mm_graph_node->add_slot_float_universal(translate_y);
	mm_graph_node->add_slot_float_universal(rotate);
	mm_graph_node->add_slot_float_universal(scale_x);
	mm_graph_node->add_slot_float_universal(scale_y);

	Array arr;
	arr.push_back("Clamp");
	arr.push_back("Repeat");
	arr.push_back("Extend");

	mm_graph_node->add_slot_enum("get_mode", "set_mode", "Mode", arr);
}

void MMTransform::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMTransform::_get_value_for(const Vector2 &uv, const int pseed) {
	//$i($mode(transform2($uv, vec2($translate_x*(2.0*$tx($uv)-1.0), $translate_y*(2.0*$ty($uv)-1.0)), $rotate*0.01745329251*(2.0*$r($uv)-1.0), vec2($scale_x*(2.0*$sx($uv)-1.0), $scale_y*(2.0*$sy($uv)-1.0)))))",;
	//Mode:;
	//Clamp -> transform2_clamp;
	//Repeat -> fract;
	//Extend -> "";
	Vector2 tr = Vector2(float(translate_x->get_default_value()) * (2.0 * float(translate_x->get_value_or_zero(uv)) - 1.0), float(translate_y->get_default_value()) * (2.0 * float(translate_y->get_value_or_zero(uv)) - 1.0));
	float rot = float(rotate->get_default_value()) * 0.01745329251 * (2.0 * float(rotate->get_value_or_zero(uv)) - 1.0);
	Vector2 sc = Vector2(float(scale_x->get_default_value()) * (2.0 * float(scale_x->get_value_or_zero(uv)) - 1.0), float(scale_y->get_default_value()) * (2.0 * float(scale_y->get_value_or_zero(uv)) - 1.0));
	Vector2 nuv = MMAlgos::transform2(uv, tr, rot, sc);

	if (mode == 0) {
		nuv = MMAlgos::transform2_clamp(nuv);
	}

	else if (mode == 1) {
		nuv = MMAlgos::fractv2(nuv);
	}

	return input->get_value(nuv);
}

MMTransform::MMTransform() {
	mode = 0;
}

MMTransform::~MMTransform() {
}

void MMTransform::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMTransform::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMTransform::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMTransform::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMTransform::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_translate_x"), &MMTransform::get_translate_x);
	ClassDB::bind_method(D_METHOD("set_translate_x", "value"), &MMTransform::set_translate_x);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "translate_x", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_translate_x", "get_translate_x");

	ClassDB::bind_method(D_METHOD("get_translate_y"), &MMTransform::get_translate_y);
	ClassDB::bind_method(D_METHOD("set_translate_y", "value"), &MMTransform::set_translate_y);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "translate_y", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_translate_y", "get_translate_y");

	ClassDB::bind_method(D_METHOD("get_rotate"), &MMTransform::get_rotate);
	ClassDB::bind_method(D_METHOD("set_rotate", "value"), &MMTransform::set_rotate);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "rotate", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_rotate", "get_rotate");

	ClassDB::bind_method(D_METHOD("get_scale_x"), &MMTransform::get_scale_x);
	ClassDB::bind_method(D_METHOD("set_scale_x", "value"), &MMTransform::set_scale_x);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scale_x", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_scale_x", "get_scale_x");

	ClassDB::bind_method(D_METHOD("get_scale_y"), &MMTransform::get_scale_y);
	ClassDB::bind_method(D_METHOD("set_scale_y", "value"), &MMTransform::set_scale_y);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scale_y", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_scale_y", "get_scale_y");

	ClassDB::bind_method(D_METHOD("get_mode"), &MMTransform::get_mode);
	ClassDB::bind_method(D_METHOD("set_mode", "value"), &MMTransform::set_mode);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mode"), "set_mode", "get_mode");
}
