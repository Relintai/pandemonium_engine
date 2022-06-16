#ifndef FILL_TO_RANDOM_COLOR_H
#define FILL_TO_RANDOM_COLOR_H


class FillToRandomColor : public MMNode {
 GDCLASS(FillToRandomColor, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Color get_edge_color();
 void set_edge_color(const Color &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Color get_edge_color();
 void set_edge_color(const Color &val);

 FillToRandomColor();
 ~FillToRandomColor();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(Color) 
 Color edge_color = Color(1, 1, 1, 1);
 //edge_color
};


#endif
