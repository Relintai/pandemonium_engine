
#include "shape.h"


Ref<Resource> Shape::get_image() {
 return image;
}

void Shape::set_image(const Ref<Resource> &val) {
image = val;
}


int Shape::get_shape_type() const {
 return shape_type;
}

void Shape::set_shape_type(const int val) {
shape_type = val;
}


int Shape::get_sides() const {
 return sides;
}

void Shape::set_sides(const int val) {
sides = val;
}


Ref<Resource> Shape::get_radius() {
 return radius;
}

void Shape::set_radius(const Ref<Resource> &val) {
radius = val;
}


Ref<Resource> Shape::get_edge() {
 return edge;
}

void Shape::set_edge(const Ref<Resource> &val) {
edge = val;
}



 //tool;
 };
 //export(Resource) ;
  Ref<Resource> image;
 //export(int, "Circle,Polygon,Star,Curved Star,Rays") ;
  int shape_type = 0;
 //export(int) ;
  int sides = 6;
 //export(Resource) ;
  Ref<Resource> radius;
 //export(Resource) ;
  Ref<Resource> edge;

 void Shape::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!radius) {
   radius = MMNodeUniversalProperty.new();
   radius.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   radius.set_default_value(0.34375);
}

  radius.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  radius.slot_name = "radius";
  radius.value_step = 0.05;

  if (!edge) {
   edge = MMNodeUniversalProperty.new();
   edge.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   edge.set_default_value(0.2);
}

  edge.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  edge.slot_name = "edge";
  edge.value_step = 0.05;
  register_input_property(radius);
  register_input_property(edge);
  register_output_property(image);
}


 void Shape::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_enum("get_shape_typoe", "set_shape_typoe", "shape_type", [ "Circle", "Polygon", "Star", "Curved Star", "Rays" ]);
  //, Vector2(1, 10));
  mm_graph_node.add_slot_int("get_sides", "set_sides", "sides");
  mm_graph_node.add_slot_float_universal(radius);
  mm_graph_node.add_slot_float_universal(edge);
}


 void Shape::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color Shape::_get_value_for(const Vector2 &uv, const int pseed) {
   float c = 0;
   float rad = radius.get_value(uv);
   float edg = edge.get_value(uv);

  if (rad == 0) {
   rad = 0.0000001;
}


  if (edg == 0) {
   edg = 0.0000001;
}


  if (shape_type == ShapeType.SHAPE_TYPE_CIRCLE) {
   c = MMAlgos.shape_circle(uv, sides, rad, edg);
}


  else if (shape_type == ShapeType.SHAPE_TYPE_POLYGON) {
   c = MMAlgos.shape_polygon(uv, sides, rad, edg);
}


  else if (shape_type == ShapeType.SHAPE_TYPE_STAR) {
   c = MMAlgos.shape_star(uv, sides, rad, edg);
}


  else if (shape_type == ShapeType.SHAPE_TYPE_CURVED_STAR) {
   c = MMAlgos.shape_curved_star(uv, sides, rad, edg);
}


  else if (shape_type == ShapeType.SHAPE_TYPE_RAYS) {
   c = MMAlgos.shape_rays(uv, sides, rad, edg);
}

  return Color(c, c, c, 1);
}


 int Shape::get_shape_typoe() {
  return shape_type;
}


 void Shape::set_shape_typoe(const int val) {
  shape_type = val;
  set_dirty(true);
}


 int Shape::get_sides() {
  return sides;
}


 void Shape::set_sides(const int val) {
  sides = val;
  set_dirty(true);
}

}

 Shape::Shape() {
  image;
  shape_type = 0;
  sides = 6;
  radius;
  edge;
 }

 Shape::~Shape() {
 }


 static void Shape::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &Shape::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &Shape::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_shape_type"), &Shape::get_shape_type);
   ClassDB::bind_method(D_METHOD("set_shape_type", "value"), &Shape::set_shape_type);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "shape_type"), "set_shape_type", "get_shape_type");


   ClassDB::bind_method(D_METHOD("get_sides"), &Shape::get_sides);
   ClassDB::bind_method(D_METHOD("set_sides", "value"), &Shape::set_sides);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "sides"), "set_sides", "get_sides");


   ClassDB::bind_method(D_METHOD("get_radius"), &Shape::get_radius);
   ClassDB::bind_method(D_METHOD("set_radius", "value"), &Shape::set_radius);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "radius", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_radius", "get_radius");


   ClassDB::bind_method(D_METHOD("get_edge"), &Shape::get_edge);
   ClassDB::bind_method(D_METHOD("set_edge", "value"), &Shape::set_edge);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "edge", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_edge", "get_edge");


  ClassDB::bind_method(D_METHOD("_init_properties"), &Shape::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &Shape::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &Shape::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &Shape::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_shape_typoe"), &Shape::get_shape_typoe);
  ClassDB::bind_method(D_METHOD("set_shape_typoe", "val"), &Shape::set_shape_typoe);
  ClassDB::bind_method(D_METHOD("get_sides"), &Shape::get_sides);
  ClassDB::bind_method(D_METHOD("set_sides", "val"), &Shape::set_sides);

 }



