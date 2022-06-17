
#include "bricks.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMBricks::get_out_bricks_pattern() {
	return out_bricks_pattern;
}

void MMBricks::set_out_bricks_pattern(const Ref<MMNodeUniversalProperty> &val) {
	out_bricks_pattern = val;
}

Ref<MMNodeUniversalProperty> MMBricks::get_out_random_color() {
	return out_random_color;
}

void MMBricks::set_out_random_color(const Ref<MMNodeUniversalProperty> &val) {
	out_random_color = val;
}

Ref<MMNodeUniversalProperty> MMBricks::get_out_position_x() {
	return out_position_x;
}

void MMBricks::set_out_position_x(const Ref<MMNodeUniversalProperty> &val) {
	out_position_x = val;
}

Ref<MMNodeUniversalProperty> MMBricks::get_out_position_y() {
	return out_position_y;
}

void MMBricks::set_out_position_y(const Ref<MMNodeUniversalProperty> &val) {
	out_position_y = val;
}

Ref<MMNodeUniversalProperty> MMBricks::get_out_brick_uv() {
	return out_brick_uv;
}

void MMBricks::set_out_brick_uv(const Ref<MMNodeUniversalProperty> &val) {
	out_brick_uv = val;
}

Ref<MMNodeUniversalProperty> MMBricks::get_out_corner_uv() {
	return out_corner_uv;
}

void MMBricks::set_out_corner_uv(const Ref<MMNodeUniversalProperty> &val) {
	out_corner_uv = val;
}

Ref<MMNodeUniversalProperty> MMBricks::get_out_direction() {
	return out_direction;
}

void MMBricks::set_out_direction(const Ref<MMNodeUniversalProperty> &val) {
	out_direction = val;
}

int MMBricks::get_type() const {
	return type;
}

void MMBricks::set_type(const int val) {
	type = val;
	set_dirty(true);
}

int MMBricks::get_repeat() const {
	return repeat;
}

void MMBricks::set_repeat(const int val) {
	repeat = val;
	set_dirty(true);
}

Vector2 MMBricks::get_col_row() {
	return col_row;
}

void MMBricks::set_col_row(const Vector2 &val) {
	col_row = val;
	set_dirty(true);
}

float MMBricks::get_offset() const {
	return offset;
}

void MMBricks::set_offset(const float val) {
	offset = val;
	set_dirty(true);
}

Ref<MMNodeUniversalProperty> MMBricks::get_mortar() {
	return mortar;
}

void MMBricks::set_mortar(const Ref<MMNodeUniversalProperty> &val) {
	mortar = val;
}

Ref<MMNodeUniversalProperty> MMBricks::get_bevel() {
	return bevel;
}

void MMBricks::set_bevel(const Ref<MMNodeUniversalProperty> &val) {
	bevel = val;
}

Ref<MMNodeUniversalProperty> MMBricks::get_roundness() {
	return roundness;
}

void MMBricks::set_roundness(const Ref<MMNodeUniversalProperty> &val) {
	roundness = val;
}

float MMBricks::get_corner() const {
	return corner;
}

void MMBricks::set_corner(const float val) {
	corner = val;
	set_dirty(true);
}

void MMBricks::_init_properties() {
	if (!out_bricks_pattern.is_valid()) {
		out_bricks_pattern.instance();
		out_bricks_pattern->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_bricks_pattern->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_random_color.is_valid()) {
		out_random_color.instance();
		out_random_color->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_random_color->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_position_x.is_valid()) {
		out_position_x.instance();
		out_position_x->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_position_x->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_position_y.is_valid()) {
		out_position_y.instance();
		out_position_y->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_position_y->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_brick_uv.is_valid()) {
		out_brick_uv.instance();
		out_brick_uv->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_brick_uv->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_corner_uv.is_valid()) {
		out_corner_uv.instance();
		out_corner_uv->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_corner_uv->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_direction.is_valid()) {
		out_direction.instance();
		out_direction->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_direction->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!mortar.is_valid()) {
		mortar.instance();
		mortar->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		mortar->set_default_value(0.1);
	}

	mortar->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	mortar->set_slot_name("Mortar");
	mortar->set_value_step(0.01);
	mortar->set_value_range(Vector2(0, 0.5));

	if (!bevel.is_valid()) {
		bevel.instance();
		bevel->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		bevel->set_default_value(0.1);
	}

	bevel->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	bevel->set_slot_name("Bevel");
	bevel->set_value_step(0.01);
	bevel->set_value_range(Vector2(0, 0.5));

	if (!roundness.is_valid()) {
		roundness.instance();
		roundness->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		roundness->set_default_value(0.1);
	}

	roundness->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	roundness->set_slot_name("Roundness");
	roundness->set_value_step(0.01);
	roundness->set_value_range(Vector2(0, 0.5));

	register_output_property(out_bricks_pattern);
	register_output_property(out_random_color);
	register_output_property(out_position_x);
	register_output_property(out_position_y);
	register_output_property(out_brick_uv);
	register_output_property(out_corner_uv);
	register_output_property(out_direction);
	register_input_property(mortar);
	register_input_property(bevel);
	register_input_property(roundness);
}

