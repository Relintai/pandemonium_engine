
#include "sdf3d_render.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dRender::get_input() {
	return input;
}

void MMSdf3dRender::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMSdf3dRender::get_out_height_map() {
	return out_height_map;
}

void MMSdf3dRender::set_out_height_map(const Ref<MMNodeUniversalProperty> &val) {
	out_height_map = val;
}

Ref<MMNodeUniversalProperty> MMSdf3dRender::get_out_normal_map() {
	return out_normal_map;
}

void MMSdf3dRender::set_out_normal_map(const Ref<MMNodeUniversalProperty> &val) {
	out_normal_map = val;
}

Ref<MMNodeUniversalProperty> MMSdf3dRender::get_out_color_map() {
	return out_color_map;
}

void MMSdf3dRender::set_out_color_map(const Ref<MMNodeUniversalProperty> &val) {
	out_color_map = val;
}

void MMSdf3dRender::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_VECTOR2);
	}

	//for some reason this doesn't work, todo check;
	//	input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name("Input");

	if (!out_height_map.is_valid()) {
		out_height_map.instance();
		out_height_map->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_height_map->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_normal_map.is_valid()) {
		out_normal_map.instance();
		out_normal_map->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_normal_map->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!out_color_map.is_valid()) {
		out_color_map.instance();
		out_color_map->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	out_color_map->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	register_output_property(out_height_map);
	register_output_property(out_normal_map);
	register_output_property(out_color_map);
	register_input_property(input);
}

void MMSdf3dRender::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(out_height_map);
	mm_graph_node->add_slot_texture_universal(out_normal_map);
	mm_graph_node->add_slot_texture_universal(out_color_map);
}

void MMSdf3dRender::_render(const Ref<MMMaterial> &material) {
	Ref<Image> height_map;
	Ref<Image> normal_map;
	Ref<Image> color_map;

	height_map.instance();
	normal_map.instance();
	color_map.instance();

	height_map->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	normal_map->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	color_map->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);

	height_map->lock();
	normal_map->lock();
	color_map->lock();

	float w = material->image_size.x;
	float h = material->image_size.y;
	//float pseed = Math::randf() + Math::rand();

	for (int x = 0; x < material->image_size.x; ++x) { //x in range(material.image_size.x)
		for (int y = 0; y < material->image_size.y; ++y) { //y in range(material.image_size.y)
			Vector2 uv = Vector2(x / w, y / h);
			Vector2 raymarch = sdf3d_raymarch(uv);
			//HeightMap - float - The generated height map;
			//1.0-$(name_uv)_d.x;
			float hmf = 1.0 - raymarch.x;
			Color height_map_col = Color(hmf, hmf, hmf, 1);
			//NormalMap - rgb - The generated normal map;
			//vec3(0.5) + 0.5* normal_$name(vec3($uv-vec2(0.5), 1.0-$(name_uv)_d.x));
			Vector2 nuv = uv - Vector2(0.5, 0.5);
			Vector3 n = sdf3d_normal(Vector3(nuv.x, nuv.y, 1.0 - raymarch.x));
			Vector3 nn = Vector3(0.5, 0.5, 0.5) + 0.5 * n;
			Color normal_map_col = Color(nn.x, nn.y, nn.z, 1);
			//ColorMap - float - The generated color index map;
			//$(name_uv)_d.y;
			Color color_map_col = Color(raymarch.y, raymarch.y, raymarch.y, 1);
			height_map->set_pixel(x, y, height_map_col);
			normal_map->set_pixel(x, y, normal_map_col);
			color_map->set_pixel(x, y, color_map_col);
		}
	}

	height_map->unlock();
	normal_map->unlock();
	color_map->unlock();

	out_height_map->set_value(height_map);
	out_normal_map->set_value(normal_map);
	out_color_map->set_value(color_map);
}

Color MMSdf3dRender::_get_value_for(const Vector2 &uv, const int pseed) {
	return Color();
}

