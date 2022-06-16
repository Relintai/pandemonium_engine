#ifndef SD_OP_ANNULAR_SHAPE_H
#define SD_OP_ANNULAR_SHAPE_H


class SdOpAnnularShape : public MMNode {
 GDCLASS(SdOpAnnularShape, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 float get_width() const;
 void set_width(const float val);

 int get_ripples() const;
 void set_ripples(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _get_property_value(const Vector2 &uv);
 float get_width();
 void set_width(const float val);
 int get_ripples();
 void set_ripples(const int val);

 SdOpAnnularShape();
 ~SdOpAnnularShape();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(float) 
 float width = 0.1;
 //export(int) 
 int ripples = 1;
 //width
 //ripples
};


#endif
