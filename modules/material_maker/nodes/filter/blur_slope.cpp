
#include "blur_slope.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMBlurSlope::get_image() {
	return image;
}

void MMBlurSlope::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMBlurSlope::get_input() {
	return input;
}

void MMBlurSlope::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMBlurSlope::get_sigma() {
	return sigma;
}

void MMBlurSlope::set_sigma(const Ref<MMNodeUniversalProperty> &val) {
	sigma = val;
}

void MMBlurSlope::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color());
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input1    ");

	if (!sigma.is_valid()) {
		sigma.instance();
		sigma->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		sigma->set_default_value(50);
	}

	sigma->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	sigma->set_slot_name("Sigma");

	register_input_property(input);
	register_output_property(image);
	register_input_property(sigma);
}

void MMBlurSlope::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_float_universal(sigma);
}

void MMBlurSlope::_render(const Ref<MMMaterial> &material) {
	size = MAX(material->image_size.x, material->image_size.y);
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Ref<Image> MMBlurSlope::_render_image(const Ref<MMMaterial> &material) {
	Ref<Image> img;
	img.instance();

	img->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	img->lock();

	float w = img->get_width();
	float h = img->get_width();
	//float pseed = Math::randf() + Math::rand();

	for (int x = 0; x < img->get_width(); ++x) { //x in range(img.get_width())

		for (int y = 0; y < img->get_height(); ++y) { //y in range(img.get_height())
			Vector2 v = Vector2(x / w, y / h);
			Color col = slope_blur(v, size);
			img->set_pixel(x, y, col);
		}
	}

	img->unlock();

	return img;
}

//slope_blur.mmg;
/*
vec4 $(name) _fct(vec2 uv) {
	float dx = 1.0 / $size;
	float v = $heightmap(uv);
	vec2 slope = vec2($heightmap(uv + vec2(dx, 0.0)) - v, $heightmap(uv + vec2(0.0, dx)) - v);
	float slope_strength = length(slope) * $size;
	vec2 norm_slope = (slope_strength == 0.0) ? vec2(0.0, 1.0) : normalize(slope);
	vec2 e = dx * norm_slope;
	vec4 rv = vec4(0.0);
	float sum = 0.0;
	float sigma = max($sigma * slope_strength, 0.0001);
	for (float i = 0.0; i <= 50.0; i += 1.0) {
		float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718 * sigma * sigma);
		rv += $in(uv + i * e) * coef;
		sum += coef;
	}
	return rv / sum;
}
*/

Color MMBlurSlope::slope_blur(const Vector2 &uv, const float psize) {
	float dx = 1.0 / psize;
	float v = sigma->get_value(uv);
	float sx = sigma->get_value(uv + Vector2(dx, 0.0));
	float sy = sigma->get_value(uv + Vector2(0.0, dx));

	Vector2 slope = Vector2(sx - v, sy - v);
	float slope_strength = slope.length() * psize;
	Vector2 norm_slope = (slope_strength == 0.0) ? Vector2(0.0, 1.0) : slope.normalized();
	Vector2 e = dx * norm_slope;
	Color rv = Color();
	float sum = 0.0;
	float sigma = MAX(v * slope_strength, 0.0001);

	for (float i = 0.0; i <= 50.0; i += 1.0) {
		float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718 * sigma * sigma);
		rv += Color(input->get_value(uv + i * e)) * coef;
		sum += coef;
	}

	return rv / sum;
}

MMBlurSlope::MMBlurSlope() {
	size = 0;
}

MMBlurSlope::~MMBlurSlope() {
}

void MMBlurSlope::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMBlurSlope::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMBlurSlope::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMBlurSlope::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMBlurSlope::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_sigma"), &MMBlurSlope::get_sigma);
	ClassDB::bind_method(D_METHOD("set_sigma", "value"), &MMBlurSlope::set_sigma);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sigma", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_sigma", "get_sigma");

	ClassDB::bind_method(D_METHOD("slope_blur", "uv", "psize"), &MMBlurSlope::slope_blur);
}
