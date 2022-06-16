#ifndef SD_OP_CIRCLE_REPEAT_H
#define SD_OP_CIRCLE_REPEAT_H


class SdOpCircleRepeat : public MMNode {
 GDCLASS(SdOpCircleRepeat, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 int get_count() const;
 void set_count(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _get_property_value(const Vector2 &uv);
 int get_count();
 void set_count(const int val);

 SdOpCircleRepeat();
 ~SdOpCircleRepeat();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(int) 
 int count = 6;
 //count
};


#endif
