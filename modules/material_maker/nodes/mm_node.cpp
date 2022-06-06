
#include "mm_node.h"

Vector2 MMNode::get_graph_position() {
	return graph_position;
}

void MMNode::set_graph_position(const Vector2 &val) {
	graph_position = val;
}

Array MMNode::get_input_properties() {
	return input_properties;
}

void MMNode::set_input_properties(const Array &val) {
	input_properties = val;
}

Array MMNode::get_output_properties() {
	return output_properties;
}

void MMNode::set_output_properties(const Array &val) {
	output_properties = val;
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
	dirty = val;
}

//tool;
//export(Vector2) ;
Vector2 graph_position = Vector2();
Array input_properties = ;
Array output_properties = ;
bool properties_initialized = false;
bool dirty = true;
//MMMateial;

bool MMNode::render(const Variant &material) {
	if (!dirty) {
		return false;
	}

	for (p in input_properties) {
		if (p.input_property && p.input_property.owner.dirty) {
			return false;
		}
	}

	_render(material);
	dirty = false;
	return true;
}

//MMMateial;

void MMNode::_render(const Variant &material) {
	pass;
}

Image MMNode::render_image(const Variant &material) {
	Ref<Image> image = Image.new();
	image.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
	image.lock();
	float w = image.get_width();
	float h = image.get_height();
	float pseed = randf() + randi();

	for (int x = 0; x < image.get_width(); ++x) { //x in range(image.get_width())

		for (int y = 0; y < image.get_height(); ++y) { //y in range(image.get_height())
			Vector2 v = Vector2(x / w, y / h);
			Color col = get_value_for(v, pseed);
			image.set_pixel(x, y, col);
		}
	}

	image.unlock();
	return image;
}

Color MMNode::get_value_for(const Vector2 &uv, const int pseed) {
	return Color();
}

void MMNode::init_properties() {
	if (!properties_initialized) {
		properties_initialized = true;
		_init_properties();
	}
}

void MMNode::_init_properties() {
	pass;
}

void MMNode::register_methods(const Variant &mm_graph_node) {
	init_properties();
	_register_methods(mm_graph_node);
}

void MMNode::_register_methods(const Variant &mm_graph_node) {
	pass;
}

Vector2 MMNode::get_graph_position() {
	return graph_position;
}

void MMNode::set_graph_position(const Vector2 &pos) {
	graph_position = pos;
	emit_changed();
}

void MMNode::register_input_property(const MMNodeUniversalProperty &prop) {
	prop.owner = self;

	if (!prop.is_connected("changed", self, "on_input_property_changed")) {
		prop.connect("changed", self, "on_input_property_changed");
	}

	input_properties.append(prop);
}

void MMNode::unregister_input_property(const MMNodeUniversalProperty &prop) {
	if (prop.owner == self) {
		prop.owner = null;
	}

	if (prop.is_connected("changed", self, "on_input_property_changed")) {
		prop.disconnect("changed", self, "on_input_property_changed");
	}

	input_properties.erase(prop);
}

void MMNode::register_output_property(const MMNodeUniversalProperty &prop) {
	prop.owner = self;
	output_properties.append(prop);
}

void MMNode::unregister_output_property(const MMNodeUniversalProperty &prop) {
	if (prop.owner == self) {
		prop.owner = null;
	}

	output_properties.erase(prop);
}

void MMNode::set_dirty(const bool val) {
	bool changed = val != dirty;
	dirty = val;

	if (changed) {
		emit_changed();
	}
}

void MMNode::on_input_property_changed() {
	set_dirty(true);
	emit_changed();
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
}

MMNode::MMNode() {
	graph_position = Vector2();
	input_properties = ;
	output_properties = ;
	properties_initialized = false;
	dirty = true;
}

MMNode::~MMNode() {
}

static void MMNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_graph_position"), &MMNode::get_graph_position);
	ClassDB::bind_method(D_METHOD("set_graph_position", "value"), &MMNode::set_graph_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "graph_position"), "set_graph_position", "get_graph_position");

	ClassDB::bind_method(D_METHOD("get_input_properties"), &MMNode::get_input_properties);
	ClassDB::bind_method(D_METHOD("set_input_properties", "value"), &MMNode::set_input_properties);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "input_properties"), "set_input_properties", "get_input_properties");

	ClassDB::bind_method(D_METHOD("get_output_properties"), &MMNode::get_output_properties);
	ClassDB::bind_method(D_METHOD("set_output_properties", "value"), &MMNode::set_output_properties);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "output_properties"), "set_output_properties", "get_output_properties");

	ClassDB::bind_method(D_METHOD("get_properties_initialized"), &MMNode::get_properties_initialized);
	ClassDB::bind_method(D_METHOD("set_properties_initialized", "value"), &MMNode::set_properties_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "properties_initialized"), "set_properties_initialized", "get_properties_initialized");

	ClassDB::bind_method(D_METHOD("get_dirty"), &MMNode::get_dirty);
	ClassDB::bind_method(D_METHOD("set_dirty", "value"), &MMNode::set_dirty);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "dirty"), "set_dirty", "get_dirty");

	ClassDB::bind_method(D_METHOD("render", "material"), &MMNode::render);
	ClassDB::bind_method(D_METHOD("_render", "material"), &MMNode::_render);
	ClassDB::bind_method(D_METHOD("render_image", "material"), &MMNode::render_image);
	ClassDB::bind_method(D_METHOD("get_value_for", "uv", "pseed"), &MMNode::get_value_for);
	ClassDB::bind_method(D_METHOD("init_properties"), &MMNode::init_properties);
	ClassDB::bind_method(D_METHOD("_init_properties"), &MMNode::_init_properties);
	ClassDB::bind_method(D_METHOD("register_methods", "mm_graph_node"), &MMNode::register_methods);
	ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &MMNode::_register_methods);
	ClassDB::bind_method(D_METHOD("get_graph_position"), &MMNode::get_graph_position);
	ClassDB::bind_method(D_METHOD("set_graph_position", "pos"), &MMNode::set_graph_position);
	ClassDB::bind_method(D_METHOD("register_input_property", "prop"), &MMNode::register_input_property);
	ClassDB::bind_method(D_METHOD("unregister_input_property", "prop"), &MMNode::unregister_input_property);
	ClassDB::bind_method(D_METHOD("register_output_property", "prop"), &MMNode::register_output_property);
	ClassDB::bind_method(D_METHOD("unregister_output_property", "prop"), &MMNode::unregister_output_property);
	ClassDB::bind_method(D_METHOD("set_dirty", "val"), &MMNode::set_dirty);
	ClassDB::bind_method(D_METHOD("on_input_property_changed"), &MMNode::on_input_property_changed);
}
