
#include "sd_shape_line.h"


Ref<Resource> SdShapeLine::get_output() {
 return output;
}

void SdShapeLine::set_output(const Ref<Resource> &val) {
output = val;
}


Vector2 SdShapeLine::get_A() {
 return A;
}

void SdShapeLine::set_A(const Vector2 &val) {
A = val;
}


Vector2 SdShapeLine::get_B() {
 return B;
}

void SdShapeLine::set_B(const Vector2 &val) {
B = val;
}


float SdShapeLine::get_width() const {
 return width;
}

void SdShapeLine::set_width(const float val) {
width = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(Vector2) ;
  Vector2 A = Vector2(-0.3, -0.3);
 //export(Vector2) ;
  Vector2 B = Vector2(0.3, 0.3);
 //export(float) ;
  float width = 0.1;

 void SdShapeLine::_init() {
  init_points_11();
}


 void SdShapeLine::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_output_property(output);
}


 void SdShapeLine::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_vector2("get_a", "set_a", "A", 0.01);
  mm_graph_node.add_slot_vector2("get_b", "set_b", "B", 0.01);
  mm_graph_node.add_slot_float("get_width", "set_width", "Width", 0.01);
  mm_graph_node.add_slot_curve();
}


 float SdShapeLine::_get_property_value(const Vector2 &uv) {
   Vector2 line = MMAlgos.sdf_line(uv, A, B, width);
  //$(name_uv)_sdl.x - $r * $profile($(name_uv)_sdl.y);
  return line.x - width * MMAlgos.curve(line.y, points_array);
}

 //a;

 Vector2 SdShapeLine::get_a() {
  return A;
}


 void SdShapeLine::set_a(const Vector2 &val) {
  A = val;
  emit_changed();
  output.emit_changed();
}

 //b;

 Vector2 SdShapeLine::get_b() {
  return B;
}


 void SdShapeLine::set_b(const Vector2 &val) {
  B = val;
  emit_changed();
  output.emit_changed();
}

 //width;

 float SdShapeLine::get_width() {
  return width;
}


 void SdShapeLine::set_width(const float val) {
  width = val;
  emit_changed();
  output.emit_changed();
}


 void SdShapeLine::_curve_changed() {
  emit_changed();
  output.emit_changed();
}

}

 SdShapeLine::SdShapeLine() {
  output;
  A = Vector2(-0.3, -0.3);
  B = Vector2(0.3, 0.3);
  width = 0.1;
 }

 SdShapeLine::~SdShapeLine() {
 }


 static void SdShapeLine::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdShapeLine::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdShapeLine::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_A"), &SdShapeLine::get_A);
   ClassDB::bind_method(D_METHOD("set_A", "value"), &SdShapeLine::set_A);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "A"), "set_A", "get_A");


   ClassDB::bind_method(D_METHOD("get_B"), &SdShapeLine::get_B);
   ClassDB::bind_method(D_METHOD("set_B", "value"), &SdShapeLine::set_B);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "B"), "set_B", "get_B");


   ClassDB::bind_method(D_METHOD("get_width"), &SdShapeLine::get_width);
   ClassDB::bind_method(D_METHOD("set_width", "value"), &SdShapeLine::set_width);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");


  ClassDB::bind_method(D_METHOD("_init"), &SdShapeLine::_init);
  ClassDB::bind_method(D_METHOD("_init_properties"), &SdShapeLine::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdShapeLine::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdShapeLine::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_a"), &SdShapeLine::get_a);
  ClassDB::bind_method(D_METHOD("set_a", "val"), &SdShapeLine::set_a);
  ClassDB::bind_method(D_METHOD("get_b"), &SdShapeLine::get_b);
  ClassDB::bind_method(D_METHOD("set_b", "val"), &SdShapeLine::set_b);
  ClassDB::bind_method(D_METHOD("get_width"), &SdShapeLine::get_width);
  ClassDB::bind_method(D_METHOD("set_width", "val"), &SdShapeLine::set_width);
  ClassDB::bind_method(D_METHOD("_curve_changed"), &SdShapeLine::_curve_changed);

 }



