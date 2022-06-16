
#include "repeat.h"


Ref<Resource> Repeat::get_input() {
 return input;
}

void Repeat::set_input(const Ref<Resource> &val) {
input = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;

 void Repeat::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color(0, 0, 0, 1));
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Apply    >>>";
  //input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_COLOR;
  input.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.get_value_from_owner = true;
  register_input_property(input);
  register_output_property(input);
}


 void Repeat::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
}


 void Repeat::_get_property_value(const Vector2 &uv) {
  return input.get_value(MMAlgos.fractv2(uv), true);
}

}

 Repeat::Repeat() {
  input;
 }

 Repeat::~Repeat() {
 }


 static void Repeat::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Repeat::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Repeat::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Repeat::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Repeat::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &Repeat::_get_property_value);

 }



