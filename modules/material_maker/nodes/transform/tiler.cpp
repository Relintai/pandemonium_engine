
#include "tiler.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMTiler::get_input() {
	return input;
}

void MMTiler::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMTiler::get_in_mask() {
	return in_mask;
}

void MMTiler::set_in_mask(const Ref<MMNodeUniversalProperty> &val) {
	in_mask = val;
}

Ref<MMNodeUniversalProperty> MMTiler::get_output() {
	return output;
}

void MMTiler::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

Ref<MMNodeUniversalProperty> MMTiler::get_instance_map() {
	return instance_map;
}

void MMTiler::set_instance_map(const Ref<MMNodeUniversalProperty> &val) {
	instance_map = val;
}

Vector2 MMTiler::get_tile() {
	return tile;
}

void MMTiler::set_tile(const Vector2 &val) {
	tile = val;
	set_dirty(true);
}

float MMTiler::get_overlap() const {
	return overlap;
}

void MMTiler::set_overlap(const float val) {
	overlap = val;
	set_dirty(true);
}

int MMTiler::get_select_inputs() const {
	return select_inputs;
}

void MMTiler::set_select_inputs(const int val) {
	select_inputs = val;
	set_dirty(true);
}

Vector2 MMTiler::get_scale() {
	return scale;
}

void MMTiler::set_scale(const Vector2 &val) {
	scale = val;
	set_dirty(true);
}

float MMTiler::get_fixed_offset() const {
	return fixed_offset;
}

void MMTiler::set_fixed_offset(const float val) {
	fixed_offset = val;
	set_dirty(true);
}

float MMTiler::get_rnd_offset() const {
	return rnd_offset;
}

void MMTiler::set_rnd_offset(const float val) {
	rnd_offset = val;
	set_dirty(true);
}

float MMTiler::get_rnd_rotate() const {
	return rnd_rotate;
}

void MMTiler::set_rnd_rotate(const float val) {
	rnd_rotate = val;
	set_dirty(true);
}

float MMTiler::get_rnd_scale() const {
	return rnd_scale;
}

void MMTiler::set_rnd_scale(const float val) {
	rnd_scale = val;
	set_dirty(true);
}

float MMTiler::get_rnd_value() const {
	return rnd_value;
}

void MMTiler::set_rnd_value(const float val) {
	rnd_value = val;
	set_dirty(true);
}

bool MMTiler::get_variations() const {
	return variations;
}

void MMTiler::set_variations(const bool val) {
	variations = val;
	set_dirty(true);
}

void MMTiler::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		input->set_default_value(0);
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input    ");

	if (!in_mask.is_valid()) {
		in_mask.instance();
		in_mask->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		in_mask->set_default_value(1);
	}

	in_mask->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	in_mask->set_slot_name(">>>    Mask    ");

	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!instance_map.is_valid()) {
		instance_map.instance();
		instance_map->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	instance_map->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	register_input_property(input);
	register_input_property(in_mask);
	register_output_property(output);
	register_output_property(instance_map);
}

void MMTiler::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_label_universal(in_mask);
	mm_graph_node->add_slot_texture_universal(output);
	mm_graph_node->add_slot_texture_universal(instance_map);
	mm_graph_node->add_slot_vector2("get_tile", "set_tile", "Tile", 1);
	mm_graph_node->add_slot_float("get_overlap", "set_overlap", "Overlap", 1);

	Array arr;
	arr.push_back("1");
	arr.push_back("4");
	arr.push_back("16");

	mm_graph_node->add_slot_enum("get_select_inputs", "set_select_inputs", "Select inputs", arr);
	mm_graph_node->add_slot_vector2("get_scale", "set_scale", "Scale", 0.01);
	mm_graph_node->add_slot_float("get_fixed_offset", "set_fixed_offset", "Fixed Offset", 0.01);
	mm_graph_node->add_slot_float("get_rnd_offset", "set_rnd_offset", "Rnd Offset", 0.01);
	mm_graph_node->add_slot_float("get_rnd_rotate", "set_rnd_rotate", "Rnd Rotate", 0.1);
	mm_graph_node->add_slot_float("get_rnd_scale", "set_rnd_scale", "Rnd Scale", 0.01);
	mm_graph_node->add_slot_float("get_rnd_value", "set_rnd_value", "Rnd Value", 0.01);
}

//mm_graph_node.add_slot_bool("get_variations", "set_variations", "Variations");

