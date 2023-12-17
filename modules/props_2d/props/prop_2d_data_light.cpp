

#include "prop_2d_data_light.h"

#include "prop_2d_data.h"

#include "scene/2d/light_2d.h"

Color Prop2DDataLight::get_light_color() const {
	return _light_color;
}
void Prop2DDataLight::set_light_color(const Color value) {
	_light_color = value;
}

int Prop2DDataLight::get_light_size_x() const {
	return _light_size_x;
}
void Prop2DDataLight::set_light_size_x(const int value) {
	_light_size_x = value;
}

int Prop2DDataLight::get_light_size_y() const {
	return _light_size_y;
}
void Prop2DDataLight::set_light_size_y(const int value) {
	_light_size_y = value;
}

float Prop2DDataLight::get_texture_scale() const {
	return _texture_scale;
}
void Prop2DDataLight::set_texture_scale(const float value) {
	_texture_scale = value;
}

float Prop2DDataLight::get_energy() const {
	return _energy;
}
void Prop2DDataLight::set_energy(const float value) {
	_energy = value;
}

Ref<Texture> Prop2DDataLight::get_texture() const {
	return _texture;
}
void Prop2DDataLight::set_texture(const Ref<Texture> value) {
	_texture = value;
}

bool Prop2DDataLight::_processor_handles(Node *node) {
	Light2D *i = Object::cast_to<Light2D>(node);

	return i;
}

void Prop2DDataLight::_processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry) {
	Light2D *i = Object::cast_to<Light2D>(node);

	ERR_FAIL_COND(!i);

	Ref<Prop2DDataLight> l;

	if (entry.is_valid()) {
		l = entry;
	} else {
		l.instance();
	}

	l->set_light_color(i->get_color());

	Ref<Texture> tex = i->get_texture();

	int w = 0;
	int h = 0;

	if (tex.is_valid()) {
		w = tex->get_width();
		h = tex->get_height();
	}

	l->set_light_size_x(w);
	l->set_light_size_y(h);

	l->set_texture_scale(i->get_texture_scale());
	l->set_energy(i->get_energy());
	l->set_texture(tex);

	Prop2DDataEntry::_processor_process(prop_data, node, transform, l);
}

Node *Prop2DDataLight::_processor_get_node_for(const Transform2D &transform, Node *node) {
	Light2D *i = nullptr;

	if (!node) {
		i = memnew(Light2D);
	} else {
		i = Object::cast_to<Light2D>(node);
	}

	i->set_color(get_light_color());

	i->set_texture_scale(get_texture_scale());
	i->set_energy(get_energy());
	i->set_texture(get_texture());

	return Prop2DDataEntry::_processor_get_node_for(transform, i);
}

Prop2DDataLight::Prop2DDataLight() {
	_light_size_x = 0;
	_light_size_y = 0;
	_texture_scale = 0;
	_energy = 0;
}
Prop2DDataLight::~Prop2DDataLight() {
}

void Prop2DDataLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_light_color"), &Prop2DDataLight::get_light_color);
	ClassDB::bind_method(D_METHOD("set_light_color", "value"), &Prop2DDataLight::set_light_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_light_color", "get_light_color");

	ClassDB::bind_method(D_METHOD("get_light_size_x"), &Prop2DDataLight::get_light_size_x);
	ClassDB::bind_method(D_METHOD("set_light_size_x", "value"), &Prop2DDataLight::set_light_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_size_x"), "set_light_size_x", "get_light_size_x");

	ClassDB::bind_method(D_METHOD("get_light_size_y"), &Prop2DDataLight::get_light_size_y);
	ClassDB::bind_method(D_METHOD("set_light_size_y", "value"), &Prop2DDataLight::set_light_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_size_y"), "set_light_size_y", "get_light_size_y");

	ClassDB::bind_method(D_METHOD("get_texture_scale"), &Prop2DDataLight::get_texture_scale);
	ClassDB::bind_method(D_METHOD("set_texture_scale", "value"), &Prop2DDataLight::set_texture_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "texture_scale"), "set_texture_scale", "get_texture_scale");

	ClassDB::bind_method(D_METHOD("get_energy"), &Prop2DDataLight::get_energy);
	ClassDB::bind_method(D_METHOD("set_energy", "value"), &Prop2DDataLight::set_energy);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "energy"), "set_energy", "get_energy");

	ClassDB::bind_method(D_METHOD("get_texture"), &Prop2DDataLight::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "value"), &Prop2DDataLight::set_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
}
