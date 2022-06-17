
#include "truchet.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMTruchet::get_image() {
	return image;
}

void MMTruchet::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

int MMTruchet::get_shape() const {
	return shape;
}

void MMTruchet::set_shape(const int val) {
	shape = val;
	set_dirty(true);
}

float MMTruchet::get_size() const {
	return size;
}

void MMTruchet::set_size(const float val) {
	size = val;
	set_dirty(true);
}

void MMTruchet::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	register_output_property(image);
}

void MMTruchet::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("Line");
	arr.push_back("Circle");

	mm_graph_node->add_slot_enum("get_shape", "set_shape", "Shape", arr);
	mm_graph_node->add_slot_float("get_size", "set_size", "Size", 1);
}

void MMTruchet::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMTruchet::_get_value_for(const Vector2 &uv, const int pseed) {
	if (shape == 0) {
		return MMAlgos::truchet1c(uv, size, pseed);
	}

	else if (shape == 1) {
		return MMAlgos::truchet2c(uv, size, pseed);
	}

	return Color();
}

MMTruchet::MMTruchet() {
	shape = 0;
	size = 4;
}

MMTruchet::~MMTruchet() {
}

void MMTruchet::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMTruchet::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMTruchet::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_shape"), &MMTruchet::get_shape);
	ClassDB::bind_method(D_METHOD("set_shape", "value"), &MMTruchet::set_shape);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shape"), "set_shape", "get_shape");

	ClassDB::bind_method(D_METHOD("get_size"), &MMTruchet::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &MMTruchet::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "size"), "set_size", "get_size");
}
