
#include "sdf3d_shape_sphere.h"


Ref<Resource> Sdf3dShapeSphere::get_output() {
 return output;
}

void Sdf3dShapeSphere::set_output(const Ref<Resource> &val) {
output = val;
}


float Sdf3dShapeSphere::get_radius() const {
 return radius;
}

void Sdf3dShapeSphere::set_radius(const float val) {
radius = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;
 //export(float) ;
  float radius = 0.5;

 void Sdf3dShapeSphere::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_VECTOR2;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_output_property(output);
}


 void Sdf3dShapeSphere::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_float("get_radius", "set_radius", "Radius", 0.01);
}


 Vector2 Sdf3dShapeSphere::_get_property_value_sdf3d(const Vector3 &uv3) {
  return MMAlgos.sdf3d_sphere(uv3, radius);
}

 //radius;

 float Sdf3dShapeSphere::get_radius() {
  return radius;
}


 void Sdf3dShapeSphere::set_radius(const float val) {
  radius = val;
  emit_changed();
  output.emit_changed();
}

}

 Sdf3dShapeSphere::Sdf3dShapeSphere() {
  output;
  radius = 0.5;
 }

 Sdf3dShapeSphere::~Sdf3dShapeSphere() {
 }


 static void Sdf3dShapeSphere::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dShapeSphere::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dShapeSphere::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_radius"), &Sdf3dShapeSphere::get_radius);
   ClassDB::bind_method(D_METHOD("set_radius", "value"), &Sdf3dShapeSphere::set_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Sdf3dShapeSphere::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Sdf3dShapeSphere::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value_sdf3d", "uv3"), &Sdf3dShapeSphere::_get_property_value_sdf3d);
  ClassDB::bind_method(D_METHOD("get_radius"), &Sdf3dShapeSphere::get_radius);
  ClassDB::bind_method(D_METHOD("set_radius", "val"), &Sdf3dShapeSphere::set_radius);

 }



