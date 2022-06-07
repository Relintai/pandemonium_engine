
#include "mm_node_universal_property.h"

#include "mm_node.h"

int MMNodeUniversalProperty::get_default_type() const {
	return default_type;
}

void MMNodeUniversalProperty::set_default_type(const int val) {
	default_type = val;
}

int MMNodeUniversalProperty::get_default_int() const {
	return default_int;
}

void MMNodeUniversalProperty::set_default_int(const int val) {
	default_int = val;
}

float MMNodeUniversalProperty::get_default_float() const {
	return default_float;
}

void MMNodeUniversalProperty::set_default_float(const float val) {
	default_float = val;
}

Vector2 MMNodeUniversalProperty::get_default_vector2() {
	return default_vector2;
}

void MMNodeUniversalProperty::set_default_vector2(const Vector2 &val) {
	default_vector2 = val;
}

Vector3 MMNodeUniversalProperty::get_default_vector3() {
	return default_vector3;
}

void MMNodeUniversalProperty::set_default_vector3(const Vector3 &val) {
	default_vector3 = val;
}

Color MMNodeUniversalProperty::get_default_color() {
	return default_color;
}

void MMNodeUniversalProperty::set_default_color(const Color &val) {
	default_color = val;
}

Ref<Image> MMNodeUniversalProperty::get_default_image() {
	return default_image;
}

void MMNodeUniversalProperty::set_default_image(const Ref<Image> &val) {
	default_image = val;
}

bool MMNodeUniversalProperty::get_get_value_from_owner() const {
	return get_value_from_owner;
}

void MMNodeUniversalProperty::set_get_value_from_owner(const bool val) {
	get_value_from_owner = val;
}

bool MMNodeUniversalProperty::get_force_override() const {
	return force_override;
}

void MMNodeUniversalProperty::set_force_override(const bool val) {
	force_override = val;
}

Ref<Image> MMNodeUniversalProperty::get_override_image() {
	return override_image;
}

void MMNodeUniversalProperty::set_override_image(const Ref<Image> &val) {
	override_image = val;
}

int MMNodeUniversalProperty::get_input_slot_type() const {
	return input_slot_type;
}

void MMNodeUniversalProperty::set_input_slot_type(const int val) {
	input_slot_type = val;
}

int MMNodeUniversalProperty::get_output_slot_type() const {
	return output_slot_type;
}

void MMNodeUniversalProperty::set_output_slot_type(const int val) {
	output_slot_type = val;
}

String MMNodeUniversalProperty::get_slot_name() {
	return slot_name;
}

void MMNodeUniversalProperty::set_slot_name(const String &val) {
	slot_name = val;
}

float MMNodeUniversalProperty::get_value_step() const {
	return value_step;
}

void MMNodeUniversalProperty::set_value_step(const float val) {
	value_step = val;
}

Vector2 MMNodeUniversalProperty::get_value_range() {
	return value_range;
}

void MMNodeUniversalProperty::set_value_range(const Vector2 &val) {
	value_range = val;
}

Ref<MMNodeUniversalProperty> MMNodeUniversalProperty::get_input_property() {
	return input_property;
}

void MMNodeUniversalProperty::set_input_property(const Ref<MMNodeUniversalProperty> &val) {
	if (input_property == val) {
		return;
	}

	if (input_property.is_valid()) {
		input_property->disconnect("changed", this, "on_input_property_changed");
	}

	input_property = val;

	if (input_property.is_valid()) {
		input_property->connect("changed", this, "on_input_property_changed");
	}

	emit_changed();
}

// Because in UndiRedo if you pass null as the only argument it will look;
// for a method with no arguments;

void MMNodeUniversalProperty::unset_input_property() {
	set_input_property(Ref<MMNodeUniversalProperty>());
}

void MMNodeUniversalProperty::on_input_property_changed() {
	emit_changed();
}

Ref<MMNode> MMNodeUniversalProperty::get_owner() {
	return owner;
}

void MMNodeUniversalProperty::set_owner(const Ref<MMNode> &val) {
	owner = val;
}