void MMBricks::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(out_bricks_pattern);
	mm_graph_node->add_slot_texture_universal(out_random_color);
	mm_graph_node->add_slot_texture_universal(out_position_x);
	mm_graph_node->add_slot_texture_universal(out_position_y);
	mm_graph_node->add_slot_texture_universal(out_brick_uv);
	mm_graph_node->add_slot_texture_universal(out_corner_uv);
	mm_graph_node->add_slot_texture_universal(out_direction);

	Array arr;
	arr.push_back("Running Bond");
	arr.push_back("Running Bond (2)");
	arr.push_back("HerringBone");
	arr.push_back("Basket Weave");
	arr.push_back("Spanish Bond");

	mm_graph_node->add_slot_enum("get_type", "set_type", "Type", arr);
	mm_graph_node->add_slot_int("get_repeat", "set_repeat", "Repeat");
	//, Vector2(1, 32))//, Vector2(0, 32));
	mm_graph_node->add_slot_vector2("get_col_row", "set_col_row", "Col, Row");
	mm_graph_node->add_slot_float("get_offset", "set_offset", "Offset");
	mm_graph_node->add_slot_float_universal(mortar);
	mm_graph_node->add_slot_float_universal(bevel);
	mm_graph_node->add_slot_float_universal(roundness);
	mm_graph_node->add_slot_float("get_corner", "set_corner", "Corner");
}

