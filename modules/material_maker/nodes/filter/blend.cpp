
#include "blend.h"


Ref<Resource> Blend::get_image() {
 return image;
}

void Blend::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Blend::get_input1() {
 return input1;
}

void Blend::set_input1(const Ref<Resource> &val) {
input1 = val;
}


Ref<Resource> Blend::get_input2() {
 return input2;
}

void Blend::set_input2(const Ref<Resource> &val) {
input2 = val;
}


int Blend::get_blend_type() const {
 return blend_type;
}

void Blend::set_blend_type(const int val) {
blend_type = val;
}


Ref<Resource> Blend::get_opacity() {
 return opacity;
}

void Blend::set_opacity(const Ref<Resource> &val) {
opacity = val;
}



 //tool;
 };
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input1;
 //export(Resource) ;
  Ref<Resource> input2;
 //export(int, "Normal,Dissolve,Multiply,Screen,Overlay,Hard Light,Soft Light,Burn,Dodge,Lighten,Darken,Difference") ;
  int blend_type = 0;
 //export(Resource) ;
  Ref<Resource> opacity;

 void Blend::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!input1) {
   input1 = MMNodeUniversalProperty.new();
   input1.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input1.set_default_value(Color(1, 1, 1, 1));
}

  input1.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input1.slot_name = ">>>    Input1    ";

  if (!input2) {
   input2 = MMNodeUniversalProperty.new();
   input2.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input2.set_default_value(Color(1, 1, 1, 1));
}

  input2.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input2.slot_name = ">>>    Input2    ";

  if (!opacity) {
   opacity = MMNodeUniversalProperty.new();
   opacity.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   opacity.set_default_value(0.5);
   opacity.value_range = Vector2(0, 1);
   opacity.value_step = 0.01;
}

  opacity.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  opacity.slot_name = "opacity";
  register_input_property(input1);
  register_input_property(input2);
  register_output_property(image);
  register_input_property(opacity);
}


 void Blend::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_blend_type", "set_blend_type", "blend_type", [ "Normal", "Dissolve", "Multiply", "Screen", "Overlay", "Hard Light", "Soft Light", "Burn", "Dodge", "Lighten", "Darken", "Difference" ]);
  mm_graph_node.add_slot_label_universal(input1);
  mm_graph_node.add_slot_label_universal(input2);
  mm_graph_node.add_slot_float_universal(opacity);
}


 void Blend::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Blend::_get_value_for(const Vector2 &uv, const int pseed) {
   Vector3 b = Vector3();
  //vec4 $(name_uv)_s1 = $s1($uv);
   Color s1 = input1.get_value(uv);
  //vec4 $(name_uv)_s2 = $s2($uv);
   Color s2 = input2.get_value(uv);
  //float $(name_uv)_a = $amount*$a($uv);
   float a = opacity.get_value(uv);
  //vec4(blend_$blend_type($uv, $(name_uv)_s1.rgb, $(name_uv)_s2.rgb, $(name_uv)_a*$(name_uv)_s1.a), min(1.0, $(name_uv)_s2.a+$(name_uv)_a*$(name_uv)_s1.a));
  //"Normal,Dissolve,Multiply,Screen,Overlay,Hard Light,Soft Light,Burn,Dodge,Lighten,Darken,Difference";

  if (blend_type == BlendType.NORMAL) {
   b = MMAlgos.blend_normal(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}


  else if (blend_type == BlendType.DISSOLVE) {
   b = MMAlgos.blend_dissolve(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}


  else if (blend_type == BlendType.MULTIPLY) {
   b = MMAlgos.blend_multiply(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}


  else if (blend_type == BlendType.SCREEN) {
   b = MMAlgos.blend_screen(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}


  else if (blend_type == BlendType.OVERLAY) {
   b = MMAlgos.blend_overlay(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}


  else if (blend_type == BlendType.HARD_LIGHT) {
   b = MMAlgos.blend_hard_light(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}


  else if (blend_type == BlendType.SOFT_LIGHT) {
   b = MMAlgos.blend_soft_light(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}


  else if (blend_type == BlendType.BURN) {
   b = MMAlgos.blend_burn(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}


  else if (blend_type == BlendType.DODGE) {
   b = MMAlgos.blend_dodge(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}


  else if (blend_type == BlendType.LIGHTEN) {
   b = MMAlgos.blend_lighten(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}


  else if (blend_type == BlendType.DARKEN) {
   b = MMAlgos.blend_darken(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}


  else if (blend_type == BlendType.DIFFRENCE) {
   b = MMAlgos.blend_difference(uv, Vector3(s1.r, s1.g, s1.b), Vector3(s2.r, s2.g, s2.b), a * s1.a);
}

  return Color(b.x, b.y, b.z, min(1, s2.a + a * s1.a));
}


 int Blend::get_blend_type() {
  return blend_type;
}


 void Blend::set_blend_type(const int val) {
  blend_type = val;
  set_dirty(true);
}

}

 Blend::Blend() {
  image;
  input1;
  input2;
  blend_type = 0;
  opacity;
 }

 Blend::~Blend() {
 }


 static void Blend::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Blend::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Blend::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input1"), &Blend::get_input1);
   ClassDB::bind_method(D_METHOD("set_input1", "value"), &Blend::set_input1);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input1", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input1", "get_input1");


   ClassDB::bind_method(D_METHOD("get_input2"), &Blend::get_input2);
   ClassDB::bind_method(D_METHOD("set_input2", "value"), &Blend::set_input2);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input2", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input2", "get_input2");


   ClassDB::bind_method(D_METHOD("get_blend_type"), &Blend::get_blend_type);
   ClassDB::bind_method(D_METHOD("set_blend_type", "value"), &Blend::set_blend_type);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "blend_type"), "set_blend_type", "get_blend_type");


   ClassDB::bind_method(D_METHOD("get_opacity"), &Blend::get_opacity);
   ClassDB::bind_method(D_METHOD("set_opacity", "value"), &Blend::set_opacity);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "opacity", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_opacity", "get_opacity");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Blend::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Blend::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Blend::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Blend::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_blend_type"), &Blend::get_blend_type);
  ClassDB::bind_method(D_METHOD("set_blend_type", "val"), &Blend::set_blend_type);

 }



