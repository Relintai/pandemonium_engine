
#include "color_noise.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMColorNoise::get_image() {
	return image;
}

void MMColorNoise::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

int MMColorNoise::get_size() const {
	return size;
}

void MMColorNoise::set_size(const int val) {
	size = val;
	set_dirty(true);
}

void MMColorNoise::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	register_output_property(image);
}

void MMColorNoise::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	//, Vector2(1, 10));
	mm_graph_node->add_slot_int("get_size", "set_size", "Size");
}

Color MMColorNoise::_get_value_for(const Vector2 &uv, const int pseed) {
	float ps = 1.0 / float(pseed);
	//color_dots($(uv), 1.0/$(size), $(seed));
	return MMAlgos::noise_color(uv, float(size), ps);
}

void MMColorNoise::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

MMColorNoise::MMColorNoise() {
	size = 8;
}

MMColorNoise::~MMColorNoise() {
}

void MMColorNoise::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMColorNoise::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMColorNoise::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_size"), &MMColorNoise::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &MMColorNoise::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");
}
