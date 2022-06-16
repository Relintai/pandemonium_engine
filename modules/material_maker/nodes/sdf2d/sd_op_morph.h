#ifndef SD_OP_MORPH_H
#define SD_OP_MORPH_H


class SdOpMorph : public MMNode {
 GDCLASS(SdOpMorph, MMNode);

 public:

 Ref<Resource> get_input1();
 void set_input1(const Ref<Resource> &val);

 Ref<Resource> get_input2();
 void set_input2(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 float get_amount() const;
 void set_amount(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 float _get_property_value(const Vector2 &uv);
 float get_amount();
 void set_amount(const float val);
 void on_input_changed();

 SdOpMorph();
 ~SdOpMorph();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input1;
 //export(Resource) 
 Ref<Resource> input2;
 //export(Resource) 
 Ref<Resource> output;
 //export(float) 
 float amount = 0.5;
 //amount
};


#endif
