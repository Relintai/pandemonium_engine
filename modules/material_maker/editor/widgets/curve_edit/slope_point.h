#ifndef SLOPE_POINT_H
#define SLOPE_POINT_H


class SlopePoint : public Control {
 GDCLASS(SlopePoint, Control);

 public:

 float get_distance() const;
 void set_distance(const float val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 void _ready();
 void _draw();
 void _on_ControlPoint_gui_input(const Variant &event);

 SlopePoint();
 ~SlopePoint();

 protected:
 static void _bind_methods();

 //tool
 //export 
 float distance = ;
 Variant  = false;
 const OFFSET = -Vector2(0, 0);
};


#endif
