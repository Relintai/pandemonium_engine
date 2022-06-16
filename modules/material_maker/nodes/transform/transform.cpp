
#include "transform.h"


Ref<Resource> Transform::get_image() {
 return image;
}

void Transform::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Transform::get_input() {
 return input;
}

void Transform::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Transform::get_translate_x() {
 return translate_x;
}

void Transform::set_translate_x(const Ref<Resource> &val) {
translate_x = val;
}


Ref<Resource> Transform::get_translate_y() {
 return translate_y;
}

void Transform::set_translate_y(const Ref<Resource> &val) {
translate_y = val;
}


Ref<Resource> Transform::get_rotate() {
 return rotate;
}

void Transform::set_rotate(const Ref<Resource> &val) {
rotate = val;
}


Ref<Resource> Transform::get_scale_x() {
 return scale_x;
}

void Transform::set_scale_x(const Ref<Resource> &val) {
scale_x = val;
}


Ref<Resource> Transform::get_scale_y() {
 return scale_y;
}

void Transform::set_scale_y(const Ref<Resource> &val) {
scale_y = val;
}


int Transform::get_mode() const {
 return mode;
}

void Transform::set_mode(const int val) {
mode = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> translate_x;
 //export(Resource) ;
  Ref<Resource> translate_y;
 //export(Resource) ;
  Ref<Resource> rotate;
 //export(Resource) ;
  Ref<Resource> scale_x;
 //export(Resource) ;
  Ref<Resource> scale_y;
 //export(int, "Clamp,Repeat,Extend") ;
  int mode = 0;

 void Transform::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color(0, 0, 0, 1));
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input    ";

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  //image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  //image.force_override = true;

  if (!translate_x) {
   translate_x = MMNodeUniversalProperty.new();
   translate_x.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   translate_x.set_default_value(0);
   translate_x.value_step = 0.01;
}

  translate_x.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  translate_x.slot_name = "Translate X";

  if (!translate_y) {
   translate_y = MMNodeUniversalProperty.new();
   translate_y.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   translate_y.set_default_value(0);
   translate_y.value_step = 0.01;
}

  translate_y.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  translate_y.slot_name = "Translate Y";

  if (!rotate) {
   rotate = MMNodeUniversalProperty.new();
   rotate.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   rotate.set_default_value(0);
   rotate.value_step = 0.01;
}

  rotate.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  rotate.slot_name = "Rotate";

  if (!scale_x) {
   scale_x = MMNodeUniversalProperty.new();
   scale_x.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   scale_x.set_default_value(1);
   scale_x.value_step = 0.01;
}

  scale_x.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  scale_x.slot_name = "Scale X";

  if (!scale_y) {
   scale_y = MMNodeUniversalProperty.new();
   scale_y.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   scale_y.set_default_value(1);
   scale_y.value_step = 0.01;
}

  scale_y.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  scale_y.slot_name = "Scale Y";
  register_input_property(input);
  register_output_property(image);
  register_input_property(translate_x);
  register_input_property(translate_y);
  register_input_property(rotate);
  register_input_property(scale_x);
  register_input_property(scale_y);
}


 void Transform::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_float_universal(translate_x);
  mm_graph_node.add_slot_float_universal(translate_y);
  mm_graph_node.add_slot_float_universal(rotate);
  mm_graph_node.add_slot_float_universal(scale_x);
  mm_graph_node.add_slot_float_universal(scale_y);
  mm_graph_node.add_slot_enum("get_mode", "set_mode", "Mode", [ "Clamp", "Repeat", "Extend" ]);
}


 void Transform::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Transform::_get_value_for(const Vector2 &uv, const int pseed) {
  //$i($mode(transform2($uv, vec2($translate_x*(2.0*$tx($uv)-1.0), $translate_y*(2.0*$ty($uv)-1.0)), $rotate*0.01745329251*(2.0*$r($uv)-1.0), vec2($scale_x*(2.0*$sx($uv)-1.0), $scale_y*(2.0*$sy($uv)-1.0)))))",;
  //Mode:;
  //Clamp -> transform2_clamp;
  //Repeat -> fract;
  //Extend -> "";
   Vector2 tr = Vector2(translate_x.get_default_value() * (2.0 * translate_x.get_value_or_zero(uv) - 1.0), translate_y.get_default_value() *(2.0 * translate_y.get_value_or_zero(uv) - 1.0));
   float rot = rotate.get_default_value() * 0.01745329251*(2.0 * rotate.get_value_or_zero(uv) - 1.0);
   Vector2 sc = Vector2(scale_x.get_default_value() *(2.0 * scale_x.get_value_or_zero(uv) - 1.0), scale_y.get_default_value() *(2.0 * scale_y.get_value_or_zero(uv) - 1.0));
   Vector2 nuv = MMAlgos.transform2(uv, tr, rot, sc);

  if (mode == 0) {
   nuv = MMAlgos.transform2_clamp(nuv);
}


  else if (mode == 1) {
   nuv = MMAlgos.fractv2(nuv);
}

  return input.get_value(nuv);
}

 //mode;

 int Transform::get_mode() {
  return mode;
}


 void Transform::set_mode(const int val) {
  mode = val;
  set_dirty(true);
}

}

 Transform::Transform() {
  image;
  input;
  translate_x;
  translate_y;
  rotate;
  scale_x;
  scale_y;
  mode = 0;
 }

 Transform::~Transform() {
 }


 static void Transform::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Transform::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Transform::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Transform::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Transform::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_translate_x"), &Transform::get_translate_x);
   ClassDB::bind_method(D_METHOD("set_translate_x", "value"), &Transform::set_translate_x);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "translate_x", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_translate_x", "get_translate_x");


   ClassDB::bind_method(D_METHOD("get_translate_y"), &Transform::get_translate_y);
   ClassDB::bind_method(D_METHOD("set_translate_y", "value"), &Transform::set_translate_y);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "translate_y", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_translate_y", "get_translate_y");


   ClassDB::bind_method(D_METHOD("get_rotate"), &Transform::get_rotate);
   ClassDB::bind_method(D_METHOD("set_rotate", "value"), &Transform::set_rotate);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "rotate", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_rotate", "get_rotate");


   ClassDB::bind_method(D_METHOD("get_scale_x"), &Transform::get_scale_x);
   ClassDB::bind_method(D_METHOD("set_scale_x", "value"), &Transform::set_scale_x);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scale_x", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_scale_x", "get_scale_x");


   ClassDB::bind_method(D_METHOD("get_scale_y"), &Transform::get_scale_y);
   ClassDB::bind_method(D_METHOD("set_scale_y", "value"), &Transform::set_scale_y);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scale_y", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_scale_y", "get_scale_y");


   ClassDB::bind_method(D_METHOD("get_mode"), &Transform::get_mode);
   ClassDB::bind_method(D_METHOD("set_mode", "value"), &Transform::set_mode);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "mode"), "set_mode", "get_mode");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Transform::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Transform::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Transform::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Transform::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_mode"), &Transform::get_mode);
  ClassDB::bind_method(D_METHOD("set_mode", "val"), &Transform::set_mode);

 }



