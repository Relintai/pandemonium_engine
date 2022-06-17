
#include "scratches.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMScratches::get_image() {
	return image;
}

void MMScratches::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Vector2 MMScratches::get_size() {
	return size;
}

void MMScratches::set_size(const Vector2 &val) {
	size = val;
	set_dirty(true);
}

int MMScratches::get_layers() const {
	return layers;
}

void MMScratches::set_layers(const int val) {
	layers = val;
	set_dirty(true);
}

float MMScratches::get_waviness() const {
	return waviness;
}

void MMScratches::set_waviness(const float val) {
	waviness = val;
	set_dirty(true);
}

int MMScratches::get_angle() const {
	return angle;
}

void MMScratches::set_angle(const int val) {
	angle = val;
	set_dirty(true);
}

float MMScratches::get_randomness() const {
	return randomness;
}

void MMScratches::set_randomness(const float val) {
	randomness = val;
	set_dirty(true);
}

void MMScratches::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	register_output_property(image);
}

void MMScratches::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_vector2("get_size", "set_size", "Size", 0.01);
	mm_graph_node->add_slot_int("get_layers", "set_layers", "Layers");
	mm_graph_node->add_slot_float("get_waviness", "set_waviness", "Waviness", 0.01);
	mm_graph_node->add_slot_int("get_angle", "set_angle", "Angle");
	mm_graph_node->add_slot_float("get_randomness", "set_randomness", "Randomness", 0.01);
}

void MMScratches::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMScratches::_get_value_for(const Vector2 &uv, const int pseed) {
	//scratches($uv, int($layers), vec2($length, $width), $waviness, $angle, $randomness, vec2(float($seed), 0.0));
	return MMAlgos::scratchesc(uv, layers, size, waviness, angle, randomness, Vector2(pseed, 0.0));
}

MMScratches::MMScratches() {
	size = Vector2(0.25, 0.4);
	layers = 4;
	waviness = 0.51;
	angle = 0;
	randomness = 0.44;
}

MMScratches::~MMScratches() {
}

void MMScratches::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMScratches::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMScratches::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_size"), &MMScratches::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &MMScratches::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");

	ClassDB::bind_method(D_METHOD("get_layers"), &MMScratches::get_layers);
	ClassDB::bind_method(D_METHOD("set_layers", "value"), &MMScratches::set_layers);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "layers"), "set_layers", "get_layers");

	ClassDB::bind_method(D_METHOD("get_waviness"), &MMScratches::get_waviness);
	ClassDB::bind_method(D_METHOD("set_waviness", "value"), &MMScratches::set_waviness);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "waviness"), "set_waviness", "get_waviness");

	ClassDB::bind_method(D_METHOD("get_angle"), &MMScratches::get_angle);
	ClassDB::bind_method(D_METHOD("set_angle", "value"), &MMScratches::set_angle);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "angle"), "set_angle", "get_angle");

	ClassDB::bind_method(D_METHOD("get_randomness"), &MMScratches::get_randomness);
	ClassDB::bind_method(D_METHOD("set_randomness", "value"), &MMScratches::set_randomness);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "randomness"), "set_randomness", "get_randomness");
}
