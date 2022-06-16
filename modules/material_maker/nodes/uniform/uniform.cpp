
#include "uniform.h"


Ref<Resource> Uniform::get_uniform() {
 return uniform;
}

void Uniform::set_uniform(const Ref<Resource> &val) {
uniform = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> uniform;

 void Uniform::_init_properties() {

  if (!uniform) {
   uniform = MMNodeUniversalProperty.new();
   uniform.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   uniform.set_default_value(Color(1, 1, 1, 1));
}

  uniform.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_COLOR;
  register_output_property(uniform);
}


 void Uniform::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_color_universal(uniform);
}


 Color Uniform::_get_value_for(const Vector2 &uv, const int pseed) {
  return uniform.get_value(uv);
}

}

 Uniform::Uniform() {
  uniform;
 }

 Uniform::~Uniform() {
 }


 static void Uniform::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_uniform"), &Uniform::get_uniform);
   ClassDB::bind_method(D_METHOD("set_uniform", "value"), &Uniform::set_uniform);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "uniform", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_uniform", "get_uniform");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Uniform::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Uniform::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Uniform::_get_value_for);

 }