Variant MMNodeUniversalProperty::get_value(const Vector2 &uv, const bool skip_owner_val) {
	if (get_value_from_owner && !skip_owner_val) {
		return get_owner_value(uv);
	}

	if (!input_property.is_valid()) {
		return get_default_value(uv);
	}

	if (default_type == input_property->default_type) {
		return input_property->get_value(uv);
	}

	if (default_type == DEFAULT_TYPE_INT) {
		return to_int(input_property->get_value(uv));
	} else if (default_type == DEFAULT_TYPE_FLOAT) {
		return to_float(input_property->get_value(uv));
	} else if (default_type == DEFAULT_TYPE_VECTOR2) {
		return to_vector2(input_property->get_value(uv));
	} else if (default_type == DEFAULT_TYPE_VECTOR3) {
		return to_vector3(input_property->get_value(uv));
	} else if (default_type == DEFAULT_TYPE_COLOR) {
		return to_color(input_property->get_value(uv));
	} else if (default_type == DEFAULT_TYPE_IMAGE) {
		return to_color(input_property->get_value(uv));
	}

	return input_property->get_value(uv);
}

Variant MMNodeUniversalProperty::get_owner_value(const Vector2 &uv) {
	if (default_type == DEFAULT_TYPE_INT) {
		return to_int(owner->get_property_value(uv));
	} else if (default_type == DEFAULT_TYPE_FLOAT) {
		return to_float(owner->get_property_value(uv));
	} else if (default_type == DEFAULT_TYPE_VECTOR2) {
		return to_vector2(owner->get_property_value(uv));
	} else if (default_type == DEFAULT_TYPE_VECTOR3) {
		return to_vector3(owner->get_property_value(uv));
	} else if (default_type == DEFAULT_TYPE_COLOR) {
		return to_color(owner->get_property_value(uv));
	} else if (default_type == DEFAULT_TYPE_IMAGE) {
		return to_color(owner->get_property_value(uv));
	}

	return Variant();
}

Variant MMNodeUniversalProperty::get_value_or_zero(const Vector2 &uv, const bool skip_owner_val) {
	if (get_value_from_owner && !skip_owner_val) {
		return get_owner_value(uv);
	}

	if (!input_property.is_valid()) {
		return get_zero_value();
	}

	if (default_type == input_property->default_type) {
		return input_property->get_value(uv);
	}

	if (default_type == DEFAULT_TYPE_INT) {
		return to_int(input_property->get_value(uv));
	} else if (default_type == DEFAULT_TYPE_FLOAT) {
		return to_float(input_property->get_value(uv));
	} else if (default_type == DEFAULT_TYPE_VECTOR2) {
		return to_vector2(input_property->get_value(uv));
	} else if (default_type == DEFAULT_TYPE_VECTOR3) {
		return to_vector3(input_property->get_value(uv));
	} else if (default_type == DEFAULT_TYPE_COLOR) {
		return to_color(input_property->get_value(uv));
	} else if (default_type == DEFAULT_TYPE_IMAGE) {
		return to_color(input_property->get_value(uv));
	}

	return input_property->get_value(uv);
}

Vector2 MMNodeUniversalProperty::get_value_sdf3d(const Vector3 &uv3, const bool skip_owner_val) {
	if (get_value_from_owner && !skip_owner_val) {
		return owner->get_property_value_sdf3d(uv3);
	}

	if (!input_property.is_valid()) {
		return default_vector2;
	}

	return input_property->get_value_sdf3d(uv3);
}

int MMNodeUniversalProperty::to_int(const Variant &val) {
	if (val.get_type() == Variant::INT) {
		return val;
	}

	if (val.get_type() == Variant::REAL) {
		return int(val);
	}

	if (val.get_type() == Variant::VECTOR2) {
		Vector2 v = val;
		return int(v.x);
	}

	if (val.get_type() == Variant::VECTOR3) {
		Vector3 v = val;
		return int(v.x);
	}

	if (val.get_type() == Variant::COLOR) {
		Color v = val;
		return int(v.r);
	}

	return 0;
}

float MMNodeUniversalProperty::to_float(const Variant &val) {
	if (val.get_type() == Variant::REAL) {
		return val;
	}

	if (val.get_type() == Variant::INT) {
		return float(val);
	}

	if (val.get_type() == Variant::VECTOR2) {
		Vector2 v = val;
		return float(v.x);
	}

	if (val.get_type() == Variant::VECTOR3) {
		Vector3 v = val;
		return float(v.x);
	}

	if (val.get_type() == Variant::COLOR) {
		Color v = val;
		return float(v.r);
	}

	return 0.0;
}

Vector2 MMNodeUniversalProperty::to_vector2(const Variant &val) {
	if (val.get_type() == Variant::VECTOR2) {
		return val;
	}

	if (val.get_type() == Variant::INT) {
		return Vector2(val, val);
	}

	if (val.get_type() == Variant::REAL) {
		return Vector2(val, val);
	}

	if (val.get_type() == Variant::VECTOR3) {
		Vector3 v = val;
		return Vector2(v.x, v.y);
	}

	if (val.get_type() == Variant::COLOR) {
		Color v = val;
		return Vector2(v.r, v.g);
	}

	return Vector2();
}

