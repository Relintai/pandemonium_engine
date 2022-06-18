
#include "voronoi.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMVoronoi::get_out_nodes() {
	return out_nodes;
}

void MMVoronoi::set_out_nodes(const Ref<MMNodeUniversalProperty> &val) {
	out_nodes = val;
}

Ref<MMNodeUniversalProperty> MMVoronoi::get_out_borders() {
	return out_borders;
}

void MMVoronoi::set_out_borders(const Ref<MMNodeUniversalProperty> &val) {
	out_borders = val;
}

Ref<MMNodeUniversalProperty> MMVoronoi::get_out_random_color() {
	return out_random_color;
}

void MMVoronoi::set_out_random_color(const Ref<MMNodeUniversalProperty> &val) {
	out_random_color = val;
}

Ref<MMNodeUniversalProperty> MMVoronoi::get_out_fill() {
	return out_fill;
}

void MMVoronoi::set_out_fill(const Ref<MMNodeUniversalProperty> &val) {
	out_fill = val;
}

Vector2 MMVoronoi::get_scale() {
	return scale;
}

void MMVoronoi::set_scale(const Vector2 &val) {
	scale = val;
	set_dirty(true);
}

Vector2 MMVoronoi::get_stretch() {
	return stretch;
}

void MMVoronoi::set_stretch(const Vector2 &val) {
	stretch = val;
	set_dirty(true);
}

float MMVoronoi::get_intensity() const {
	return intensity;
}

void MMVoronoi::set_intensity(const float val) {
	intensity = val;
	set_dirty(true);
}

float MMVoronoi::get_randomness() const {
	return randomness;
}

void MMVoronoi::set_randomness(const float val) {
	randomness = val;
	set_dirty(true);
}

void MMVoronoi::_init_properties() {
	if (!out_nodes.is_valid()) {
		out_nodes.instance();
		out_nodes->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_nodes->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_borders.is_valid()) {
		out_borders.instance();
		out_borders->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_borders->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_random_color.is_valid()) {
		out_random_color.instance();
		out_random_color->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_random_color->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_fill.is_valid()) {
		out_fill.instance();
		out_fill->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_fill->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	register_output_property(out_nodes);
	register_output_property(out_borders);
	register_output_property(out_random_color);
	register_output_property(out_fill);
}

void MMVoronoi::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(out_nodes);
	mm_graph_node->add_slot_texture_universal(out_borders);
	mm_graph_node->add_slot_texture_universal(out_random_color);
	mm_graph_node->add_slot_texture_universal(out_fill);
	//, Vector2(1, 32))//, Vector2(0, 32));
	mm_graph_node->add_slot_vector2("get_scale", "set_scale", "Scale", 0.1);
	//, Vector2(1, 32))//, Vector2(0, 32));
	mm_graph_node->add_slot_vector2("get_stretch", "set_stretch", "stretch", 0.01);
	//, Vector2(1, 32))//, Vector2(0, 32));
	mm_graph_node->add_slot_float("get_intensity", "set_intensity", "Intensity", 0.01);
	//, Vector2(1, 32))//, Vector2(0, 32));
	mm_graph_node->add_slot_float("get_randomness", "set_randomness", "Randomness", 0.01);
}

