#ifndef FILL_TO_RANDOM_GREY_H
#define FILL_TO_RANDOM_GREY_H


class FillToRandomGrey : public MMNode {
 GDCLASS(FillToRandomGrey, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 float get_edge_color() const;
 void set_edge_color(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 float get_edge_color();
 void set_edge_color(const float val);

 FillToRandomGrey();
 ~FillToRandomGrey();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(float) 
 float edge_color = 1;
 //edge_color
};


#endif
