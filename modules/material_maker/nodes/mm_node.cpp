
#include "mm_node.h"

#include "core/object.h"
#include "mm_material.h"
#include "mm_node_universal_property.h"
#include "scene/main/node.h"

Vector2 MMNode::get_graph_position() {
	return graph_position;
}

void MMNode::set_graph_position(const Vector2 &pos) {
	graph_position = pos;
	emit_changed();
}

Vector<Variant> MMNode::get_input_properties() {
	Vector<Variant> r;
	for (int i = 0; i < input_properties.size(); i++) {
		r.push_back(input_properties[i].get_ref_ptr());
	}
	return r;
}

void MMNode::set_input_properties(const Vector<Variant> &val) {
	input_properties.clear();
	for (int i = 0; i < val.size(); i++) {
		Ref<MMNode> e = Ref<MMNode>(val[i]);
		input_properties.push_back(e);
	}
}

Vector<Variant> MMNode::get_output_properties() {
	Vector<Variant> r;
	for (int i = 0; i < output_properties.size(); i++) {
		r.push_back(output_properties[i].get_ref_ptr());
	}
	return r;
}

void MMNode::set_output_properties(const Vector<Variant> &val) {
	output_properties.clear();
	for (int i = 0; i < val.size(); i++) {
		Ref<MMNode> e = Ref<MMNode>(val[i]);
		output_properties.push_back(e);
	}
}

bool MMNode::get_properties_initialized() const {
	return properties_initialized;
}

void MMNode::set_properties_initialized(const bool val) {
	properties_initialized = val;
}

bool MMNode::get_dirty() const {
	return dirty;
}

void MMNode::set_dirty(const bool val) {
	bool changed = val != dirty;
	dirty = val;

	if (changed) {
		emit_changed();
	}
}

bool MMNode::render(const Ref<MMMaterial> &material) {
	if (!dirty) {
		return false;
	}

	for (int i = 0; i < input_properties.size(); ++i) {
		Ref<MMNodeUniversalProperty> p = input_properties[i];

		if (!p.is_valid()) {
			continue;
		}

		if (p->get_input_property().is_valid() && p->get_input_property()->get_owner()->get_dirty()) {
			return false;
		}
	}

	call("_render", material);

	dirty = false;
	return true;
}

void MMNode::_render(const Ref<MMMaterial> &material) {
}

Ref<Image> MMNode::render_image(const Ref<MMMaterial> &material) {
	return call("_render_image", material);
}

Ref<Image> MMNode::_render_image(const Ref<MMMaterial> &material) {
	Ref<Image> image;
	image.instance();

	image->create(material->image_size.x, material->image_size.y, false, Image::FORMAT_RGBA8);
	image->lock();

	float w = image->get_width();
	float h = image->get_height();
	float pseed = Math::randf() + Math::rand();

	for (int x = 0; x < image->get_width(); ++x) { //x in range(image.get_width())

		for (int y = 0; y < image->get_height(); ++y) { //y in range(image.get_height())
			Vector2 v = Vector2(x / w, y / h);
			Color col = get_value_for(v, pseed);
			image->set_pixel(x, y, col);
		}
	}

	image->unlock();
	return image;
}

Color MMNode::get_value_for(const Vector2 &uv, const int pseed) {
	return call("_get_value_for", uv, pseed);
}

Color MMNode::_get_value_for(const Vector2 &uv, const int pseed) {
	return Color();
}

void MMNode::init_properties() {
	if (!properties_initialized) {
		properties_initialized = true;
		call("_init_properties");
	}
}

void MMNode::_init_properties() {
}

void MMNode::register_methods(Node *mm_graph_node) {
	init_properties();
	call("_register_methods", mm_graph_node);
}

void MMNode::_register_methods_bind(Node *mm_graph_node) {
	_register_methods(mm_graph_node);
}

void MMNode::_register_methods(Node *mm_graph_node) {
}

void MMNode::register_input_property(const Ref<MMNodeUniversalProperty> &p_prop) {
	Ref<MMNodeUniversalProperty> prop = p_prop;

	prop->set_owner(this);

	if (!prop->is_connected("changed", this, "on_input_property_changed")) {
		prop->connect("changed", this, "on_input_property_changed");
	}

	input_properties.push_back(prop);
}

void MMNode::unregister_input_property(const Ref<MMNodeUniversalProperty> &p_prop) {
	Ref<MMNodeUniversalProperty> prop = p_prop;

	if (prop->get_owner() == this) {
		prop->set_owner(nullptr);
	}

	if (prop->is_connected("changed", this, "on_input_property_changed")) {
		prop->disconnect("changed", this, "on_input_property_changed");
	}

	input_properties.erase(prop);
}

void MMNode::register_output_property(const Ref<MMNodeUniversalProperty> &p_prop) {
	Ref<MMNodeUniversalProperty> prop = p_prop;

	prop->set_owner(this);
	output_properties.push_back(prop);
}

void MMNode::unregister_output_property(const Ref<MMNodeUniversalProperty> &p_prop) {
	Ref<MMNodeUniversalProperty> prop = p_prop;

	if (prop->get_owner() == this) {
		prop->set_owner(nullptr);
	}

	output_properties.erase(prop);
}

void MMNode::on_input_property_changed() {
	set_dirty(true);
	emit_changed();
}

Variant MMNode::get_property_value(const Vector2 &uv) {
	return call("_get_property_value", uv);
}

