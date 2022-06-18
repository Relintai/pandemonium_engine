
#include "fbm_noise.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMFbmNoise::get_image() {
	return image;
}

void MMFbmNoise::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

int MMFbmNoise::get_type() const {
	return type;
}

void MMFbmNoise::set_type(const int val) {
	type = val;
	set_dirty(true);
}

Vector2 MMFbmNoise::get_scale() {
	return scale;
}

void MMFbmNoise::set_scale(const Vector2 &val) {
	scale = val;
	set_dirty(true);
}

int MMFbmNoise::get_folds() const {
	return folds;
}

void MMFbmNoise::set_folds(const int val) {
	folds = val;
	set_dirty(true);
}

int MMFbmNoise::get_iterations() const {
	return iterations;
}

void MMFbmNoise::set_iterations(const int val) {
	iterations = val;
	set_dirty(true);
}

float MMFbmNoise::get_persistence() const {
	return persistence;
}

void MMFbmNoise::set_persistence(const float val) {
	persistence = val;
	set_dirty(true);
}

void MMFbmNoise::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	register_output_property(image);
}

void MMFbmNoise::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("Value");
	arr.push_back("Perlin");
	arr.push_back("Simplex");
	arr.push_back("Cellular1");
	arr.push_back("Cellular2");
	arr.push_back("Cellular3");
	arr.push_back("Cellular4");
	arr.push_back("Cellular5");
	arr.push_back("Cellular6");

	//, Vector2(0, 1));
	mm_graph_node->add_slot_enum("get_type", "set_type", "Type", arr);
	//, Vector2(1, 10));
	mm_graph_node->add_slot_vector2("get_scale", "set_scale", "Scale");
	//, Vector2(0, 1));
	mm_graph_node->add_slot_int("get_folds", "set_folds", "folds");
	//, Vector2(0, 1));
	mm_graph_node->add_slot_int("get_iterations", "set_iterations", "Iterations");
	//, Vector2(0, 1));
	mm_graph_node->add_slot_float("get_persistence", "set_persistence", "Persistence", 0.01);
}

Color MMFbmNoise::_get_value_for(const Vector2 &uv, const int pseed) {
	float ps = 1.0 / float(pseed);
	//"Value,Perlin,Simplex,Cellular1,Cellular2,Cellular3,Cellular4,Cellular5,Cellular6";

	if (type == 0) {
		return MMAlgos::fbmval(uv, scale, folds, iterations, persistence, ps);
	} else if (type == 1) {
		return MMAlgos::perlin(uv, scale, folds, iterations, persistence, ps);
	} else if (type == 2) {
		return MMAlgos::simplex(uv, scale, folds, iterations, persistence, ps);
	} else if (type == 3) {
		return MMAlgos::cellular(uv, scale, folds, iterations, persistence, ps);
	} else if (type == 4) {
		return MMAlgos::cellular2(uv, scale, folds, iterations, persistence, ps);
	} else if (type == 5) {
		return MMAlgos::cellular3(uv, scale, folds, iterations, persistence, ps);
	} else if (type == 6) {
		return MMAlgos::cellular4(uv, scale, folds, iterations, persistence, ps);
	} else if (type == 7) {
		return MMAlgos::cellular5(uv, scale, folds, iterations, persistence, ps);
	} else if (type == 8) {
		return MMAlgos::cellular6(uv, scale, folds, iterations, persistence, ps);
	}

	return Color();
}

void MMFbmNoise::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

MMFbmNoise::MMFbmNoise() {
	type = 0;
	scale = Vector2(2, 2);
	folds = 0;
	iterations = 5;
	persistence = 0.5;
}

MMFbmNoise::~MMFbmNoise() {
}

void MMFbmNoise::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMFbmNoise::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMFbmNoise::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_type"), &MMFbmNoise::get_type);
	ClassDB::bind_method(D_METHOD("set_type", "value"), &MMFbmNoise::set_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");

	ClassDB::bind_method(D_METHOD("get_scale"), &MMFbmNoise::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "value"), &MMFbmNoise::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");

	ClassDB::bind_method(D_METHOD("get_folds"), &MMFbmNoise::get_folds);
	ClassDB::bind_method(D_METHOD("set_folds", "value"), &MMFbmNoise::set_folds);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "folds"), "set_folds", "get_folds");

	ClassDB::bind_method(D_METHOD("get_iterations"), &MMFbmNoise::get_iterations);
	ClassDB::bind_method(D_METHOD("set_iterations", "value"), &MMFbmNoise::set_iterations);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "iterations"), "set_iterations", "get_iterations");

	ClassDB::bind_method(D_METHOD("get_persistence"), &MMFbmNoise::get_persistence);
	ClassDB::bind_method(D_METHOD("set_persistence", "value"), &MMFbmNoise::set_persistence);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "persistence"), "set_persistence", "get_persistence");
}
