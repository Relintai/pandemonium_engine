#ifndef SD_OP_BOOL_H
#define SD_OP_BOOL_H


class SdOpBool : public MMNode {
 GDCLASS(SdOpBool, MMNode);

 public:

 Ref<Resource> get_input1();
 void set_input1(const Ref<Resource> &val);

 Ref<Resource> get_input2();
 void set_input2(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 int get_operation() const;
 void set_operation(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 float _get_property_value(const Vector2 &uv);
 int get_operation();
 void set_operation(const int val);
 void on_input_changed();

 SdOpBool();
 ~SdOpBool();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input1;
 //export(Resource) 
 Ref<Resource> input2;
 //export(Resource) 
 Ref<Resource> output;
 //export(int, "Union,Substraction,Intersection") 
 int operation = 0;
 //operation
};


#endif
