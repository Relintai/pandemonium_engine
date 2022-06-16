
#include "pattern.h"


Ref<Resource> Pattern::get_image() {
 return image;
}

void Pattern::set_image(const Ref<Resource> &val) {
image = val;
}


int Pattern::get_combiner_type() const {
 return combiner_type;
}

void Pattern::set_combiner_type(const int val) {
combiner_type = val;
}


int Pattern::get_combiner_axis_type_x() const {
 return combiner_axis_type_x;
}

void Pattern::set_combiner_axis_type_x(const int val) {
combiner_axis_type_x = val;
}


int Pattern::get_combiner_axis_type_y() const {
 return combiner_axis_type_y;
}

void Pattern::set_combiner_axis_type_y(const int val) {
combiner_axis_type_y = val;
}


Vector2 Pattern::get_repeat() {
 return repeat;
}

void Pattern::set_repeat(const Vector2 &val) {
repeat = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(int, "Multiply,Add,Max,Min,Xor,Pow") ;
  int combiner_type = 0;
 //export(int, "Sine,Triangle,Square,Sawtooth,Constant,Bounce") ;
  int combiner_axis_type_x = 0;
 //export(int, "Sine,Triangle,Square,Sawtooth,Constant,Bounce") ;
  int combiner_axis_type_y = 0;
 //export(Vector2) ;
  Vector2 repeat = Vector2(4, 4);

 void Pattern::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void Pattern::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_combiner_type", "set_combiner_type", "Combiner Type", [ "Multiply", "Add" , "Max", "Min", "Xor", "Pow" ]);
  mm_graph_node.add_slot_enum("get_combiner_axis_type_x", "set_combiner_axis_type_x", "Combiner Axis type", [ "Sine", "Triangle", "Square", "Sawtooth", "Constant", "Bounce" ]);
  mm_graph_node.add_slot_enum("get_combiner_axis_type_y", "set_combiner_axis_type_y", "", [ "Sine", "Triangle", "Square", "Sawtooth", "Constant", "Bounce" ]);
  //, Vector2(0, 32));
  mm_graph_node.add_slot_vector2("get_repeat", "set_repeat", "Repeat", 1);
}


 void Pattern::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Pattern::_get_value_for(const Vector2 &uv, const int pseed) {
   float f = MMAlgos.pattern(uv, repeat.x, repeat.y, combiner_type, combiner_axis_type_x, combiner_axis_type_y);
  return Color(f, f, f, 1);
}

 //combiner_type;

 int Pattern::get_combiner_type() {
  return combiner_type;
}


 void Pattern::set_combiner_type(const int val) {
  combiner_type = val;
  set_dirty(true);
}

 //combiner_axis_type_x;

 int Pattern::get_combiner_axis_type_x() {
  return combiner_axis_type_x;
}


 void Pattern::set_combiner_axis_type_x(const int val) {
  combiner_axis_type_x = val;
  set_dirty(true);
}

 //combiner_axis_type_y;

 int Pattern::get_combiner_axis_type_y() {
  return combiner_axis_type_y;
}


 void Pattern::set_combiner_axis_type_y(const int val) {
  combiner_axis_type_y = val;
  set_dirty(true);
}

 //repeat;

 Vector2 Pattern::get_repeat() {
  return repeat;
}


 void Pattern::set_repeat(const Vector2 &val) {
  repeat = val;
  set_dirty(true);
}

}

 Pattern::Pattern() {
  image;
  combiner_type = 0;
  combiner_axis_type_x = 0;
  combiner_axis_type_y = 0;
  repeat = Vector2(4, 4);
 }

 Pattern::~Pattern() {
 }


 static void Pattern::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Pattern::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Pattern::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_combiner_type"), &Pattern::get_combiner_type);
   ClassDB::bind_method(D_METHOD("set_combiner_type", "value"), &Pattern::set_combiner_type);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "combiner_type"), "set_combiner_type", "get_combiner_type");


   ClassDB::bind_method(D_METHOD("get_combiner_axis_type_x"), &Pattern::get_combiner_axis_type_x);
   ClassDB::bind_method(D_METHOD("set_combiner_axis_type_x", "value"), &Pattern::set_combiner_axis_type_x);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "combiner_axis_type_x"), "set_combiner_axis_type_x", "get_combiner_axis_type_x");


   ClassDB::bind_method(D_METHOD("get_combiner_axis_type_y"), &Pattern::get_combiner_axis_type_y);
   ClassDB::bind_method(D_METHOD("set_combiner_axis_type_y", "value"), &Pattern::set_combiner_axis_type_y);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "combiner_axis_type_y"), "set_combiner_axis_type_y", "get_combiner_axis_type_y");


   ClassDB::bind_method(D_METHOD("get_repeat"), &Pattern::get_repeat);
   ClassDB::bind_method(D_METHOD("set_repeat", "value"), &Pattern::set_repeat);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "repeat"), "set_repeat", "get_repeat");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Pattern::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Pattern::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Pattern::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Pattern::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_combiner_type"), &Pattern::get_combiner_type);
  ClassDB::bind_method(D_METHOD("set_combiner_type", "val"), &Pattern::set_combiner_type);
  ClassDB::bind_method(D_METHOD("get_combiner_axis_type_x"), &Pattern::get_combiner_axis_type_x);
  ClassDB::bind_method(D_METHOD("set_combiner_axis_type_x", "val"), &Pattern::set_combiner_axis_type_x);
  ClassDB::bind_method(D_METHOD("get_combiner_axis_type_y"), &Pattern::get_combiner_axis_type_y);
  ClassDB::bind_method(D_METHOD("set_combiner_axis_type_y", "val"), &Pattern::set_combiner_axis_type_y);
  ClassDB::bind_method(D_METHOD("get_repeat"), &Pattern::get_repeat);
  ClassDB::bind_method(D_METHOD("set_repeat", "val"), &Pattern::set_repeat);

 }



