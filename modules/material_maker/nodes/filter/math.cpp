
#include "math.h"


Ref<Resource> Math::get_image() {
 return image;
}

void Math::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> Math::get_a() {
 return a;
}

void Math::set_a(const Ref<Resource> &val) {
a = val;
}


Ref<Resource> Math::get_b() {
 return b;
}

void Math::set_b(const Ref<Resource> &val) {
b = val;
}


Ref<Resource> Math::get_output() {
 return output;
}

void Math::set_output(const Ref<Resource> &val) {
output = val;
}


int Math::get_operation() const {
 return operation;
}

void Math::set_operation(const int val) {
operation = val;
}


bool Math::get_clamp_result() const {
 return clamp_result;
}

void Math::set_clamp_result(const bool val) {
clamp_result = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> a;
 //export(Resource) ;
  Ref<Resource> b;
 //export(Resource) ;
  Ref<Resource> output;
 //export(int, "A+B,A-B,A*B,A/B,log(A),log2(A),pow(A; B),abs(A),round(A),floor(A),ceil(A),trunc(A),fract(A),min(A; B),max(A; B),A<B,cos(A*B),sin(A*B),tan(A*B),sqrt(1-A*A)") ;
  int operation = 0;
 //export(bool) ;
  bool clamp_result = false;

 void Math::_init_properties() {

  if (!a) {
   a = MMNodeUniversalProperty.new();
   a.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   a.set_default_value(0);
}

  a.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  a.slot_name = ">>>    A    ";
  a.value_step = 0.01;
  a.value_range = Vector2(0, 1);

  if (!b) {
   b = MMNodeUniversalProperty.new();
   b.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   b.set_default_value(0);
}

  b.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  b.slot_name = ">>>    B    ";
  b.value_step = 0.01;
  b.value_range = Vector2(0, 1);

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  //image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  //image.force_override = true;

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   output.set_default_value(0);
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  output.slot_name = "     Output    >>>";
  output.get_value_from_owner = true;
  register_input_property(a);
  register_input_property(b);
  register_output_property(output);
  register_output_property(image);
}


 void Math::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(a);
  mm_graph_node.add_slot_label_universal(b);
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_operation", "set_operation", "Operation", [ "A+B", "A-B", "A*B", "A/B", "log(A)", "log2(A)", "pow(A, B)", "abs(A)", "round(A)", "floor(A)", "ceil(A)", "trunc(A)", "fract(A)", "min(A, B)", "max(A, B)", "A<B", "cos(A*B)", "sin(A*B)", "tan(A*B)", "sqrt(1-A*A)" ]);
  mm_graph_node.add_slot_bool("get_clamp_result", "set_clamp_result", "Clamp result");
}


 float Math::_get_property_value(const Vector2 &uv) {
   float af = a.get_value(uv);
   float bf = b.get_value(uv);
   float f = 0;
  //"A+B",;

  if (operation == 0) {
   f = af + bf;
}

  //"A-B",;

  else if (operation == 1) {
   f = af - bf;
}

  //"A*B",;

  else if (operation == 2) {
   f = af * bf;
}

  //"A/B",;

  else if (operation == 3) {

   if (bf == 0) {
    bf = 0.000001;
}

   f = af / bf;
}

  //"log(A)",;

  else if (operation == 4) {
   //todo needs to be implemented;
   f = log(af);
}

  //"log2(A)",;

  else if (operation == 5) {
   //todo needs to be implemented;
   f = log(af);
}

  //"pow(A, B)",;

  else if (operation == 6) {
   f = pow(af, bf);
}

  //"abs(A)",;

  else if (operation == 7) {
   f = abs(af);
}

  //"round(A)",;

  else if (operation == 8) {
   f = round(af);
}

  //"floor(A)",;

  else if (operation == 9) {
   f = floor(af);
}

  //"ceil(A)",;

  else if (operation == 10) {
   f = ceil(af);
}

  //"trunc(A)",;

  else if (operation == 11) {
   f = int(af);
}

  //"fract(A)",;

  else if (operation == 12) {
   f = MMAlgos.fractf(af);
}

  //"min(A, B)",;

  else if (operation == 13) {
   f = min(af, bf);
}

  //"max(A, B)",;

  else if (operation == 14) {
   f = max(af, bf);
}

  //"A<B",;

  else if (operation == 15) {
   f = af < bf;
}

  //"cos(A*B)",;

  else if (operation == 16) {
   f = cos(af * bf);
}

  //"sin(A*B)",;

  else if (operation == 17) {
   f = sin(af * bf);
}

  //"tan(A*B)",;

  else if (operation == 18) {
   f = tan(af * bf);
}

  //"sqrt(1-A*A)";

  else if (operation == 19) {
   f = sqrt(1 - af * af);
}


  if (clamp_result) {
   f = clamp(f, 0, 1);
}

  return f;
}


 void Math::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Math::_get_value_for(const Vector2 &uv, const int pseed) {
   float f = get_property_value(uv);
  return Color(f, f, f, 1);
}

 //operation;

 int Math::get_operation() {
  return operation;
}


 void Math::set_operation(const int val) {
  operation = val;
  set_dirty(true);
  output.emit_changed();
}

 //clamp_result;

 bool Math::get_clamp_result() {
  return clamp_result;
}


 void Math::set_clamp_result(const bool val) {
  clamp_result = val;
  set_dirty(true);
  output.emit_changed();
}

}

 Math::Math() {
  image;
  a;
  b;
  output;
  operation = 0;
  clamp_result = false;
 }

 Math::~Math() {
 }


 static void Math::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Math::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Math::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_a"), &Math::get_a);
   ClassDB::bind_method(D_METHOD("set_a", "value"), &Math::set_a);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "a", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_a", "get_a");


   ClassDB::bind_method(D_METHOD("get_b"), &Math::get_b);
   ClassDB::bind_method(D_METHOD("set_b", "value"), &Math::set_b);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "b", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_b", "get_b");


   ClassDB::bind_method(D_METHOD("get_output"), &Math::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &Math::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_operation"), &Math::get_operation);
   ClassDB::bind_method(D_METHOD("set_operation", "value"), &Math::set_operation);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "operation"), "set_operation", "get_operation");


   ClassDB::bind_method(D_METHOD("get_clamp_result"), &Math::get_clamp_result);
   ClassDB::bind_method(D_METHOD("set_clamp_result", "value"), &Math::set_clamp_result);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "clamp_result"), "set_clamp_result", "get_clamp_result");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Math::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Math::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &Math::_get_property_value);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Math::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Math::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_operation"), &Math::get_operation);
  ClassDB::bind_method(D_METHOD("set_operation", "val"), &Math::set_operation);
  ClassDB::bind_method(D_METHOD("get_clamp_result"), &Math::get_clamp_result);
  ClassDB::bind_method(D_METHOD("set_clamp_result", "val"), &Math::set_clamp_result);

 }



