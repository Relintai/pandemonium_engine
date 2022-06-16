#ifndef FILL_CHANNEL_H
#define FILL_CHANNEL_H


class FillChannel : public MMNode {
 GDCLASS(FillChannel, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_value();
 void set_value(const Ref<Resource> &val);

 int get_channel() const;
 void set_channel(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_channel();
 void set_channel(const int val);

 FillChannel();
 ~FillChannel();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> value;
 //export(int, "R,G,B,A") 
 int channel = 3;
};


#endif
