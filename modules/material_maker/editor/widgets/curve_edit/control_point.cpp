
#include "control_point.h"


Variant ControlPoint::get_Variant() {
 return Variant;
}

void ControlPoint::set_Variant(const Variant &val) {
Variant = val;
}


bool ControlPoint::get_moving() const {
 return moving;
}

void ControlPoint::set_moving(const bool val) {
moving = val;
}


float ControlPoint::get_min_x() const {
 return min_x;
}

void ControlPoint::set_min_x(const float val) {
min_x = val;
}


float ControlPoint::get_max_x() const {
 return max_x;
}

void ControlPoint::set_max_x(const float val) {
max_x = val;
}


float ControlPoint::get_min_y() const {
 return min_y;
}

void ControlPoint::set_min_y(const float val) {
min_y = val;
}


float ControlPoint::get_max_y() const {
 return max_y;
}

void ControlPoint::set_max_y(const float val) {
max_y = val;
}



 //tool;
 //var MMCurve = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
  bool moving = false;
  float min_x = ;
  float max_x = ;
  float min_y = ;
  float max_y = ;
 const OFFSET : Vector2 = Vector2(3, 3);
 signal moved(index);
 signal removed(index);

 void ControlPoint::_ready() {
  // Replace with function body.;
  pass;
}


 void ControlPoint::_draw() {
  //	var current_theme : Theme = get_node("/root/MainWindow").theme;
  //	var color : Color = current_theme.get_color("font_color", "Label");
   Color color = Color(1, 1, 1, 1);

  for (c in get_children()) {

   if (c.visible) {
    draw_line(OFFSET, c.rect_position+OFFSET, color);
}

}

  draw_rect(Rect2(0, 0, 7, 7), color);
}

 //p : MMCurve.Point;

 void ControlPoint::initialize(const Variant &p) {
  rect_position = get_parent().transform_point(p.p)-OFFSET;

  if (p.ls != INF) {
   $LeftSlope.rect_position = $LeftSlope.distance*(get_parent().rect_size*Vector2(1.0, -p.ls)).normalized();
}


  if (p.rs != INF) {
   $RightSlope.rect_position = $RightSlope.distance*(get_parent().rect_size*Vector2(1.0, -p.rs)).normalized();
}

}


 void ControlPoint::set_constraint(const float x, const float X, const float y, const float Y) {
  min_x = x;
  max_x = X;
  min_y = y;
  max_y = Y;
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

   if (rect_position.x < min_x) {
    rect_position.x = min_x;
}


   else if (rect_position.x > max_x) {
    rect_position.x = max_x;
}


   if (rect_position.y < min_y) {
    rect_position.y = min_y;
}


   else if (rect_position.y > max_y) {
    rect_position.y = max_y;
}

   emit_signal("moved", get_index());
}

}


 void ControlPoint::update_tangents() {
  update();
  emit_signal("moved", get_index());
}

}

 ControlPoint::ControlPoint() {
 //var MMCurve = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
  moving = false;
  min_x = ;
  max_x = ;
  min_y = ;
  max_y = ;
 }

 ControlPoint::~ControlPoint() {
 }


 static void ControlPoint::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_Variant"), &ControlPoint::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &ControlPoint::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_moving"), &ControlPoint::get_moving);
   ClassDB::bind_method(D_METHOD("set_moving", "value"), &ControlPoint::set_moving);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "moving"), "set_moving", "get_moving");


   ClassDB::bind_method(D_METHOD("get_min_x"), &ControlPoint::get_min_x);
   ClassDB::bind_method(D_METHOD("set_min_x", "value"), &ControlPoint::set_min_x);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "min_x"), "set_min_x", "get_min_x");


   ClassDB::bind_method(D_METHOD("get_max_x"), &ControlPoint::get_max_x);
   ClassDB::bind_method(D_METHOD("set_max_x", "value"), &ControlPoint::set_max_x);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_x"), "set_max_x", "get_max_x");


   ClassDB::bind_method(D_METHOD("get_min_y"), &ControlPoint::get_min_y);
   ClassDB::bind_method(D_METHOD("set_min_y", "value"), &ControlPoint::set_min_y);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "min_y"), "set_min_y", "get_min_y");


   ClassDB::bind_method(D_METHOD("get_max_y"), &ControlPoint::get_max_y);
   ClassDB::bind_method(D_METHOD("set_max_y", "value"), &ControlPoint::set_max_y);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_y"), "set_max_y", "get_max_y");


  ClassDB::bind_method(D_METHOD("_ready"), &ControlPoint::_ready);
  ClassDB::bind_method(D_METHOD("_draw"), &ControlPoint::_draw);
  ClassDB::bind_method(D_METHOD("initialize", "p"), &ControlPoint::initialize);
  ClassDB::bind_method(D_METHOD("set_constraint", "x", "X", "y", "Y"), &ControlPoint::set_constraint);
  ClassDB::bind_method(D_METHOD("_on_ControlPoint_gui_input", "event"), &ControlPoint::_on_ControlPoint_gui_input);
  ClassDB::bind_method(D_METHOD("update_tangents"), &ControlPoint::update_tangents);

 }



