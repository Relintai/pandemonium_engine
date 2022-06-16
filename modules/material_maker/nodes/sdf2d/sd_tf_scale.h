#ifndef SD_TF_SCALE_H
#define SD_TF_SCALE_H


class SdTfScale : public MMNode {
 GDCLASS(SdTfScale, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 float get_scale() const;
 void set_scale(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _get_property_value(const Vector2 &uv);
 float get_scale();
 void set_scale(const float val);

 SdTfScale();
 ~SdTfScale();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(float) 
 float scale = 1;
 //scale
};


#endif
