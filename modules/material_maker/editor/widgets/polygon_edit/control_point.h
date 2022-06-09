#ifndef CONTROL_POINT_H
#define CONTROL_POINT_H


class ControlPoint : public Control {
 GDCLASS(ControlPoint, Control);

 public:

 bool get_moving() const;
 void set_moving(const bool val);

 void _draw();
 void initialize(const Vector2 &p);
 void _on_ControlPoint_gui_input(const Variant &event);

 ControlPoint();
 ~ControlPoint();

 protected:
 static void _bind_methods();

 //tool
 bool moving = false;
 const OFFSET : Vector2 = Vector2(3, 3);
 signal moved(index);
 signal removed(index);
};


#endif
