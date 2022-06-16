#ifndef GREYSCALE_H
#define GREYSCALE_H


class Greyscale : public MMNode {
 GDCLASS(Greyscale, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 int get_type() const;
 void set_type(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_type();
 void set_type(const int val);

 Greyscale();
 ~Greyscale();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(int, "Lightness,Average,Luminosity,Min,Max") 
 int type = 2;
 //type
};


#endif
