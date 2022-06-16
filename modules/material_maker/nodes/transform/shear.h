#ifndef SHEAR_H
#define SHEAR_H


class Shear : public MMNode {
 GDCLASS(Shear, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 int get_direction() const;
 void set_direction(const int val);

 float get_amount() const;
 void set_amount(const float val);

 float get_center() const;
 void set_center(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_direction();
 void set_direction(const int val);
 float get_amount();
 void set_amount(const float val);
 float get_center();
 void set_center(const float val);

 Shear();
 ~Shear();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(int, "Horizontal,Vertical") 
 int direction = 0;
 //export(float) 
 float amount = 1;
 //export(float) 
 float center = 0;
 //direction
 //amount
 //center
};


#endif
