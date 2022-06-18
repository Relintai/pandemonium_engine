
#include "decompose.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMDecompose::get_input() {
	return input;
}

void MMDecompose::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMDecompose::get_out_r() {
	return out_r;
}

void MMDecompose::set_out_r(const Ref<MMNodeUniversalProperty> &val) {
	out_r = val;
}

Ref<MMNodeUniversalProperty> MMDecompose::get_out_g() {
	return out_g;
}

void MMDecompose::set_out_g(const Ref<MMNodeUniversalProperty> &val) {
	out_g = val;
}

Ref<MMNodeUniversalProperty> MMDecompose::get_out_b() {
	return out_b;
}

void MMDecompose::set_out_b(const Ref<MMNodeUniversalProperty> &val) {
	out_b = val;
}

Ref<MMNodeUniversalProperty> MMDecompose::get_out_a() {
	return out_a;
}

void MMDecompose::set_out_a(const Ref<MMNodeUniversalProperty> &val) {
	out_a = val;
}

void MMDecompose::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color(0, 0, 0, 1));
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input    ");

	if (!out_r.is_valid()) {
		out_r.instance();
		out_r->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_r->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_g.is_valid()) {
		out_g.instance();
		out_g->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_g->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_b.is_valid()) {
		out_b.instance();
		out_b->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_b->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_a.is_valid()) {
		out_a.instance();
		out_a->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_a->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	register_input_property(input);
	register_output_property(out_r);
	register_output_property(out_g);
	register_output_property(out_b);
	register_output_property(out_a);
}

void MMDecompose::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(out_r);
	mm_graph_node->add_slot_texture_universal(out_g);
	mm_graph_node->add_slot_texture_universal(out_b);
	mm_graph_node->add_slot_texture_universal(out_a);
}

void MMDecompose::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img_r;
	Ref<Image> img_g;
	Ref<Image> img_b;
	Ref<Image> img_a;

	img_r.instance();
	img_g.instance();
	img_b.instance();
	img_a.instance();

	img_r->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	img_g->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	img_b->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	img_a->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);

	img_r->lock();
	img_g->lock();
	img_b->lock();
	img_a->lock();

	float w = material->image_size.x;
	float h = material->image_size.y;
	//float pseed = Math::randf() + Math::rand();

	for (int x = 0; x < material->image_size.x; ++x) { //x in range(material.image_size.x)
		for (int y = 0; y < material->image_size.y; ++y) { //y in range(material.image_size.y)
			Vector2 uv = Vector2(x / w, y / h);
			Color c = input->get_value(uv);
			img_r->set_pixel(x, y, Color(c.r, c.r, c.r, 1));
			img_g->set_pixel(x, y, Color(c.g, c.g, c.g, 1));
			img_b->set_pixel(x, y, Color(c.b, c.b, c.b, 1));
			img_a->set_pixel(x, y, Color(c.a, c.a, c.a, c.a));
		}
	}

	img_r->unlock();
	img_g->unlock();
	img_b->unlock();
	img_a->unlock();

	out_r->set_value(img_r);
	out_g->set_value(img_g);
	out_b->set_value(img_b);
	out_a->set_value(img_a);
}

Color MMDecompose::_get_value_for(const Vector2 &uv, const int pseed) {
	return Color();
}

MMDecompose::MMDecompose() {
}

MMDecompose::~MMDecompose() {
}

void MMDecompose::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &MMDecompose::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMDecompose::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_out_r"), &MMDecompose::get_out_r);
	ClassDB::bind_method(D_METHOD("set_out_r", "value"), &MMDecompose::set_out_r);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_r", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_r", "get_out_r");

	ClassDB::bind_method(D_METHOD("get_out_g"), &MMDecompose::get_out_g);
	ClassDB::bind_method(D_METHOD("set_out_g", "value"), &MMDecompose::set_out_g);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_g", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_g", "get_out_g");

	ClassDB::bind_method(D_METHOD("get_out_b"), &MMDecompose::get_out_b);
	ClassDB::bind_method(D_METHOD("set_out_b", "value"), &MMDecompose::set_out_b);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_b", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_b", "get_out_b");

	ClassDB::bind_method(D_METHOD("get_out_a"), &MMDecompose::get_out_a);
	ClassDB::bind_method(D_METHOD("set_out_a", "value"), &MMDecompose::set_out_a);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_a", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_a", "get_out_a");
}
