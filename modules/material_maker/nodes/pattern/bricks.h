#ifndef BRICKS_H
#define BRICKS_H


class Bricks : public MMNode {
 GDCLASS(Bricks, MMNode);

 public:

 Ref<Resource> get_out_bricks_pattern();
 void set_out_bricks_pattern(const Ref<Resource> &val);

 Ref<Resource> get_out_random_color();
 void set_out_random_color(const Ref<Resource> &val);

 Ref<Resource> get_out_position_x();
 void set_out_position_x(const Ref<Resource> &val);

 Ref<Resource> get_out_position_y();
 void set_out_position_y(const Ref<Resource> &val);

 Ref<Resource> get_out_brick_uv();
 void set_out_brick_uv(const Ref<Resource> &val);

 Ref<Resource> get_out_corner_uv();
 void set_out_corner_uv(const Ref<Resource> &val);

 Ref<Resource> get_out_direction();
 void set_out_direction(const Ref<Resource> &val);

 int get_type() const;
 void set_type(const int val);

 int get_repeat() const;
 void set_repeat(const int val);

 Vector2 get_col_row();
 void set_col_row(const Vector2 &val);

 float get_offset() const;
 void set_offset(const float val);

 Ref<Resource> get_mortar();
 void set_mortar(const Ref<Resource> &val);

 Ref<Resource> get_bevel();
 void set_bevel(const Ref<Resource> &val);

 Ref<Resource> get_roundness();
 void set_roundness(const Ref<Resource> &val);

 float get_corner() const;
 void set_corner(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_type();
 void set_type(const int val);
 int get_repeat();
 void set_repeat(const int val);
 Vector2 get_col_row();
 void set_col_row(const Vector2 &val);
 float get_offset();
 void set_offset(const float val);
 float get_corner();
 void set_corner(const float val);

 Bricks();
 ~Bricks();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> out_bricks_pattern;
 //export(Resource) 
 Ref<Resource> out_random_color;
 //export(Resource) 
 Ref<Resource> out_position_x;
 //export(Resource) 
 Ref<Resource> out_position_y;
 //export(Resource) 
 Ref<Resource> out_brick_uv;
 //export(Resource) 
 Ref<Resource> out_corner_uv;
 //export(Resource) 
 Ref<Resource> out_direction;
 //export(int, "Running Bond,Running Bond (2),HerringBone,Basket Weave,Spanish Bond") 
 int type = 0;
 //export(int) 
 int repeat = 1;
 //export(Vector2) 
 Vector2 col_row = Vector2(4, 4);
 //export(float) 
 float offset = 0.5;
 //export(Resource) 
 Ref<Resource> mortar;
 //export(Resource) 
 Ref<Resource> bevel;
 //export(Resource) 
 Ref<Resource> roundness;
 //export(float) 
 float corner = 0.3;
 //type
 //repeat
 //col_row
 //offset
 //corner
};


#endif
