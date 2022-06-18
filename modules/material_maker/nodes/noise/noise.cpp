
#include "noise.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMNoise::get_image() {
	return image;
}

void MMNoise::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

int MMNoise::get_grid_size() const {
	return grid_size;
}

void MMNoise::set_grid_size(const int val) {
	grid_size = val;
	set_dirty(true);
}

float MMNoise::get_density() const {
	return density;
}

void MMNoise::set_density(const float val) {
	density = val;
	set_dirty(true);
}

void MMNoise::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	register_output_property(image);
}

void MMNoise::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	//, Vector2(1, 10));
	mm_graph_node->add_slot_int("get_grid_size", "set_grid_size", "Grid Size");
	//, Vector2(0, 1));
	mm_graph_node->add_slot_float("get_density", "set_density", "Density", 0.01);
}

Color MMNoise::_get_value_for(const Vector2 &uv, const int pseed) {
	float ps = 1.0 / float(pseed);
	//return dots(uv, 1.0/$(size), $(density), $(seed));
	float f = MMAlgos::dots(uv, 1.0 / float(grid_size), density, ps);
	return Color(f, f, f, 1);
}

void MMNoise::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

MMNoise::MMNoise() {
	grid_size = 16;
	density = 0.5;
}

MMNoise::~MMNoise() {
}

void MMNoise::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMNoise::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMNoise::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_grid_size"), &MMNoise::get_grid_size);
	ClassDB::bind_method(D_METHOD("set_grid_size", "value"), &MMNoise::set_grid_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "grid_size"), "set_grid_size", "get_grid_size");

	ClassDB::bind_method(D_METHOD("get_density"), &MMNoise::get_density);
	ClassDB::bind_method(D_METHOD("set_density", "value"), &MMNoise::set_density);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "density"), "set_density", "get_density");
}