void MMVoronoi::_render(const Ref<MMMaterial> &material) {
	Ref<Image> nodes;
	Ref<Image> borders;
	Ref<Image> random_color;
	Ref<Image> fill;

	nodes.instance();
	borders.instance();
	random_color.instance();
	fill.instance();

	nodes->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	borders->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	random_color->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	fill->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);

	nodes->lock();
	borders->lock();
	random_color->lock();
	fill->lock();

	float w = material->image_size.x;
	float h = material->image_size.y;
	float pseed = Math::randf() + Math::rand();

	for (int x = 0; x < material->image_size.x; ++x) { //x in range(material.image_size.x)
		for (int y = 0; y < material->image_size.y; ++y) { //y in range(material.image_size.y)

			Vector2 uv = Vector2(x / w, y / h);
			float ps = 1.0 / float(pseed);
			//vec4 $(name_uv)_xyzw = voronoi($uv, vec2($scale_x, $scale_y), vec2($stretch_y, $stretch_x), $intensity, $randomness, $seed);
			Color voronoi = MMAlgos::voronoi(uv, scale, stretch, intensity, randomness, ps);
			//Nodes - float - A greyscale pattern based on the distance to cell centers;
			//$(name_uv)_xyzw.z;
			Color nodes_col = Color(voronoi.b, voronoi.b, voronoi.b, 1);
			//Borders - float - A greyscale pattern based on the distance to borders;
			//$(name_uv)_xyzw.w;
			Color borders_col = Color(voronoi.a, voronoi.a, voronoi.a, 1);
			//Random color - rgb - A color pattern that assigns a random color to each cell;
			//rand3(fract(floor($(name_uv)_xyzw.xy)/vec2($scale_x, $scale_y)));
			Vector3 rv3 = MMAlgos::rand3(MMAlgos::fractv2(Vector2(voronoi.r, voronoi.g) / scale));
			Color random_color_col = Color(rv3.x, rv3.y, rv3.z, 1);
			//Fill - rgba - An output that should be plugged into a Fill companion node;
			//vec4(fract(($(name_uv)_xyzw.xy-1.0)/vec2($scale_x, $scale_y)), vec2(2.0)/vec2($scale_x, $scale_y));
			Vector2 fv21 = MMAlgos::fractv2((Vector2(voronoi.r, voronoi.g) - Vector2(1, 1)) / scale);
			Vector2 fv22 = Vector2(2, 2) / scale;
			Color fill_col = Color(fv21.x, fv21.y, fv22.x, fv22.y);

			nodes->set_pixel(x, y, nodes_col);
			borders->set_pixel(x, y, borders_col);
			random_color->set_pixel(x, y, random_color_col);
			fill->set_pixel(x, y, fill_col);
		}
	}

	nodes->unlock();
	borders->unlock();
	random_color->unlock();
	fill->unlock();

	out_nodes->set_value(nodes);
	out_borders->set_value(borders);
	out_random_color->set_value(random_color);
	out_fill->set_value(fill);
}

Color MMVoronoi::_get_value_for(const Vector2 &uv, const int pseed) {
	return Color();
}

MMVoronoi::MMVoronoi() {
	scale = Vector2(4, 4);
	stretch = Vector2(1, 1);
	intensity = 1;
	randomness = 0.85;
}

MMVoronoi::~MMVoronoi() {
}

void MMVoronoi::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_out_nodes"), &MMVoronoi::get_out_nodes);
	ClassDB::bind_method(D_METHOD("set_out_nodes", "value"), &MMVoronoi::set_out_nodes);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_nodes", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_nodes", "get_out_nodes");

	ClassDB::bind_method(D_METHOD("get_out_borders"), &MMVoronoi::get_out_borders);
	ClassDB::bind_method(D_METHOD("set_out_borders", "value"), &MMVoronoi::set_out_borders);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_borders", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_borders", "get_out_borders");

	ClassDB::bind_method(D_METHOD("get_out_random_color"), &MMVoronoi::get_out_random_color);
	ClassDB::bind_method(D_METHOD("set_out_random_color", "value"), &MMVoronoi::set_out_random_color);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_random_color", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_random_color", "get_out_random_color");

	ClassDB::bind_method(D_METHOD("get_out_fill"), &MMVoronoi::get_out_fill);
	ClassDB::bind_method(D_METHOD("set_out_fill", "value"), &MMVoronoi::set_out_fill);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_fill", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_out_fill", "get_out_fill");

	ClassDB::bind_method(D_METHOD("get_scale"), &MMVoronoi::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "value"), &MMVoronoi::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");

	ClassDB::bind_method(D_METHOD("get_stretch"), &MMVoronoi::get_stretch);
	ClassDB::bind_method(D_METHOD("set_stretch", "value"), &MMVoronoi::set_stretch);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "stretch"), "set_stretch", "get_stretch");

	ClassDB::bind_method(D_METHOD("get_intensity"), &MMVoronoi::get_intensity);
	ClassDB::bind_method(D_METHOD("set_intensity", "value"), &MMVoronoi::set_intensity);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "intensity"), "set_intensity", "get_intensity");

	ClassDB::bind_method(D_METHOD("get_randomness"), &MMVoronoi::get_randomness);
	ClassDB::bind_method(D_METHOD("set_randomness", "value"), &MMVoronoi::set_randomness);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "randomness"), "set_randomness", "get_randomness");
}
