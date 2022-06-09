
#include "control_point.h"


bool ControlPoint::get_moving() const {
 return moving;
}

void ControlPoint::set_moving(const bool val) {
moving = val;
}



 //tool;
  bool moving = false;
 const OFFSET : Vector2 = Vector2(3, 3);
 signal moved(index);
 signal removed(index);

 void ControlPoint::_draw() {
  //	var current_theme : Theme = get_node("/root/MainWindow").theme;
  //	var color : Color = current_theme.get_color("font_color", "Label");
   Color color = Color(1, 1, 1, 1);
  draw_rect(Rect2(0, 0, 7, 7), color);
}


 void ControlPoint::initialize(const Vector2 &p) {
  rect_position = get_parent().transform_point(p) - OFFSET;
}


 void ControlPoint::_on_ControlPoint_gui_input(const Variant &event) {

  if (event is InputEventMouseButton) {

   if (event.button_index == BUTTON_LEFT) {

    if (event.pressed) {
     moving = true;
}


    else {
     moving = false;
     get_parent().update_controls();
}

}


   else if (event.button_index == BUTTON_RIGHT && event.pressed) {
    emit_signal("removed", get_index());
}

}


  else if (moving && event is InputEventMouseMotion) {
   rect_position += event.relative;
   emit_signal("moved", get_index());
}

}

}

 ControlPoint::ControlPoint() {
  moving = false;
 }

 ControlPoint::~ControlPoint() {
 }


 static void ControlPoint::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_moving"), &ControlPoint::get_moving);
   ClassDB::bind_method(D_METHOD("set_moving", "value"), &ControlPoint::set_moving);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "moving"), "set_moving", "get_moving");


  ClassDB::bind_method(D_METHOD("_draw"), &ControlPoint::_draw);
  ClassDB::bind_method(D_METHOD("initialize", "p"), &ControlPoint::initialize);
  ClassDB::bind_method(D_METHOD("_on_ControlPoint_gui_input", "event"), &ControlPoint::_on_ControlPoint_gui_input);

 }



