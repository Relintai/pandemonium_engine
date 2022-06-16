#ifndef SHAPE_H
#define SHAPE_H


class Shape : public MMNode {
 GDCLASS(Shape, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 int get_shape_type() const;
 void set_shape_type(const int val);

 int get_sides() const;
 void set_sides(const int val);

 Ref<Resource> get_radius();
 void set_radius(const Ref<Resource> &val);

 Ref<Resource> get_edge();
 void set_edge(const Ref<Resource> &val);

 enum ShapeType  {

  SHAPE_TYPE_CIRCLE = 0,
  SHAPE_TYPE_POLYGON = 1,
  SHAPE_TYPE_STAR = 2,
  SHAPE_TYPE_CURVED_STAR = 3,
  SHAPE_TYPE_RAYS = 4,
};

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_shape_typoe();
 void set_shape_typoe(const int val);
 int get_sides();
 void set_sides(const int val);

 Shape();
 ~Shape();

 protected:
 static void _bind_methods();

 //tool
 };
 //export(Resource) 
 Ref<Resource> image;
 //export(int, "Circle,Polygon,Star,Curved Star,Rays") 
 int shape_type = 0;
 //export(int) 
 int sides = 6;
 //export(Resource) 
 Ref<Resource> radius;
 //export(Resource) 
 Ref<Resource> edge;
};


#endif
