#ifndef MATH_H
#define MATH_H


class Math : public MMNode {
 GDCLASS(Math, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_a();
 void set_a(const Ref<Resource> &val);

 Ref<Resource> get_b();
 void set_b(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 int get_operation() const;
 void set_operation(const int val);

 bool get_clamp_result() const;
 void set_clamp_result(const bool val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 float _get_property_value(const Vector2 &uv);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_operation();
 void set_operation(const int val);
 bool get_clamp_result();
 void set_clamp_result(const bool val);

 Math();
 ~Math();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> a;
 //export(Resource) 
 Ref<Resource> b;
 //export(Resource) 
 Ref<Resource> output;
 //export(int, "A+B,A-B,A*B,A/B,log(A),log2(A),pow(A; B),abs(A),round(A),floor(A),ceil(A),trunc(A),fract(A),min(A; B),max(A; B),A<B,cos(A*B),sin(A*B),tan(A*B),sqrt(1-A*A)") 
 int operation = 0;
 //export(bool) 
 bool clamp_result = false;
 //operation
 //clamp_result
};


#endif
