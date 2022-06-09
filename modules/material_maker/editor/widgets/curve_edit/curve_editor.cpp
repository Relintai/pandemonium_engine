
#include "curve_editor.h"



 //tool;
 signal value_changed(value);

 void CurveEditor::_ready() {
  update_controls();
}


 void CurveEditor::set_curve(const Variant &c) {
  curve = c;
  update();
  update_controls();
}


 void CurveEditor::update_controls() {

  if (!curve) {
   return;
}


  for (c in get_children()) {
   c.queue_free();
}

   Variant  = curve.get_points();

  for (i in points.size()) {
    Variant  = points[i];
   //var control_point = preload("res://addons/mat_maker_gd/widgets/curve_edit/control_point.tscn").instance();
   add_child(control_point);
   control_point.initialize(p);
   control_point.rect_position = transform_point(p.p)-control_point.OFFSET;

   if (i == 0 || i == points.size()-1) {
    control_point.set_constraint(control_point.rect_position.x, control_point.rect_position.x, -control_point.OFFSET.y, rect_size.y-control_point.OFFSET.y);

    if (i == 0) {
     control_point.get_child(0).visible = false;
}


    else {
     control_point.get_child(1).visible = false;
}

}


   else {
     Variant  = transform_point(points[i-1].p).x+1;
     Variant  = transform_point(points[i+1].p).x-1;
    control_point.set_constraint(min_x, max_x, -control_point.OFFSET.y, rect_size.y-control_point.OFFSET.y);
}

   control_point.connect("moved", self, "_on_ControlPoint_moved");
   control_point.connect("removed", self, "_on_ControlPoint_removed");
}

  emit_signal("value_changed", curve);
}


 void CurveEditor::_on_ControlPoint_moved(const Variant &index) {
   Array points = curve.get_points();
   Variant  = get_child(index);
  points[index].p = reverse_transform_point(control_point.rect_position+control_point.OFFSET);

  if (control_point.has_node("LeftSlope")) {
    Variant  = control_point.get_node("LeftSlope").rect_position/rect_size;

   if (slope_vector.x != 0) {
    points[index].ls = -slope_vector.y / slope_vector.x;
}

}


  if (control_point.has_node("RightSlope")) {
    Variant  = control_point.get_node("RightSlope").rect_position/rect_size;

   if (slope_vector.x != 0) {
    points[index].rs = -slope_vector.y / slope_vector.x;
}

}

  curve.set_points(points, false);
  update();
  emit_signal("value_changed", curve);
}


 void CurveEditor::_on_ControlPoint_removed(const Variant &index) {

  if (curve.remove_point(index)) {
   update();
   update_controls();
}

}


 void CurveEditor::_on_CurveEditor_gui_input(const Variant &event) {

  if (event is InputEventMouseButton) {

   if (event.button_index == BUTTON_LEFT && event.doubleclick) {
     Variant  = reverse_transform_point(get_local_mouse_position());
    curve.add_point(new_point_position.x, new_point_position.y, 0.0, 0.0);
    update_controls();
}

}

}


 void CurveEditor::_on_resize() {
  ._on_resize();
  update_controls();
}

}

 CurveEditor::CurveEditor() {
 }

 CurveEditor::~CurveEditor() {
 }


 static void CurveEditor::_bind_methods() {
  ClassDB::bind_method(D_METHOD("_ready"), &CurveEditor::_ready);
  ClassDB::bind_method(D_METHOD("set_curve", "c"), &CurveEditor::set_curve);
  ClassDB::bind_method(D_METHOD("update_controls"), &CurveEditor::update_controls);
  ClassDB::bind_method(D_METHOD("_on_ControlPoint_moved", "index"), &CurveEditor::_on_ControlPoint_moved);
  ClassDB::bind_method(D_METHOD("_on_ControlPoint_removed", "index"), &CurveEditor::_on_ControlPoint_removed);
  ClassDB::bind_method(D_METHOD("_on_CurveEditor_gui_input", "event"), &CurveEditor::_on_CurveEditor_gui_input);
  ClassDB::bind_method(D_METHOD("_on_resize"), &CurveEditor::_on_resize);

 }