void MMBricks::_render(const Ref<MMMaterial> &material) {
	Ref<Image> bricks_pattern;
	Ref<Image> random_color;
	Ref<Image> position_x;
	Ref<Image> position_y;
	Ref<Image> brick_uv;
	Ref<Image> corner_uv;
	Ref<Image> direction;

	bricks_pattern.instance();
	random_color.instance();
	position_x.instance();
	position_y.instance();
	brick_uv.instance();
	corner_uv.instance();
	direction.instance();

	bricks_pattern->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	random_color->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	position_x->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	position_y->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	brick_uv->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	corner_uv->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	direction->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);

	bricks_pattern->lock();
	random_color->lock();
	position_x->lock();
	position_y->lock();
	brick_uv->lock();
	corner_uv->lock();
	direction->lock();

	float w = material->image_size.x;
	float h = material->image_size.y;
	float pseed = Math::randf() + Math::rand();

	for (int x = 0; x < material->image_size.x; ++x) { //x in range(material.image_size.x)
		for (int y = 0; y < material->image_size.y; ++y) { //y in range(material.image_size.y)
			Vector2 uv = Vector2(x / w, y / h);
			//vec4 $(name_uv)_rect = bricks_$pattern($uv, vec2($columns, $rows), $repeat, $row_offset);
			Color brick_rect = Color();
			//"Running Bond,Running Bond (2),HerringBone,Basket Weave,Spanish Bond";

			if (type == 0) {
				brick_rect = MMAlgos::bricks_rb(uv, col_row, repeat, offset);
			} else if (type == 1) {
				brick_rect = MMAlgos::bricks_rb2(uv, col_row, repeat, offset);
			} else if (type == 2) {
				brick_rect = MMAlgos::bricks_hb(uv, col_row, repeat, offset);
			} else if (type == 3) {
				brick_rect = MMAlgos::bricks_bw(uv, col_row, repeat, offset);
			} else if (type == 4) {
				brick_rect = MMAlgos::bricks_sb(uv, col_row, repeat, offset);
			}

			//vec4 $(name_uv) = brick($uv, $(name_uv)_rect.xy, $(name_uv)_rect.zw, $mortar*$mortar_map($uv), $round*$round_map($uv), max(0.001, $bevel*$bevel_map($uv)));
			Color brick = MMAlgos::brick(uv, Vector2(brick_rect.r, brick_rect.g), Vector2(brick_rect.b, brick_rect.a), mortar->get_value(uv), roundness->get_value(uv), MAX(0.001, float(bevel->get_value(uv))));
			//MMBricks pattern (float) - A greyscale image that shows the bricks pattern;
			//$(name_uv).x;
			Color bricks_pattern_col = Color(brick.r, brick.r, brick.r, 1);
			//Random color (rgb) - A random color for each brick;
			//brick_random_color($(name_uv)_rect.xy, $(name_uv)_rect.zw, float($seed));
			Vector3 brc = MMAlgos::brick_random_color(Vector2(brick_rect.r, brick_rect.g), Vector2(brick_rect.b, brick_rect.a), 1 / float(pseed));
			Color random_color_col = Color(brc.x, brc.y, brc.z, 1);
			//Position.x (float) - The position of each brick along the X axis",;
			//$(name_uv).y;
			Color position_x_col = Color(brick.g, brick.g, brick.g, 1);
			//Position.y (float) - The position of each brick along the Y axis;
			//$(name_uv).z;
			Color position_y_col = Color(brick.b, brick.b, brick.b, 1);
			//Brick UV (rgb) - An UV map output for each brick, to be connected to the Map input of a CustomUV node;
			//brick_uv($uv, $(name_uv)_rect.xy, $(name_uv)_rect.zw, float($seed));
			Vector3 buv = MMAlgos::brick_uv(uv, Vector2(brick_rect.r, brick_rect.g), Vector2(brick_rect.b, brick_rect.a), pseed);
			Color brick_uv_col = Color(buv.x, buv.y, buv.z, 1);
			//Corner UV (rgb) - An UV map output for each brick corner, to be connected to the Map input of a CustomUV node;
			//brick_corner_uv($uv, $(name_uv)_rect.xy, $(name_uv)_rect.zw, $mortar*$mortar_map($uv), $corner, float($seed));
			Vector3 bcuv = MMAlgos::brick_corner_uv(uv, Vector2(brick_rect.r, brick_rect.g), Vector2(brick_rect.b, brick_rect.a), mortar->get_value(uv), corner, pseed);
			Color corner_uv_col = Color(bcuv.x, bcuv.y, bcuv.z, 1);
			//Direction (float) - The direction of each brick (white: horizontal, black: vertical);
			//0.5*(sign($(name_uv)_rect.z-$(name_uv)_rect.x-$(name_uv)_rect.w+$(name_uv)_rect.y)+1.0);
			float d = 0.5 * (SGN(brick_rect.b - brick_rect.r - brick_rect.a + brick_rect.g) + 1.0);
			Color direction_col = Color(d, d, d, 1);

			bricks_pattern->set_pixel(x, y, bricks_pattern_col);
			random_color->set_pixel(x, y, random_color_col);
			position_x->set_pixel(x, y, position_x_col);
			position_y->set_pixel(x, y, position_y_col);
			brick_uv->set_pixel(x, y, brick_uv_col);
			corner_uv->set_pixel(x, y, corner_uv_col);
			direction->set_pixel(x, y, direction_col);
		}
	}

	bricks_pattern->unlock();
	random_color->unlock();
	position_x->unlock();
	position_y->unlock();
	brick_uv->unlock();
	corner_uv->unlock();
	direction->unlock();

	out_bricks_pattern->set_value(bricks_pattern);
	out_random_color->set_value(random_color);
	out_position_x->set_value(position_x);
	out_position_y->set_value(position_y);
	out_brick_uv->set_value(brick_uv);
	out_corner_uv->set_value(corner_uv);
	out_direction->set_value(direction);
}

Color MMBricks::_get_value_for(const Vector2 &uv, const int pseed) {
	return Color();
}

