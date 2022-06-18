
#include "blur_gaussian.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMBlurGaussian::get_image() {
	return image;
}

void MMBlurGaussian::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMBlurGaussian::get_input() {
	return input;
}

void MMBlurGaussian::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMBlurGaussian::get_sigma() {
	return sigma;
}

void MMBlurGaussian::set_sigma(const Ref<MMNodeUniversalProperty> &val) {
	sigma = val;
}

int MMBlurGaussian::get_direction() const {
	return direction;
}

void MMBlurGaussian::set_direction(const int val) {
	direction = val;
	set_dirty(true);
}

int MMBlurGaussian::get_size() const {
	return size;
}

void MMBlurGaussian::set_size(const int val) {
	size = val;
	set_dirty(true);
}

void MMBlurGaussian::_init_properties() {
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

void MMBlurGaussian::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_int_universal(sigma);

	Array arr;
	arr.push_back("Both");
	arr.push_back("X");
	arr.push_back("Y");

	mm_graph_node->add_slot_enum("get_direction", "set_direction", "Direction", arr);
}

void MMBlurGaussian::_render(const Ref<MMMaterial> &material) {
	size = MAX(material->image_size.x, material->image_size.y);
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Ref<Image> MMBlurGaussian::_render_image(const Ref<MMMaterial> &material) {
	Ref<Image> img;
	img.instance();

	img->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	img->lock();

	float w = img->get_width();
	float h = img->get_width();
	float pseed = Math::randf() + Math::rand();

	if (direction == 0) {
		for (int x = 0; x < img->get_width(); ++x) { //x in range(img.get_width())

			for (int y = 0; y < img->get_height(); ++y) { //y in range(img.get_height())
				Vector2 v = Vector2(x / w, y / h);
				Color col = get_value_x(v, pseed);
				img->set_pixel(x, y, col);
			}
		}

		img->unlock();
		image->set_value(img);

		Ref<Image> image2;
		image2.instance();

		image2->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
		image2->lock();

		for (int x = 0; x < img->get_width(); ++x) { //x in range(img.get_width())
			for (int y = 0; y < img->get_height(); ++y) { //y in range(img.get_height())
				Vector2 v = Vector2(x / w, y / h);
				Color col = get_value_y_img(v, pseed);
				image2->set_pixel(x, y, col);
			}
		}

		image2->unlock();

		return image2;
	}

	if (direction == 1) {
		for (int x = 0; x < img->get_width(); ++x) { //x in range(img.get_width())

			for (int y = 0; y < img->get_height(); ++y) { //y in range(img.get_height())
				Vector2 v = Vector2(x / w, y / h);
				Color col = get_value_x(v, pseed);
				img->set_pixel(x, y, col);
			}
		}
	}

	if (direction == 2) {
		for (int x = 0; x < img->get_width(); ++x) { //x in range(img.get_width())

			for (int y = 0; y < img->get_height(); ++y) { //y in range(img.get_height())
				Vector2 v = Vector2(x / w, y / h);
				Color col = get_value_y(v, pseed);
				img->set_pixel(x, y, col);
			}
		}
	}

	img->unlock();

	return img;
}

Color MMBlurGaussian::get_value_x(const Vector2 &uv, const int pseed) {
	float sig_def = sigma->get_default_value(uv);
	float sig = sigma->get_value(uv);
	return gaussian_blur_x(uv, size, sig_def, sig);
}

Color MMBlurGaussian::get_value_y(const Vector2 &uv, const int pseed) {
	float sig_def = sigma->get_default_value(uv);
	float sig = sigma->get_value(uv);
	return gaussian_blur_y(uv, size, sig_def, sig);
}

Color MMBlurGaussian::get_value_y_img(const Vector2 &uv, const int pseed) {
	float sig_def = sigma->get_default_value(uv);
	float sig = sigma->get_value(uv);
	return gaussian_blur_y_img(uv, size, sig_def, sig);
}

//----------------------;
//gaussian_blur_x.mmg;
//vec4 $(name)_fct(vec2 uv) {;
//	float e = 1.0 / $size;
//	vec4 rv = vec4(0.0);
//	float sum = 0.0;
//	float sigma = max(0.000001, $sigma * $amount(uv));
//;
//	for (float i = -50.0; i <= 50.0; i += 1.0) {;
//		float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718 * sigma * sigma);
//		rv += $in(uv+vec2(i*e, 0.0))*coef;
//		sum += coef;
//	};
//;
//	return rv/sum;
//};

Color MMBlurGaussian::gaussian_blur_x(const Vector2 &uv, const float psize, const float psigma, const float pamount) {
	float e = 1.0 / psize;
	Color rv = Color();
	float sum = 0.0;
	//pamount(uv));
	float sigma = MAX(0.000001, psigma * pamount);
	float i = -50;
	//for (float i = -50.0; i <= 50.0; i += 1.0) {;

	while (i <= 50) {
		float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718 * sigma * sigma);
		rv += Color(input->get_value(uv + Vector2(i * e, 0.0))) * coef;
		sum += coef;
		i += 1;
	}

	return rv / sum;
}

