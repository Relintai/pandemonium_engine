#ifndef FILL_TO_SIZE_H
#define FILL_TO_SIZE_H


class FillToSize : public MMNode {
 GDCLASS(FillToSize, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 int get_formula() const;
 void set_formula(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_formula();
 void set_formula(const int val);

 FillToSize();
 ~FillToSize();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(int, "Area,Width,Height,Max(W,H)") 
 int formula = 0;
 //formula
};


#endif
