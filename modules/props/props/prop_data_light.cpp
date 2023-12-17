

#include "prop_data_light.h"

#include "prop_data.h"

#include "scene/3d/light.h"

Color PropDataLight::get_light_color() const {
	return _light_color;
}
void PropDataLight::set_light_color(const Color value) {
	_light_color = value;
}

int PropDataLight::get_light_size() const {
	return _light_size;
}
void PropDataLight::set_light_size(const int value) {
	_light_size = value;
}

bool PropDataLight::_processor_handles(Node *node) {
	OmniLight *i = Object::cast_to<OmniLight>(node);

	return i;
}

void PropDataLight::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	OmniLight *i = Object::cast_to<OmniLight>(node);

	ERR_FAIL_COND(!i);

	if (i->is_editor_only()) {
		return;
	}

	Ref<PropDataLight> l;
	l.instance();
	l->set_light_color(i->get_color());
	l->set_light_size(i->get_param(Light::PARAM_RANGE));
	l->set_transform(transform * i->get_transform());
	prop_data->add_prop(l);
}

Node *PropDataLight::_processor_get_node_for(const Transform &transform) {
	OmniLight *i = memnew(OmniLight);

	i->set_color(get_light_color());
	i->set_param(Light::PARAM_RANGE, get_light_size());
	i->set_transform(get_transform());

	return i;
}

PropDataLight::PropDataLight() {
	_light_size = 0;
}
PropDataLight::~PropDataLight() {
}

void PropDataLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_light_color"), &PropDataLight::get_light_color);
	ClassDB::bind_method(D_METHOD("set_light_color", "value"), &PropDataLight::set_light_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_light_color", "get_light_color");

	ClassDB::bind_method(D_METHOD("get_light_size"), &PropDataLight::get_light_size);
	ClassDB::bind_method(D_METHOD("set_light_size", "value"), &PropDataLight::set_light_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_size"), "set_light_size", "get_light_size");
}
