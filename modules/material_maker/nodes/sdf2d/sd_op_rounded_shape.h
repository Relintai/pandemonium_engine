#ifndef SD_OP_ROUNDED_SHAPE_H
#define SD_OP_ROUNDED_SHAPE_H


class SdOpRoundedShape : public MMNode {
 GDCLASS(SdOpRoundedShape, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 float get_radius() const;
 void set_radius(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _get_property_value(const Vector2 &uv);
 float get_radius();
 void set_radius(const float val);

 SdOpRoundedShape();
 ~SdOpRoundedShape();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(float) 
 float radius = 0;
 //radius
};


#endif
