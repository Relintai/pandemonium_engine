#ifndef SWAP_CHANNELS_H
#define SWAP_CHANNELS_H


class SwapChannels : public MMNode {
 GDCLASS(SwapChannels, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 int get_op_r() const;
 void set_op_r(const int val);

 int get_op_g() const;
 void set_op_g(const int val);

 int get_op_b() const;
 void set_op_b(const int val);

 int get_op_a() const;
 void set_op_a(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 float apply(const int op, const Color &val);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_op_r();
 void set_op_r(const int val);
 int get_op_g();
 void set_op_g(const int val);
 int get_op_b();
 void set_op_b(const int val);
 int get_op_a();
 void set_op_a(const int val);

 SwapChannels();
 ~SwapChannels();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(int, "0,1,R,-R,G,-G,B,-B,A,-A") 
 int op_r = 2;
 //export(int, "0,1,R,-R,G,-G,B,-B,A,-A") 
 int op_g = 4;
 //export(int, "0,1,R,-R,G,-G,B,-B,A,-A") 
 int op_b = 6;
 //export(int, "0,1,R,-R,G,-G,B,-B,A,-A") 
 int op_a = 8;
 //op_r
 //op_g
 //op_b
 //op_a
};


#endif