Variant MMNode::_get_property_value(const Vector2 &uv) {
	return Variant();
}

Vector2 MMNode::get_property_value_sdf3d(const Vector3 &uv3) {
	return call("_get_property_value_sdf3d", uv3);
}

Vector2 MMNode::_get_property_value_sdf3d(const Vector3 &uv3) {
	return Vector2();
}

// Add it to the MMAlgos bing class instead.;
// Not a perfect fit, but a better fit.;
//func editor_register_node_class(category : String, cls : String);
// -> c++ method, adds node to the editor gui (add button);
// in gdscript a plugin should instance an MMNode and call it to populate the add menu;
// with MMNodes;
// in c++ it should have a static counterpart.;
// register_types should populate c++ types with this;
//func editor_unregister_node_class(category : String, cls : String);
//func editor_register_node_script(category : String, script_path : String);
// same as the above, but for scripts;
//func editor_unregister_node_script(category : String, cls : String);

MMNode::MMNode() {
	properties_initialized = false;
	dirty = true;
}

MMNode::~MMNode() {
}

void MMNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_graph_position"), &MMNode::get_graph_position);
	ClassDB::bind_method(D_METHOD("set_graph_position", "value"), &MMNode::set_graph_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "graph_position"), "set_graph_position", "get_graph_position");

	ClassDB::bind_method(D_METHOD("get_input_properties"), &MMNode::get_input_properties);
	ClassDB::bind_method(D_METHOD("set_input_properties", "value"), &MMNode::set_input_properties);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "input_properties", PROPERTY_HINT_NONE, "17/17:MMNodeUniversalProperty", PROPERTY_USAGE_DEFAULT, "MMNodeUniversalProperty"), "set_input_properties", "get_input_properties");

	ClassDB::bind_method(D_METHOD("get_output_properties"), &MMNode::get_output_properties);
	ClassDB::bind_method(D_METHOD("set_output_properties", "value"), &MMNode::set_output_properties);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "output_properties", PROPERTY_HINT_NONE, "17/17:MMNodeUniversalProperty", PROPERTY_USAGE_DEFAULT, "MMNodeUniversalProperty"), "set_output_properties", "get_output_properties");

	ClassDB::bind_method(D_METHOD("get_properties_initialized"), &MMNode::get_properties_initialized);
	ClassDB::bind_method(D_METHOD("set_properties_initialized", "value"), &MMNode::set_properties_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "properties_initialized", PROPERTY_HINT_NONE, "", 0), "set_properties_initialized", "get_properties_initialized");

	ClassDB::bind_method(D_METHOD("get_dirty"), &MMNode::get_dirty);
	ClassDB::bind_method(D_METHOD("set_dirty", "value"), &MMNode::set_dirty);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "dirty", PROPERTY_HINT_NONE, "", 0), "set_dirty", "get_dirty");

	BIND_VMETHOD(MethodInfo("_render", PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "MMMaterial")));
	ClassDB::bind_method(D_METHOD("render", "material"), &MMNode::render);
	ClassDB::bind_method(D_METHOD("_render", "material"), &MMNode::_render);

	BIND_VMETHOD(MethodInfo("_render_image", PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "MMMaterial")));
	ClassDB::bind_method(D_METHOD("render_image", "material"), &MMNode::render_image);
	ClassDB::bind_method(D_METHOD("_render_image", "material"), &MMNode::_render_image);

	BIND_VMETHOD(MethodInfo("_get_value_for", PropertyInfo(Variant::VECTOR2, "uv"), PropertyInfo(Variant::INT, "pseed")));
	ClassDB::bind_method(D_METHOD("get_value_for", "uv", "pseed"), &MMNode::get_value_for);
	ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &MMNode::_get_value_for);

	BIND_VMETHOD(MethodInfo("_init_properties"));
	ClassDB::bind_method(D_METHOD("init_properties"), &MMNode::init_properties);
	ClassDB::bind_method(D_METHOD("_init_properties"), &MMNode::_init_properties);

	BIND_VMETHOD(MethodInfo("_register_methods", PropertyInfo(Variant::OBJECT, "mm_graph_node")));
	ClassDB::bind_method(D_METHOD("register_methods", "mm_graph_node"), &MMNode::register_methods);
	ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &MMNode::_register_methods_bind);

	ClassDB::bind_method(D_METHOD("register_input_property", "prop"), &MMNode::register_input_property);
	ClassDB::bind_method(D_METHOD("unregister_input_property", "prop"), &MMNode::unregister_input_property);

	ClassDB::bind_method(D_METHOD("register_output_property", "prop"), &MMNode::register_output_property);
	ClassDB::bind_method(D_METHOD("unregister_output_property", "prop"), &MMNode::unregister_output_property);

	ClassDB::bind_method(D_METHOD("on_input_property_changed"), &MMNode::on_input_property_changed);

	BIND_VMETHOD(MethodInfo("_get_property_value", PropertyInfo(Variant::VECTOR2, "uv")));
	ClassDB::bind_method(D_METHOD("get_property_value", "uv"), &MMNode::get_property_value);
	ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &MMNode::_get_property_value);

	BIND_VMETHOD(MethodInfo("_get_property_value_sdf3d", PropertyInfo(Variant::VECTOR3, "uv3")));
	ClassDB::bind_method(D_METHOD("get_property_value_sdf3d", "uv3"), &MMNode::get_property_value_sdf3d);
	ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &MMNode::_get_property_value_sdf3d);
}
