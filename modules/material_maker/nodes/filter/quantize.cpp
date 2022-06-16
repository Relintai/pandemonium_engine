
#include "quantize.h"


Ref<Resource> Quantize::get_image() {
 return image;
}

void Quantize::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Quantize::get_input() {
 return input;
}

void Quantize::set_input(const Ref<Resource> &val) {
input = val;
}


int Quantize::get_steps() const {
 return steps;
}

void Quantize::set_steps(const int val) {
steps = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(int) ;
  int steps = 4;

 void Quantize::_init_properties() {

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
  register_input_property(input);
  register_output_property(image);
}


 void Quantize::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_int("get_steps", "set_steps", "Steps");
}


 void Quantize::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Quantize::_get_value_for(const Vector2 &uv, const int pseed) {
   Color c = input.get_value(uv);
  //vec4(floor($in($uv).rgb*$steps)/$steps, $in($uv).a);
   Vector3 v = MMAlgos.floorv3(Vector3(c.r, c.g, c.b) * steps) / float(steps);
  return Color(v.x, v.y, v.z, c.a);
}

 //steps;

 int Quantize::get_steps() {
  return steps;
}


 void Quantize::set_steps(const int val) {
  steps = val;
  set_dirty(true);
}

}

 Quantize::Quantize() {
  image;
  input;
  steps = 4;
 }

 Quantize::~Quantize() {
 }


 static void Quantize::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Quantize::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Quantize::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &Quantize::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &Quantize::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_steps"), &Quantize::get_steps);
   ClassDB::bind_method(D_METHOD("set_steps", "value"), &Quantize::set_steps);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "steps"), "set_steps", "get_steps");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Quantize::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Quantize::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Quantize::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Quantize::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_steps"), &Quantize::get_steps);
  ClassDB::bind_method(D_METHOD("set_steps", "val"), &Quantize::set_steps);

 }



