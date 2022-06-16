#ifndef FILL_TO_POSITION_H
#define FILL_TO_POSITION_H


class FillToPosition : public MMNode {
 GDCLASS(FillToPosition, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 int get_axis() const;
 void set_axis(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_axis();
 void set_axis(const int val);

 FillToPosition();
 ~FillToPosition();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(int, "X,Y,Radial") 
 int axis = 2;
 //axis
};


#endif