//vec2 raymarch_$name(vec2 uv) {;
//	vec3 ro = vec3(uv-vec2(0.5), 1.0);
//	vec3 rd = vec3(0.0, 0.0, -1.0);
//	float dO = 0.0;
//	float c = 0.0;
//;
//	for (int i=0; i < 100; i++) {;
//		vec3 p = ro + rd*dO;
//		vec2 dS = $sdf(p);
//		dO += dS.x;
//;
//		if (dO >= 1.0) {;
//			break;
//		} else if (dS.x < 0.0001) {;
//			c = dS.y;
//			break;
//		};
//	};
//;
//	return vec2(dO, c);
//};

Vector2 MMSdf3dRender::sdf3d_raymarch(const Vector2 &uv) {
	Vector3 ro = Vector3(uv.x - 0.5, uv.y - 0.5, 1.0);
	Vector3 rd = Vector3(0.0, 0.0, -1.0);
	float dO = 0.0;
	float c = 0.0;

	for (int i = 0; i < 100; ++i) { //i in range(100)
		Vector3 p = ro + rd * dO;
		Vector2 dS = input->get_value_sdf3d(p);
		dO += dS.x;

		if ((dO >= 1.0)) {
			break;
		}

		else if ((dS.x < 0.0001)) {
			c = dS.y;
			break;
		}
	}

	return Vector2(dO, c);
}

//vec3 normal_$name(vec3 p) {;
//	if (p.z <= 0.0) {;
//		return vec3(0.0, 0.0, 1.0);
//	};
//;
//	float d = $sdf(p).x;
//	float e = .001;
//	vec3 n = d - vec3(;
//		$sdf(p-vec3(e, 0.0, 0.0)).x,;
//		$sdf(p-vec3(0.0, e, 0.0)).x,;
//		$sdf(p-vec3(0.0, 0.0, e)).x);
//;
//	return vec3(-1.0, -1.0, -1.0)*normalize(n);
//};

Vector3 MMSdf3dRender::sdf3d_normal(const Vector3 &p) {
	if ((p.z <= 0.0)) {
		return Vector3(0.0, 0.0, 1.0);
	}

	float d = input->get_value_sdf3d(p).x;
	float e = .001;
	Vector3 n = Vector3(d - input->get_value_sdf3d(p - Vector3(e, 0.0, 0.0)).x, d - input->get_value_sdf3d(p - Vector3(0.0, e, 0.0)).x, d - input->get_value_sdf3d(p - Vector3(0.0, 0.0, e)).x);
	return Vector3(-1.0, -1.0, -1.0) * n.normalized();
}

MMSdf3dRender::MMSdf3dRender() {
}

MMSdf3dRender::~MMSdf3dRender() {
}

void MMSdf3dRender::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &MMSdf3dRender::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMSdf3dRender::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_out_height_map"), &MMSdf3dRender::get_out_height_map);
	ClassDB::bind_method(D_METHOD("set_out_height_map", "value"), &MMSdf3dRender::set_out_height_map);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_height_map", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_out_height_map", "get_out_height_map");

	ClassDB::bind_method(D_METHOD("get_out_normal_map"), &MMSdf3dRender::get_out_normal_map);
	ClassDB::bind_method(D_METHOD("set_out_normal_map", "value"), &MMSdf3dRender::set_out_normal_map);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_normal_map", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_out_normal_map", "get_out_normal_map");

	ClassDB::bind_method(D_METHOD("get_out_color_map"), &MMSdf3dRender::get_out_color_map);
	ClassDB::bind_method(D_METHOD("set_out_color_map", "value"), &MMSdf3dRender::set_out_color_map);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "out_color_map", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_out_color_map", "get_out_color_map");

	ClassDB::bind_method(D_METHOD("sdf3d_raymarch", "uv"), &MMSdf3dRender::sdf3d_raymarch);
	ClassDB::bind_method(D_METHOD("sdf3d_normal", "p"), &MMSdf3dRender::sdf3d_normal);
}
