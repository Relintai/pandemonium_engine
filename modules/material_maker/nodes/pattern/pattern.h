#ifndef PATTERN_H
#define PATTERN_H


class Pattern : public MMNode {
 GDCLASS(Pattern, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 int get_combiner_type() const;
 void set_combiner_type(const int val);

 int get_combiner_axis_type_x() const;
 void set_combiner_axis_type_x(const int val);

 int get_combiner_axis_type_y() const;
 void set_combiner_axis_type_y(const int val);

 Vector2 get_repeat();
 void set_repeat(const Vector2 &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_combiner_type();
 void set_combiner_type(const int val);
 int get_combiner_axis_type_x();
 void set_combiner_axis_type_x(const int val);
 int get_combiner_axis_type_y();
 void set_combiner_axis_type_y(const int val);
 Vector2 get_repeat();
 void set_repeat(const Vector2 &val);

 Pattern();
 ~Pattern();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(int, "Multiply,Add,Max,Min,Xor,Pow") 
 int combiner_type = 0;
 //export(int, "Sine,Triangle,Square,Sawtooth,Constant,Bounce") 
 int combiner_axis_type_x = 0;
 //export(int, "Sine,Triangle,Square,Sawtooth,Constant,Bounce") 
 int combiner_axis_type_y = 0;
 //export(Vector2) 
 Vector2 repeat = Vector2(4, 4);
 //combiner_type
 //combiner_axis_type_x
 //combiner_axis_type_y
 //repeat
};


#endif
