
#include "weave.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMWeave::get_out_main() {
	return out_main;
}

void MMWeave::set_out_main(const Ref<MMNodeUniversalProperty> &val) {
	out_main = val;
}

Ref<MMNodeUniversalProperty> MMWeave::get_out_horizontal_map() {
	return out_horizontal_map;
}

void MMWeave::set_out_horizontal_map(const Ref<MMNodeUniversalProperty> &val) {
	out_horizontal_map = val;
}

Ref<MMNodeUniversalProperty> MMWeave::get_out_vertical_map() {
	return out_vertical_map;
}

void MMWeave::set_out_vertical_map(const Ref<MMNodeUniversalProperty> &val) {
	out_vertical_map = val;
}

Vector2 MMWeave::get_size() {
	return size;
}

void MMWeave::set_size(const Vector2 &val) {
	size = val;
	set_dirty(true);
}

Ref<MMNodeUniversalProperty> MMWeave::get_width() {
	return width;
}

void MMWeave::set_width(const Ref<MMNodeUniversalProperty> &val) {
	width = val;
}

int MMWeave::get_stitch() const {
	return stitch;
}

void MMWeave::set_stitch(const int val) {
	stitch = val;
	set_dirty(true);
}

void MMWeave::_init_properties() {
	if (!out_main.is_valid()) {
		out_main.instance();
		out_main->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_main->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_horizontal_map.is_valid()) {
		out_horizontal_map.instance();
		out_horizontal_map->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_horizontal_map->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_vertical_map.is_valid()) {
		out_vertical_map.instance();
		out_vertical_map->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_vertical_map->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!width.is_valid()) {
		width.instance();
		width->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_VECTOR2);
		width->set_default_value(Vector2(0.9, 0.9));
	}

	width->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	width->set_slot_name("Width");
	width->set_value_step(0.01);
	width->set_value_range(Vector2(0, 1));

	register_output_property(out_main);
	register_output_property(out_horizontal_map);
	register_output_property(out_vertical_map);
	register_input_property(width);
}

void MMWeave::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(out_main);
	mm_graph_node->add_slot_texture_universal(out_horizontal_map);
	mm_graph_node->add_slot_texture_universal(out_vertical_map);
	//, Vector2(1, 32))//, Vector2(0, 32));
	mm_graph_node->add_slot_vector2("get_size", "set_size", "Size");
	mm_graph_node->add_slot_vector2_universal(width);
	mm_graph_node->add_slot_int("get_stitch", "set_stitch", "Stitch");
}

void MMWeave::_render(const Ref<MMMaterial> &material) {
	Ref<Image> main_pattern;
	Ref<Image> horizontal_map;
	Ref<Image> vertical_map;

	main_pattern.instance();
	horizontal_map.instance();
	vertical_map.instance();

	main_pattern->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	horizontal_map->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	vertical_map->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);

	main_pattern->lock();
	horizontal_map->lock();
	vertical_map->lock();

	float w = material->image_size.x;
	float h = material->image_size.y;
	//float pseed = Math::randf() + Math::rand();

	for (int x = 0; x < material->image_size.x; ++x) { //x in range(material.image_size.x)
		for (int y = 0; y < material->image_size.y; ++y) { //y in range(material.image_size.y)
			Vector2 uv = Vector2(x / w, y / h);
			Vector2 width_val = width->get_value(uv);
			//vec3 $(name_uv) = weave2($uv, vec2($columns, $rows), $stitch, $width_x*$width_map($uv), $width_y*$width_map($uv));
			Vector3 weave = MMAlgos::weave2(uv, size, stitch, width_val.x, width_val.y);
			//Outputs:;
			//Output (float) - Shows the generated greyscale weave pattern.;
			//$(name_uv).x;
			Color main_pattern_col = Color(weave.x, weave.x, weave.x, 1);
			//Horizontal mask (float) - Horizontal mask;
			//$(name_uv).y;
			Color horizontal_map_col = Color(weave.y, weave.y, weave.y, 1);
			//Vertical mask (float) - Mask for vertical stripes;
			//$(name_uv).z;
			Color vertical_map_col = Color(weave.z, weave.z, weave.z, 1);

			main_pattern->set_pixel(x, y, main_pattern_col);
			horizontal_map->set_pixel(x, y, horizontal_map_col);
			vertical_map->set_pixel(x, y, vertical_map_col);
		}
	}

	main_pattern->unlock();
	horizontal_map->unlock();
	vertical_map->unlock();

	out_main->set_value(main_pattern);
	out_horizontal_map->set_value(horizontal_map);
	out_vertical_map->set_value(vertical_map);
}

Color MMWeave::_get_value_for(const Vector2 &uv, const int pseed) {
	return Color();
}

MMWeave::MMWeave() {
	size = Vector2(4, 4);
	stitch = 1;
}

MMWeave::~MMWeave() {
}

void MMWeave::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_out_main"), &MMWeave::get_out_main);
	ClassDB::bind_method(D_METHOD("set_out_main", "value"), &MMWeave::set_out_main);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_main", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_main", "get_out_main");

	ClassDB::bind_method(D_METHOD("get_out_horizontal_map"), &MMWeave::get_out_horizontal_map);
	ClassDB::bind_method(D_METHOD("set_out_horizontal_map", "value"), &MMWeave::set_out_horizontal_map);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_horizontal_map", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_horizontal_map", "get_out_horizontal_map");

	ClassDB::bind_method(D_METHOD("get_out_vertical_map"), &MMWeave::get_out_vertical_map);
	ClassDB::bind_method(D_METHOD("set_out_vertical_map", "value"), &MMWeave::set_out_vertical_map);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_vertical_map", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_vertical_map", "get_out_vertical_map");

	ClassDB::bind_method(D_METHOD("get_size"), &MMWeave::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &MMWeave::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");

	ClassDB::bind_method(D_METHOD("get_width"), &MMWeave::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "value"), &MMWeave::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "width", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_width", "get_width");

	ClassDB::bind_method(D_METHOD("get_stitch"), &MMWeave::get_stitch);
	ClassDB::bind_method(D_METHOD("set_stitch", "value"), &MMWeave::set_stitch);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stitch"), "set_stitch", "get_stitch");
}