Vector3 MMNodeUniversalProperty::to_vector3(const Variant &val) {
	if (val.get_type() == Variant::VECTOR3) {
		return val;
	}

	if (val.get_type() == Variant::INT) {
		return Vector3(val, val, val);
	}

	if (val.get_type() == Variant::REAL) {
		return Vector3(val, val, val);
	}

	if (val.get_type() == Variant::VECTOR2) {
		Vector2 v = val;
		return Vector3(v.x, v.y, 0);
	}

	if (val.get_type() == Variant::COLOR) {
		Color v = val;
		return Vector3(v.r, v.g, v.b);
	}

	return Vector3();
}

Color MMNodeUniversalProperty::to_color(const Variant &val) {
	if (val.get_type() == Variant::COLOR) {
		return val;
	}

	if (val.get_type() == Variant::INT) {
		return Color(val, val, val, 1);
	}

	if (val.get_type() == Variant::REAL) {
		return Color(val, val, val, 1);
	}

	if (val.get_type() == Variant::VECTOR2) {
		Vector2 v = val;
		return Color(v.x, v.y, 0, 1);
	}

	if (val.get_type() == Variant::VECTOR3) {
		Vector3 v = val;
		return Color(v.x, v.y, v.z, 1);
	}

	return Color();
}

void MMNodeUniversalProperty::set_value(const Variant &val) {
	if (default_type == DEFAULT_TYPE_IMAGE) {
		override_image = val;
		emit_changed();
		return;
	}

	set_default_value(val);
}

Variant MMNodeUniversalProperty::get_zero_value() {
	if (default_type == DEFAULT_TYPE_INT) {
		return 0;
	} else if (default_type == DEFAULT_TYPE_FLOAT) {
		return 0.0;
	} else if (default_type == DEFAULT_TYPE_VECTOR2) {
		return Vector2();
	} else if (default_type == DEFAULT_TYPE_VECTOR3) {
		return Vector3();
	} else if (default_type == DEFAULT_TYPE_COLOR) {
		return Color();
	} else if (default_type == DEFAULT_TYPE_IMAGE) {
		return Color();
	}

	return Variant();
}

Variant MMNodeUniversalProperty::get_default_value(const Vector2 &uv) {
	if (default_type == DEFAULT_TYPE_INT) {
		return default_int;
	} else if (default_type == DEFAULT_TYPE_FLOAT) {
		return default_float;
	} else if (default_type == DEFAULT_TYPE_VECTOR2) {
		return default_vector2;
	} else if (default_type == DEFAULT_TYPE_VECTOR3) {
		return default_vector3;
	} else if (default_type == DEFAULT_TYPE_COLOR) {
		return default_color;
	} else if (default_type == DEFAULT_TYPE_IMAGE) {
		Ref<Image> image = default_image;

		if (override_image.is_valid()) {
			image = override_image;
		}

		if (!image.is_valid()) {
			return default_color;
		}

		image->lock();
		int x = uv.x * image->get_width();
		int y = uv.y * image->get_height();
		x = CLAMP(x, 0, image->get_width() - 1);
		y = CLAMP(y, 0, image->get_width() - 1);
		Color c = image->get_pixel(x, y);
		image->unlock();
		return c;
	}

	return Variant();
}

void MMNodeUniversalProperty::set_default_value(const Variant &val) {
	if (default_type == DEFAULT_TYPE_INT) {
		default_int = val;
	} else if (default_type == DEFAULT_TYPE_FLOAT) {
		default_float = val;
	} else if (default_type == DEFAULT_TYPE_VECTOR2) {
		default_vector2 = val;
	} else if (default_type == DEFAULT_TYPE_VECTOR3) {
		default_vector3 = val;
	} else if (default_type == DEFAULT_TYPE_COLOR) {
		default_color = val;
	} else if (default_type == DEFAULT_TYPE_IMAGE) {
		default_image = val;
	}

	emit_changed();
}

Ref<Image> MMNodeUniversalProperty::get_active_image() {
	if (!force_override && input_property.is_valid()) {
		return input_property->get_active_image();
	}

	if (override_image.is_valid()) {
		return override_image;
	}

	return default_image;
}

MMNodeUniversalProperty::MMNodeUniversalProperty() {
	default_type = DEFAULT_TYPE_INT;
	default_int = 0;
	default_float = 0;
	get_value_from_owner = false;
	force_override = false;
	input_slot_type = SLOT_TYPE_NONE;
	output_slot_type = SLOT_TYPE_NONE;
	value_step = 0.1;
	value_range = Vector2(-1000, 1000);
}

