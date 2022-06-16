#ifndef SD_TF_ROTATE_H
#define SD_TF_ROTATE_H


class SdTfRotate : public MMNode {
 GDCLASS(SdTfRotate, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 float get_angle() const;
 void set_angle(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _get_property_value(const Vector2 &uv);
 float get_angle();
 void set_angle(const float val);

 SdTfRotate();
 ~SdTfRotate();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(float) 
 float angle = 0;
 //angle
};


#endif
