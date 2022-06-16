#ifndef SD_SHAPE_CIRCLE_H
#define SD_SHAPE_CIRCLE_H


class SdShapeCircle : public MMNode {
 GDCLASS(SdShapeCircle, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 Vector2 get_center();
 void set_center(const Vector2 &val);

 float get_radius() const;
 void set_radius(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 float _get_property_value(const Vector2 &uv);
 Vector2 get_center();
 void set_center(const Vector2 &val);
 float get_radius();
 void set_radius(const float val);

 SdShapeCircle();
 ~SdShapeCircle();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(Vector2) 
 Vector2 center = Vector2(0, 0);
 //export(float) 
 float radius = 0.4;
 //center
 //radius
};


#endif
