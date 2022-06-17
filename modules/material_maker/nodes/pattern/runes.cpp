
#include "runes.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMRunes::get_image() {
	return image;
}

void MMRunes::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Vector2 MMRunes::get_size() {
	return size;
}

void MMRunes::set_size(const Vector2 &val) {
	size = val;
	set_dirty(true);
}

void MMRunes::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	register_output_property(image);
}

void MMRunes::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_vector2("get_size", "set_size", "Size", 1);
}

void MMRunes::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMRunes::_get_value_for(const Vector2 &uv, const int pseed) {
	float ps = 1.0 / float(pseed);
	//Rune(vec2($columns, $rows)*$uv, float($seed));
	return MMAlgos::runesc(uv, size, ps);
}

MMRunes::MMRunes() {
	size = Vector2(4, 4);
}

MMRunes::~MMRunes() {
}

void MMRunes::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMRunes::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMRunes::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_size"), &MMRunes::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &MMRunes::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");
}
