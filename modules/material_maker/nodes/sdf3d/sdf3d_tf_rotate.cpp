
#include "sdf3d_tf_rotate.h"


Ref<Resource> Sdf3dTfRotate::get_input() {
 return input;
}

void Sdf3dTfRotate::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> Sdf3dTfRotate::get_output() {
 return output;
}

void Sdf3dTfRotate::set_output(const Ref<Resource> &val) {
output = val;
}


Vector3 Sdf3dTfRotate::get_rotation() {
 return rotation;
}

void Sdf3dTfRotate::set_rotation(const Vector3 &val) {
rotation = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> output;
 //export(Vector3) ;
  Vector3 rotation = Vector3();

 void Sdf3dTfRotate::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  //	input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_VECTOR2;
  input.slot_name = ">>>   Input        ";

  if (!input.is_connected("changed", self, "on_input_changed")) {
   input.connect("changed", self, "on_input_changed");
}


  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_input_property(input);
  register_output_property(output);
}


 void Sdf3dTfRotate::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_vector3("get_rotation", "set_rotation", "Rotation", 0.01);
}


 Vector2 Sdf3dTfRotate::_get_property_value_sdf3d(const Vector3 &uv3) {
  //$in(rotate3d($uv, -vec3($ax, $ay, $az)*0.01745329251));
  return input.get_value_sdf3d(MMAlgos.rotate3d(uv3, -rotation * 0.01745329251));
}

 //rotation;

 Vector3 Sdf3dTfRotate::get_rotation() {
  return rotation;
}


 void Sdf3dTfRotate::set_rotation(const Vector3 &val) {
  rotation = val;
  emit_changed();
  output.emit_changed();
}


 void Sdf3dTfRotate::on_input_changed() {
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dTfRotate::Sdf3dTfRotate() {
  input;
  output;
  rotation = Vector3();
 }

 Sdf3dTfRotate::~Sdf3dTfRotate() {
 }


 static void Sdf3dTfRotate::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &Sdf3dTfRotate::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Sdf3dTfRotate::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dTfRotate::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dTfRotate::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_rotation"), &Sdf3dTfRotate::get_rotation);
   ClassDB::bind_method(D_METHOD("set_rotation", "value"), &Sdf3dTfRotate::set_rotation);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "rotation"), "set_rotation", "get_rotation");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dTfRotate::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dTfRotate::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dTfRotate::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_rotation"), &Sdf3dTfRotate::get_rotation);
  ClassDB::bind_method(D_METHOD("set_rotation", "val"), &Sdf3dTfRotate::set_rotation);
  ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dTfRotate::on_input_changed);

 }



