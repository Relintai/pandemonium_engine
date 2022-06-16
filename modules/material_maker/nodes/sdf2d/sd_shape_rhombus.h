#ifndef SD_SHAPE_RHOMBUS_H
#define SD_SHAPE_RHOMBUS_H


class SdShapeRhombus : public MMNode {
 GDCLASS(SdShapeRhombus, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 Vector2 get_center();
 void set_center(const Vector2 &val);

 Vector2 get_size();
 void set_size(const Vector2 &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 float _get_property_value(const Vector2 &uv);
 Vector2 get_center();
 void set_center(const Vector2 &val);
 Vector2 get_size();
 void set_size(const Vector2 &val);

 SdShapeRhombus();
 ~SdShapeRhombus();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(Vector2) 
 Vector2 center = Vector2(0, 0);
 //export(Vector2) 
 Vector2 size = Vector2(0.3, 0.2);
 //center
 //size
};


#endif
