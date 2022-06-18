
#include "gradient.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMGradient::get_image() {
	return image;
}

void MMGradient::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

float MMGradient::get_repeat() const {
	return repeat;
}

void MMGradient::set_repeat(const float val) {
	repeat = val;
	set_dirty(true);
}

float MMGradient::get_rotate() const {
	return rotate;
}

void MMGradient::set_rotate(const float val) {
	rotate = val;
	set_dirty(true);
}

void MMGradient::_init_properties() {
	if (!image.is_valid()) {
		image.instance();

		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	register_output_property(image);
}

void MMGradient::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_float("get_repeat", "set_repeat", "repeat");
	mm_graph_node->add_slot_float("get_rotate", "set_rotate", "rotate");
	mm_graph_node->add_slot_gradient();
}

void MMGradient::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMGradient::_get_value_for(const Vector2 &uv, const int pseed) {
	if (interpolation_type == 0) {
		return MMAlgos::normal_gradient_type_1(uv, repeat, rotate, points);
	} else if (interpolation_type == 1) {
		return MMAlgos::normal_gradient_type_2(uv, repeat, rotate, points);
	} else if (interpolation_type == 2) {
		return MMAlgos::normal_gradient_type_3(uv, repeat, rotate, points);
	} else if (interpolation_type == 3) {
		return MMAlgos::normal_gradient_type_4(uv, repeat, rotate, points);
	}

	return Color(1, 1, 1, 1);
}

Color MMGradient::_get_gradient_color(const float x) {
	if (interpolation_type == 0) {
		return MMAlgos::gradient_type_1(x, points);
	} else if (interpolation_type == 1) {
		return MMAlgos::gradient_type_2(x, points);
	} else if (interpolation_type == 2) {
		return MMAlgos::gradient_type_3(x, points);
	} else if (interpolation_type == 3) {
		return MMAlgos::gradient_type_4(x, points);
	}

	return Color(1, 1, 1, 1);
}

MMGradient::MMGradient() {
	repeat = 1;
	rotate = 0;
}

MMGradient::~MMGradient() {
}

void MMGradient::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMGradient::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMGradient::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_repeat"), &MMGradient::get_repeat);
	ClassDB::bind_method(D_METHOD("set_repeat", "value"), &MMGradient::set_repeat);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "repeat"), "set_repeat", "get_repeat");

	ClassDB::bind_method(D_METHOD("get_rotate"), &MMGradient::get_rotate);
	ClassDB::bind_method(D_METHOD("set_rotate", "value"), &MMGradient::set_rotate);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotate"), "set_rotate", "get_rotate");
}
