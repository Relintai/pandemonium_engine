
#include "slope_point.h"


float SlopePoint::get_distance() const {
 return distance;
}

void SlopePoint::set_distance(const float val) {
distance = val;
}


Variant SlopePoint::get_Variant() {
 return Variant;
}

void SlopePoint::set_Variant(const Variant &val) {
Variant = val;
}



 //tool;
 //export ;
  float distance = ;
  Variant  = false;
 const OFFSET = -Vector2(0, 0);

 void SlopePoint::_ready() {
  // Replace with function body.;
  pass;
}


 void SlopePoint::_draw() {
  //	var current_theme : Theme = get_node("/root/MainWindow").theme;
  //	var color : Color = current_theme.get_color("font_color", "Label");
   Color color = Color(1, 1, 1, 1);
  draw_circle(Vector2(3.0, 3.0), 3.0, color);
}


 void SlopePoint::_on_ControlPoint_gui_input(const Variant &event) {

  if (event is InputEventMouseButton) {

   if (event.button_index == BUTTON_LEFT) {

    if (event.pressed) {

     if (event.doubleclick) {
       Variant  = get_parent();
       Vector2 vector = ;

      if (get_index() == 0) {
       vector = parent.rect_position-parent.get_parent().get_child(parent.get_index()-1).rect_position;
}


      else {
       vector = parent.get_parent().get_child(parent.get_index()+1).rect_position-parent.rect_position;
}

      vector = distance*vector.normalized();
      rect_position = vector-OFFSET;

      if (event.control) {
       get_parent().get_child(1-get_index()).rect_position = -vector-OFFSET;
}

      get_parent().update_tangents();
}


     else {
      moving = true;
}

}


    else {
     moving = false;
}

}

}


  else if (moving && event is InputEventMouseMotion) {
    Variant  = get_global_mouse_position()-get_parent().get_global_rect().position+OFFSET;
   vector *= sign(vector.x);
   vector = distance*vector.normalized();
   rect_position = vector-OFFSET;

   if (event.control) {
    get_parent().get_child(1-get_index()).rect_position = -vector-OFFSET;
}

   get_parent().update_tangents();
}

}

}

 SlopePoint::SlopePoint() {
  distance = ;
   = false;
 }

 SlopePoint::~SlopePoint() {
 }


 static void SlopePoint::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_distance"), &SlopePoint::get_distance);
   ClassDB::bind_method(D_METHOD("set_distance", "value"), &SlopePoint::set_distance);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "distance"), "set_distance", "get_distance");


   ClassDB::bind_method(D_METHOD("get_Variant"), &SlopePoint::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &SlopePoint::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


  ClassDB::bind_method(D_METHOD("_ready"), &SlopePoint::_ready);
  ClassDB::bind_method(D_METHOD("_draw"), &SlopePoint::_draw);
  ClassDB::bind_method(D_METHOD("_on_ControlPoint_gui_input", "event"), &SlopePoint::_on_ControlPoint_gui_input);

 }



