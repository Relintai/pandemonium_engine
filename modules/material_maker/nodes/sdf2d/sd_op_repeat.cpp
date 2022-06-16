
#include "sd_op_repeat.h"


Ref<Resource> SdOpRepeat::get_output() {
 return output;
}

void SdOpRepeat::set_output(const Ref<Resource> &val) {
output = val;
}


int SdOpRepeat::get_x() const {
 return x;
}

void SdOpRepeat::set_x(const int val) {
x = val;
}


int SdOpRepeat::get_y() const {
 return y;
}

void SdOpRepeat::set_y(const int val) {
y = val;
}


float SdOpRepeat::get_random_rotation() const {
 return random_rotation;
}

void SdOpRepeat::set_random_rotation(const float val) {
random_rotation = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(int) ;
  int x = 3;
 //export(int) ;
  int y = 3;
 //export(float) ;
  float random_rotation = 0.5;

 void SdOpRepeat::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  output.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  //output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>    Apply    >>>";
  output.get_value_from_owner = true;
  register_input_property(output);
  register_output_property(output);
}


 void SdOpRepeat::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_int("get_x", "set_x", "X");
  mm_graph_node.add_slot_int("get_y", "set_y", "Y");
  mm_graph_node.add_slot_float("get_random_rotation", "set_random_rotation", "Random rotation", 0.01);
}


 void SdOpRepeat::_get_property_value(const Vector2 &uv) {
  //todo add this as a parameter;
   int pseed = 123123;
  //$in(repeat_2d($uv, vec2(1.0/$rx, 1.0/$ry), float($seed), $r));
   Vector2 new_uv = MMAlgos.repeat_2d(uv, Vector2(1.0 / float(x), 1.0/ float(y)), 1.0/float(pseed), random_rotation);
  return output.get_value(new_uv, true);
}

 //x;

 int SdOpRepeat::get_x() {
  return x;
}


 void SdOpRepeat::set_x(const int val) {
  x = val;
  emit_changed();
  output.emit_changed();
}

 //y;

 int SdOpRepeat::get_y() {
  return y;
}


 void SdOpRepeat::set_y(const int val) {
  y = val;
  emit_changed();
  output.emit_changed();
}

 //random_rotation;

 float SdOpRepeat::get_random_rotation() {
  return random_rotation;
}


 void SdOpRepeat::set_random_rotation(const float val) {
  random_rotation = val;
  emit_changed();
  output.emit_changed();
}

}

 SdOpRepeat::SdOpRepeat() {
  output;
  x = 3;
  y = 3;
  random_rotation = 0.5;
 }

 SdOpRepeat::~SdOpRepeat() {
 }


 static void SdOpRepeat::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdOpRepeat::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdOpRepeat::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_x"), &SdOpRepeat::get_x);
   ClassDB::bind_method(D_METHOD("set_x", "value"), &SdOpRepeat::set_x);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "x"), "set_x", "get_x");


   ClassDB::bind_method(D_METHOD("get_y"), &SdOpRepeat::get_y);
   ClassDB::bind_method(D_METHOD("set_y", "value"), &SdOpRepeat::set_y);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "y"), "set_y", "get_y");


   ClassDB::bind_method(D_METHOD("get_random_rotation"), &SdOpRepeat::get_random_rotation);
   ClassDB::bind_method(D_METHOD("set_random_rotation", "value"), &SdOpRepeat::set_random_rotation);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "random_rotation"), "set_random_rotation", "get_random_rotation");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdOpRepeat::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdOpRepeat::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdOpRepeat::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_x"), &SdOpRepeat::get_x);
  ClassDB::bind_method(D_METHOD("set_x", "val"), &SdOpRepeat::set_x);
  ClassDB::bind_method(D_METHOD("get_y"), &SdOpRepeat::get_y);
  ClassDB::bind_method(D_METHOD("set_y", "val"), &SdOpRepeat::set_y);
  ClassDB::bind_method(D_METHOD("get_random_rotation"), &SdOpRepeat::get_random_rotation);
  ClassDB::bind_method(D_METHOD("set_random_rotation", "val"), &SdOpRepeat::set_random_rotation);

 }



