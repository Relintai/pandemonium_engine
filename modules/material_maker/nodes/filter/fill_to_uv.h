#ifndef FILL_TO_UV_H
#define FILL_TO_UV_H


class FillToUv : public MMNode {
 GDCLASS(FillToUv, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 int get_mode() const;
 void set_mode(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_mode();
 void set_mode(const int val);

 FillToUv();
 ~FillToUv();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(int, "Stretch,Square") 
 int mode = 0;
 //mode
};


#endif
