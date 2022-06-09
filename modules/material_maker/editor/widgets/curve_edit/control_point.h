#ifndef CONTROL_POINT_H
#define CONTROL_POINT_H


class ControlPoint : public Control {
 GDCLASS(ControlPoint, Control);

 public:

 Variant get_Variant();
 void set_Variant(const Variant &val);

 bool get_moving() const;
 void set_moving(const bool val);

 float get_min_x() const;
 void set_min_x(const float val);

 float get_max_x() const;
 void set_max_x(const float val);

 float get_min_y() const;
 void set_min_y(const float val);

 float get_max_y() const;
 void set_max_y(const float val);

 void _ready();
 void _draw();
 void initialize(const Variant &p);
 void set_constraint(const float x, const float X, const float y, const float Y);
 void _on_ControlPoint_gui_input(const Variant &event);
 void update_tangents();

 ControlPoint();
 ~ControlPoint();

 protected:
 static void _bind_methods();

 //tool
 Variant  = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
 bool moving = false;
 float min_x = ;
 float max_x = ;
 float min_y = ;
 float max_y = ;
 const OFFSET : Vector2 = Vector2(3, 3);
 signal moved(index);
 signal removed(index);
 //p : MMCurve.Point
};


#endif