void MMTiler::_render(const Ref<MMMaterial> &material) {
	Ref<Image> output_img;
	output_img.instance();
	Ref<Image> instance_map_img;
	instance_map_img.instance();

	output_img->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	instance_map_img->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	output_img->lock();
	instance_map_img->lock();
	float w = material->image_size.x;
	float h = material->image_size.y;
	float pseed = Math::randf() + Math::rand();
	float ps = 1.0 / float(pseed);
	int ix = int(material->image_size.x);
	int iy = int(material->image_size.y);

	for (int x = 0; x < ix; ++x) { //x in range(ix)
		for (int y = 0; y < iy; ++y) { //y in range(iy)
			Vector2 uv = Vector2(x / w, y / h);
			//vec4 $(name_uv)_rch = tiler_$(name)($uv, vec2($tx, $ty), int($overlap), vec2(float($seed)));
			Color rch = tiler_calc(uv, tile, overlap, Vector2(ps, ps));
			//Output, float, Shows the generated pattern;
			//$(name_uv)_rch.a;
			Color output_img_col = Color(rch.a, rch.a, rch.a, 1);
			//Instance map, rgb, Shows a random color for each instance of the input image;
			//$(name_uv)_rch.rgb;
			Color instance_map_img_col = Color(rch.r, rch.g, rch.b, 1);
			output_img->set_pixel(x, y, output_img_col);
			instance_map_img->set_pixel(x, y, instance_map_img_col);
		}
	}

	output_img->unlock();
	instance_map_img->unlock();
	output->set_value(output_img);
	instance_map->set_value(instance_map_img);
}

Color MMTiler::_get_value_for(const Vector2 &uv, const int pseed) {
	return Color();
}

Color MMTiler::tiler_calc(const Vector2 &uv, const Vector2 &tile, const int overlap, const Vector2 &_seed) {
	float c = 0.0;
	Vector3 rc = Vector3();
	Vector3 rc1 = Vector3();
	//for (int dx = -overlap; dx <= overlap; ++dx) {;

	for (int dx = -overlap; dx < overlap; ++dx) { //dx in range(-overlap, overlap)
		//for (int dy = -overlap; dy <= overlap; ++dy) {;

		for (int dy = -overlap; dy < overlap; ++dy) { //dy in range(-overlap, overlap)
			Vector2 pos = MMAlgos::fractv2((MMAlgos::floorv2(uv * tile) + Vector2(dx, dy) + Vector2(0.5, 0.5)) / tile - Vector2(0.5, 0.5));
			Vector2 vseed = MMAlgos::rand2(pos + _seed);
			rc1 = MMAlgos::rand3(vseed);
			pos = MMAlgos::fractv2(pos + Vector2(fixed_offset / tile.x, 0.0) * floor(MMAlgos::modf(pos.y * tile.y, 2.0)) + rnd_offset * vseed / tile);
			float mask = in_mask->get_value(MMAlgos::fractv2(pos + Vector2(0.5, 0.5)));

			if ((mask > 0.01)) {
				Vector2 pv = MMAlgos::fractv2(uv - pos) - Vector2(0.5, 0.5);
				vseed = MMAlgos::rand2(vseed);
				float angle = (vseed.x * 2.0 - 1.0) * rnd_rotate * 0.01745329251;
				float ca = cos(angle);
				float sa = sin(angle);
				pv = Vector2(ca * pv.x + sa * pv.y, -sa * pv.x + ca * pv.y);
				pv *= (vseed.y - 0.5) * 2.0 * rnd_scale + 1.0;
				pv /= scale;
				pv += Vector2(0.5, 0.5);
				vseed = MMAlgos::rand2(vseed);
				Vector2 clamped_pv = MMAlgos::clampv2(pv, Vector2(), Vector2(1, 1));

				if ((pv.x != clamped_pv.x || pv.y != clamped_pv.y)) {
					continue;
				}

				//1,  " ";
				//4, "pv = clamp(0.5*(pv+floor(rand2(seed)*2.0)), vec2(0.0), vec2(1.0));";
				//16, "pv = clamp(0.25*(pv+floor(rand2(seed)*4.0)), vec2(0.0), vec2(1.0));";

				if (select_inputs == 1) {
					pv = MMAlgos::clampv2(0.5 * (pv + MMAlgos::floorv2(MMAlgos::rand2(vseed) * 2.0)), Vector2(), Vector2(1, 1));
				}

				else if (select_inputs == 2) {
					pv = MMAlgos::clampv2(0.25 * (pv + MMAlgos::floorv2(MMAlgos::rand2(vseed) * 4.0)), Vector2(), Vector2(1, 1));
				}

				//float c1 = $in.variation(pv, $variations ? vseed.x : 0.0) * mask * (1.0-$value*vseed.x);
				float c1 = float(input->get_value(pv)) * mask * (1.0 - rnd_value * vseed.x);
				c = MAX(c, c1);
				rc = rc.linear_interpolate(rc1, MMAlgos::step(c, c1));
			}
		}
	}

	return Color(rc.x, rc.y, rc.z, c);
}