MMNodeUniversalProperty::~MMNodeUniversalProperty() {
}

void MMNodeUniversalProperty::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		if (input_property.is_valid()) {
			input_property->connect("changed", this, "on_input_property_changed");
		}
	}
}

void MMNodeUniversalProperty::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_default_type"), &MMNodeUniversalProperty::get_default_type);
	ClassDB::bind_method(D_METHOD("set_default_type", "value"), &MMNodeUniversalProperty::set_default_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "default_type"), "set_default_type", "get_default_type");

	ClassDB::bind_method(D_METHOD("get_default_int"), &MMNodeUniversalProperty::get_default_int);
	ClassDB::bind_method(D_METHOD("set_default_int", "value"), &MMNodeUniversalProperty::set_default_int);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "default_int"), "set_default_int", "get_default_int");

	ClassDB::bind_method(D_METHOD("get_default_float"), &MMNodeUniversalProperty::get_default_float);
	ClassDB::bind_method(D_METHOD("set_default_float", "value"), &MMNodeUniversalProperty::set_default_float);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "default_float"), "set_default_float", "get_default_float");

	ClassDB::bind_method(D_METHOD("get_default_vector2"), &MMNodeUniversalProperty::get_default_vector2);
	ClassDB::bind_method(D_METHOD("set_default_vector2", "value"), &MMNodeUniversalProperty::set_default_vector2);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "default_vector2"), "set_default_vector2", "get_default_vector2");

	ClassDB::bind_method(D_METHOD("get_default_vector3"), &MMNodeUniversalProperty::get_default_vector3);
	ClassDB::bind_method(D_METHOD("set_default_vector3", "value"), &MMNodeUniversalProperty::set_default_vector3);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "default_vector3"), "set_default_vector3", "get_default_vector3");

	ClassDB::bind_method(D_METHOD("get_default_color"), &MMNodeUniversalProperty::get_default_color);
	ClassDB::bind_method(D_METHOD("set_default_color", "value"), &MMNodeUniversalProperty::set_default_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "default_color"), "set_default_color", "get_default_color");

	ClassDB::bind_method(D_METHOD("get_default_image"), &MMNodeUniversalProperty::get_default_image);
	ClassDB::bind_method(D_METHOD("set_default_image", "value"), &MMNodeUniversalProperty::set_default_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "default_image", PROPERTY_HINT_RESOURCE_TYPE, "Image"), "set_default_image", "get_default_image");

	ClassDB::bind_method(D_METHOD("get_get_value_from_owner"), &MMNodeUniversalProperty::get_get_value_from_owner);
	ClassDB::bind_method(D_METHOD("set_get_value_from_owner", "value"), &MMNodeUniversalProperty::set_get_value_from_owner);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "get_value_from_owner"), "set_get_value_from_owner", "get_get_value_from_owner");

	ClassDB::bind_method(D_METHOD("get_force_override"), &MMNodeUniversalProperty::get_force_override);
	ClassDB::bind_method(D_METHOD("set_force_override", "value"), &MMNodeUniversalProperty::set_force_override);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "force_override"), "set_force_override", "get_force_override");

	ClassDB::bind_method(D_METHOD("get_override_image"), &MMNodeUniversalProperty::get_override_image);
	ClassDB::bind_method(D_METHOD("set_override_image", "value"), &MMNodeUniversalProperty::set_override_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "override_image", PROPERTY_HINT_RESOURCE_TYPE, "Image", 0), "set_override_image", "get_override_image");

	ClassDB::bind_method(D_METHOD("get_input_slot_type"), &MMNodeUniversalProperty::get_input_slot_type);
	ClassDB::bind_method(D_METHOD("set_input_slot_type", "value"), &MMNodeUniversalProperty::set_input_slot_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "input_slot_type"), "set_input_slot_type", "get_input_slot_type");

	ClassDB::bind_method(D_METHOD("get_output_slot_type"), &MMNodeUniversalProperty::get_output_slot_type);
	ClassDB::bind_method(D_METHOD("set_output_slot_type", "value"), &MMNodeUniversalProperty::set_output_slot_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "output_slot_type"), "set_output_slot_type", "get_output_slot_type");

	ClassDB::bind_method(D_METHOD("get_slot_name"), &MMNodeUniversalProperty::get_slot_name);
	ClassDB::bind_method(D_METHOD("set_slot_name", "value"), &MMNodeUniversalProperty::set_slot_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "slot_name"), "set_slot_name", "get_slot_name");

	ClassDB::bind_method(D_METHOD("get_value_step"), &MMNodeUniversalProperty::get_value_step);
	ClassDB::bind_method(D_METHOD("set_value_step", "value"), &MMNodeUniversalProperty::set_value_step);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "value_step"), "set_value_step", "get_value_step");

	ClassDB::bind_method(D_METHOD("get_value_range"), &MMNodeUniversalProperty::get_value_range);
	ClassDB::bind_method(D_METHOD("set_value_range", "value"), &MMNodeUniversalProperty::set_value_range);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "value_range"), "set_value_range", "get_value_range");

	ClassDB::bind_method(D_METHOD("get_input_property"), &MMNodeUniversalProperty::get_input_property);
	ClassDB::bind_method(D_METHOD("set_input_property", "value"), &MMNodeUniversalProperty::set_input_property);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_property", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input_property", "get_input_property");
	ClassDB::bind_method(D_METHOD("unset_input_property"), &MMNodeUniversalProperty::unset_input_property);
	ClassDB::bind_method(D_METHOD("on_input_property_changed"), &MMNodeUniversalProperty::on_input_property_changed);

	ClassDB::bind_method(D_METHOD("get_owner"), &MMNodeUniversalProperty::get_owner);
	ClassDB::bind_method(D_METHOD("set_owner", "value"), &MMNodeUniversalProperty::set_owner);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "owner", PROPERTY_HINT_RESOURCE_TYPE, "MMNode", 0), "set_owner", "get_owner");

	ClassDB::bind_method(D_METHOD("get_value", "uv", "skip_owner_val"), &MMNodeUniversalProperty::get_value, false);
	ClassDB::bind_method(D_METHOD("get_owner_value", "uv"), &MMNodeUniversalProperty::get_owner_value);
	ClassDB::bind_method(D_METHOD("get_value_or_zero", "uv", "skip_owner_val"), &MMNodeUniversalProperty::get_value_or_zero, false);
	ClassDB::bind_method(D_METHOD("get_value_sdf3d", "uv3", "skip_owner_val"), &MMNodeUniversalProperty::get_value_sdf3d, false);

	ClassDB::bind_method(D_METHOD("to_int", "val"), &MMNodeUniversalProperty::to_int);
	ClassDB::bind_method(D_METHOD("to_float", "val"), &MMNodeUniversalProperty::to_float);
	ClassDB::bind_method(D_METHOD("to_vector2", "val"), &MMNodeUniversalProperty::to_vector2);
	ClassDB::bind_method(D_METHOD("to_vector3", "val"), &MMNodeUniversalProperty::to_vector3);
	ClassDB::bind_method(D_METHOD("to_color", "val"), &MMNodeUniversalProperty::to_color);
	ClassDB::bind_method(D_METHOD("set_value", "val"), &MMNodeUniversalProperty::set_value);
	ClassDB::bind_method(D_METHOD("get_zero_value"), &MMNodeUniversalProperty::get_zero_value);
	ClassDB::bind_method(D_METHOD("get_default_value", "uv"), &MMNodeUniversalProperty::get_default_value, Vector2());
	ClassDB::bind_method(D_METHOD("set_default_value", "val"), &MMNodeUniversalProperty::set_default_value);
	ClassDB::bind_method(D_METHOD("get_active_image"), &MMNodeUniversalProperty::get_active_image);

	BIND_ENUM_CONSTANT(SLOT_TYPE_NONE);
	BIND_ENUM_CONSTANT(SLOT_TYPE_IMAGE);
	BIND_ENUM_CONSTANT(SLOT_TYPE_INT);
	BIND_ENUM_CONSTANT(SLOT_TYPE_FLOAT);
	BIND_ENUM_CONSTANT(SLOT_TYPE_VECTOR2);
	BIND_ENUM_CONSTANT(SLOT_TYPE_VECTOR3);
	BIND_ENUM_CONSTANT(SLOT_TYPE_COLOR);
	BIND_ENUM_CONSTANT(SLOT_TYPE_UNIVERSAL);

	BIND_ENUM_CONSTANT(DEFAULT_TYPE_INT);
	BIND_ENUM_CONSTANT(DEFAULT_TYPE_FLOAT);
	BIND_ENUM_CONSTANT(DEFAULT_TYPE_VECTOR2);
	BIND_ENUM_CONSTANT(DEFAULT_TYPE_VECTOR3);
	BIND_ENUM_CONSTANT(DEFAULT_TYPE_COLOR);
	BIND_ENUM_CONSTANT(DEFAULT_TYPE_IMAGE);
}