MMBricks::MMBricks() {
	type = 0;
	repeat = 1;
	col_row = Vector2(4, 4);
	offset = 0.5;
	corner = 0.3;
}

MMBricks::~MMBricks() {
}

void MMBricks::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_out_bricks_pattern"), &MMBricks::get_out_bricks_pattern);
	ClassDB::bind_method(D_METHOD("set_out_bricks_pattern", "value"), &MMBricks::set_out_bricks_pattern);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_bricks_pattern", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_bricks_pattern", "get_out_bricks_pattern");

	ClassDB::bind_method(D_METHOD("get_out_random_color"), &MMBricks::get_out_random_color);
	ClassDB::bind_method(D_METHOD("set_out_random_color", "value"), &MMBricks::set_out_random_color);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_random_color", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_random_color", "get_out_random_color");

	ClassDB::bind_method(D_METHOD("get_out_position_x"), &MMBricks::get_out_position_x);
	ClassDB::bind_method(D_METHOD("set_out_position_x", "value"), &MMBricks::set_out_position_x);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_position_x", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_position_x", "get_out_position_x");

	ClassDB::bind_method(D_METHOD("get_out_position_y"), &MMBricks::get_out_position_y);
	ClassDB::bind_method(D_METHOD("set_out_position_y", "value"), &MMBricks::set_out_position_y);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_position_y", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_position_y", "get_out_position_y");

	ClassDB::bind_method(D_METHOD("get_out_brick_uv"), &MMBricks::get_out_brick_uv);
	ClassDB::bind_method(D_METHOD("set_out_brick_uv", "value"), &MMBricks::set_out_brick_uv);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_brick_uv", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_brick_uv", "get_out_brick_uv");

	ClassDB::bind_method(D_METHOD("get_out_corner_uv"), &MMBricks::get_out_corner_uv);
	ClassDB::bind_method(D_METHOD("set_out_corner_uv", "value"), &MMBricks::set_out_corner_uv);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_corner_uv", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_corner_uv", "get_out_corner_uv");

	ClassDB::bind_method(D_METHOD("get_out_direction"), &MMBricks::get_out_direction);
	ClassDB::bind_method(D_METHOD("set_out_direction", "value"), &MMBricks::set_out_direction);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_direction", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_direction", "get_out_direction");

	ClassDB::bind_method(D_METHOD("get_type"), &MMBricks::get_type);
	ClassDB::bind_method(D_METHOD("set_type", "value"), &MMBricks::set_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");

	ClassDB::bind_method(D_METHOD("get_repeat"), &MMBricks::get_repeat);
	ClassDB::bind_method(D_METHOD("set_repeat", "value"), &MMBricks::set_repeat);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "repeat"), "set_repeat", "get_repeat");

	ClassDB::bind_method(D_METHOD("get_col_row"), &MMBricks::get_col_row);
	ClassDB::bind_method(D_METHOD("set_col_row", "value"), &MMBricks::set_col_row);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "col_row"), "set_col_row", "get_col_row");

	ClassDB::bind_method(D_METHOD("get_offset"), &MMBricks::get_offset);
	ClassDB::bind_method(D_METHOD("set_offset", "value"), &MMBricks::set_offset);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "offset"), "set_offset", "get_offset");

	ClassDB::bind_method(D_METHOD("get_mortar"), &MMBricks::get_mortar);
	ClassDB::bind_method(D_METHOD("set_mortar", "value"), &MMBricks::set_mortar);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mortar", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_mortar", "get_mortar");

	ClassDB::bind_method(D_METHOD("get_bevel"), &MMBricks::get_bevel);
	ClassDB::bind_method(D_METHOD("set_bevel", "value"), &MMBricks::set_bevel);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bevel", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_bevel", "get_bevel");

	ClassDB::bind_method(D_METHOD("get_roundness"), &MMBricks::get_roundness);
	ClassDB::bind_method(D_METHOD("set_roundness", "value"), &MMBricks::set_roundness);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "roundness", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_roundness", "get_roundness");

	ClassDB::bind_method(D_METHOD("get_corner"), &MMBricks::get_corner);
	ClassDB::bind_method(D_METHOD("set_corner", "value"), &MMBricks::set_corner);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "corner"), "set_corner", "get_corner");
}
