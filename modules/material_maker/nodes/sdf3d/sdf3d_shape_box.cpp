
#include "sdf3d_shape_box.h"


Ref<Resource> Sdf3dShapeBox::get_output() {
 return output;
}

void Sdf3dShapeBox::set_output(const Ref<Resource> &val) {
output = val;
}


Vector3 Sdf3dShapeBox::get_size() {
 return size;
}

void Sdf3dShapeBox::set_size(const Vector3 &val) {
size = val;
}


float Sdf3dShapeBox::get_radius() const {
 return radius;
}

void Sdf3dShapeBox::set_radius(const float val) {
radius = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(Vector3) ;
  Vector3 size = Vector3(0.3, 0.25, 0.25);
 //export(float) ;
  float radius = 0.01;

 void Sdf3dShapeBox::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_output_property(output);
}


 void Sdf3dShapeBox::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_vector3("get_size", "set_size", "Size", 0.01);
  mm_graph_node.add_slot_float("get_radius", "set_radius", "Radius", 0.01);
}


 Vector2 Sdf3dShapeBox::_get_property_value_sdf3d(const Vector3 &uv3) {
  return MMAlgos.sdf3d_box(uv3, size.x, size.y, size.z, radius);
}

 //size;

 Vector3 Sdf3dShapeBox::get_size() {
  return size;
}


 void Sdf3dShapeBox::set_size(const Vector3 &val) {
  size = val;
  emit_changed();
  output.emit_changed();
}

 //radius;

 float Sdf3dShapeBox::get_radius() {
  return radius;
}


 void Sdf3dShapeBox::set_radius(const float val) {
  radius = val;
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dShapeBox::Sdf3dShapeBox() {
  output;
  size = Vector3(0.3, 0.25, 0.25);
  radius = 0.01;
 }

 Sdf3dShapeBox::~Sdf3dShapeBox() {
 }


 static void Sdf3dShapeBox::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dShapeBox::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dShapeBox::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_size"), &Sdf3dShapeBox::get_size);
   ClassDB::bind_method(D_METHOD("set_size", "value"), &Sdf3dShapeBox::set_size);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "size"), "set_size", "get_size");


   ClassDB::bind_method(D_METHOD("get_radius"), &Sdf3dShapeBox::get_radius);
   ClassDB::bind_method(D_METHOD("set_radius", "value"), &Sdf3dShapeBox::set_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dShapeBox::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dShapeBox::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dShapeBox::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_size"), &Sdf3dShapeBox::get_size);
  ClassDB::bind_method(D_METHOD("set_size", "val"), &Sdf3dShapeBox::set_size);
  ClassDB::bind_method(D_METHOD("get_radius"), &Sdf3dShapeBox::get_radius);
  ClassDB::bind_method(D_METHOD("set_radius", "val"), &Sdf3dShapeBox::set_radius);

 }



