#ifndef SD_SHAPE_ARC_H
#define SD_SHAPE_ARC_H


class SdShapeArc : public MMNode {
 GDCLASS(SdShapeArc, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 Vector2 get_angle();
 void set_angle(const Vector2 &val);

 float get_radius() const;
 void set_radius(const float val);

 float get_width() const;
 void set_width(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 float _get_property_value(const Vector2 &uv);
 Vector2 get_angle();
 void set_angle(const Vector2 &val);
 float get_radius();
 void set_radius(const float val);
 float get_width();
 void set_width(const float val);

 SdShapeArc();
 ~SdShapeArc();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(Vector2) 
 Vector2 angle = Vector2(30, 150);
 //export(float) 
 float radius = 0.3;
 //export(float) 
 float width = 0.1;
 //angle
 //radius
 //width
};


#endif
