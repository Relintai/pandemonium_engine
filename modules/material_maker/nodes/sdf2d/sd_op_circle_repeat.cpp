
#include "sd_op_circle_repeat.h"


Ref<Resource> SdOpCircleRepeat::get_output() {
 return output;
}

void SdOpCircleRepeat::set_output(const Ref<Resource> &val) {
output = val;
}


int SdOpCircleRepeat::get_count() const {
 return count;
}

void SdOpCircleRepeat::set_count(const int val) {
count = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(int) ;
  int count = 6;

 void SdOpCircleRepeat::_init_properties() {

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


 void SdOpCircleRepeat::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_int("get_count", "set_count", "Count");
}


 void SdOpCircleRepeat::_get_property_value(const Vector2 &uv) {
  //$in(circle_repeat_transform_2d($uv-vec2(0.5), $c)+vec2(0.5));
   Vector2 new_uv = MMAlgos.circle_repeat_transform_2d(uv - Vector2(0.5, 0.5), count) + Vector2(0.5, 0.5);
  return output.get_value(new_uv, true);
}

 //count;

 int SdOpCircleRepeat::get_count() {
  return count;
}


 void SdOpCircleRepeat::set_count(const int val) {
  count = val;
  emit_changed();
  output.emit_changed();
}

}

 SdOpCircleRepeat::SdOpCircleRepeat() {
  output;
  count = 6;
 }

 SdOpCircleRepeat::~SdOpCircleRepeat() {
 }


 static void SdOpCircleRepeat::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdOpCircleRepeat::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdOpCircleRepeat::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_count"), &SdOpCircleRepeat::get_count);
   ClassDB::bind_method(D_METHOD("set_count", "value"), &SdOpCircleRepeat::set_count);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "count"), "set_count", "get_count");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdOpCircleRepeat::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdOpCircleRepeat::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdOpCircleRepeat::_get_property_value);
  ClassDB::bind_method(D_METHOD("get_count"), &SdOpCircleRepeat::get_count);
  ClassDB::bind_method(D_METHOD("set_count", "val"), &SdOpCircleRepeat::set_count);

 }