MMTiler::MMTiler() {
	tile = Vector2(4, 4);
	overlap = 1;
	select_inputs = 0;
	scale = Vector2(0.5, 0.5);
	fixed_offset = 0;
	rnd_offset = 0.25;
	rnd_rotate = 45;
	rnd_scale = 0.2;
	rnd_value = 2;
	variations = false;
}

MMTiler::~MMTiler() {
}

void MMTiler::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &MMTiler::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMTiler::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_in_mask"), &MMTiler::get_in_mask);
	ClassDB::bind_method(D_METHOD("set_in_mask", "value"), &MMTiler::set_in_mask);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "in_mask", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_in_mask", "get_in_mask");

	ClassDB::bind_method(D_METHOD("get_output"), &MMTiler::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMTiler::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_instance_map"), &MMTiler::get_instance_map);
	ClassDB::bind_method(D_METHOD("set_instance_map", "value"), &MMTiler::set_instance_map);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "instance_map", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_instance_map", "get_instance_map");

	ClassDB::bind_method(D_METHOD("get_tile"), &MMTiler::get_tile);
	ClassDB::bind_method(D_METHOD("set_tile", "value"), &MMTiler::set_tile);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "tile"), "set_tile", "get_tile");

	ClassDB::bind_method(D_METHOD("get_overlap"), &MMTiler::get_overlap);
	ClassDB::bind_method(D_METHOD("set_overlap", "value"), &MMTiler::set_overlap);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "overlap"), "set_overlap", "get_overlap");

	ClassDB::bind_method(D_METHOD("get_select_inputs"), &MMTiler::get_select_inputs);
	ClassDB::bind_method(D_METHOD("set_select_inputs", "value"), &MMTiler::set_select_inputs);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "select_inputs"), "set_select_inputs", "get_select_inputs");

	ClassDB::bind_method(D_METHOD("get_scale"), &MMTiler::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "value"), &MMTiler::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");

	ClassDB::bind_method(D_METHOD("get_fixed_offset"), &MMTiler::get_fixed_offset);
	ClassDB::bind_method(D_METHOD("set_fixed_offset", "value"), &MMTiler::set_fixed_offset);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "fixed_offset"), "set_fixed_offset", "get_fixed_offset");

	ClassDB::bind_method(D_METHOD("get_rnd_offset"), &MMTiler::get_rnd_offset);
	ClassDB::bind_method(D_METHOD("set_rnd_offset", "value"), &MMTiler::set_rnd_offset);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rnd_offset"), "set_rnd_offset", "get_rnd_offset");

	ClassDB::bind_method(D_METHOD("get_rnd_rotate"), &MMTiler::get_rnd_rotate);
	ClassDB::bind_method(D_METHOD("set_rnd_rotate", "value"), &MMTiler::set_rnd_rotate);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rnd_rotate"), "set_rnd_rotate", "get_rnd_rotate");

	ClassDB::bind_method(D_METHOD("get_rnd_scale"), &MMTiler::get_rnd_scale);
	ClassDB::bind_method(D_METHOD("set_rnd_scale", "value"), &MMTiler::set_rnd_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rnd_scale"), "set_rnd_scale", "get_rnd_scale");

	ClassDB::bind_method(D_METHOD("get_rnd_value"), &MMTiler::get_rnd_value);
	ClassDB::bind_method(D_METHOD("set_rnd_value", "value"), &MMTiler::set_rnd_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rnd_value"), "set_rnd_value", "get_rnd_value");

	ClassDB::bind_method(D_METHOD("get_variations"), &MMTiler::get_variations);
	ClassDB::bind_method(D_METHOD("set_variations", "value"), &MMTiler::set_variations);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "variations"), "set_variations", "get_variations");

	ClassDB::bind_method(D_METHOD("tiler_calc", "uv", "tile", "overlap", "_seed"), &MMTiler::tiler_calc);
}