//----------------------;
//gaussian_blur_y.mmg;
//vec4 $(name)_fct(vec2 uv) {;
//	float e = 1.0/$size;
//	vec4 rv = vec4(0.0);
//	float sum = 0.0;
//	float sigma = max(0.000001, $sigma*$amount(uv));
//	for (float i = -50.0; i <= 50.0; i += 1.0) {;
//		float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718*sigma*sigma);
//		rv += $in(uv+vec2(0.0, i*e))*coef;
//		sum += coef;
//	};
//;
//	return rv/sum;
//};

Color MMBlurGaussian::gaussian_blur_y(const Vector2 &uv, const float psize, const float psigma, const float pamount) {
	float e = 1.0 / psize;
	Color rv = Color();
	float sum = 0.0;
	//pamount(uv));
	float sigma = MAX(0.000001, psigma * pamount);
	float i = -50;
	//for (float i = -50.0; i <= 50.0; i += 1.0) {;

	while (i <= 50) {
		float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718 * sigma * sigma);
		rv += Color(input->get_value(uv + Vector2(0.0, i * e))) * coef;
		sum += coef;
		i += 1;
	}

	return rv / sum;
}

Color MMBlurGaussian::gaussian_blur_y_img(const Vector2 &uv, const float psize, const float psigma, const float pamount) {
	float e = 1.0 / psize;
	Color rv = Color();
	float sum = 0.0;
	//pamount(uv));
	float sigma = MAX(0.000001, psigma * pamount);
	float i = -50;
	//for (float i = -50.0; i <= 50.0; i += 1.0) {;

	while (i <= 50) {
		float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718 * sigma * sigma);
		rv += Color(image->get_value(uv + Vector2(0.0, i * e))) * coef;
		sum += coef;
		i += 1;
	}

	return rv / sum;
}

MMBlurGaussian::MMBlurGaussian() {
	direction = 0;
	size = 0;
}

MMBlurGaussian::~MMBlurGaussian() {
}

void MMBlurGaussian::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMBlurGaussian::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMBlurGaussian::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMBlurGaussian::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMBlurGaussian::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_sigma"), &MMBlurGaussian::get_sigma);
	ClassDB::bind_method(D_METHOD("set_sigma", "value"), &MMBlurGaussian::set_sigma);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sigma", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_sigma", "get_sigma");

	ClassDB::bind_method(D_METHOD("get_direction"), &MMBlurGaussian::get_direction);
	ClassDB::bind_method(D_METHOD("set_direction", "value"), &MMBlurGaussian::set_direction);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "direction"), "set_direction", "get_direction");

	ClassDB::bind_method(D_METHOD("get_size"), &MMBlurGaussian::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &MMBlurGaussian::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");

	ClassDB::bind_method(D_METHOD("get_value_x", "uv", "pseed"), &MMBlurGaussian::get_value_x);
	ClassDB::bind_method(D_METHOD("get_value_y", "uv", "pseed"), &MMBlurGaussian::get_value_y);
	ClassDB::bind_method(D_METHOD("get_value_y_img", "uv", "pseed"), &MMBlurGaussian::get_value_y_img);

	ClassDB::bind_method(D_METHOD("gaussian_blur_x", "uv", "psize", "psigma", "pamount"), &MMBlurGaussian::gaussian_blur_x);
	ClassDB::bind_method(D_METHOD("gaussian_blur_y", "uv", "psize", "psigma", "pamount"), &MMBlurGaussian::gaussian_blur_y);
	ClassDB::bind_method(D_METHOD("gaussian_blur_y_img", "uv", "psize", "psigma", "pamount"), &MMBlurGaussian::gaussian_blur_y_img);
}
