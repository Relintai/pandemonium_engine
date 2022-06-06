#ifndef MM_NODE_UNIVERSAL_PROPERTY_H
#define MM_NODE_UNIVERSAL_PROPERTY_H

class MMNodeUniversalProperty : public Resource {
	GDCLASS(MMNodeUniversalProperty, Resource);

public:
	int get_default_type() const;
	void set_default_type(const int val);

	int get_default_int() const;
	void set_default_int(const int val);

	float get_default_float() const;
	void set_default_float(const float val);

	Vector2 get_default_vector2();
	void set_default_vector2(const Vector2 &val);

	Vector3 get_default_vector3();
	void set_default_vector3(const Vector3 &val);

	Color get_default_color();
	void set_default_color(const Color &val);

	Ref<Image> get_default_image();
	void set_default_image(const Ref<Image> &val);

	bool get_get_value_from_owner() const;
	void set_get_value_from_owner(const bool val);

	bool get_force_override() const;
	void set_force_override(const bool val);

	Ref<Image> get_override_image();
	void set_override_image(const Ref<Image> &val);

	Ref<Resource> get_input_property();
	void set_input_property(const Ref<Resource> &val);

	int get_input_slot_type() const;
	void set_input_slot_type(const int val);

	int get_output_slot_type() const;
	void set_output_slot_type(const int val);

	String get_slot_name();
	void set_slot_name(const String &val);

	float get_value_step() const;
	void set_value_step(const float val);

	Vector2 get_value_range();
	void set_value_range(const Vector2 &val);

	Variant get_Variant();
	void set_Variant(const Variant &val);

	enum SlotTypes {

		SLOT_TYPE_NONE = -1,
		SLOT_TYPE_IMAGE = 0,
		SLOT_TYPE_INT = 1,
		SLOT_TYPE_FLOAT = 2,
		SLOT_TYPE_VECTOR2 = 3,
		SLOT_TYPE_VECTOR3 = 4,
		SLOT_TYPE_COLOR = 5,
		SLOT_TYPE_UNIVERSAL = 6,
	};

	enum MMNodeUniversalPropertyDefaultType {

		DEFAULT_TYPE_INT = 0,
		DEFAULT_TYPE_FLOAT = 1,
		DEFAULT_TYPE_VECTOR2 = 2,
		DEFAULT_TYPE_VECTOR3 = 3,
		DEFAULT_TYPE_COLOR = 4,
		DEFAULT_TYPE_IMAGE = 5,
	};

	void _init();
	void get_value(const Vector2 &uv, const bool skip_owner_val = false);
	void get_owner_value(const Vector2 &uv);
	void get_value_or_zero(const Vector2 &uv, const bool skip_owner_val = false);
	Vector2 get_value_sdf3d(const Vector3 &uv3, const bool skip_owner_val = false);
	int to_int(const Variant &val);
	float to_float(const Variant &val);
	Vector2 to_vector2(const Variant &val);
	Vector3 to_vector3(const Variant &val);
	Color to_color(const Variant &val);
	void set_value(const Variant &val);
	void get_zero_value();
	void get_default_value(const Vector2 &uv = Vector2());
	void set_default_value(const Variant &val);
	Image get_active_image();
	void set_input_property(const MMNodeUniversalProperty &val);
	void unset_input_property();
	void on_input_property_changed();

	MMNodeUniversalProperty();
	~MMNodeUniversalProperty();

protected:
	static void _bind_methods();

	//tool
};
}
;
//export(int, "Int,Float,Vector2,Vector3,Color,Image")
int default_type = ;
//export(int)
int default_int = ;
//export(float)
float default_float = ;
//export(Vector2)
Vector2 default_vector2 = ;
//export(Vector3)
Vector3 default_vector3 = ;
//export(Color)
Color default_color = ;
//export(Image)
Ref<Image> default_image;
bool get_value_from_owner = false;
bool force_override = false;
//This is not exported on purpose!
Ref<Image> override_image;
//Should be a MMNodeUniversalProperty, but can't set it up like that
//export(Resource)
Ref<Resource> input_property;
int input_slot_type = SlotTypes.SLOT_TYPE_NONE;
int output_slot_type = SlotTypes.SLOT_TYPE_NONE;
String slot_name = ;
float value_step = 0.1;
Vector2 value_range = Vector2(-1000, 1000);
//MMNode
Variant;
// Because in UndiRedo if you pass null as the only argument it will look
// for a method with no arguments
}
;

#endif
