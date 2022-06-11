#ifndef GRADIENT_BASE_H
#define GRADIENT_BASE_H


class GradientBase : public MMNode {
 GDCLASS(GradientBase, MMNode);

 public:

 int get_interpolation_type() const;
 void set_interpolation_type(const int val);

 PoolRealArray get_points();
 void set_points(const PoolRealArray &val);

 Color get_gradient_color(const float x);
 int get_interpolation_type();
 void set_interpolation_type(const int val);
 PoolRealArray get_points();
 void set_points(const PoolRealArray &val);
 float get_point_value(const int index);
 Color get_point_color(const int index);
 void add_point(const float val, const Color &color);
 int get_point_count();
 void clear();

 GradientBase();
 ~GradientBase();

 protected:
 static void _bind_methods();

 //tool
 //var Gradients = preload("res://addons/mat_maker_gd/nodes/common/gradients.gd")
 //export(int) 
 // setget set_interpolation_type, get_interpolation_type
 int interpolation_type = 1;
 //export(PoolRealArray) 
 PoolRealArray points = PoolRealArray();
};


#endif
