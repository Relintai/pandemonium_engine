
#include "tones.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "core/io/image.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMTones::get_input() {
	return input;
}

void MMTones::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMTones::get_image() {
	return image;
}

void MMTones::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Color MMTones::get_in_max() const {
	return _in_max;
}
void MMTones::set_in_max(const Color &val) {
	_in_max = val;
	set_dirty(true);
}

Color MMTones::get_in_mid() const {
	return _in_mid;
}
void MMTones::set_in_mid(const Color &val) {
	_in_mid = val;
	set_dirty(true);
}

Color MMTones::get_in_min() const {
	return _in_min;
}
void MMTones::set_in_min(const Color &val) {
	_in_min = val;
	set_dirty(true);
}

Color MMTones::get_out_max() const {
	return _out_max;
}
void MMTones::set_out_max(const Color &val) {
	_out_max = val;
	set_dirty(true);
}

Color MMTones::get_out_min() const {
	return _out_min;
}
void MMTones::set_out_min(const Color &val) {
	_out_min = val;
	set_dirty(true);
}

int MMTones::get_current_mode() {
	return _current_mode;
}
void MMTones::set_current_mode(int val) {
	_current_mode = val;
}

void MMTones::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color(0, 0, 0, 1));
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input     ");

	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	//image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	//image.force_override = true;

	register_input_property(input);
	register_output_property(image);
}

void MMTones::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);

	mm_graph_node->add_slot_tones();

	//mm_graph_node->add_slot_color("get_in_max", "set_in_max");
	//mm_graph_node->add_slot_color("get_in_mid", "set_in_mid");
	//mm_graph_node->add_slot_color("get_in_min", "set_in_min");
	//mm_graph_node->add_slot_color("get_out_max", "set_out_max");
	//mm_graph_node->add_slot_color("get_out_min", "set_out_min");
}

void MMTones::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMTones::_get_value_for(const Vector2 &uv, const int pseed) {
	Color c = input->get_value(uv);
	return MMAlgos::adjust_levels(c, _in_min, _in_mid, _in_max, _out_min, _out_max);
}

Ref<Image> MMTones::render_original_image(const Ref<MMMaterial> &material) {
	Ref<Image> image;
	image.instance();

	image->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	image->lock();

	float w = image->get_width();
	float h = image->get_height();

	for (int x = 0; x < image->get_width(); ++x) { //x in range(image.get_width())

		for (int y = 0; y < image->get_height(); ++y) { //y in range(image.get_height())
			Vector2 v = Vector2(x / w, y / h);
			Color col = input->get_value(v);
			image->set_pixel(x, y, col);
		}
	}

	image->unlock();
	return image;
}

MMTones::MMTones() {
	_in_max = Color(1, 1, 1, 1);
	_in_mid = Color(0.5, 0.5, 0.5, 0.5);
	_in_min = Color(0, 0, 0, 0);
	_out_max = Color(1, 1, 1, 1);
	_out_min = Color(0, 0, 0, 0);

	_current_mode = 0;
}

MMTones::~MMTones() {
}

void MMTones::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &MMTones::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMTones::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_image"), &MMTones::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMTones::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_in_max"), &MMTones::get_in_max);
	ClassDB::bind_method(D_METHOD("set_in_max", "value"), &MMTones::set_in_max);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "in_max"), "set_in_max", "get_in_max");

	ClassDB::bind_method(D_METHOD("get_in_mid"), &MMTones::get_in_mid);
	ClassDB::bind_method(D_METHOD("set_in_mid", "value"), &MMTones::set_in_mid);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "in_mid"), "set_in_mid", "get_in_mid");

	ClassDB::bind_method(D_METHOD("get_in_min"), &MMTones::get_in_min);
	ClassDB::bind_method(D_METHOD("set_in_min", "value"), &MMTones::set_in_min);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "in_min"), "set_in_min", "get_in_min");

	ClassDB::bind_method(D_METHOD("get_out_max"), &MMTones::get_out_max);
	ClassDB::bind_method(D_METHOD("set_out_max", "value"), &MMTones::set_out_max);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "out_max"), "set_out_max", "get_out_max");

	ClassDB::bind_method(D_METHOD("get_out_min"), &MMTones::get_out_min);
	ClassDB::bind_method(D_METHOD("set_out_min", "value"), &MMTones::set_out_min);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "out_min"), "set_out_min", "get_out_min");
}
